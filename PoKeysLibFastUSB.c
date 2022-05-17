/*

Copyright (C) 2014 Matevž Bošnak (matevz@poscope.com)

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

#ifdef POKEYSLIB_USE_LIBUSB	
	#include "libusb-1.0/libusb.h"
	#include "errno.h"

	typedef struct
	{
		libusb_context *context;
		libusb_device_handle *devh;
		int32_t deviceInterface;
	} sLibUsbDeviceData;

	int32_t checkDescriptor(struct libusb_config_descriptor * config)
	{
		int j = 0;

		if (config->bNumInterfaces > 4) return 0;

		// Check all interfaces...
		for (j = 0; j < config->bNumInterfaces; j++)
		{
			if (config->interface[j].altsetting[0].bInterfaceClass == 0xFF &&
				config->interface[j].altsetting[0].bInterfaceSubClass == 0xFF &&
				config->interface[j].altsetting[0].bNumEndpoints == 2 &&
				config->interface[j].altsetting[0].endpoint[0].wMaxPacketSize == 64 &&
				config->interface[j].altsetting[0].endpoint[1].wMaxPacketSize == 64)
			{
				// OK, we think this is a bulk interface of a PoKeys device...
				return j;
			}
		}
		return -1;
	}

	int32_t PK_EnumerateFastUSBDevices(void)
	{
		libusb_device *dev;
		int i = 0;
		libusb_device **devs;
		ssize_t cnt;
		struct libusb_config_descriptor * config = NULL;
		int numDevices = 0;
		int32_t r;

		// Initialize libusb
		r = libusb_init(NULL);
		if (r < 0) return 0;

		// List all devices
		cnt = libusb_get_device_list(NULL, &devs);		
		if (cnt < 0)
		{
			// Clear libusb context
			return 0;
		}

		while ((dev = devs[i++]) != NULL) 
		{
			struct libusb_device_descriptor desc;

			// Get descriptor and filter by VID, PID
			if (libusb_get_device_descriptor(dev, &desc) < 0) continue;
			if (desc.idVendor != 0x1DC3) continue;
			if (!(desc.idProduct == 0x1001 ||
				  desc.idProduct == 0x1011)) continue;

			r = libusb_get_config_descriptor(dev, 0, &config);
			if (r != 0) continue;

			if (checkDescriptor(config) >= 0)
			{
				numDevices++;
			}
		}

		// Release the device list and return
		libusb_free_device_list(devs, 1);

		return numDevices;
	}

	struct libusb_device_handle * findPoKeysDeviceFastInterfaceFromIndex(int32_t iDeviceIndex, int32_t * deviceInterface)
	{
		libusb_device *dev;
		int i = 0, j = 0;
		libusb_device **devs;
		ssize_t cnt;
		struct libusb_config_descriptor * config = NULL;
		struct libusb_device_handle *devh = NULL;
		int numDevices = 0;
		int32_t r;

		// Initialize libusb
		r = libusb_init(NULL);
		if (r < 0) return 0;

		// List all devices
		cnt = libusb_get_device_list(NULL, &devs);
		if (cnt < 0)
		{
			// Clear libusb context
			return NULL;
		}

		while ((dev = devs[i++]) != NULL)
		{
			struct libusb_device_descriptor desc;

			// Get descriptor and filter by VID, PID
			if (libusb_get_device_descriptor(dev, &desc) < 0) continue;
			if (desc.idVendor != 0x1DC3) continue;
			if (!(desc.idProduct == 0x1001 ||
				desc.idProduct == 0x1011)) continue;

			r = libusb_get_config_descriptor(dev, 0, &config);
			if (r != 0) continue;

			*deviceInterface = checkDescriptor(config);
			if (*deviceInterface >= 0)
			{
				if (iDeviceIndex == numDevices++)
				{
					// Connect to device
					r = libusb_open(dev, &devh);
					if (r != 0)
					{
						continue;
					}

					libusb_free_device_list(devs, 1);
					return devh;
				}				
			}
		}

		// Release the device list and return
		libusb_free_device_list(devs, 1);

		return NULL;
	}

	// Search for USB devices...
	struct libusb_device_handle * findPoKeysDeviceFastInterfaceFromSerial(libusb_context *context, int32_t serialNumber, int32_t * interfaceNumber)
	{
		libusb_device *dev;
		int i = 0, j = 0, k = 0;
		unsigned char serial_string[256] = { 0 };
		struct libusb_device_handle *devh = NULL;
		libusb_device **devs;
		ssize_t cnt;
		uint8_t serialSearch[8];
		int ret;

		struct libusb_config_descriptor * config = NULL;

		cnt = libusb_get_device_list(context, &devs);

		// Error retrieving list of devices
		if (cnt < 0)
			return devh;

		sprintf((char*)serialSearch, "x.%05lu", serialNumber % 100000);

		while ((dev = devs[i++]) != NULL) {
			struct libusb_device_descriptor desc;

			// Get descriptor and filter by VID, PID
			if (libusb_get_device_descriptor(dev, &desc) < 0) continue;
			if (desc.idVendor != 0x1DC3) continue;
			if (!(desc.idProduct == 0x1001 ||
				desc.idProduct == 0x1011)) continue;

			ret = libusb_get_config_descriptor(dev, 0, &config);
			if (ret != 0) continue;

			*interfaceNumber = checkDescriptor(config);

			if (*interfaceNumber >= 0)
			{
				// Connect to the device and retrieve string descriptor
				devh = NULL;
				ret = libusb_open(dev, &devh);

				if (ret != 0) continue;

				if (libusb_get_string_descriptor_ascii(devh, desc.iSerialNumber, serial_string, 255) < 0)
				{
					libusb_close(devh);
					devh = NULL;
					continue;
				}

				// Check serial number
				for (k = 1; k < 8 && serial_string[k] != 0; k++)
				{
					if (serial_string[k] != serialSearch[k]) break;
				}
				if (k == 7 && (serial_string[0] == '1' || serial_string[0] == '2'))
				{
					// Device found!
					break;
				}
			}

			// Check if device has 4 interfaces...
			//if (config->bNumInterfaces <= 3) continue;

			//if (config->interface[3].altsetting[0].bInterfaceClass != 0xFF || config->interface[3].altsetting[0].bInterfaceSubClass != 0xFF) continue;

			
			// Close the device
			libusb_close(devh);
			devh = NULL;
		}

		// Release the device list and return
		libusb_free_device_list(devs, 1);
		return devh;
	}

	void ClearStalls(libusb_device_handle * devh)
	{
		libusb_clear_halt(devh, 0x82);
		libusb_clear_halt(devh, 0x02);
	}

	void FreeFastUSBInterface(sLibUsbDeviceData * devData)
	{
		if (devData->devh != NULL)
		{
			//libusb_release_interface(devData->devh, 3);
			libusb_close(devData->devh);
		}

		libusb_exit(devData->context);
		free(devData);
	}


	// Use devHandle2 for USB connection to interface 3 (bulk)
	void * ConnectToFastUSBInterface(int serial)
	{
		int32_t r;
		sLibUsbDeviceData * devData = NULL;

		devData = (sLibUsbDeviceData*)malloc(sizeof(sLibUsbDeviceData));	
		devData->context = NULL;
		devData->devh = NULL;

		// Initialize libusb context 
		r = libusb_init(NULL);
		if (r < 0)
		{
			FreeFastUSBInterface(devData);
			return NULL;
		}

		devData->devh = findPoKeysDeviceFastInterfaceFromSerial(devData->context, serial, &devData->deviceInterface);
		r = errno;
		if (devData->devh != NULL)
		{
			r = libusb_claim_interface(devData->devh, devData->deviceInterface);
			if (r < 0)
			{
				FreeFastUSBInterface(devData);
				return NULL;
			}

			return (void*)devData;
		} else
		{
			FreeFastUSBInterface(devData);
			return NULL;
		}
	}

	void * PK_FastUSBConnectToDevice(uint32_t deviceIndex)
	{
		int32_t r;
		sLibUsbDeviceData * devData = NULL;

		devData = (sLibUsbDeviceData*)malloc(sizeof(sLibUsbDeviceData));
		devData->context = NULL;
		devData->devh = NULL;

		// Initialize libusb context 
		r = libusb_init(NULL);
		if (r < 0)
		{
			FreeFastUSBInterface(devData);
			return NULL;
		}

		devData->devh = findPoKeysDeviceFastInterfaceFromIndex(deviceIndex, &devData->deviceInterface);
		r = errno;
		if (devData->devh != NULL)
		{
			r = libusb_claim_interface(devData->devh, devData->deviceInterface);
			if (r < 0)
			{
				FreeFastUSBInterface(devData);
				return NULL;
			}

			return (void*)devData;
		}
		else
		{
			FreeFastUSBInterface(devData);
			return NULL;
		}
	}

	void DisconnectFromFastUSBInterface(void * devData)
	{
		if (devData != NULL)
			FreeFastUSBInterface((sLibUsbDeviceData*)devData);
	}

	

	int32_t SendRequestFastUSB(sPoKeysDevice* device)
	{
		// Initialize variables
        uint32_t waits = 0;
        uint32_t timeoutValue = 10;
		uint32_t retries = 0;
		int32_t result = 0;
		int32_t bytesTransferred = 0;

		libusb_device_handle *devh;

		if (device == NULL) return PK_ERR_GENERIC;
		if (device->devHandle2 == NULL) return PK_ERR_CANNOT_CONNECT;

		devh = ((sLibUsbDeviceData*)device->devHandle2)->devh;

		#ifdef PK_COM_DEBUG
			int i;
		#endif


		// Request sending loop
		while (retries++ < 2)
		{
			device->request[0] = 0xBB;
			device->request[6] = ++device->requestID;
			device->request[7] = getChecksum(device->request);

			result = libusb_bulk_transfer(devh, 0x02, device->request, 64, &bytesTransferred, 10);

			// In case of an error, try sending again
			if (result < 0)
			{
				//printf(" ERR %u", result);
				retries++;
				continue;
			}

            if (device->request[1] == 0x50 || device->request[1] == 0x51 || device->request[1] == 0x52)
            {
                // Delay 500 ms...
                timeoutValue = 500;
            }

			waits = 0;
			// Request receiving loop
			while (waits++ < 10)
			{
                result = libusb_bulk_transfer(devh, 0x82, device->response, 64, &bytesTransferred, timeoutValue);

				// Error is not an option
				if (result < 0)
				{
						//printf(" Receive ERR %u", result);
					if (result == LIBUSB_ERROR_TIMEOUT) // Timeout
						continue;

					break;
				}

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
					else
					{
						// Wrong checksum...
						continue;
					}
				}
				else
				{
#ifdef WIN32				
					// Wrong header...
					if (waits > 5)
					{					
						Sleep(10);
					}
#endif					
					continue;
				}
			}

			ClearStalls(devh);
		}

		return PK_ERR_TRANSFER;
	}
	
	int32_t SendRequestFastUSB_NoResponse(sPoKeysDevice* device)
	{
		// Initialize variables
        uint32_t waits = 0;
        uint32_t timeoutValue = 10;
		uint32_t retries = 0;
		int32_t result = 0;
		int32_t bytesTransferred = 0;

		libusb_device_handle *devh;

		if (device == NULL) return PK_ERR_GENERIC;
		if (device->devHandle2 == NULL) return PK_ERR_CANNOT_CONNECT;

		devh = ((sLibUsbDeviceData*)device->devHandle2)->devh;

		#ifdef PK_COM_DEBUG
			int i;
		#endif


		// Request sending loop
		while (retries++ < 2)
		{
			device->request[0] = 0xBB;
			device->request[6] = ++device->requestID;
			device->request[7] = getChecksum(device->request);

			result = libusb_bulk_transfer(devh, 0x02, device->request, 64, &bytesTransferred, 10);

			// In case of an error, try sending again
			if (result < 0)
			{
				//printf(" ERR %u", result);
				retries++;
				continue;
			}
			break;
		}

		return PK_OK;
	}

	int32_t SendRequestFastUSB_multiPart(sPoKeysDevice* device)
	{
		// Initialize variables
		uint32_t waits = 0;
		uint32_t retries = 0;
		int32_t result = 0;
		int32_t bytesTransferred = 0;
		uint8_t requestBuffer[512] = {0};
		uint8_t * requestBufferPtr = 0;
		uint32_t i;
		uint8_t checksum2 = 0;

		libusb_device_handle *devh;

		if (device == NULL) return PK_ERR_GENERIC;
		if (device->devHandle2 == NULL) return PK_ERR_CANNOT_CONNECT;

		devh = ((sLibUsbDeviceData*)device->devHandle2)->devh;

		#ifdef PK_COM_DEBUG
			int i;
		#endif


		// Request sending loop
		while (retries++ < 4)
		{
			for (i = 0; i < 8; i++)
			{
				requestBufferPtr = &requestBuffer[i * 64];

				memcpy(requestBufferPtr, device->request, 8);
				requestBufferPtr[0] = 0xBB;

				// Put packet ID and flags here...
				requestBufferPtr[2] = i;
				if (i == 0)
					requestBufferPtr[2] |= (1<<3);
				else if (i == 7)
					requestBufferPtr[2] |= (1<<4);

				requestBufferPtr[6] = ++device->requestID;
				requestBufferPtr[7] = getChecksum(requestBufferPtr);

				memcpy(requestBufferPtr + 8, device->multiPartData + i*56, 56);

				//result = libusb_bulk_transfer(devh, 0x02, requestBuffer, 64, &bytesTransferred, 10);
			}

			result = libusb_bulk_transfer(devh, 0x02, requestBuffer, 512, &bytesTransferred, 10);

			/*
			// In case of an error, try sending again
			if (result < 0)
			{
				//printf(" ERR %u", result);
				retries++;
				continue;
			}
			*/

			waits = 0;
			// Request receiving loop
			while (waits++ < 50)
			{
				result = libusb_bulk_transfer(devh, 0x82, device->response, 64, &bytesTransferred, 10);

				// Error is not an option
				if (result < 0)
				{
					ClearStalls(devh);

						//printf(" Receive ERR %u", result);
						break;
				}

				// Check the header and the request ID
				if (device->response[0] == 0xAA && device->response[6] == device->requestID)
				{
					if (device->response[1] == 0xB1)
					{
						break;
						//return PK_ERR_TRANSFER;
					}

					if (device->response[1] == device->request[1] && device->response[7] == getChecksum(device->response))
					{
						// Check second checksum...						
						for (i = 8; i < 63; i++)
						{
							checksum2 += device->response[i];
						}
						if (checksum2 != device->response[63])
						{
							//continue;
							return PK_ERR_TRANSFER;
						}

						LastRetryCount = retries;
						LastWaitCount = waits;
						// This is it. Return from this function
						return PK_OK;
					}
				}
				//Sleep(1);
			}

			return PK_ERR_TRANSFER;
		}

		return PK_ERR_TRANSFER;
	}
	
#endif
