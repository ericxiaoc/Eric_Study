#Android App操作
####启动第三方app
``` java
private void ApplicationLauncher(String PackageName, String ActivityName) {
      ComponentName componentName = new ComponentName(PackageName, ActivityName);
      Intent intent = new Intent(Intent.ACTION_MAIN);
      intent.setComponent(componentName);
      intent.addCategory(Intent.CATEGORY_LAUNCHER);
      intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_RESET_TASK_IF_NEEDED);
      mContext.startActivityAsUser(intent, UserHandle.CURRENT);
 }
```
###强制结束第三方app，需要系统权限
``` java
ActivityManager am = (ActivityManager)mContext.getSystemService(Context.ACTIVITY_SERVICE);
am.forceStopPackage(pkgName);
```
