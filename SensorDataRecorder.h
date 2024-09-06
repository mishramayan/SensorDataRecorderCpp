#pragma once
#include <iostream>
#include <initguid.h>
#include <windows.h>
#include <conio.h>
#include <sensors.h>
#include <SensorsApi.h>
#include <PortableDevice.h>


#pragma comment(lib, "Sensorsapi.lib")
#pragma comment(lib, "PortableDeviceGuids.lib")

#define D_ISENSOR_TYPE_AMBIENT_SENSOR (0)
#define D_ISENSOR_TYPE_MAX            (D_ISENSOR_TYPE_AMBIENT_SENSOR + 1)


#define AMBIENTLIGHT_SENSORVALUE_VALUE27   (28)
#define AMBIENTLIGHT_SENSORVALUE_VALUE28   (29)
#define AMBIENTLIGHT_SENSORVALUE_LUX   (0)

class SensorDataRecoder
{
public:
    SensorDataRecoder() {
        HRESULT hr = initializeSenorPanel();
        if (FAILED(hr)) {
            isInitialized = false;
        }
        else {
            isInitialized = true;
        }
    }

    bool IsAccelerometerAvailable();
    double GetAccelerationValues();

private:
    bool isInitialized;
    ISensorManager* pSensorManager = NULL;
    ISensorCollection* pSensorCollection = NULL;
    SENSOR_ID gSensor_id;

    ISensor* gIsensor;
    IPortableDeviceKeyCollection* gIpDataFields = nullptr;

    unsigned long gLightLevel;

    HRESULT initializeSenorPanel();
    ULONG getSensorsCount();
};

