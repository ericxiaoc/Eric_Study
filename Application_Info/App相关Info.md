###获取Android所有应用的包名，并进行过滤；
```
Intent appintent = new Intent(Intent.ACTION_MAIN, null);
appintent.addCategory(Intent.CATEGORY_LAUNCHER);
List<ResolveInfo> apps = mContext.getPackageManager().queryIntentActivities(appintent, 0);
for (int i = 0; i < apps.size(); i++) {
    ResolveInfo info = apps.get(i);
    String pkName = info.activityInfo.packageName;
    String cls = info.activityInfo.name;
    if (pkName.equals("com.szyh.lottteryhtd") &&
        cls.equals("com.szyh.lottteryhtd.ui.activity.SplashActivity")) {
        Slog.i(TAG,"com.szyh.lottteryhtd/.ui.activity.SplashActivity");
        packageName = "com.szyh.lottteryhtd";
        className = "com.szyh.lottteryhtd.ui.activity.SplashActivity";
        break;
	  }
}
```
