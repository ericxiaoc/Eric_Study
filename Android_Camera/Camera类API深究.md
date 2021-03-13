#Camera相关的API
###平台配置
* ubuntu 16.04 服务器
* RK3288 Android 7.1
###Camera类API
####Camera.open()；
    默认打开cameraId为0的摄像头，适用于单个摄像头调用。
####Camera.open(cameraId);
        打开指定cameraId的摄像头，cameraId可等于0，1, 2，....Android系统默认
    仅支持两路USB摄像头，可修改HAL层支持，理论上USB带宽足够，支持的个数无上限。
####Camera.autoFocus(cb);
    设置Camera自动对焦回调，cb为AutoFocusCallback回调函数。
####Camera.cancelAutoFocus();
    取消Camera自动对焦。
###Camera.startPreview();
    开始Camera预览。
###mCamera.stopPreview();
    停止Camera预览，一般在退出或后台，或者方向改变的时候使用。
###Camera.startSmoothZoom(value);
    设置Camera的焦距，value值(0 ~ Camera.Parameters#getMaxZoom)
###Camera.stopSmoothZoom();
    停止Camera变焦缩放。
###Camera.takePicture(shutter, raw, jpeg);
    
###Camera.takePicture(shutter, raw, postview, jpeg);
###Camera.getCameraInfo(cameraId, cameraInfo);
###Camera.getNumberOfCameras();
###Camera.wait();
###Camera.wait(millis);
###Camera.wait(millis, nanos);
###Camera.enableShutterSound(enabled)
###Camera.getParameters()
###Camera.reconnect();
###Camera.release();
###Camera.setAutoFocusMoveCallback(cb);
###Camera.setDisplayOrientation(degrees);
###Camera.setErrorCallback(cb);
###Camera.setFaceDetectionListener(listener);
###Camera.setOneShotPreviewCallback(cb);
###Camera.setParameters(params);
###Camera.setPreviewCallback(cb);
###Camera.setPreviewCallbackWithBuffer(cb);
###Camera.setPreviewDisplay(holder);
###Camera.setPreviewTexture(surfaceTexture);
###Camera.setZoomChangeListener(listener);

####Camera.addCallbackBuffer(callbackBuffer);
