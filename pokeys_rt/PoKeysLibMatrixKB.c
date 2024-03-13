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

int32_t PK_MatrixKBConfigurationGet(sPoKeysDevice* device)
{
    uint32_t n, key, x;
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    if (device->info.iMatrixKeyboard)
	{
		// Get matrix KB configuration
		CreateRequest(device->request, 0xCA, 10, 0, 0, 0);
		if (SendRequest(device) == PK_OK)
		{
			device->matrixKB.matrixKBconfiguration = device->response[8];
			device->matrixKB.matrixKBheight = 1 + (device->response[9] & 0x0F);
			device->matrixKB.matrixKBwidth = 1 + ((device->response[9] >> 4) & 0x0F);

			for (n = 0; n < 8; n++)
			{
				device->matrixKB.matrixKBrowsPins[n] = device->response[10 + n];
				device->matrixKB.matrixKBrowsPins[8 + n] = device->response[42 + n];

				device->matrixKB.matrixKBcolumnsPins[n] = device->response[18 + n];
			}

			for (n = 0; n < 128; n++)
			{
				device->matrixKB.macroMappingOptions[n] = ((device->response[26 + n / 8] & (1 << (n % 8))) > 0) ? 1 : 0;
			}

			device->matrixKB.matrixKBScanningDecimation = device->response[51];
		} else return PK_ERR_TRANSFER;
	}
	
	if (device->info.iKeyMapping)
	{
		// Key codes and modifiers
		for (n = 0; n < 8; n++)
		{
			CreateRequest(device->request, 0xCA, 12 + n, 0, 0, 0);
			if (SendRequest(device) == PK_OK)
			{
				for (key = 0; key < 16; key++)
                {
                    device->matrixKB.keyMappingKeyCode[n * 16 + key] = device->response[8 + key];
					device->matrixKB.keyMappingKeyModifier[n * 16 + key] = device->response[24 + key];
                }

				if (device->info.iMatrixKeyboardTriggeredMapping)
				{
					// Get trigger status
					for (x = 0; x < 8; x++)
					{
						device->matrixKB.keyMappingTriggeredKey[n * 16 + x] = ((device->response[40] & (1 << x)) > 0) ? 1 : 0;
						device->matrixKB.keyMappingTriggeredKey[n * 16 + 8 + x] = ((device->response[41] & (1 << x)) > 0) ? 1 : 0;
					}

					// Get up key codes and modifiers
					CreateRequest(device->request, 0xCA, 32 + n, 0, 0, 0);
					if (SendRequest(device) == PK_OK)
					{
						for (key = 0; key < 16; key++)
						{
							device->matrixKB.keyMappingKeyCodeUp[n * 16 + key] = device->response[8 + key];
							device->matrixKB.keyMappingKeyModifierUp[n * 16 + key] = device->response[24 + key];
						}
					}
				}	
			}	
		}
	}

	return PK_OK;
}

int32_t PK_MatrixKBConfigurationSet(sPoKeysDevice* device)
{
    uint32_t n, key, x;
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    if (device->info.iMatrixKeyboard)
	{
		// Set matrix KB configuration
		CreateRequest(device->request, 0xCA, 1, 0, 0, 0);

		device->request[8] = 0; // Inactivate the keyboard for now...
		device->request[9] = (unsigned char)(((device->matrixKB.matrixKBheight - 1) & 0x0F) + (((device->matrixKB.matrixKBwidth - 1) << 4) & 0xF0));

		for (n = 0; n < 8; n++)
		{
			device->request[10 + n] = device->matrixKB.matrixKBrowsPins[n];
			device->request[42 + n] = device->matrixKB.matrixKBrowsPins[8 + n];

			device->request[18 + n] = device->matrixKB.matrixKBcolumnsPins[n];
		}

		memset(&(device->request[26]), 0, 16);
		for (n = 0; n < 128; n++)
		{
			if (device->matrixKB.macroMappingOptions[n]) device->request[26 + n / 8] |= (1 << (n % 8));
		}

		if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;


		// Set matrix KB configuration
		CreateRequest(device->request, 0xCA, 50, 0, 0, 0);
		device->request[8] = device->matrixKB.matrixKBScanningDecimation;
		if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;
	}
	
	if (device->info.iKeyMapping)
	{
		// Key codes and modifiers
		for (n = 0; n < 8; n++)
		{
			CreateRequest(device->request, 0xCA, 2 + n, 0, 0, 0);
			for (key = 0; key < 16; key++)
            {
                device->request[8 + key] = device->matrixKB.keyMappingKeyCode[n * 16 + key];
				device->request[24 + key] = device->matrixKB.keyMappingKeyModifier[n * 16 + key];
            }
			if (device->info.iMatrixKeyboardTriggeredMapping)
			{
				device->request[40] = 0;
				device->request[41] = 0;

				// Trigger status
				for (x = 0; x < 8; x++)
				{
					if (device->matrixKB.keyMappingTriggeredKey[n * 16 + x]) device->request[40] |= (1 << x);
					if (device->matrixKB.keyMappingTriggeredKey[n * 16 + 8 + x]) device->request[41] |= (1 << x);
				}
			}
			if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

			if (device->info.iMatrixKeyboardTriggeredMapping)
			{
				// Get up key codes and modifiers
				CreateRequest(device->request, 0xCA, 22 + n, 0, 0, 0);
				for (key = 0; key < 16; key++)
				{
					device->request[8 + key] = device->matrixKB.keyMappingKeyCodeUp[n * 16 + key];
					device->request[24 + key] = device->matrixKB.keyMappingKeyModifierUp[n * 16 + key];
				}
				if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;
			}	
		}
	}


	// Resend and activate
	if (device->info.iMatrixKeyboard)
	{
		// Set matrix KB configuration
		CreateRequest(device->request, 0xCA, 1, 0, 0, 0);

		device->request[8] = device->matrixKB.matrixKBconfiguration;
		device->request[9] = (unsigned char)(((device->matrixKB.matrixKBheight - 1) & 0x0F) + (((device->matrixKB.matrixKBwidth - 1) << 4) & 0xF0));

		for (n = 0; n < 8; n++)
		{
			device->request[10 + n] = device->matrixKB.matrixKBrowsPins[n];
			device->request[42 + n] = device->matrixKB.matrixKBrowsPins[8 + n];

			device->request[18 + n] = device->matrixKB.matrixKBcolumnsPins[n];
		}

		memset(&(device->request[26]), 0, 16);
		for (n = 0; n < 128; n++)
		{
			if (device->matrixKB.macroMappingOptions[n]) device->request[26 + n / 8] |= (1 << (n % 8));
		}

		if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;
	}

	return PK_OK;
}

int32_t PK_MatrixKBStatusGet(sPoKeysDevice* device)
{
    uint32_t n;
    if (device == NULL) return PK_ERR_NOT_CONNECTED;

    if (device->info.iMatrixKeyboard)
	{
		// Get matrix KB status
		CreateRequest(device->request, 0xCA, 20, 0, 0, 0);
		if (SendRequest(device) == PK_OK)
		{
			for (n = 0; n < 128; n++)
            {
				device->matrixKB.matrixKBvalues[n] = (device->response[8 + n / 8] & (1 << (n % 8))) > 0;
            }
		} else return PK_ERR_TRANSFER;
	}

	return PK_OK;
}
                
