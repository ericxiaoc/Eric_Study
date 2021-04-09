###环境配置
* 瑞芯微RK3399
* Android 7.1系统
* Launcher3
###前言
Android7.0的代码开始，Launcher3可以在Hotseat的区域通过向上滑动打开应用列表界面。
###分析流程
* 控制上滑是否显示应用列表界面存在一个变量LAUNCHER3_ALL_APPS_PULL_UP,由这个变量来控制是否可以上滑显示AllApps；
``` java
package com.android.launcher3.config;

public final class FeatureFlags {
    private FeatureFlags() {}
    // Custom flags go below this
    public static boolean LAUNCHER3_DISABLE_ICON_NORMALIZATION = false;
    // As opposed to the new spring-loaded workspace.
    public static boolean LAUNCHER3_LEGACY_WORKSPACE_DND = false;
    public static boolean LAUNCHER3_LEGACY_FOLDER_ICON = false;
    public static boolean LAUNCHER3_USE_SYSTEM_DRAG_DRIVER = true;
    public static boolean LAUNCHER3_DISABLE_PINCH_TO_OVERVIEW = false;
    public static boolean LAUNCHER3_ALL_APPS_PULL_UP = true;
    // Feature flag to enable moving the QSB on the 0th screen of the workspace.
    public static final boolean QSB_ON_FIRST_SCREEN = false;
    // When enabled the all-apps icon is not added to the hotseat.
    public static final boolean NO_ALL_APPS_ICON = false;
    // When enabled fling down gesture on the first workspace triggers search.
    public static final boolean PULLDOWN_SEARCH = false;
    // When enabled the status bar may show dark icons based on the top of the wallpaper.
    public static final boolean LIGHT_STATUS_BAR = false;
}
```

* Launcher的拖拽事件实在DragLayer中实现的，包括上拉的onTouch的事件，而滑动的范围被限定在hotseat，也就是说hotseat之外的区域是无法生效的。
* 先来看布局文件以及初始化
``` xml
<!--拖拽层是包含以下所有的布局的，也就是说相当于是全局都有DragLayer的响应-->
<com.android.launcher3.dragndrop.DragLayer
        android:id="@+id/drag_layer"
        android:clipChildren="false"
        android:clipToPadding="false"
        android:importantForAccessibility="no"
        android:layout_width="match_parent"
        android:layout_height="match_parent">

        <!-- The workspace contains 5 screens of cells -->
        <!-- DO NOT CHANGE THE ID -->
        <!--这里就是桌面中间的最大的一块区域，主要放置图标快捷图标，创建文件夹-->
        <com.android.launcher3.Workspace
            android:id="@+id/workspace"
            android:layout_width="match_parent"
            android:layout_height="match_parent"
            android:layout_gravity="center"
            launcher:pageIndicator="@id/page_indicator" />

        <!-- DO NOT CHANGE THE ID 这里就是hotseat的布局-->
        <include layout="@layout/hotseat"
            android:id="@+id/hotseat"
            android:layout_width="match_parent"
            android:layout_height="match_parent"
            android:layout_gravity="right"
            launcher:layout_ignoreInsets="true" />

      <!--此处省略几个控件-->

      <!--如下就是allapp的布局-->
        <include layout="@layout/all_apps"
            android:id="@+id/apps_view"
            android:layout_width="match_parent"
            android:layout_height="match_parent"
            android:visibility="invisible" />

    </com.android.launcher3.dragndrop.DragLayer>
```
* 在Launcher.java中的加载,mDragLayer是父控件，根据父控件来获取子控件mWorkspace、mHotseat等
``` java
private void setupViews() {
        mLauncherView = findViewById(R.id.launcher);
        mDragLayer = (DragLayer) findViewById(R.id.drag_layer);
        mFocusHandler = mDragLayer.getFocusIndicatorHelper();
        mWorkspace = (Workspace) mDragLayer.findViewById(R.id.workspace);
        mQsbContainer = mDragLayer.findViewById(mDeviceProfile.isVerticalBarLayout()
                ? R.id.workspace_blocked_row : R.id.qsb_container);
		mDragLayer.removeView(mQsbContainer);
        mWorkspace.initParentViews(mDragLayer);

        mLauncherView.setSystemUiVisibility(View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                | View.SYSTEM_UI_FLAG_LAYOUT_STABLE);

        // Setup the drag layer
        mDragLayer.setup(this, mDragController, mAllAppsController);

        // Setup the hotseat
        mHotseat = (Hotseat) findViewById(R.id.hotseat);
        if (mHotseat != null) {
            mHotseat.setOnLongClickListener(this);
        }
      //省略下面的内容
}
```
* 接下来看看DragLayer里面的主要onTouch的事件，这里就先不讲拖拽的细节了,如下的代码监听touch事件的按下，抬起的处理，我们这里主要是分析上拉的流程，所以关注LAUNCHER3_ALL_APPS_PULL_UP这个变量的条件的主要情况；
``` java
@Override
public boolean onInterceptTouchEvent(MotionEvent ev) {
    int action = ev.getAction();

    if (action == MotionEvent.ACTION_DOWN) {
        // Cancel discovery bounce animation when a user start interacting on anywhere on
        // dray layer even if mAllAppsController is NOT the active controller.
        // TODO: handle other input other than touch
        mAllAppsController.cancelDiscoveryAnimation();
        if (handleTouchDown(ev, true)) {
            return true;
        }
    } else if (action == MotionEvent.ACTION_UP || action == MotionEvent.ACTION_CANCEL) {
        if (mTouchCompleteListener != null) {
            mTouchCompleteListener.onTouchComplete();
        }
        mTouchCompleteListener = null;
    }
    clearAllResizeFrames();

    mActiveController = null;

    if (mDragController.onInterceptTouchEvent(ev)) {
        mActiveController = mDragController;
        return true;
    }
//这里添加了一个变量的控制，也就是我们前面讲的是否可上拉显示allapps，
    if (FeatureFlags.LAUNCHER3_ALL_APPS_PULL_UP && mAllAppsController.onInterceptTouchEvent(ev)) {
        mActiveController = mAllAppsController;
        return true;
    }

    if (mPinchListener != null && mPinchListener.onInterceptTouchEvent(ev)) {
        // Stop listening for scrolling etc. (onTouchEvent() handles the rest of the pinch.)
        mActiveController = mPinchListener;
        return true;
    }
    return false;
}
```
* 继续往下分析，由于这里的变量起到是否上拉的作用，我们看看mAllAppsController.onInterceptTouchEvent(ev)事件的处理。mNoIntercept是判断是否有触摸事件的，true则直接返回掉，不进行处理接下来的事件。
``` java
//package/app/Launcher3/src/com/android/launcher3/allapps/AllAppsTransitionController.java
@Override
public boolean onInterceptTouchEvent(MotionEvent ev) {
    if (ev.getAction() == MotionEvent.ACTION_DOWN) {
        mNoIntercept = false;
        if (!mLauncher.isAllAppsVisible() && mLauncher.getWorkspace().workspaceInModalState()) {
            mNoIntercept = true;
        } else if (mLauncher.isAllAppsVisible() &&
                !mAppsView.shouldContainerScroll(ev)) {
            mNoIntercept = true;
        } else if (!mLauncher.isAllAppsVisible() && !shouldPossiblyIntercept(ev)) {
            mNoIntercept = true;
        } else {
            // Now figure out which direction scroll events the controller will start
            // calling the callbacks.
            int directionsToDetectScroll = 0;
            boolean ignoreSlopWhenSettling = false;

            if (mDetector.isIdleState()) {
                if (mLauncher.isAllAppsVisible()) {
                    directionsToDetectScroll |= VerticalPullDetector.DIRECTION_DOWN;
                } else {
                    directionsToDetectScroll |= VerticalPullDetector.DIRECTION_UP;
                }
            } else {
                if (isInDisallowRecatchBottomZone()) {
                    directionsToDetectScroll |= VerticalPullDetector.DIRECTION_UP;
                } else if (isInDisallowRecatchTopZone()) {
                    directionsToDetectScroll |= VerticalPullDetector.DIRECTION_DOWN;
                } else {
                    directionsToDetectScroll |= VerticalPullDetector.DIRECTION_BOTH;
                    ignoreSlopWhenSettling = true;
                }
            }
            mDetector.setDetectableScrollConditions(directionsToDetectScroll,
                    ignoreSlopWhenSettling);
        }
    }
    if (mNoIntercept) {
        return false;
    }
    mDetector.onTouchEvent(ev);
    if (mDetector.isSettlingState() && (isInDisallowRecatchBottomZone() || isInDisallowRecatchTopZone())) {
        return false;
    }
    return mDetector.isDraggingOrSettling();
}
```
* 分析第一个条件：综合如下分析，so返回的是false；
``` java
"package/app/Launcher3/src/com/android/launcher3/Launcher3.java"

public boolean isAllAppsVisible() {
  return isAppsViewVisible();
}
//这里的状态在主界面上默认为 mState = WORKSPACE，so返回的是false;
public boolean isAppsViewVisible() {
  Log.d("eric","mState = "+mState);
  return (mState == State.APPS) || (mOnResumeState == State.APPS);
}

"package/app/Launcher3/src/com/android/launcher3/Workspace.java"

//这里的状态mState = NORMAL,这里返回的是false;
public boolean workspaceInModalState() {
    Log.d("eric","workspace mState = "+mState);
    return mState != State.NORMAL;
}
```
* 继续分析第二个条件：isAllAppsVisible()返回的是false
``` java
else if (mLauncher.isAllAppsVisible() &&!mAppsView.shouldContainerScroll(ev)) {...}
//这里其实都不用分析，第一个条件里面分析isAllAppsVisible返回的是false,直接都返回掉了,大概看一下
/**
 * Returns whether the view itself will handle the touch event or not.
 */
public boolean shouldContainerScroll(MotionEvent ev) {
    int[] point = new int[2];
    point[0] = (int) ev.getX();
    point[1] = (int) ev.getY();
    Utilities.mapCoordInSelfToDescendent(mAppsRecyclerView, this, point);

    // IF the MotionEvent is inside the search box, and the container keeps on receiving
    // touch input, container should move down.
    //如果MotionEvent在搜索框内，并且容器继续接收触摸输入，则容器应该向下移动---百度翻译。
    if (mLauncher.getDragLayer().isEventOverView(mSearchContainer, ev)) {
        return true;
    }

    // IF the MotionEvent is inside the thumb, container should not be pulled down.
    //如果MotionEvent在thumb内，则不应向下拉容器
    if (mAppsRecyclerView.getScrollBar().isNearThumb(point[0], point[1])) {
        return false;
    }

    // IF a shortcuts container is open, container should not be pulled down.
    //如果快捷键容器已打开，则不应下拉该容器
    if (mLauncher.getOpenShortcutsContainer() != null) {
        return false;
    }

    // IF scroller is at the very top OR there is no scroll bar because there is probably not
    // enough items to scroll, THEN it's okay for the container to be pulled down.
    //如果滚动条在最顶端或者没有滚动条因为可能没有
    //足够的条目来滚动，然后可以下拉容器
    if (mAppsRecyclerView.getScrollBar().getThumbOffset().y <= 0) {
        return true;
    }
    return false;
}
```
* 分析第三个条件：如下条件分析这里也是返回false
``` java
else if (!mLauncher.isAllAppsVisible() && !shouldPossiblyIntercept(ev)) {...}
//mLauncher.isAllAppsVisible()返回的是false,继续分析shouldPossiblyIntercept;
private boolean shouldPossiblyIntercept(MotionEvent ev) {
    DeviceProfile grid = mLauncher.getDeviceProfile();//获取launcher配置文件对象
    if (mDetector.isIdleState()) {
        if (grid.isVerticalBarLayout()) {
            if (ev.getY() > mLauncher.getDeviceProfile().heightPx - mBezelSwipeUpHeight) {
                return true;
            }
        } else {
            if (mLauncher.getDragLayer().isEventOverHotseat(ev) ||
                    mLauncher.getDragLayer().isEventOverPageIndicator(ev)) {
                return true;
            }
        }
        return false;
    } else {
        return true;
    }
}
//============================
if (mDetector.isIdleState()) {...}
//package/app/Launcher3/src/com/android/launcher3/allapps/VerticalPullDetector.java

/**
 * There's no touch and there's no animation.没有触摸，也没有动画
 */
public boolean isIdleState() {
    Log.d("eric","isIdleState = "+mState);
    return mState == ScrollState.IDLE;
}
enum ScrollState {
        IDLE,           //no Touch
        DRAGGING,      // onDragStart, onDrag
        SETTLING       // onDragEnd
}
//============================
if (grid.isVerticalBarLayout()) {...}
//package/app/Launcher3/src/com/android/launcher3/DeviceProfile.java

/**
 * When {@code true}, the device is in landscape mode and the hotseat is on the right  * column. 设备处于横向模式，hotseat 位于右侧上
 * When {@code false}, either device is in portrait mode or the device is in landscape * mode and the hotseat is on the bottom row.
 * 设备处于纵向模式或横向模式，且Hotseat位于最下面
 */
public boolean isVerticalBarLayout() {
    return isLandscape && transposeLayoutWithOrientation;
}
//============================
//下面的两个条件则是进行区域的判断，是否在hotseat范围内，这里就不详细深究了,看一下大概的代码
mLauncher.getDragLayer().isEventOverHotseat(ev);
mLauncher.getDragLayer().isEventOverPageIndicator(ev);
//package/app/Launcher3/src/com/android/launcher3/dragndrop/DragLayer.java
public boolean isEventOverHotseat(MotionEvent ev) {
    return isEventOverView(mLauncher.getHotseat(), ev);
}
public boolean isEventOverView(View view, MotionEvent ev) {
    getDescendantRectRelativeToSelf(view, mHitRect);
    return mHitRect.contains((int) ev.getX(), (int) ev.getY());
}
/**
 * Determine the rect of the descendant in this DragLayer's coordinates
 *
 * @param descendant The descendant whose coordinates we want to find.
 * @param r The rect into which to place the results.
 * @return The factor by which this descendant is scaled relative to this DragLayer.
 */
public float getDescendantRectRelativeToSelf(View descendant, Rect r) {
    mTmpXY[0] = 0;
    mTmpXY[1] = 0;
    float scale = getDescendantCoordRelativeToSelf(descendant, mTmpXY);

    r.set(mTmpXY[0], mTmpXY[1],
            (int) (mTmpXY[0] + scale * descendant.getMeasuredWidth()),
            (int) (mTmpXY[1] + scale * descendant.getMeasuredHeight()));
    return scale;
}
```
* 最终运行到了else的条件里面，看如下代码
``` java
else {
    // Now figure out which direction scroll events the controller will start
    // calling the callbacks.
    //判断当前控制是往哪个方向的事件，且call回调；
    int directionsToDetectScroll = 0;
    boolean ignoreSlopWhenSettling = false;

    if (mDetector.isIdleState()) {
        if (mLauncher.isAllAppsVisible()) {
            directionsToDetectScroll |= VerticalPullDetector.DIRECTION_DOWN;
        } else {
            directionsToDetectScroll |= VerticalPullDetector.DIRECTION_UP;
        }
    } else {
        if (isInDisallowRecatchBottomZone()) {
            directionsToDetectScroll |= VerticalPullDetector.DIRECTION_UP;
        } else if (isInDisallowRecatchTopZone()) {
            directionsToDetectScroll |= VerticalPullDetector.DIRECTION_DOWN;
        } else {
            directionsToDetectScroll |= VerticalPullDetector.DIRECTION_BOTH;
            ignoreSlopWhenSettling = true;
        }
    }
    mDetector.setDetectableScrollConditions(directionsToDetectScroll,
            ignoreSlopWhenSettling);
}
```
* 关键代码来了，上面的代码都是判断当前的界面状态，触摸的位置是否可上拉，以及控制器方向的回调。mNoIntercept变量来判断是否拦截ontouch事件，最终mNoIntercept一直为false，看如下代码,onTouch事件继续往下分发，
``` java
if (mNoIntercept) {
    return false;
}
mDetector.onTouchEvent(ev);
//mDetector.isSettlingState() 判断是否滑动完成
//
if (mDetector.isSettlingState() && (isInDisallowRecatchBottomZone() || isInDisallowRecatchTopZone())) {
    return false;
}
return mDetector.isDraggingOrSettling();
}

//package/app/Launcher3/src/com/android/launcher3/allapps/VerticalPullDetector.java
public boolean isSettlingState() {
    return mState == ScrollState.SETTLING;
}
```
* 接下来调用的是VerticalPullDetector的onTouchEvent()方法，我们来看下VerticalPullDetector的主要代码，touch事件中的MotionEvent.ACTION_DOWN来记录按下的坐标位置mDownX,mDownY;touch事件中的MotionEvent.ACTION_MOVE中的mDisplacementX,mDisplacementY来计算移动的距离,然后调用setState();setState方法的作用是设置新的状态,然后通过computeVelocity计算滑动速度;最后调用reportDragging();
``` java
//packages\apps\Launcher3\src\com\android\launcher3\allapps\VerticalPullDetector.java
    enum ScrollState {
        IDLE,
        DRAGGING,      // onDragStart, onDrag
        SETTLING       // onDragEnd
    }
    ...
    interface Listener {
        void onDragStart(boolean start);
        boolean onDrag(float displacement, float velocity);
        void onDragEnd(float velocity, boolean fling);
    }
    ...
    public boolean onTouchEvent(MotionEvent ev) {
        switch (ev.getAction()) {
            case MotionEvent.ACTION_DOWN:
                mDownX = ev.getX();
                mDownY = ev.getY();
                mLastDisplacement = 0;
                mDisplacementY = 0;
                mVelocity = 0;

                if (mState == ScrollState.SETTLING && mIgnoreSlopWhenSettling) {
                    setState(ScrollState.DRAGGING);
                }
                break;
            case MotionEvent.ACTION_MOVE:
                mDisplacementX = ev.getX() - mDownX;
                mDisplacementY = ev.getY() - mDownY;
                computeVelocity(ev);
                Log.i(TAG,"moving mDisplacementY=" +mDisplacementY+",mDisplacementX=" +mDisplacementX);
                // handle state and listener calls.
                if (mState != ScrollState.DRAGGING && shouldScrollStart()) {
                    setState(ScrollState.DRAGGING);
                }
                if (mState == ScrollState.DRAGGING) {
                    reportDragging();
                }
                break;
            case MotionEvent.ACTION_UP:
                // These are synthetic events and there is no need to update internal values.
                if (mState == ScrollState.DRAGGING) {
                    setState(ScrollState.SETTLING);
                }
                break;
            ...
        }
        // Do house keeping.
        mLastDisplacement = mDisplacementY;
        mLastY = ev.getY();
        return true;
    }
```
* VerticalPullDetector 中的 Listener接口是在AllAppsTransitionController中实现的;

``` java
public AllAppsTransitionController(Launcher l) {
    mLauncher = l;
    mDetector = new VerticalPullDetector(l);
    mDetector.setListener(this);
    ...
}
```
* reportDragging方法最终回调的是AllAppsTransitionController中的onDrag;继续看onDrag方法的实现；
``` java
private boolean reportDragging() {
        float delta = mDisplacementY - mLastDisplacement;
        if (delta != 0) {
            if (DBG) {
                Log.d(TAG, String.format("onDrag disp=%.1f, velocity=%.1f",
                        mDisplacementY, mVelocity));
            }

            return mListener.onDrag(mDisplacementY - mSubtractDisplacement, mVelocity);
        }
        return true;
    }
//package/app/Launcher3/src/com/android/launcher3/allapps/AllAppsTransitionController.java
@Override
public boolean onDrag(float displacement, float velocity) {
    if (mAppsView == null) {
        return false;   // early termination.
    }

    mContainerVelocity = velocity;

    float shift = Math.min(Math.max(0, mShiftStart + displacement), mShiftRange);
    setProgress(shift / mShiftRange);

    return true;
}
```
* setState方法里面通过当前的状态，来回调对应的onDragStart、onDragEnd方法；
``` java
private void setState(ScrollState newState) {
    if (DBG) {
        Log.d(TAG, "setState:" + mState + "->" + newState);
    }
    // onDragStart and onDragEnd is reported ONLY on state transition
    if (newState == ScrollState.DRAGGING) {
        initializeDragging();
        if (mState == ScrollState.IDLE) {
            reportDragStart(false /* recatch */);
        } else if (mState == ScrollState.SETTLING) {
            reportDragStart(true /* recatch */);
        }
    }
    if (newState == ScrollState.SETTLING) {
        reportDragEnd();
    }

    mState = newState;
}
...
private boolean reportDragStart(boolean recatch) {
    mListener.onDragStart(!recatch);
    if (DBG) {
        Log.d(TAG, "onDragStart recatch:" + recatch);
    }
    return true;
}
...
private void reportDragEnd() {
    if (DBG) {
        Log.d(TAG, String.format("onScrolEnd disp=%.1f, velocity=%.1f",
                mDisplacementY, mVelocity));
    }
    mListener.onDragEnd(mVelocity, Math.abs(mVelocity) > RELEASE_VELOCITY_PX_MS);

}
```
* onDragStart里面只是进行了一些初始化，重新创建了一个mCurrentAnimation对象，主要还是看一下onDrag中的setProgress方法,根据progress的值计算出相对应的颜色、透明度，偏移量，并且设置到相应的view上面。
``` java
/**
* @param progress       value between 0 and 1, 0 shows all apps and 1 shows workspace
*            值介于0和1之间，0显示所有应用程序，1显示工作区
*/
public void setProgress(float progress) {
   float shiftPrevious = mProgress * mShiftRange;
   mProgress = progress;
   float shiftCurrent = progress * mShiftRange;

   float workspaceHotseatAlpha = Utilities.boundToRange(progress, 0f, 1f);
   float alpha = 1 - workspaceHotseatAlpha;
   float interpolation = mAccelInterpolator.getInterpolation(workspaceHotseatAlpha);

   int color = (Integer) mEvaluator.evaluate(mDecelInterpolator.getInterpolation(alpha),
           mHotseatBackgroundColor, mAllAppsBackgroundColor);
   int bgAlpha = Color.alpha((int) mEvaluator.evaluate(alpha,
           mHotseatBackgroundColor, mAllAppsBackgroundColor));

   mAppsView.setRevealDrawableColor(ColorUtils.setAlphaComponent(color, bgAlpha));
   mAppsView.getContentView().setAlpha(alpha);
   mAppsView.setTranslationY(shiftCurrent);

   if (!mLauncher.getDeviceProfile().isVerticalBarLayout()) {
       mWorkspace.setHotseatTranslationAndAlpha(Workspace.Direction.Y, -mShiftRange + shiftCurrent,
               interpolation);
   } else {
       mWorkspace.setHotseatTranslationAndAlpha(Workspace.Direction.Y,
               PARALLAX_COEFFICIENT * (-mShiftRange + shiftCurrent),
               interpolation);
   }

   if (mIsTranslateWithoutWorkspace) {
       return;
   }
   mWorkspace.setWorkspaceYTranslationAndAlpha(
           PARALLAX_COEFFICIENT * (-mShiftRange + shiftCurrent), interpolation);

   if (!mDetector.isDraggingState()) {
       mContainerVelocity = mDetector.computeVelocity(shiftCurrent - shiftPrevious,
               System.currentTimeMillis());
   }

   mCaretController.updateCaret(progress, mContainerVelocity, mDetector.isDraggingState());
   updateLightStatusBar(shiftCurrent);
}
```
* 最后我们看一下MotionEvent.ACTION_UP里面的方法，通过setState方法回调到
``` java
case MotionEvent.ACTION_UP:
    // These are synthetic events and there is no need to update internal values.
    if (mState == ScrollState.DRAGGING) {
        setState(ScrollState.SETTLING);
    }
    break;
...
//package/app/Launcher3/src/com/android/launcher3/allapps/AllAppsTransitionController.java
@Override
public void onDragEnd(float velocity, boolean fling) {
    if (mAppsView == null) {
        return; // early termination.
    }

    if (fling) { //是否是快速滑动事件
        if (velocity < 0) { //表示向上滑动
            calculateDuration(velocity, mAppsView.getTranslationY());

            if (!mLauncher.isAllAppsVisible()) {
                mLauncher.getUserEventDispatcher().logActionOnContainer(
                        LauncherLogProto.Action.FLING,
                        LauncherLogProto.Action.UP,
                        LauncherLogProto.HOTSEAT);
            }
            mLauncher.showAppsView(true /* animated */,
                    false /* updatePredictedApps */,
                    false /* focusSearchBar */);
        } else {
            calculateDuration(velocity, Math.abs(mShiftRange - mAppsView.getTranslationY()));
            mLauncher.showWorkspace(true);
        }
        // snap to top or bottom using the release velocity
    } else {
        if (mAppsView.getTranslationY() > mShiftRange / 2) {//说明在一半以下
            calculateDuration(velocity, Math.abs(mShiftRange - mAppsView.getTranslationY()));
            mLauncher.showWorkspace(true);
        } else {
            calculateDuration(velocity, Math.abs(mAppsView.getTranslationY()));
            if (!mLauncher.isAllAppsVisible()) {
                mLauncher.getUserEventDispatcher().logActionOnContainer(
                        LauncherLogProto.Action.SWIPE,
                        LauncherLogProto.Action.UP,
                        LauncherLogProto.HOTSEAT);
            }
            mLauncher.showAppsView(true, /* animated */
                    false /* updatePredictedApps */,
                    false /* focusSearchBar */);
        }
    }
}
```
* 首先通过fling来判断是否是快速滑动事件，通过(velocity < 0)来判断是否是向上滑，总结起来就是在Hotseat区域快速往上滑可以直接进入all apps; 其他方向则继续停留Workspace;
* 若不是快速的滑动，慢慢的往上滑动，拖动Hotseat的区域过半则进入all apps，反则停留在Workspace，如下图的效果；
![demo.mp4_20210313_112524](assets/demo.mp4_20210313_112524.gif)
* 这里就不分析一下mLauncher.showWorkspace()以及mLauncher.showAppsView()的实现了，下章见！

#####参考文献
https://blog.csdn.net/lmpt90/article/details/89308908
