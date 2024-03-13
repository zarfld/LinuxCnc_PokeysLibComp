/*

Copyright (C) 2013 Matevž Bošnak (matevz@poscope.com)

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA 

*/

#include <stdlib.h>
#include "PoKeysLib.h"
#include "PoKeysLibCore.h"
#include "PoKeysLibCoreSockets.h"
#include "string.h"
#include "stdio.h"
//#define PK_COM_DEBUG

int32_t PKI_CheckInterface(struct hid_device_info * devInfo)
{
    if (devInfo->interface_number == 1)
        return 1;

    return 0;
}

// Connection specific commands
int32_t PK_EnumerateUSBDevices()
{
    int32_t numDevices = 0;
    struct hid_device_info *devs, *cur_dev;

    devs = hid_enumerate(0x1DC3, 0x1001);
    cur_dev = devs;

    while (cur_dev)
    {
        /*printf("Device Found\n");
        printf("  Serial:      %ls\n", cur_dev->serial_number);
        printf("  Product:      %ls\n", cur_dev->product_string);
        printf("  Interface:    %d\n",  cur_dev->interface_number);
        printf("\n");*/

        if (PKI_CheckInterface(cur_dev))
        {
            numDevices++;
        }

        cur_dev = cur_dev->next;
    }
    hid_free_enumeration(devs);

    devs = hid_enumerate(0x1DC3, 0x1002);
    cur_dev = devs;

    while (cur_dev)
    {
        /*printf("Device Found\n");
        printf("  Serial:      %ls\n", cur_dev->serial_number);
        printf("  Product:      %ls\n", cur_dev->product_string);
        printf("  Interface:    %d\n",  cur_dev->interface_number);
        printf("\n");*/
        if (cur_dev->interface_number == -1) numDevices++;
        cur_dev = cur_dev->next;
    }
    hid_free_enumeration(devs);

#ifdef POKEYSLIB_USE_LIBUSB
	numDevices += PK_EnumerateFastUSBDevices();
#endif
    return numDevices;
}

int32_t PK_GetCurrentDeviceConnectionType(sPoKeysDevice* device)
{
	return device->connectionType;
}

void InitializeNewDevice(sPoKeysDevice* device)
{
    uint32_t i;    
	memset(&device->info, 0, sizeof(sPoKeysDevice_Info));
    memset(&device->DeviceData, 0, sizeof(sPoKeysDevice_Data));

    device->netDeviceData = 0;

	memset(&device->matrixKB, 0, sizeof(sMatrixKeyboard));
	memset(&device->PWM, 0, sizeof(sPoKeysPWM));
	memset(&device->LCD, 0, sizeof(sPoKeysLCD));
	

	device->FastEncodersConfiguration = 0;
	device->FastEncodersOptions = 0;
	device->UltraFastEncoderConfiguration = 0;
	device->UltraFastEncoderOptions = 0;

	device->UltraFastEncoderFilter = 0;

	memset(device->request, 0, 64);
	memset(device->response, 0, 64);

	device->sendRetries = 3;
    device->readRetries = 10;
    device->socketTimeout = 100;

    PK_DeviceDataGet(device);

    device->Pins = (sPoKeysPinData*)malloc(sizeof(sPoKeysPinData) * device->info.iPinCount);
	memset(device->Pins, 0, sizeof(sPoKeysPinData) * device->info.iPinCount);


	for (i = 0; i < device->info.iPinCount; i++)
	{
		if (PK_IsCounterAvailable(device, i))
		{
			device->Pins[i].DigitalCounterAvailable = 1;
		} else
		{
			device->Pins[i].DigitalCounterAvailable = 0;
		}
	}

	device->Encoders = (sPoKeysEncoder*)malloc(sizeof(sPoKeysEncoder) * device->info.iEncodersCount);
	memset(device->Encoders, 0, sizeof(sPoKeysEncoder) * device->info.iEncodersCount);

    if (device->info.iEasySensors)
    {
        device->EasySensors = (sPoKeysEasySensor*)malloc(sizeof(sPoKeysEasySensor) * device->info.iEasySensors);
        memset(device->EasySensors, 0, sizeof(sPoKeysEasySensor) * device->info.iEasySensors);
    } else
    {
        device->EasySensors = NULL;
    }

	device->PWM.PWMduty = (uint32_t*)malloc(sizeof(uint32_t) * device->info.iPWMCount);
	memset(device->PWM.PWMduty, 0, sizeof(uint32_t) * device->info.iPWMCount);

	if (device->info.iPWMCount > 0)
	{
		device->PWM.PWMenabledChannels = (unsigned char*)malloc(sizeof(unsigned char) * device->info.iPWMCount);
		memset(device->PWM.PWMenabledChannels, 0, sizeof(unsigned char) * device->info.iPWMCount);
	}
	else
	{
		device->PWM.PWMenabledChannels = NULL;
	}
	device->PWM.PWMpinIDs = (unsigned char*)malloc(sizeof(unsigned char) * device->info.iPWMCount);
	memset(device->PWM.PWMpinIDs, 0, sizeof(unsigned char) * device->info.iPWMCount);

	PK_FillPWMPinNumbers(device);

	device->PoExtBusData = (unsigned char*)malloc(sizeof(unsigned char) * device->info.iPoExtBus);

    device->MatrixLED = (sPoKeysMatrixLED*)malloc(sizeof(sPoKeysMatrixLED) * device->info.iMatrixLED);
	memset(device->MatrixLED, 0, sizeof(sPoKeysMatrixLED) * device->info.iMatrixLED);

    memset(&device->PEv2, 0, sizeof(sPoKeysPEv2));

    device-> multiPartBuffer = malloc(512);
    if (device->multiPartBuffer <= 0) device->multiPartBuffer = 0;

#ifdef USE_ALIGN_TEST
    device->alignTest1 = 1;
    device->alignTest2 = 2;
    device->alignTest3 = 3;
    device->alignTest4 = 4;
    device->alignTest5 = 5;
    device->alignTest6 = 6;
    device->alignTest7 = 7;
    device->alignTest8 = 8;
    device->alignTest9 = 9;
    device->alignTest10 = 10;
    device->alignTest11 = 11;
#endif
}

void CleanDevice(sPoKeysDevice* device)
{    
	free(device->Pins);
	device->Pins = NULL;
	free(device->Encoders);
	device->Encoders = NULL;
	free(device->PWM.PWMduty);
	device->PWM.PWMduty = NULL;
	free(device->PWM.PWMenabledChannels);
	device->PWM.PWMenabledChannels = NULL;
	free(device->PWM.PWMpinIDs);
	device->PWM.PWMpinIDs = NULL;
	free(device->PoExtBusData);
	device->PoExtBusData = NULL;
	free(device->MatrixLED);
	device->MatrixLED = NULL;

    if (device->multiPartBuffer != NULL)
    {
        free(device->multiPartBuffer);
        device->multiPartBuffer = NULL;
    }

    if (device->EasySensors != NULL)
    {
        free(device->EasySensors);
        device->EasySensors = NULL;
    }

    if (device->netDeviceData != NULL)
    {
        free(device->netDeviceData);
        device->netDeviceData = NULL;
    }
}

void PK_ReleaseDeviceStructure(sPoKeysDevice* device)
{
    CleanDevice(device);
}

void PK_CloneDeviceStructure(sPoKeysDevice* original, sPoKeysDevice *destination)
{
    // Reserve memory...
    destination->Pins = (sPoKeysPinData*)malloc(sizeof(sPoKeysPinData) * original->info.iPinCount);
    destination->Encoders = (sPoKeysEncoder*)malloc(sizeof(sPoKeysEncoder) * original->info.iEncodersCount);
    destination->PWM.PWMduty = (uint32_t*)malloc(sizeof(uint32_t) * original->info.iPWMCount);
    destination->PWM.PWMenabledChannels = (unsigned char*)malloc(sizeof(unsigned char) * original->info.iPWMCount);
	if (original->info.iPWMCount == 0) destination->PWM.PWMenabledChannels = NULL;
    destination->PWM.PWMpinIDs = (unsigned char*)malloc(sizeof(unsigned char) * original->info.iPWMCount);
    destination->MatrixLED = (sPoKeysMatrixLED*)malloc(sizeof(sPoKeysMatrixLED) * original->info.iMatrixLED);

    if (original->info.iEasySensors)
    {
        destination->EasySensors = (sPoKeysEasySensor*)malloc(sizeof(sPoKeysEasySensor) * original->info.iEasySensors);
    } else
    {
        destination->EasySensors = 0;
    }

    // Network device information structure...
    if (original->netDeviceData != 0)
    {
        destination->netDeviceData = (sPoKeysNetworkDeviceInfo *)malloc(sizeof(sPoKeysNetworkDeviceInfo));
        memcpy(destination->netDeviceData, original->netDeviceData, sizeof(sPoKeysNetworkDeviceInfo));
    } else
    {
        destination->netDeviceData = 0;
    }
    destination->PoExtBusData = (unsigned char*)malloc(sizeof(unsigned char) * original->info.iPoExtBus);


    // Copy data
    destination->devHandle = original->devHandle;
    destination->devHandle2 = original->devHandle2;

    destination->info = original->info;
    destination->DeviceData = original->DeviceData;

    memcpy(&destination->Pins[0], &original->Pins[0],
            original->info.iPinCount * sizeof(sPoKeysPinData));
    memcpy(&destination->Encoders[0], &original->Encoders[0],
            original->info.iEncodersCount * sizeof(sPoKeysEncoder));


    if (original->info.iEasySensors)
    {
        memcpy(&destination->EasySensors[0], &original->EasySensors[0],
                original->info.iEasySensors * sizeof(sPoKeysEasySensor));
    }

    destination->matrixKB = original->matrixKB;

    destination->PWM.PWMperiod = original->PWM.PWMperiod;
    memcpy(destination->PWM.PWMduty, original->PWM.PWMduty,
           sizeof(uint32_t) * original->info.iPWMCount);
    memcpy(destination->PWM.PWMenabledChannels, original->PWM.PWMenabledChannels,
           sizeof(unsigned char) * original->info.iPWMCount);
	memcpy(destination->PWM.PWMpinIDs, original->PWM.PWMpinIDs,
           sizeof(unsigned char) * original->info.iPWMCount);

    memcpy(destination->MatrixLED, original->MatrixLED,
           sizeof(sPoKeysMatrixLED) * original->info.iMatrixLED);

    destination->LCD = original->LCD;

    destination->PoNETmodule = original->PoNETmodule;
    destination->PoIL = original->PoIL;
    destination->RTC = original->RTC;

    destination->FastEncodersConfiguration =    original->FastEncodersConfiguration;
    destination->FastEncodersOptions =          original->FastEncodersOptions;
    destination->UltraFastEncoderConfiguration =original->UltraFastEncoderConfiguration;
    destination->UltraFastEncoderOptions =      original->UltraFastEncoderOptions;
    destination->UltraFastEncoderFilter =       original->UltraFastEncoderFilter;

    memcpy(destination->PoExtBusData, original->PoExtBusData, sizeof(unsigned char) * original->info.iPoExtBus);

    destination->connectionType = original->connectionType;
    destination->requestID = original->requestID;

}

void * PK_FastUSBConnectToDevice(uint32_t deviceIndex);

sPoKeysDevice* PK_ConnectToDevice(uint32_t deviceIndex)
{
    int32_t numDevices = 0;
    struct hid_device_info *devs, *cur_dev;
    sPoKeysDevice* tmpDevice;
    void * devData;

    devs = hid_enumerate(0x1DC3, 0x1001);
    cur_dev = devs;

    while (cur_dev)
    {
        if (cur_dev->interface_number == 1)
        {
            if (numDevices == deviceIndex)
            {
				tmpDevice = (sPoKeysDevice*)malloc(sizeof(sPoKeysDevice));

                //printf("Connect to this device...");
				tmpDevice->devHandle = (void*)hid_open_path(cur_dev->path);
                tmpDevice->devHandle2 = NULL;

				tmpDevice->connectionType = PK_DeviceType_USBDevice;


				if (tmpDevice->devHandle != NULL)
				{
                    InitializeNewDevice(tmpDevice);
				} else
				{
					free(tmpDevice);
					tmpDevice = NULL;
				}
                //hid_set_nonblocking(devHandle);
                hid_free_enumeration(devs);
                return tmpDevice;
            }
            numDevices++;
        }
        cur_dev = cur_dev->next;
    }
    hid_free_enumeration(devs);

    // Continue with 0x1002 devices
    devs = hid_enumerate(0x1DC3, 0x1002);
    cur_dev = devs;

    while (cur_dev)
    {
        if (cur_dev->interface_number == -1)
        {
            if (numDevices == deviceIndex)
            {
                tmpDevice = (sPoKeysDevice*)malloc(sizeof(sPoKeysDevice));

                //printf("Connect to this device...");
                tmpDevice->devHandle = (void*)hid_open_path(cur_dev->path);
                tmpDevice->devHandle2 = NULL;

                tmpDevice->connectionType = PK_DeviceType_USBDevice;

                if (tmpDevice->devHandle != NULL)
                {
                    InitializeNewDevice(tmpDevice);
                } else
                {
                    free(tmpDevice);
                    tmpDevice = NULL;
                }
                //hid_set_nonblocking(devHandle);
                hid_free_enumeration(devs);
                return tmpDevice;
            }
            numDevices++;
        }
        cur_dev = cur_dev->next;
    }
    hid_free_enumeration(devs);

#ifdef POKEYSLIB_USE_LIBUSB
	// Try connecting to the bulk interface of the PoKeys device...
    devData = PK_FastUSBConnectToDevice(deviceIndex - numDevices);

	//void * devData = ConnectToFastUSBInterface(serialNumber);
	if (devData != NULL)
	{
		tmpDevice = (sPoKeysDevice*)malloc(sizeof(sPoKeysDevice));

		tmpDevice->devHandle = NULL;
		tmpDevice->devHandle2 = devData;

		tmpDevice->connectionType = PK_DeviceType_FastUSBDevice;
		InitializeNewDevice(tmpDevice);
		return tmpDevice;
	}
#endif
    return NULL;
}

// Flags:
// bits 7-1: deviceType specifier (2 - PoKeys56, 3 - PoKeys58, 4 - PoKeys16)
sPoKeysDevice* PK_ConnectToPoKeysDevice_USB(uint32_t serialNumber, uint32_t flags)
{
    int32_t numDevices = 0;
    struct hid_device_info *devs, *cur_dev;
    int32_t k;
    sPoKeysDevice* tmpDevice;
    uint8_t serialSearch[8];

    int devRange = 0;
    uint8_t deviceTypeRequested = (flags >> 1) & 0x7F;

#ifdef POKEYSLIB_USE_LIBUSB
	// Try connecting to fast USB interface first
	void * devData = ConnectToFastUSBInterface(serialNumber);
	if (devData != NULL)
	{
		tmpDevice = (sPoKeysDevice*)malloc(sizeof(sPoKeysDevice));

		tmpDevice->devHandle = NULL;
        tmpDevice->devHandle2 = devData;

        tmpDevice->connectionType = PK_DeviceType_FastUSBDevice;
        InitializeNewDevice(tmpDevice);	
		return tmpDevice;
	}
#endif

    devs = hid_enumerate(0x1DC3, 0x1001);
    cur_dev = devs;

    sprintf((char*)serialSearch, "x.%05u", serialNumber % 100000);
    //sprintf(serialSearch58, "3.%05u", serialNumber % 100000);

    while (cur_dev)
    {
        if ((cur_dev->interface_number == 1 && devRange == 0) ||
            (cur_dev->interface_number == 0 && devRange == 1))
        {
            if (cur_dev->serial_number != 0 && cur_dev->serial_number[0] != 'P')
            {
                // Check the serial number first
                for (k = 1; k < 8 && cur_dev->serial_number[k] != 0; k++)
                {
                    if (cur_dev->serial_number[k] != serialSearch[k]) break;
                }

                if (deviceTypeRequested == 2 && cur_dev->serial_number[0] != '2') k = 0;
                if (deviceTypeRequested == 3 && cur_dev->serial_number[0] != '3') k = 0;
                if (deviceTypeRequested == 4 && cur_dev->serial_number[0] != '4') k = 0;

                if (k == 7)
                {
                    tmpDevice = (sPoKeysDevice*)malloc(sizeof(sPoKeysDevice));

                    //printf("Connect to this device...");
                    tmpDevice->devHandle = (void*)hid_open_path(cur_dev->path);
                    tmpDevice->devHandle2 = 0;

                    tmpDevice->connectionType = PK_DeviceType_USBDevice;
                    if (tmpDevice->devHandle != NULL)
                    {
                        InitializeNewDevice(tmpDevice);
					}
					else
                    {
                        free(tmpDevice);
                        tmpDevice = NULL;
                    }
                    //hid_set_nonblocking(devHandle);
                    hid_free_enumeration(devs);
                    return tmpDevice;
                }
			}
			else
            {
                // Old, PoKeys55 device - we must to connect and read the serial number...
                tmpDevice = (sPoKeysDevice*)malloc(sizeof(sPoKeysDevice));
                tmpDevice->devHandle = (void*)hid_open_path(cur_dev->path);
                tmpDevice->devHandle2 = 0;

                if (tmpDevice->devHandle != NULL)
                {
                    InitializeNewDevice(tmpDevice);
				}
				else
                {
                    free(tmpDevice);
                    tmpDevice = NULL;
                    hid_free_enumeration(devs);
                    return NULL;
                }

                tmpDevice->connectionType = PK_DeviceType_USBDevice;
                if (tmpDevice->DeviceData.SerialNumber == serialNumber)
                {
	                hid_free_enumeration(devs);
                    return tmpDevice;
				}
				else
                {
                    CleanDevice(tmpDevice);
                    free(tmpDevice);
                }
            }

            numDevices++;
        }
        cur_dev = cur_dev->next;

        if (cur_dev == NULL)
        {
            devRange++;
            switch (devRange)
            {
                case 1:
                    hid_free_enumeration(devs);
                    devs = hid_enumerate(0x1DC3, 0x1001);
                    cur_dev = devs;
                    break;
            }
        }
    }
    hid_free_enumeration(devs);
	return NULL;
}

// Flags:
// bit 0: use UDP
sPoKeysDevice* PK_ConnectToPoKeysDevice_Ethernet(uint32_t serialNumber, uint32_t checkForNetworkDevicesAndTimeout, uint32_t flags)
{
	int32_t k;
	sPoKeysDevice* tmpDevice;
	sPoKeysNetworkDeviceSummary * devices;
	int32_t iNet;

    if (checkForNetworkDevicesAndTimeout)
    {
        devices = (sPoKeysNetworkDeviceSummary*)malloc(sizeof(sPoKeysNetworkDeviceSummary) * 16);
        iNet = PK_SearchNetworkDevices(devices, checkForNetworkDevicesAndTimeout, serialNumber);

        if (iNet > 16) iNet = 16;

        for (k = 0; k < iNet; k++)
        {
            //printf("\nNetwork device found, serial = %lu at %u.%u.%u.%u", devices[k].SerialNumber, devices[k].IPaddress[0], devices[k].IPaddress[1], devices[k].IPaddress[2], devices[k].IPaddress[3]);
            if (devices[k].SerialNumber == serialNumber)
            {
                if (flags & 1) devices[k].useUDP = 1;
                tmpDevice = PK_ConnectToNetworkDevice(&devices[k]);
                if (tmpDevice == NULL)
                {
                    //CleanDevice(tmpDevice);
                    free(tmpDevice);
                    //printf("\nProblem connecting to the device...");
				}
				else
                {
                    free(devices);
                    InitializeNewDevice(tmpDevice);
                    return tmpDevice;
                }
            }
        }
        free(devices);
    }

    return NULL;
}


// Flags:
// bit 8: force Ethernet
// bits 7-1: deviceType specifier (2 - PoKeys56, 3 - PoKeys58, 4 - PoKeys16)
// bit 0: use UDP
sPoKeysDevice* PK_ConnectToPoKeysDevice(uint32_t serialNumber, uint32_t checkForNetworkDevicesAndTimeout, uint32_t flags)
{
	sPoKeysDevice* tmpDevice = NULL;

	// If Ethernet devices are forced and scan interval is greater than 0...
	if ((flags & (1 << 8)) && checkForNetworkDevicesAndTimeout > 0)
	{
		tmpDevice = PK_ConnectToPoKeysDevice_Ethernet(serialNumber, checkForNetworkDevicesAndTimeout, flags);
	}

	// Otherwise, check USB first
	if (tmpDevice == NULL)
	{
		tmpDevice = PK_ConnectToPoKeysDevice_USB(serialNumber, flags);
	}
	// Then Ethernet
	if (tmpDevice == NULL && ((flags & (1 << 8)) == 0) && checkForNetworkDevicesAndTimeout > 0)
	{
		tmpDevice = PK_ConnectToPoKeysDevice_Ethernet(serialNumber, checkForNetworkDevicesAndTimeout, flags);
	}
	return tmpDevice;
}


sPoKeysDevice* PK_ConnectToDeviceWSerial(uint32_t serialNumber, uint32_t checkForNetworkDevicesAndTimeout)
{
    return PK_ConnectToPoKeysDevice(serialNumber, checkForNetworkDevicesAndTimeout, 0);
}

sPoKeysDevice* PK_ConnectToDeviceWSerial_UDP(uint32_t serialNumber, uint32_t checkForNetworkDevicesAndTimeout)
{
    return PK_ConnectToPoKeysDevice(serialNumber, checkForNetworkDevicesAndTimeout, 1);
}

void PK_DisconnectDevice(sPoKeysDevice* device)
{
    if (device != NULL)
    {
		if (device->connectionType == PK_DeviceType_NetworkDevice) 
		{
			PK_DisconnectNetworkDevice(device);
		} else
		{
#ifdef POKEYSLIB_USE_LIBUSB
			// Disconnect the fast USB interface...
			DisconnectFromFastUSBInterface(device->devHandle2);
			device->devHandle2 = NULL;
#endif

			if ((hid_device*)device->devHandle != NULL)
			{
				hid_close((hid_device*)device->devHandle);
			}
		}

		CleanDevice(device);
		free(device);
    }
}

int32_t CreateRequest(unsigned char * request, unsigned char type, unsigned char param1, unsigned char param2, unsigned char param3, unsigned char param4)
{
    if (request == NULL) return PK_ERR_NOT_CONNECTED;

    memset(request, 0, 64);

    request[1] = type;
    request[2] = param1;
    request[3] = param2;
    request[4] = param3;
    request[5] = param4;

    return PK_OK;
}

int32_t PK_CustomRequest(sPoKeysDevice* device, unsigned char type, unsigned char param1, unsigned char param2, unsigned char param3, unsigned char param4)
{
    device->request[1] = type;
    device->request[2] = param1;
    device->request[3] = param2;
    device->request[4] = param3;
    device->request[5] = param4;

    return SendRequest(device);
}

uint8_t getChecksum(uint8_t * data)
{
    uint8_t temp = 0;
    uint32_t i;

    for (i = 0; i < 7; i++)
    {
            temp += data[i];
    }
    return temp;
}

int32_t LastRetryCount = 0;
int32_t LastWaitCount = 0;


//#define PK_COM_DEBUG
int32_t SendRequest_multiPart(sPoKeysDevice* device)
{
    if (device == NULL) return PK_ERR_GENERIC;
	if (device->connectionType == PK_DeviceType_NetworkDevice)
	{
        return SendEthRequestBig(device);
		//return SendEthRequest(device);
        //return PK_ERR_TRANSFER;
	}

#ifdef POKEYSLIB_USE_LIBUSB
	if (device->connectionType == PK_DeviceType_FastUSBDevice)
		return SendRequestFastUSB_multiPart(device);
	else
		return PK_ERR_TRANSFER;
#else
    return PK_ERR_TRANSFER;
#endif
}


//#define PK_COM_DEBUG
int32_t SendRequest(sPoKeysDevice* device)
{
    // Initialize variables
    uint32_t waits = 0;
    uint32_t retries = 0;
    int32_t result = 0;
    uint8_t bufferOut[65] = {0};

    #ifdef PK_COM_DEBUG
        int i;
    #endif

    hid_device * devHandle;



    if (device == NULL) return PK_ERR_GENERIC;
	if (device->connectionType == PK_DeviceType_NetworkDevice)
	{
		return SendEthRequest(device);
	}
#ifdef POKEYSLIB_USE_LIBUSB
		if (device->connectionType == PK_DeviceType_FastUSBDevice)
			return SendRequestFastUSB(device);
#endif
    devHandle = (hid_device*)device->devHandle;


    if (devHandle == NULL) return PK_ERR_GENERIC;

    // Request sending loop
    while (retries++ < 2)
    {
        device->request[0] = 0xBB;
        device->request[6] = ++device->requestID;
        device->request[7] = getChecksum(device->request);

        memcpy(bufferOut + 1, device->request, 64);

        #ifdef PK_COM_DEBUG
                printf("\n * SEND ");
                for (i = 0; i < 10; i++)
                {
                        printf("%X ", bufferOut[i]);
                }
        #endif

        result = hid_write(devHandle, bufferOut, 65);

        // In case of an error, try sending again
        if (result < 0)
        {
            //printf(" ERR %u", result);
            retries++;
            continue;
        }

        waits = 0;

        // Request receiving loop
        while (waits++ < 50)
        {
            result = hid_read(devHandle, device->response, 65);

            // Error is not an option
            if (result < 0)
            {
                    //printf(" Receive ERR %u", result);
                    break;
            }

#ifdef PK_COM_DEBUG
        printf("\n * RECV ");
        for (i = 0; i < 10; i++)
        {
                printf("%X ", device->response[i]);
        }

        printf(" (request ID: %X ?= %X", device->response[6], device->requestID);
#endif

                // Check the header and the request ID
                if (device->response[0] == 0xAA && device->response[6] == device->requestID)
                {
                    if (device->response[7] == getChecksum(device->response))
                    {
                        LastRetryCount = retries;
                        LastWaitCount = waits;
                        // This is it. Return from this function
                        return PK_OK;
                    }
                }
            }
    }

    return PK_ERR_TRANSFER;
}

//#define PK_COM_DEBUG
int32_t SendRequest_NoResponse(sPoKeysDevice* device)
{
    // Initialize variables
    uint32_t waits = 0;
    uint32_t retries = 0;
    int32_t result = 0;
    uint8_t bufferOut[65] = {0};

    #ifdef PK_COM_DEBUG
        int i;
    #endif

    hid_device * devHandle;



    if (device == NULL) return PK_ERR_GENERIC;
	if (device->connectionType == PK_DeviceType_NetworkDevice)
	{
		return SendEthRequest_NoResponse(device);
	}
#ifdef POKEYSLIB_USE_LIBUSB
		if (device->connectionType == PK_DeviceType_FastUSBDevice)
			return SendRequestFastUSB_NoResponse(device);
#endif
    devHandle = (hid_device*)device->devHandle;


    if (devHandle == NULL) return PK_ERR_GENERIC;

    // Request sending loop
    while (retries++ < 2)
    {
        device->request[0] = 0xBB;
        device->request[6] = ++device->requestID;
        device->request[7] = getChecksum(device->request);

        memcpy(bufferOut + 1, device->request, 64);

        #ifdef PK_COM_DEBUG
                printf("\n * SEND ");
                for (i = 0; i < 10; i++)
                {
                        printf("%X ", bufferOut[i]);
                }
        #endif

        result = hid_write(devHandle, bufferOut, 65);

        // In case of an error, try sending again
        if (result < 0)
        {
            //printf(" ERR %u", result);
            retries++;
            continue;
        }
    }

    return PK_ERR_TRANSFER;
}



// ******************************************************************************************
// These functions are used to access the device data without using the structures...
// ******************************************************************************************
void PK_SL_SetPinFunction(sPoKeysDevice* device, uint8_t pin, uint8_t function)
{
    device->Pins[pin].PinFunction = function;
}

uint8_t PK_SL_GetPinFunction(sPoKeysDevice* device, uint8_t pin)
{
    return device->Pins[pin].PinFunction;
}

void PK_SL_DigitalOutputSet(sPoKeysDevice* device, uint8_t pin, uint8_t value)
{
    device->Pins[pin].DigitalValueSet = value;
}

uint8_t PK_SL_DigitalInputGet(sPoKeysDevice* device, uint8_t pin)
{
    return device->Pins[pin].DigitalValueGet;
}

uint32_t PK_SL_AnalogInputGet(sPoKeysDevice* device, uint8_t pin)
{
    return device->Pins[pin].AnalogValue;
}

