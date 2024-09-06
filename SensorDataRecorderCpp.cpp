#include "SensorDataRecorder.h"

HRESULT SensorDataRecoder::initializeSenorPanel() {
    // Step 1: Initialize the Sensor API
    HRESULT bSuccess = CoInitialize(0);

    if (FAILED(bSuccess)) {
        std::cerr << "CoInitialize failed with error code: 0x" << std::hex << bSuccess << std::endl;
        return bSuccess;
    }

    //Step 2: Enumerate sensors and count them.
    HRESULT hr = CoCreateInstance(CLSID_SensorManager, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pSensorManager));
    return hr;
}

ULONG SensorDataRecoder::getSensorsCount() {
    ULONG numSensors = 0;
    HRESULT hr = pSensorManager->GetSensorsByCategory(SENSOR_CATEGORY_ALL, &pSensorCollection);
    hr = pSensorCollection->GetCount(&numSensors);
    if (SUCCEEDED(hr) && numSensors > 0)
    {
        // The variable 'sensorCount' now holds the number of sensors available in the system.
        // You can use this value as needed.
        std::cout << "Number of sensors in the system: " << numSensors << std::endl;
    }
    return numSensors;
}
//Sensor_PersistentUniqueId	00410000-0001-0100-0000-000000000000
bool SensorDataRecoder::IsAccelerometerAvailable() {
    ULONG numSensors = getSensorsCount();

    for (ULONG i = 0; i < numSensors; i++) {
        gIsensor = nullptr;
        HRESULT hr = pSensorCollection->GetAt(i, &gIsensor);

        if (FAILED(hr)) {
            std::cout << "Failed to get Sensor data" << std::endl;
            continue;
        }

        SENSOR_ID gSensor_id;
        hr = gIsensor->GetID(&gSensor_id);
        //Accelerometer Sensor_PersistentUniqueId	00730000-0000-0000-0000-000000000000

            if ((gSensor_id.Data1 == 0x00730000) &&
                (gSensor_id.Data2 == 0x0000) &&
                (gSensor_id.Data3 == 0x0000)) {
                /*printf("Accelerometer Sensor ID: {%08lX-%04hX-%04hX-%02hhX%02hhX-%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX}\n",
                    gSensor_id.Data1, gSensor_id.Data2, gSensor_id.Data3,
                    gSensor_id.Data4[0], gSensor_id.Data4[1], gSensor_id.Data4[2], gSensor_id.Data4[3],
                    gSensor_id.Data4[4], gSensor_id.Data4[5], gSensor_id.Data4[6], gSensor_id.Data4[7]);*/
                return true;
            }
    }
    std::cout << "Failed to get Sensor data" << std::endl;
    return false;
}

double SensorDataRecoder::GetAccelerationValues() {
    ISensorDataReport* pDataReport = nullptr;
    double accelerationX = 0;
    double accelerationY = 0;
    double accelerationZ = 0;
    HRESULT hr = gIsensor->GetData(&pDataReport);
    if (SUCCEEDED(hr)) {
        PROPVARIANT propVariantAcc_X;
        PROPVARIANT propVariantAcc_Y;
        PROPVARIANT propVariantAcc_Z;

        PropVariantInit(&propVariantAcc_X);
        PropVariantInit(&propVariantAcc_Y);
        PropVariantInit(&propVariantAcc_Z);

        // Get the sensor reading from the ISensorDataReport object
        hr = pDataReport->GetSensorValue(
            SENSOR_DATA_TYPE_ACCELERATION_X_G,
            &propVariantAcc_X);

        hr = pDataReport->GetSensorValue(
            SENSOR_DATA_TYPE_ACCELERATION_Y_G,
            &propVariantAcc_Y);

        hr = pDataReport->GetSensorValue(
            SENSOR_DATA_TYPE_ACCELERATION_Z_G,
            &propVariantAcc_Z);

        if (SUCCEEDED(hr)) {
            // Print the LUX value
            //std::cout << propVariantAcc_X.vt << std::endl;
            if (propVariantAcc_X.vt == VT_R8) {
                accelerationX = propVariantAcc_X.dblVal;
                pDataReport->Release();
                PropVariantClear(&propVariantAcc_X);
            }
            if (propVariantAcc_Y.vt == VT_R8) {
                accelerationY = propVariantAcc_Y.dblVal;
                pDataReport->Release();
                PropVariantClear(&propVariantAcc_Y);
            }
            if (propVariantAcc_Z.vt == VT_R8) {
                accelerationZ = propVariantAcc_Z.dblVal;
                pDataReport->Release();
                PropVariantClear(&propVariantAcc_Z);
            }

            std::cout << "Acceleration_X: " << accelerationX << " | Acceleration_Y: " << accelerationY << " | Acceleration_Z: " << accelerationZ << std::endl;
        }

        pDataReport->Release();

    }
    return accelerationX;
}


int main() {
    SensorDataRecoder senrec;
    if (senrec.IsAccelerometerAvailable()) {
        while (true) {
            senrec.GetAccelerationValues();
        }
    }
    
}