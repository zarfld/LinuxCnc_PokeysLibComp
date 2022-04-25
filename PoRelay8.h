#include "PoKeysLib.h"


// PoRelay8 module data
typedef struct
{
    sPoKeysDevice* device;
    uint8_t i2cAddress;
    uint8_t TYPE[2];
    uint8_t FW_ver[2];
    uint32_t DeviceID;

    uint8_t statusIn;
    uint8_t statusOut[10];

    // 0 .. Can , 1 .. PoExtBus , 2 .. I2C
    uint8_t SendOption;
    /*
    0 Device's I2C address 0x7B
    1 PoExtBus daisy-chain position (data index) 0
    2 CAN daisy-chain position (data index) 0
    3 Number of PoRelay8 devices on CAN bus (additional
    CAN frames are sent if more than 8 PoRelay8 devices
    present)
    10
    4 Failsafe timeout (in ms) 5000
    5 Disable CRC check on PoExtBus 0
    6 CAN bus timing option
    0 � default CAN bitrate of 250 kbit/s
    125 � CAN bitrate of 125 kbit/s
    250 � CAN bitrate of 250 kbit/s
    500 � CAN bitrate of 500 kbit/s
    1000 � CAN bitrate of 1000 kbit/s
    0
    7 CAN bus message ID 0x108
    8 PoIL master enable switch and PoIL core ID 0
    */


    uint8_t PoExtBus_Position;
    uint8_t CAN_Position;
    uint8_t CAN_PoRelayCount;
    uint8_t Failsafe_timeout;
    uint8_t DisableCRC;
    uint8_t CAN_TimingOption;
    uint8_t CAN_MessageID;
    uint8_t POIL_MasterEnable;
} sPoRelay8;


// PoRelay8 commands
POKEYSDECL int32_t PK_PoRelay8_DeviceIdentification(sPoKeysDevice* device, sPoRelay8 Relay);

POKEYSDECL int32_t PK_PoRelay8_DeviceIdentification(sPoKeysDevice* device, sPoRelay8 Relay);
POKEYSDECL int32_t PK_PoRelay8_ConfigurationRead(sPoKeysDevice* device, sPoRelay8 Relay);
POKEYSDECL int32_t PK_PoRelay8_ConfigurationWrite(sPoKeysDevice* device, sPoRelay8 Relay);
POKEYSDECL int32_t PK_PoRelay8_ConfigurationSave(sPoKeysDevice* device, sPoRelay8 Relay);
POKEYSDECL int32_t PK_PoRelay8_SetOutputs(sPoKeysDevice* device, sPoRelay8 Relay);
POKEYSDECL int32_t PK_PoRelay8_SetOutputsArray(sPoKeysDevice* device, sPoRelay8 Relay);
POKEYSDECL int32_t PK_PoRelay8_SetOutputsX1(sPoKeysDevice* device, uint8_t OutPuts);
POKEYSDECL int32_t PK_PoRelay8_SetOutputsX10(sPoKeysDevice* device, uint8_t OutPuts[10]);

POKEYSDECL int32_t PK_PoRelay8_ReEnablePoExtBus(sPoKeysDevice* device, sPoRelay8 Relay);

POKEYSDECL int32_t PK_PoRelay8_POILcommand(sPoKeysDevice* device, uint8_t CanMsgId, uint8_t CanMsgData);
POKEYSDECL int32_t PK_PoRelay8_SendMessage2CanBus(sPoKeysDevice* device, uint8_t Flags, uint8_t CanMsgId, uint8_t CanMsgData);
POKEYSDECL int32_t PK_PoRelay8_CanStateUpdate(sPoKeysDevice* device);
POKEYSDECL int32_t PK_PoRelay8_CanStateUpdate2(sPoKeysDevice* device);
POKEYSDECL int32_t PK_PoRelay8_CanStateUpdateSingle(sPoKeysDevice* device);
POKEYSDECL int32_t PK_PoRelay8_CanGeneral(sPoKeysDevice* device);

