###MTK Android 8.1 Camera加载流程
#####HAL加载Camera流程
* 首先在imgsensor_drv.cpp中会获取Camera的list，以及设置Camera的主副属性。
``` c
/vendor/mediatek/proprietary/hardware/mtkcam/legacy/platform/mt8163/hal/sensor/imgsensor_drv.cpp

MINT32
ImgSensorDrv::impSearchSensor(pfExIdChk pExIdChkCbf)
{
    MUINT32 SensorEnum = (MUINT32) DUAL_CAMERA_MAIN_SENSOR;
    MUINT32 i,id[KDIMGSENSOR_MAX_INVOKE_DRIVERS] = {0,0};
    MBOOL SensorConnect=TRUE;
    UCHAR cBuf[64];
    MINT32 err = SENSOR_NO_ERROR;
    MINT32 err2 = SENSOR_NO_ERROR;
    ACDK_SENSOR_INFO_STRUCT SensorInfo;
    ACDK_SENSOR_CONFIG_STRUCT SensorConfigData;
    ACDK_SENSOR_RESOLUTION_INFO_STRUCT SensorResolution;
    MINT32 sensorDevs = SENSOR_NONE;
    IMAGE_SENSOR_TYPE sensorType = IMAGE_SENSOR_TYPE_UNKNOWN;
    IMGSENSOR_SOCKET_POSITION_ENUM socketPos = IMGSENSOR_SOCKET_POS_NONE;


    //! If imp sensor search process already done before,
    //! only need to return the sensorDevs, not need to
    //! search again.
    if (SENSOR_DOES_NOT_EXIST != m_mainSensorId) {
        //been processed.
        LOG_MSG("[impSearchSensor] Already processed \n");
        if (BAD_SENSOR_INDEX != m_mainSensorIdx) {
            sensorDevs |= SENSOR_MAIN;
        }
        if (BAD_SENSOR_INDEX != m_main2SensorIdx) {
            sensorDevs |= SENSOR_MAIN_2;
        }
        if (BAD_SENSOR_INDEX != m_subSensorIdx) {
            sensorDevs |= SENSOR_SUB;
        }

        #ifdef  ATVCHIP_MTK_ENABLE

            sensorDevs |= SENSOR_ATV;

        #endif


        return sensorDevs;
    }

    GetSensorInitFuncList(&m_pstSensorInitFunc);

    LOG_MSG("SENSOR search start \n");

    if (-1 != m_fdSensor) {
        ::close(m_fdSensor);
        m_fdSensor = -1;
    }
    sprintf(cBuf,"/dev/%s",CAMERA_HW_DEVNAME);
    m_fdSensor = ::open(cBuf, O_RDWR);
    if (m_fdSensor < 0) {
         LOG_ERR("[impSearchSensor]: error opening %s: %s \n", cBuf, strerror(errno));
        return sensorDevs;
    }

    // search main/main_2/sub 3 sockets
#ifdef MTK_MAIN2_IMGSENSOR
    for (SensorEnum = DUAL_CAMERA_MAIN_SENSOR; SensorEnum <= DUAL_CAMERA_MAIN_2_SENSOR; SensorEnum <<= 1)  {
        LOG_MSG("impSearchSensor search to main_2\n");
#else
   #ifdef MTK_SUB_IMGSENSOR
    for (SensorEnum = DUAL_CAMERA_MAIN_SENSOR; SensorEnum <= DUAL_CAMERA_SUB_SENSOR; SensorEnum <<= 1)  {
        LOG_MSG("impSearchSensor search to sub\n");
   #else
    for (SensorEnum = DUAL_CAMERA_MAIN_SENSOR; SensorEnum < DUAL_CAMERA_SUB_SENSOR; SensorEnum <<= 1)  {
        LOG_MSG("impSearchSensor search to main\n");
   #endif
#endif


        //
        for (i = 0; i < MAX_NUM_OF_SUPPORT_SENSOR; i++) {
            //end of driver list
            if (m_pstSensorInitFunc[i].getCameraDefault == NULL) {
                LOG_MSG("m_pstSensorInitFunc[i].getCameraDefault is NULL: %d \n", i);
                break;
            }
                //set sensor driver
            id[KDIMGSENSOR_INVOKE_DRIVER_0] = (SensorEnum << KDIMGSENSOR_DUAL_SHIFT) | i;
            LOG_MSG("set sensor driver id =%x\n", id[KDIMGSENSOR_INVOKE_DRIVER_0]);
            err = ioctl(m_fdSensor, KDIMGSENSORIOC_X_SET_DRIVER,&id[KDIMGSENSOR_INVOKE_DRIVER_0] );
                if (err < 0) {
                    LOG_ERR("ERROR:KDCAMERAHWIOC_X_SET_DRIVER\n");
                }


                //err = open();
                err = ioctl(m_fdSensor, KDIMGSENSORIOC_T_CHECK_IS_ALIVE);


                if (err < 0) {
                    LOG_MSG("[impSearchSensor] Err-ctrlCode (%s) \n", strerror(errno));
                }
            //
            sensorType = this->getCurrentSensorType((SENSOR_DEV_ENUM)SensorEnum);
            //
            socketPos = this->getSocketPosition((CAMERA_DUAL_CAMERA_SENSOR_ENUM)SensorEnum);
                //check extra ID , from EEPROM maybe
                //may need to keep power here
                if (NULL != pExIdChkCbf) {
                    err2 = pExIdChkCbf();
                    if (err2 < 0) {
                        LOG_ERR("Error:pExIdChkCbf() \n");
                    }
                }

                //power off sensor
                //close(SensorEnum);//ToDo: Check if necessary

                if (err < 0 || err2 < 0) {
                    LOG_MSG("sensor ID mismatch\n");
                }
                else {
                    if (SensorEnum == DUAL_CAMERA_MAIN_SENSOR) {
                //m_mainSensorIdx = i;
                //m_mainSensorId = m_pstSensorInitFunc[m_mainSensorIdx].SensorId;
                m_mainSensorDrv.index[m_mainSensorDrv.number] = i;
                m_mainSensorDrv.type[m_mainSensorDrv.number] = sensorType;
                if ( IMAGE_SENSOR_TYPE_RAW == sensorType && BAD_SENSOR_INDEX == m_mainSensorDrv.firstRawIndex ) {
                    m_mainSensorDrv.firstRawIndex = i;
                }
                else if ( IMAGE_SENSOR_TYPE_YUV == sensorType && BAD_SENSOR_INDEX == m_mainSensorDrv.firstYuvIndex ) {
                    m_mainSensorDrv.firstYuvIndex = i;
                }
                m_mainSensorDrv.position = socketPos;
                m_mainSensorDrv.sensorID = m_pstSensorInitFunc[m_mainSensorDrv.index[m_mainSensorDrv.number]].SensorId;
                // LOG_MSG("MAIN sensor m_mainSensorDrv.number=%d, m_mainSensorDrv.index=%d\n",m_mainSensorDrv.number,m_mainSensorDrv.index[m_mainSensorDrv.number]);
                m_mainSensorDrv.number++;
                //
                m_pMainSensorInfo = m_pstSensorInitFunc[i].pSensorInfo;
                if  ( m_pMainSensorInfo )
                {
                    NSFeature::SensorInfoBase* pSensorInfo = m_pstSensorInitFunc[i].pSensorInfo;
                    LOG_MSG("found <%#x/%s/%s>", pSensorInfo->GetID(), pSensorInfo->getDrvName(), pSensorInfo->getDrvMacroName());
                }
                else
                {
                    LOG_WRN("m_pMainSensorInfo==NULL\n");
                }
                LOG_MSG("MAIN sensor found:[%d]/[0x%x]/[%d]/[%d] \n",i,id[KDIMGSENSOR_INVOKE_DRIVER_0],sensorType,socketPos);
                //break;
            }
            else if (SensorEnum == DUAL_CAMERA_MAIN_2_SENSOR) {
                //m_main2SensorIdx = i;
                //m_main2SensorId = m_pstSensorInitFunc[m_main2SensorIdx].SensorId;

                m_main2SensorDrv.index[m_main2SensorDrv.number] = i;
                m_main2SensorDrv.type[m_main2SensorDrv.number] = sensorType;
                if ( IMAGE_SENSOR_TYPE_RAW == sensorType && BAD_SENSOR_INDEX == m_main2SensorDrv.firstRawIndex ) {
                    m_main2SensorDrv.firstRawIndex = i;
                }
                else if ( IMAGE_SENSOR_TYPE_YUV == sensorType && BAD_SENSOR_INDEX == m_main2SensorDrv.firstYuvIndex ) {
                    m_main2SensorDrv.firstYuvIndex = i;
                }
                m_main2SensorDrv.position = socketPos;
                m_main2SensorDrv.sensorID = m_pstSensorInitFunc[m_main2SensorDrv.index[m_main2SensorDrv.number]].SensorId;
                //LOG_MSG("MAIN2 sensor m_main2SensorDrv.number=%d, m_main2SensorDrv.index=%d\n",m_main2SensorDrv.number,m_main2SensorDrv.index[m_main2SensorDrv.number]);
                m_main2SensorDrv.number++;
                //
                m_pMain2SensorInfo = m_pstSensorInitFunc[i].pSensorInfo;
                if  ( m_pMain2SensorInfo )
                {
                    NSFeature::SensorInfoBase* pSensorInfo = m_pstSensorInitFunc[i].pSensorInfo;
                    LOG_MSG("found <%#x/%s/%s>", pSensorInfo->GetID(), pSensorInfo->getDrvName(), pSensorInfo->getDrvMacroName());
                }
                else
                {
                    LOG_WRN("m_pMain2SensorInfo==NULL\n");
                }
                LOG_MSG("MAIN_2 sensor found:[%d]/[0x%x]/[%d]/[%d] \n",i,id[KDIMGSENSOR_INVOKE_DRIVER_0],sensorType,socketPos);
            }
            else if (SensorEnum == DUAL_CAMERA_SUB_SENSOR) {
                //m_subSensorIdx = i;
                //m_subSensorId = m_pstSensorInitFunc[m_subSensorIdx].SensorId;
                m_subSensorDrv.index[m_subSensorDrv.number] = i;
                m_subSensorDrv.type[m_subSensorDrv.number] = sensorType;
                if ( IMAGE_SENSOR_TYPE_RAW == sensorType && BAD_SENSOR_INDEX == m_subSensorDrv.firstRawIndex ) {
                    m_subSensorDrv.firstRawIndex = i;
                }
                else if ( IMAGE_SENSOR_TYPE_YUV == sensorType && BAD_SENSOR_INDEX == m_subSensorDrv.firstYuvIndex ) {
                    m_subSensorDrv.firstYuvIndex = i;
                }
                m_subSensorDrv.position = socketPos;
                m_subSensorDrv.sensorID = m_pstSensorInitFunc[m_subSensorDrv.index[m_subSensorDrv.number]].SensorId;
                //LOG_MSG("SUB sensor m_subSensorDrv.number=%d, m_subSensorDrv.index=%d\n",m_subSensorDrv.number,m_subSensorDrv.index[m_subSensorDrv.number]);
                m_subSensorDrv.number++;
                //
                m_pSubSensorInfo = m_pstSensorInitFunc[i].pSensorInfo;
                if  ( m_pSubSensorInfo )
                {
                    NSFeature::SensorInfoBase* pSensorInfo = m_pstSensorInitFunc[i].pSensorInfo;
                    LOG_MSG("found <%#x/%s/%s>", pSensorInfo->GetID(), pSensorInfo->getDrvName(), pSensorInfo->getDrvMacroName());
                }
                else
                {
                    LOG_WRN("m_pSubSensorInfo==NULL\n");
                }
                LOG_MSG("SUB sensor found:[%d]/[0x%x]/[%d]/[%d] \n",i,id[KDIMGSENSOR_INVOKE_DRIVER_0],sensorType,socketPos);
                //break;
            }
        }//

        }
    }
    //close system call may be off sensor power. check first!!!
    if(m_fdSensor >= 0)
    {
        ::close(m_fdSensor);
        m_fdSensor = -1;
    }

    //
    if (BAD_SENSOR_INDEX != m_mainSensorDrv.index[0]) {
        m_mainSensorId = m_mainSensorDrv.sensorID;
        //init to choose first
        m_mainSensorIdx = m_mainSensorDrv.index[0];
        sensorDevs |= SENSOR_MAIN;
    }
    if (BAD_SENSOR_INDEX != m_main2SensorDrv.index[0]) {
        m_main2SensorId = m_main2SensorDrv.sensorID;
        //init to choose first
        m_main2SensorIdx = m_main2SensorDrv.index[0];
        sensorDevs |= SENSOR_MAIN_2;
    }
    if (BAD_SENSOR_INDEX != m_subSensorDrv.index[0]) {
        m_subSensorId = m_subSensorDrv.sensorID;
        //init to choose first
        m_subSensorIdx = m_subSensorDrv.index[0];
        sensorDevs |= SENSOR_SUB;
    }

    #ifdef  ATVCHIP_MTK_ENABLE

        sensorDevs |= SENSOR_ATV;

    #endif


    if (sensorDevs == SENSOR_NONE) {
        LOG_ERR( "Error No sensor found!! \n");
    }
    //
    LOG_MSG("SENSOR search end: 0x%x /[0x%x][%d]/[0x%x][%d]/[0x%x][%d]\n", sensorDevs,
    m_mainSensorId,m_mainSensorIdx,m_main2SensorId,m_main2SensorIdx,m_subSensorId,m_subSensorIdx);

    return sensorDevs;
}//
```
#####kernel加载Camera流程
