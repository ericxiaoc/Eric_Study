###Android 8.1 添加自定义服务
####1. 编写aidl文件
``` java
frameworks/base/core/java/android/app/ICustomManager.aidl

package android.app;
/**
 * System private API for talking with the custom manager service.
 *
*/
interface ICustomManager
{
    void CustomSystemDate(String mtime);
}
```
####2. 注册aidl文件
声明我们需要的aidl文件.
``` makefile
frameworks/base/Android.mk

LOCAL_SRC_FILES += \
      #......省略
      core/java/android/app/ICustomManager.aidl \
```
####3. 添加实现服务service
``` java
frameworks/base/services/core/java/com/android/server/CustomManagerService.java
public class CustomManagerService extends ICustomManager.Stub
{
	private static final String TAG = "CustomManagerService";

	private Context mContext;

	CustomManagerService(Context context)
	{
		Log.d(TAG, "enter CustomManagerService");
		mContext = context;
	}

  public void CustomSystemDate(String mtime)
  {
  		Log.d(TAG,"XHSetSystemDate Parameter !!!");
  }
```
####4. 添加管理manager
``` java
frameworks/base/core/java/android/app/CustomManager.java

package android.app;
import android.content.Context;
import android.content.res.Configuration;
import android.os.RemoteException;
import android.os.ServiceManager;
import android.content.Intent;
import android.util.Log;

public class CustomManager
{
    private static final String TAG = "CustomManager";

    private final ICustomManager mService;

    CustomManager()
    {
    		Log.d(TAG, "enter CustomManager");
        mService = ICustomManager.Stub.asInterface(ServiceManager.getService(Context.CUSTOM_SERVICE));
    }

    public void CustomSystemDate(String mtime){
    	 try
        {
             mService.CustomSystemDate(mtime);
        }
        catch (RemoteException ex) {

        }
    }
}
```
####5. 在Context.java添加常量
``` java
frameworks/base/core/java/android/content/Context.java

public static final String CUSTOM_SERVICE = "customApi";

@StringDef({
            //......省略
            CUSTOM_SERVICE,
            WIFI_AWARE_SERVICE,
```
####6. 注册系统服务
``` java
frameworks/base/core/java/android/app/SystemServiceRegistry.java

+        registerService(Context.CUSTOM_SERVICE, CustomManager.class,
+                new CachedServiceFetcher<CustomManager>() {
+            @Override
+            public CustomManager createService(ContextImpl ctx) {
+                return new CustomManager();
+            }});

         registerService(Context.WIFI_RTT_SERVICE, RttManager.class,
                 new CachedServiceFetcher<RttManager>() {
```
####7. 开机启动
``` java
frameworks/base/services/java/com/android/server/SystemServer.java
+import com.android.server.CustomManagerService;

 import com.mediatek.server.MtkSystemServer;

@@ -727,6 +728,7 @@ public final class SystemServer {
         MmsServiceBroker mmsService = null;
         HardwarePropertiesManagerService hardwarePropertiesService = null;

+               CustomManagerService     customManagerService=null;
         boolean disableStorage = SystemProperties.getBoolean("config.disable_storage", false);
@@ -1393,7 +1395,14 @@ public final class SystemServer {
                 mSystemServiceManager.startService(ContextHubSystemService.class);
                 traceEnd();
             }

+          traceBeginAndSlog("StartCustomManagerService");
+            try {
+                    Slog.i(TAG, "CustomManagerService");
+                    customManagerService = new CustomManagerService(context);
+                    ServiceManager.addService(Context.CUSTOM_SERVICE, customManagerService);
+            } catch (Throwable e) {
+                    Slog.e(TAG, "Failure starting CustomManagerService", e);
+            }
             traceBeginAndSlog("StartDiskStatsService");
             try {
                 ServiceManager.addService("diskstats", new DiskStatsService(context));

```
####8. 添加selinux权限
``` c++
//====================================================================
modified:   system/sepolicy/prebuilts/api/26.0/nonplat_sepolicy.cil
(typeattribute customApi_service_26_0)
(roletype object_r customApi_service_26_0)
//...省略
(typeattributeset system_api_service(... customApi_service_26_0))
(typeattributeset system_server_service(... customApi_service_26_0))
(typeattributeset service_manager_type(... customApi_service_26_0))

//====================================================================

modified:   system/sepolicy/prebuilts/api/26.0/private/service_contexts
customApi 			u:object_r:customApi_service:s0

//====================================================================

modified:   system/sepolicy/prebuilts/api/26.0/public/service.te
type customApi_service, system_api_service, system_server_service, service_manager_type;

//====================================================================

modified:   system/sepolicy/private/compat/26.0/26.0.cil
(typeattributeset customApi_service_26_0 (customApi_service))

//====================================================================

modified:   system/sepolicy/private/service_contexts
customApi 				u:object_r:customApi_service:s0

//====================================================================

modified:   system/sepolicy/public/service.te
type customApi_service, system_api_service, system_server_service, service_manager_type;

//====================================================================
```
####9. 检查服务是否添加成功
刷完机开机后，adb shell 进入后使用 service list | grep custom
``` shell
C:\Users\Eric>adb shell
M5:/ $ service list | grep custom
service list | grep custom
40      customApi: [android.app.ICustomManager]
M5:/ $
```
如上即为成功。
####10. 剥离单独的jar包
由于添加的系统服务，正常是需要导入编译后的frameworks.jar,整个framework的jar大小比较大，so我们单独剥离我们的jar包供app使用。
``` java
frameworks/base/customApi/java/com/android/customApi/CustomApi.java

package com.android.customApi;
import android.content.Context;
import android.content.res.Configuration;
import android.os.RemoteException;
import android.os.ServiceManager;
import android.content.Intent;
import android.util.Log;
import android.app.ICustomManager;

public class CustomApi
{
    private static final String TAG = "CustomApi";

    private final ICustomManager mService;

    public CustomApi()
    {
    		Log.d(TAG, "enter CustomManager");
        mService = ICustomManager.Stub.asInterface(ServiceManager.getService(Context.CUSTOM_SERVICE));
    }

    public void CustomSystemDate(String mtime){
    	 try
        {
             mService.CustomSystemDate(mtime);
        }
             catch (RemoteException ex) {
        }
    }
}
```
``` makefile
frameworks/base/customApi/Android.mk

LOCAL_PATH:= $(call my-dir)

# the library
# ============================================================
include $(CLEAR_VARS)

LOCAL_SRC_FILES := $(call all-java-files-under, java)

LOCAL_MODULE := android.customApi

LOCAL_JACK_ENABLED := disabled

include $(BUILD_STATIC_JAVA_LIBRARY)
include $(CLEAR_VARS)
include $(BUILD_MULTI_PREBUILT)

# additionally, build unit tests in a separate .apk
include $(call all-makefiles-under,$(LOCAL_PATH))
```
####11. 添加到系统编译规则
``` makefile
build/make/core/pathmap.mk
FRAMEWORKS_BASE_SUBDIRS := \
            lowpan \
            keystore \
            rs \
+           customApi \
         )
#=======================================================
build/make/target/product/base.mk
PRODUCT_PACKAGES += \
     telecom \
     vdc \
     vold \
     wm \
+    android.customApi
#=======================================================
PRODUCT_PACKAGES += \
     audio.primary.default \
     local_time.default \
     vibrator.default \
     power.default \
+    android.customApi
```
####12. 编译jar包测试
``` shell
#编译方法：
mmm frameworks/base/customApi/
#生成的jar包路径：
out\target\common\obj\JAVA_LIBRARIES\android.customApi_intermediates\classes.jar
```
    Android Studio 导入jar包
    1. 把工程切换到Project视图下
    2. 把jar拷贝到libs目录
    3. 右键--> Add As Library，Create Library默认选app
    4. 测试jar包功能
``` java
package com.eric.customapi;

import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import com.android.customApi.CustomApi;

public class MainActivity extends AppCompatActivity {

    CustomApi customApi;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        customApi = new CustomApi();
        customApi.CustomSystemDate("test");
    }
}
```
最后service输出打印，添加成功。
