主要编译脚本为ota_from_target_files、edify_generator.py
###检验时间戳
``` c
def AssertOlderBuild(self, timestamp, timestamp_text):
    """Assert that the build on the device is older (or the same as)
    the given timestamp."""
    self.script.append(
        ('(!less_than_int(%s, getprop("ro.build.date.utc"))) || '
         'abort("Can\'t install this package (%s) over newer '
         'build (" + getprop("ro.build.date") + ").");'
         ) % (timestamp, timestamp_text))
```
#####去掉时间戳检验：
修改如下：
``` c
#if not OPTIONS.omit_prereq:
  #ts = GetBuildProp("ro.build.date.utc", OPTIONS.info_dict)
  #ts_text = GetBuildProp("ro.build.date", OPTIONS.info_dict)
  #script.AssertOlderBuild(ts, ts_text)
```


###检验系统指纹
``` c
def AssertSomeFingerprint(self, *fp):
    """Assert that the current recovery build fingerprint is one of *fp."""
    if not fp:
      raise ValueError("must specify some fingerprints")
    cmd = (
           ' ||\n    '.join([('getprop("ro.build.fingerprint") == "%s"')
                        % i for i in fp]) +
           ' ||\n    abort("Package expects build fingerprint of %s; this '
           'device has " + getprop("ro.build.fingerprint") + ".");'
           ) % (" or ".join(fp),)
    self.script.append(cmd)
```
#####去掉系统指纹检验
修改如下：
``` c
if oem_props is None:
-    script.AssertSomeFingerprint(source_fp, target_fp)
+    #script.AssertSomeFingerprint(source_fp, target_fp)
+    print "ignore ro.build.fingerprint changed; skipping."
   else:
     script.AssertSomeThumbprint(
         GetBuildProp("ro.build.thumbprint", OPTIONS.target_info_dict),
@@ -1136,7 +1137,8 @@ def WriteIncrementalOTAPackage(target_zip, source_zip, output_zip):
   source_fp = CalculateFingerprint(oem_props, oem_dict, OPTIONS.source_info_dict)

   if oem_props is None:
-    script.AssertSomeFingerprint(source_fp, target_fp)
+    #script.AssertSomeFingerprint(source_fp, target_fp)
+    print "ignore ro.build.fingerprint changed; skipping."
   else:
     script.AssertSomeThumbprint(
         GetBuildProp("ro.build.thumbprint", OPTIONS.target_info_dict),
```
