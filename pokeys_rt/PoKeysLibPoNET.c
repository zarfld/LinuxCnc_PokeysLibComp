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

int32_t PK_PoNETGetPoNETStatus(sPoKeysDevice* device)
{
  if (device == NULL) return PK_ERR_NOT_CONNECTED;

  CreateRequest(device->request, 0xDD, 0x00, 0, 0, 0);
  if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

  device->PoNETmodule.PoNETstatus = device->response[8];

  return PK_OK;
}

int32_t PK_PoNETGetModuleSettings(sPoKeysDevice* device)
{
  if (device == NULL) return PK_ERR_NOT_CONNECTED;

  CreateRequest(device->request, 0xDD, 0x10, device->PoNETmodule.moduleID, 0, 0);
  if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

  device->PoNETmodule.i2cAddress = device->response[8];
  device->PoNETmodule.moduleType = device->response[9];
  device->PoNETmodule.moduleSize = device->response[10];
  device->PoNETmodule.moduleOptions = device->response[11];

  return PK_OK;
}



int32_t PK_PoNETGetModuleStatusRequest(sPoKeysDevice* device)
{
  if (device == NULL) return PK_ERR_NOT_CONNECTED;

  CreateRequest(device->request, 0xDD, 0x50, 0x10, device->PoNETmodule.moduleID, 0);
  if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;
  
  return PK_OK;
}

int32_t PK_PoNETGetModuleStatus(sPoKeysDevice* device)
{
  if (device == NULL) return PK_ERR_NOT_CONNECTED;

  CreateRequest(device->request, 0xDD, 0x50, 0x30, 0, 0);
  if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;

  if (device->response[3] != 1) return PK_ERR_GENERIC;
  if (device->response[8] != 0) return PK_ERR_GENERIC;
  memcpy(device->PoNETmodule.statusIn, device->response + 9, 16);

  return PK_OK;
}



int32_t PK_PoNETSetModuleStatus(sPoKeysDevice* device)
{
  uint32_t i;

  if (device == NULL) return PK_ERR_NOT_CONNECTED;

  CreateRequest(device->request, 0xDD, 0x55, device->PoNETmodule.moduleID, 0, 0);
  for (i = 0; i < 16; i++)
  {
    device->request[8 + i] = device->PoNETmodule.statusOut[i];
  }
  
  if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;
  return PK_OK;
}



int32_t PK_PoNETSetModulePWM(sPoKeysDevice* device)
{
  if (device == NULL) return PK_ERR_NOT_CONNECTED;

  CreateRequest(device->request, 0xDD, 0x70, device->PoNETmodule.moduleID, device->PoNETmodule.PWMduty, 0);
  
  if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;
  return PK_OK;
}

int32_t PK_PoNETGetModuleLightRequest(sPoKeysDevice* device)
{
  if (device == NULL) return PK_ERR_NOT_CONNECTED;

  CreateRequest(device->request, 0xDD, 0x60, 0x10, device->PoNETmodule.moduleID, 0);
  
  if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;
  return PK_OK;
}

int32_t PK_PoNETGetModuleLight(sPoKeysDevice* device)
{
  if (device == NULL) return PK_ERR_NOT_CONNECTED;

  CreateRequest(device->request, 0xDD, 0x60, 0x30, device->PoNETmodule.moduleID, 0);
  
  if (SendRequest(device) != PK_OK) return PK_ERR_TRANSFER;
  if (device->response[8] != 0) return PK_ERR_GENERIC;

  device->PoNETmodule.lightValue = device->response[9];

  return PK_OK;
}


