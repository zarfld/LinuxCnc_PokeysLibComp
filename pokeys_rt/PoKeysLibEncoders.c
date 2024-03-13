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

#include "PoKeysLib.h"
#include "PoKeysLibCore.h"

int32_t PK_EncoderConfigurationGet(sPoKeysDevice* device)
{
    uint32_t i;
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

	if (device->info.iBasicEncoderCount)
	{
		// Get basic encoder options
		CreateRequest(device->request, 0xC4, 0, 0, 0, 0);
		if (SendRequest(device) == PK_OK)
		{
			for (i = 0; i < device->info.iBasicEncoderCount; i++)
			{
				device->Encoders[i].encoderOptions = device->response[8 + i];
			}
		} else return PK_ERR_TRANSFER;

		// Get channel mappings
		CreateRequest(device->request, 0xC5, 0, 0, 0, 0);
		if (SendRequest(device) == PK_OK)
		{
			for (i = 0; i < device->info.iBasicEncoderCount; i++)
			{
				device->Encoders[i].channelApin = device->response[8 + i];
				device->Encoders[i].channelBpin = device->response[33 + i];
			}
		} else return PK_ERR_TRANSFER;

		if (device->info.iKeyMapping)
		{
			// Direction A key mapping
			CreateRequest(device->request, 0xC6, 0, 0, 0, 0);
			if (SendRequest(device) == PK_OK)
			{
				for (i = 0; i < device->info.iBasicEncoderCount; i++)
				{
					device->Encoders[i].dirAkeyCode = device->response[8 + i];
					device->Encoders[i].dirAkeyModifier = device->response[33 + i];
				}
			} else return PK_ERR_TRANSFER;

			// Direction B key mapping
			CreateRequest(device->request, 0xC7, 0, 0, 0, 0);
			if (SendRequest(device) == PK_OK)
			{
				for (i = 0; i < device->info.iBasicEncoderCount; i++)
				{
					device->Encoders[i].dirBkeyCode = device->response[8 + i];
					device->Encoders[i].dirBkeyModifier = device->response[33 + i];
				}
			} else return PK_ERR_TRANSFER;
		}

		if (device->info.iFastEncoders)
		{
			// Get fast encoders configuration
			CreateRequest(device->request, 0xCE, 2, 0, 0, 0);
			if (SendRequest(device) == PK_OK)
			{
				device->FastEncodersConfiguration = device->response[2];
				device->FastEncodersOptions = device->response[3];
			} else return PK_ERR_TRANSFER;
		}
		if (device->info.iUltraFastEncoders)
		{
			// Get ultra fast encoders configuration
			CreateRequest(device->request, 0x1C, 0xFF, 0, 0, 0);
			if (SendRequest(device) == PK_OK)
			{
				device->UltraFastEncoderConfiguration = device->response[2];
				device->UltraFastEncoderOptions = device->response[3];
				device->UltraFastEncoderFilter = *((unsigned int*)&(device->response[8]));
			} else return PK_ERR_TRANSFER;
		}

    } else
    {
        return PK_ERR_NOT_SUPPORTED;
    }
	return PK_OK;
}

int32_t PK_EncoderConfigurationSet(sPoKeysDevice* device)
{
    uint32_t i;
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    if (device->info.iBasicEncoderCount)
	{
		// Set basic encoder options
		CreateRequest(device->request, 0xC4, 1, 0, 0, 0);
		for (i = 0; i < device->info.iBasicEncoderCount; i++)
		{
			device->request[8 + i] = device->Encoders[i].encoderOptions;
		}
		if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

		// Set channel mappings
		CreateRequest(device->request, 0xC5, 1, 0, 0, 0);
		for (i = 0; i < device->info.iBasicEncoderCount; i++)
		{
			device->request[8 + i] = device->Encoders[i].channelApin;
			device->request[33 + i] = device->Encoders[i].channelBpin;
		}
		if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

		if (device->info.iKeyMapping)
		{
			// Direction A key mapping
			CreateRequest(device->request, 0xC6, 1, 0, 0, 0);
			for (i = 0; i < device->info.iBasicEncoderCount; i++)
			{
				device->request[8 + i] = device->Encoders[i].dirAkeyCode;
				device->request[33 + i] = device->Encoders[i].dirAkeyModifier;
			}
			if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

			// Direction B key mapping
			CreateRequest(device->request, 0xC7, 1, 0, 0, 0);
			for (i = 0; i < device->info.iBasicEncoderCount; i++)
			{
				device->request[8 + i] = device->Encoders[i].dirBkeyCode;
				device->request[33 + i] = device->Encoders[i].dirBkeyModifier;
			}
			if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;
		}

		if (device->info.iFastEncoders)
		{
			// Set fast encoders configuration
			CreateRequest(device->request, 0xCE, device->FastEncodersConfiguration, device->FastEncodersOptions, 0, 0);
			if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;
		}
		if (device->info.iUltraFastEncoders)
		{
			// Set ultra fast encoders configuration
			CreateRequest(device->request, 0x1C, device->UltraFastEncoderConfiguration, device->UltraFastEncoderOptions, 0, 0);
			*((unsigned int*)&(device->request[8])) = device->UltraFastEncoderFilter;
			if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;
		}

    } else
    {
        return PK_ERR_NOT_SUPPORTED;
    }
	return PK_OK;
}

int32_t PK_EncoderValuesGet(sPoKeysDevice* device)
{
    uint32_t i;
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    if (device->info.iBasicEncoderCount >= 13)
	{
		// Read the first 13 encoders
		CreateRequest(device->request, 0xCD, 0, 0, 0, 0);
		if (SendRequest(device) == PK_OK)
		{
			for (i = 0; i < 13; i++)
			{
				device->Encoders[i].encoderValue = *((unsigned int*)&device->response[8 + i * 4]);
			}
		} else return PK_ERR_TRANSFER;
	}
	
	if (device->info.iBasicEncoderCount >= 25 && device->info.iFastEncoders == 0)
	{
		// Read the next 12 encoders
		CreateRequest(device->request, 0xCD, 1, 0, 0, 0);
		if (SendRequest(device) == PK_OK)
		{
			for (i = 0; i < 12; i++)
			{
				device->Encoders[13 + i].encoderValue = *((unsigned int*)&device->response[8 + i * 4]);
			}
		} else return PK_ERR_TRANSFER;
	} else if (device->info.iBasicEncoderCount >= 25 && device->info.iUltraFastEncoders != 0)
	{
		// Read the next 12 encoders and ultra fast encoder
		CreateRequest(device->request, 0xCD, 1, 0, 0, 0);
		if (SendRequest(device) == PK_OK)
		{
			for (i = 0; i < 13; i++)
			{
				device->Encoders[13 + i].encoderValue = *((unsigned int*)&device->response[8 + i * 4]);
			}
		} else return PK_ERR_TRANSFER;
		
		// Read the test mode values for the ultra-fast encoder
		CreateRequest(device->request, 0x85, 0x37, 0, 0, 0);
		if (SendRequest(device) == PK_OK)
		{
			device->PEv2.EncoderIndexCount = *((uint32_t*)&device->response[12]);
			device->PEv2.EncoderTicksPerRotation = *((uint32_t*)&device->response[16]);
			device->PEv2.EncoderVelocity = *((uint32_t*)&device->response[20]);

			/*for (i = 0; i < 13; i++)
			{
				device->Encoders[13 + i].encoderValue = *((unsigned int*)&device->response[8 + i * 4]);
			}*/
		}
	}
	return PK_OK;
}

int32_t PK_EncoderValuesSet(sPoKeysDevice* device)
{
    uint32_t i;
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    if (device->info.iBasicEncoderCount >= 13)
	{
		// Read the first 13 encoders
		CreateRequest(device->request, 0xCD, 10, 0, 0, 0);
		for (i = 0; i < 13; i++)
		{
			*((unsigned int*)&device->request[8 + i * 4]) = device->Encoders[i].encoderValue;
		}
		if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;
	}
	
	if (device->info.iBasicEncoderCount >= 25)
	{
		// Read the next 12 encoders
		CreateRequest(device->request, 0xCD, 11, 0, 0, 0);
		for (i = 0; i < 13; i++)
		{
			*((unsigned int*)&device->request[8 + i * 4]) = device->Encoders[13 + i].encoderValue;
		}
		if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;
	}
	return PK_OK;
}

uint32_t PK_SL_EncoderValueGet(sPoKeysDevice* device, uint8_t index)
{
    if (device == NULL) return 0;
    return device->Encoders[index].encoderValue;
}
