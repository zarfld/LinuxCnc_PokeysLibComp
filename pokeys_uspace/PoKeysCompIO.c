#include "PoKeysLib.h"
#include "rtapi.h"
// #include "rtapi_app.h"
#include "hal.h"
#include "stdio.h"


typedef struct {
	hal_u32_t* deb_out;
	hal_u32_t* deb_setval;
	hal_float_t* value;
	hal_bit_t* enable;
	hal_float_t offset;
	hal_float_t scale;
	hal_float_t high_limit;
	hal_float_t low_limit;
	hal_float_t max_v;
	hal_u32_t PinId;
}one_adcout_data_t;

typedef struct {
	hal_float_t* value_raw;
	hal_float_t* value;
	hal_float_t scale;
	hal_float_t offset;
}one_adcin_data_t;

typedef struct {
	hal_bit_t* digin_in;
	hal_bit_t* digin_in_not;
	hal_bit_t* digout_out;
	hal_bit_t digout_invert;

	hal_u32_t* counter_value;

	hal_u32_t PinFunction;

	bool DigitalValueSet_ignore;
}one_digiIO_data_t;

typedef struct {
	one_adcout_data_t adcout[6];
	hal_u32_t adcout_pwm_period;
	hal_u32_t* adcout_deb_outv;

	one_adcin_data_t adcin[7];

	one_digiIO_data_t Pin[55];

	hal_u32_t* deb_out;
}all_IO_data_t;

static all_IO_data_t* IO_data = 0;
//const sleepdur = 200; // 

int PKIO_export_pins(char* prefix, long extra_arg, int id, all_IO_data_t* Io_data, sPoKeysDevice* dev) {
	int r = 0;
	int j = 0;
	IO_data = Io_data;

	rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: prefix: %s\n", __FILE__, __FUNCTION__, prefix);
	if (dev == NULL) {
		rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s dev == NULL\n", __FILE__, __FUNCTION__);
	}

	if (Io_data == NULL) {
		rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s Io_data == NULL\n", __FILE__, __FUNCTION__);
		IO_data = hal_malloc(sizeof(all_IO_data_t));
		if (IO_data == NULL) {
			rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: all_IO_data_t malloc failed\n", __FILE__, __FUNCTION__);
			return -1;
		}
	}
	else {
		rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: Io_data != NULL\n", __FILE__, __FUNCTION__);
		IO_data = Io_data;
	}

	// AnalogOut Pins
	int analogOutCount = dev->info.iPWMCount;
	for (j = 0; j < (analogOutCount); j++) {
		rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.adcout.%01d.deb.out\n", __FILE__, __FUNCTION__, prefix, j);
		r = hal_pin_u32_newf(HAL_OUT, &(IO_data->adcout[j].deb_out), id,
			"%s.adcout.%01d.deb.out", prefix, j);
		if (r != 0) {
			rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.adcout.%01d.deb.out failed\n", __FILE__, __FUNCTION__, prefix, j);
			return r;
		}

		rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.adcout.%01d.deb.setval\n", __FILE__, __FUNCTION__, prefix, j);
		r = hal_pin_u32_newf(HAL_OUT, &(IO_data->adcout[j].deb_setval), id,
			"%s.adcout.%01d.deb.setval", prefix, j);
		if (r != 0) {
			rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.adcout.%01d.deb.setval failed\n", __FILE__, __FUNCTION__, prefix, j);
			return r;
		}

		rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.adcout.%01d.deb.setval\n", __FILE__, __FUNCTION__, prefix, j);
		r = hal_pin_float_newf(HAL_IN, &(IO_data->adcout[j].value), id,
			"%s.adcout.%01d.value", prefix, j);
		if (r != 0) {
			rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.adcout.%01d.value failed\n", __FILE__, __FUNCTION__, prefix, j);
			return r;
		}

		rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.adcout.%01d.enable\n", __FILE__, __FUNCTION__, prefix, j);
		r = hal_pin_bit_newf(HAL_IN, &(IO_data->adcout[j].enable), id,
			"%s.adcout.%01d.enable", prefix, j);
		if (r != 0) {
			rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.adcout.%01d.enable failed\n", __FILE__, __FUNCTION__, prefix, j);
			return r;
		}

		rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.adcout.%01d.offset\n", __FILE__, __FUNCTION__, prefix, j);
		r = hal_param_float_newf(HAL_RW, &(IO_data->adcout[j].offset), id,
			"%s.adcout.%01d.offset", prefix, j);
		if (r != 0) {
			rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.adcout.%01d.offset failed\n", __FILE__, __FUNCTION__, prefix, j);
			return r;
		}

		rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.adcout.%01d.scale\n", __FILE__, __FUNCTION__, prefix, j);
		r = hal_param_float_newf(HAL_RW, &(IO_data->adcout[j].scale), id,
			"%s.adcout.%01d.scale", prefix, j);
		if (r != 0) {
			rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.adcout.%01d.scale failed\n", __FILE__, __FUNCTION__, prefix, j);
			return r;
		}

		rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.adcout.%01d.high-limit\n", __FILE__, __FUNCTION__, prefix, j);
		r = hal_param_float_newf(HAL_RW, &(IO_data->adcout[j].high_limit), id,
			"%s.adcout.%01d.high-limit", prefix, j);
		if (r != 0) {
			rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.adcout.%01d.high-limit failed\n", __FILE__, __FUNCTION__, prefix, j);
			return r;
		}

		rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.adcout.%01d.low-limit\n", __FILE__, __FUNCTION__, prefix, j);
		r = hal_param_float_newf(HAL_RW, &(IO_data->adcout[j].low_limit), id,
			"%s.adcout.%01d.low-limit", prefix, j);
		if (r != 0) {
			rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.adcout.%01d.low-limit failed\n", __FILE__, __FUNCTION__, prefix, j);
			return r;
		}

		r = hal_param_float_newf(HAL_RW, &(IO_data->adcout[j].max_v), id,
			"%s.adcout.%01d.max-v", prefix, j);
		if (r != 0) {
			rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.adcout.%01d.max-v failed\n", __FILE__, __FUNCTION__, prefix, j);
			return r;
		}

		r = hal_param_u32_newf(HAL_RO, &(IO_data->adcout[j].PinId), id,
			"%s.adcout.%01d.PinId", prefix, j);
		if (r != 0) {
			rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.adcout.%01d.PinId failed\n", __FILE__, __FUNCTION__, prefix, j);
			return r;
		}
	}
	r = hal_pin_u32_newf(HAL_OUT, &(IO_data->adcout_deb_outv), id,
		"%s.adcout.deb.outv", prefix);

	r = hal_param_u32_newf(HAL_RW, &(IO_data->adcout_pwm_period), id,
		"%s.adcout.pwm.period", prefix);
	if (r != 0) {
		rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.adcout.pwm.period failed\n", __FILE__, __FUNCTION__, prefix);
		return r;
	}

	// AnalogIn Pins
	int analogInCount = 7;
	for (j = 0; j < (analogInCount); j++) {
		r = hal_pin_float_newf(HAL_OUT, &(IO_data->adcin[j].value_raw), id,
			"%s.adcin.%01d.value-raw", prefix, j);
		if (r != 0) {
			rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.adcin.%01d.value-raw failed\n", __FILE__, __FUNCTION__, prefix, j);
			return r;
		}

		r = hal_pin_float_newf(HAL_OUT, &(IO_data->adcin[j].value), id,
			"%s.adcin.%01d.value", prefix, j);
		if (r != 0) {
			rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.adcin.%01d.value failed\n", __FILE__, __FUNCTION__, prefix, j);
			return r;
		}

		r = hal_param_float_newf(HAL_RW, &(IO_data->adcin[j].scale), id,
			"%s.adcin.%01d.scale", prefix, j);
		if (r != 0) {
			rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.adcin.%01d.scale failed\n", __FILE__, __FUNCTION__, prefix, j);
			return r;
		}

		r = hal_param_float_newf(HAL_RW, &(IO_data->adcin[j].offset), id,
			"%s.adcin.%01d.offset", prefix, j);
		if (r != 0) {
			rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.adcin.%01d.offset failed\n", __FILE__, __FUNCTION__, prefix, j);
			return r;
		}
	}

	// Digital Pins
	int digitalCount = dev->info.iPinCount;
	for (j = 0; j < (digitalCount); j++) {

		//PinFunction
		rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.Pins.%01d.PinFunction\n", __FILE__, __FUNCTION__, prefix, j);
		r = hal_param_u32_newf(HAL_RW, &(IO_data->Pin[j].PinFunction), id,
			"%s.pins.%01d.PinFunction", prefix, j);
		if (r != 0) {
			rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.digout.%01d.invert failed\n", __FILE__, __FUNCTION__, prefix, j);
			return r;
		}

		if (PK_CheckPinCapability(dev, j, PK_AllPinCap_digitalInput) == 1) {
			rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.digin.%01d.deb.out\n", __FILE__, __FUNCTION__, prefix, j);
			r = hal_pin_bit_newf(HAL_OUT, &(IO_data->Pin[j].digin_in), id,
				"%s.digin.%01d.in", prefix, j);
			if (r != 0) {
				rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.digin.%01d.in failed\n", __FILE__, __FUNCTION__, prefix, j);
				return r;
			}
			rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.digin.%01d.in-not\n", __FILE__, __FUNCTION__, prefix, j);
			r = hal_pin_bit_newf(HAL_OUT, &(IO_data->Pin[j].digin_in_not), id,
				"%s.digin.%01d.in-not", prefix, j);
			if (r != 0) {
				rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.digin.%01d.in-not failed\n", __FILE__, __FUNCTION__, prefix, j);
				return r;
			}
		}

		if (PK_CheckPinCapability(dev, j, PK_AllPinCap_digitalOutput) == 1) {
			rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.digout.%01d.out\n", __FILE__, __FUNCTION__, prefix, j);
			r = hal_pin_bit_newf(HAL_IN, &(IO_data->Pin[j].digout_out), id,
				"%s.digout.%01d.out", prefix, j);

			if (r != 0) {
				rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.digout.%01d.out failed\n", __FILE__, __FUNCTION__, prefix, j);
				return r;
			}
			IO_data->Pin[j].DigitalValueSet_ignore = 0;
			rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.digout.%01d.invert\n", __FILE__, __FUNCTION__, prefix, j);
			r = hal_param_bit_newf(HAL_RW, &(IO_data->Pin[j].digout_invert), id,
				"%s.digout.%01d.invert", prefix, j);
			if (r != 0) {
				rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.digout.%01d.invert failed\n", __FILE__, __FUNCTION__, prefix, j);
				return r;
			}

		}

		if (PK_CheckPinCapability(dev, j, PK_AllPinCap_digitalCounter) == 1) {
			rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.counter.%01d.value\n", __FILE__, __FUNCTION__, prefix, j);
			r = hal_pin_u32_newf(HAL_OUT, &(IO_data->Pin[j].counter_value), id,
				"%s.counter.%01d.value", prefix, j);
			if (r != 0) {
				rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.counter.%01d.value failed\n", __FILE__, __FUNCTION__, prefix, j);
				return r;
			}
		}

	}

	rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: %s.digin.%01d.deb.out\n", __FILE__, __FUNCTION__, prefix, j);
	r = hal_pin_u32_newf(HAL_OUT, &(IO_data->deb_out), id,
		"%s.digin.deb.out", prefix, j);
	if (r != 0) {
		rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: %s.digin.deb.out failed\n", __FILE__, __FUNCTION__, prefix, j);
		return r;
	}
	return r;
}

void PKIO_Update(sPoKeysDevice* dev) {
	bool PinConfigurationGet = false;
	bool DigitalIOGet = false;
	bool AnalogIOGet = false;
	bool AnalogIOSet = false;
	bool DigitalCounterGet = false;
	bool DigitalIOSet = false;
	bool doPwmConfig = false;
	bool loopPins = false;
	bool DoPWM = false;

	if (dev->info.iPinCount > 0) {
		*(IO_data->deb_out) = 210;
		rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_PinConfigurationGet(dev)\n", __FILE__, __FUNCTION__);
		if (PK_PinConfigurationGet(dev) == PK_OK) {
			rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_PinConfigurationGet(dev) OK\n", __FILE__, __FUNCTION__);
			PinConfigurationGet = true;
			*(IO_data->deb_out) = 211;
			loopPins = true;
		}
		else{
			rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PK_PinConfigurationGet(dev) failed\n", __FILE__, __FUNCTION__);
		}
		usleep(sleepdur);
		rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_DigitalIOGet(dev)\n", __FILE__, __FUNCTION__);
		if (PK_DigitalIOGet(dev) == PK_OK) {
			rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_DigitalIOGet(dev) OK\n", __FILE__, __FUNCTION__);
			*(IO_data->deb_out) = 212;
			DigitalIOGet = true;
			loopPins = true;
		}
		else{
			rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PK_DigitalIOGet(dev) failed\n", __FILE__, __FUNCTION__);
		}
		usleep(sleepdur);

		rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_AnalogIOGet(dev)\n", __FILE__, __FUNCTION__);
		if (PK_AnalogIOGet(dev) == PK_OK) {
			rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_AnalogIOGet(dev) OK\n", __FILE__, __FUNCTION__);
			*(IO_data->deb_out) = 213;
			AnalogIOGet = true;
			loopPins = true;
		}
		else{
			rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PK_AnalogIOGet(dev) failed\n", __FILE__, __FUNCTION__);
		}
		usleep(sleepdur);
	}

	rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: info_iDigitalCounters = %d\n", __FILE__, __FUNCTION__, dev->info.iDigitalCounters);
	if (dev->info.iDigitalCounters > 0) {
		*(IO_data->deb_out) = 214;
		rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_DigitalCounterGet(dev)\n", __FILE__, __FUNCTION__);
		if (PK_DigitalCounterGet(dev) == PK_OK) {
			rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_DigitalCounterGet(dev) OK\n", __FILE__, __FUNCTION__);
			*(IO_data->deb_out) = 215;
			DigitalCounterGet = true;
			loopPins = true;
		}
		else{
			rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PK_DigitalCounterGet(dev) failed\n", __FILE__, __FUNCTION__);
		}
		usleep(sleepdur);
	}

	rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: info_PWMCount = %d\n", __FILE__, __FUNCTION__, dev->info.iPWMCount);
	if (dev->info.iPWMCount && DoPWM) {
		*(IO_data->adcout_deb_outv) = 100;
		*(IO_data->adcout_deb_outv) = 100;
		uint32_t PWMperiod; // PWM period, shared among all channels

		uint32_t PWMduty[6];		   // PWM duty cycles (range between 0 and PWM period)
		uint8_t PWMenabledChannels[6]; // List of enabled PWM channels
		uint8_t PWMpinIDs[6];
		float PWM_SCale[6];
		float max_v[6];
		float high_limit[6];
		float low_limit[6];
		float PWM_value[6];
		float PWM_OffSet[6];
		rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_PWMConfigurationGet(dev)\n", __FILE__, __FUNCTION__);

		if (PK_PWMConfigurationGet(dev) == PK_OK) {
			rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_PWMConfigurationGet(dev) OK\n", __FILE__, __FUNCTION__);
			PWMperiod = IO_data->adcout_pwm_period;
			//PWMperiod = adcout_pwm_period;
			usleep(sleepdur);
			if (PWMperiod == 0) {
				PWMperiod = 2500;
			}
			PWMperiod = 2500;
			if (dev->PWM.PWMperiod != PWMperiod) {
				dev->PWM.PWMperiod = PWMperiod;
				doPwmConfig = true;
				PK_PWMConfigurationSet(dev);
				usleep(sleepdur);
			}

			if (IO_data->adcout_pwm_period != PWMperiod) {
				IO_data->adcout_pwm_period = PWMperiod;
			}

			// Pins_PinFunction(i)=dev->Pins[i].PinFunction;

			for (int i = 0; i < 6; i++) {
				int PwmId = 5 - i;
				*(IO_data->adcout[PwmId]).deb_setval = 100;
				//adcout_deb_out(PwmId) = 100;

				PWMduty[i] = dev->PWM.PWMduty[i];			  // PWM duty cycles (range between 0 and PWM period)
				PWMenabledChannels[i] = *(IO_data->adcout[PwmId]).enable; // List of enabled PWM channels
				PWMpinIDs[i] = dev->PWM.PWMpinIDs[i];
				IO_data->adcout[PwmId].PinId = PWMpinIDs[i];
				//adcout_PinId(PwmId) = PWMpinIDs[i];
				PWM_SCale[i] = 1;

				PWM_value[i] = *(IO_data->adcout[PwmId]).value;
				PWM_OffSet[i] = IO_data->adcout[PwmId].offset;
				max_v[i] = IO_data->adcout[PwmId].max_v;
				high_limit[i] = IO_data->adcout[PwmId].high_limit;
				low_limit[i] = IO_data->adcout[PwmId].low_limit;

				if (max_v[PwmId] == 0) {
					if ((dev->DeviceData.DeviceTypeID == PK_DeviceID_PoKeys57CNC) && (PWMpinIDs[i] = 17)) {
						max_v[PwmId] = 10.000; // Pin17 0-10V
					}
					else {
						max_v[PwmId] = 5.000; // usually pokeys57 provide 0..5V PWM
					}
				}

				if (high_limit[PwmId] = low_limit[PwmId]) {
					if (high_limit[PwmId] == 0) {
						high_limit[PwmId] = max_v[PwmId];
					}
				}

				if (PWMenabledChannels[i] == true) {
					float tmp = PWM_value[i] + PWM_OffSet[i];

					if (tmp <= low_limit[i]) {
						tmp = low_limit[i];
					}
					else if (tmp >= high_limit[i]) {
						tmp = high_limit[i];
					}

					tmp = tmp * PWM_SCale[i];

					PWMduty[i] = (uint32_t)((tmp / max_v[i]) * PWMperiod);
				}
				else {
					PWMduty[i] = 0;
				}

				*(IO_data->adcout[PwmId]).deb_setval = PWMduty[i];

				if (IO_data->adcout[PwmId].scale != PWM_SCale[i]) {
					IO_data->adcout[PwmId].scale = PWM_SCale[i];
				}

				if (IO_data->adcout[PwmId].max_v != max_v[i]) {
					IO_data->adcout[PwmId].max_v = max_v[i];
				}

				if (IO_data->adcout[PwmId].high_limit != high_limit[i]) {
					IO_data->adcout[PwmId].high_limit = high_limit[i];
				}

				if (IO_data->adcout[PwmId].low_limit != low_limit[i]) {
					IO_data->adcout[PwmId].low_limit = low_limit[i];

				}

				if (dev->PWM.PWMenabledChannels[i] != PWMenabledChannels[i]) {
					dev->PWM.PWMenabledChannels[i] = PWMenabledChannels[i];
					doPwmConfig = true;
				}

				if (dev->PWM.PWMduty[i] != PWMduty[i]) {
					dev->PWM.PWMduty[i] = PWMduty[i];
					AnalogIOSet = true;
				}
			}
		}
		else {
			rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PK_PWMConfigurationGet(dev) failed\n", __FILE__, __FUNCTION__);
		}

		if (AnalogIOSet == true) {
			rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_PWMUpdate(dev)\n", __FILE__, __FUNCTION__);
			PK_PWMUpdate(dev);
			usleep(sleepdur);
			if (PK_PWMUpdate(dev) != PK_OK) {
				rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PK_PWMUpdate(dev) failed\n", __FILE__, __FUNCTION__);
				PK_PWMUpdate(dev);
			}
			else{
				rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_PWMUpdate(dev) OK\n", __FILE__, __FUNCTION__);
			}
			usleep(sleepdur);
		}
	}
	else {
		
		*(IO_data->adcout_deb_outv) = 150;
	}
	if (loopPins == true) { // gets IO data and checks return value
		rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: read IO data\n", __FILE__, __FUNCTION__);

		sPoKeysPinData iPin;
		int AnalogPinOffset = 40;
		int AnalogPinCount = 7;

		int PwmPinOffset = 17;
		int PwmPinCount = 6;

		for (int i = 0; i < dev->info.iPinCount - 1; i++) {
			rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: Pin %d\n", __FILE__, __FUNCTION__, i);
			*(IO_data->deb_out) = 2320 + i;
			int AinNr = i - AnalogPinOffset;

			if ((AnalogIOGet = true) && (AinNr >= 0) && (AinNr < AnalogPinCount)) {
				if (IO_data->adcin[AinNr].scale == 0) {
					IO_data->adcin[AinNr].scale = 1;
				}

				float ainVal = 3.3 * dev->Pins[i].AnalogValue / 4096;

				*(IO_data->adcin[AinNr].value_raw) = ainVal;
				rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: IO_data->adcin[AinNr].value_raw(%d) = %f\n", __FILE__, __FUNCTION__, AinNr, IO_data->adcin[AinNr].value_raw);
				*(IO_data->adcin[AinNr].value) = ainVal * IO_data->adcin[AinNr].scale - IO_data->adcin[AinNr].offset;
				rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: adc value %d = %f\n", __FILE__, __FUNCTION__, AinNr, IO_data->adcin[AinNr].value);
			}

			*(IO_data->deb_out) = 2321 + i;
			if (PK_CheckPinCapability(dev, i, PK_AllPinCap_digitalInput) == 1) {
				if (DigitalIOGet == true) {
					rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: DigitalValueGet\n", __FILE__, __FUNCTION__);
					rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: dev->Pins[i].DigitalValueGet = %d\n", __FILE__, __FUNCTION__, dev->Pins[i].DigitalValueGet);

					if (dev->Pins[i].DigitalValueGet == 0) {
						rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: dev->Pins[%d].DigitalValueGet = 0\n", __FILE__, __FUNCTION__, i);
						// Pins_DigitalValueGet(i)=false;
						*(IO_data->Pin[i]).digin_in = false;
						*(IO_data->Pin[i].digin_in_not) = true;
					}
					else {
						rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: dev->Pins[%d].DigitalValueGet = 1\n", __FILE__, __FUNCTION__, i);
						// Pins_DigitalValueGet(i)=true;
						*(IO_data->Pin[i]).digin_in = true;
						*(IO_data->Pin[i]).digin_in_not = false;

					}
				}
			}

			if (PK_CheckPinCapability(dev, i, PK_AllPinCap_digitalOutput) == 1) {
				if (IO_data->Pin[i].DigitalValueSet_ignore == false) {
					bool setDigoutvalue = false;
					if (*(IO_data->Pin[i]).digout_out == 1) {
						rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: dev->Pins[%d].digout_out = 1\n", __FILE__, __FUNCTION__, i);
						if (IO_data->Pin[i].digout_invert == 0) {
							rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: dev->Pins[%d].digout_invert = 0\n", __FILE__, __FUNCTION__, i);
							setDigoutvalue = true;
						}
					}
					else {
						if (IO_data->Pin[i].digout_invert == 1) {
							rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: dev->Pins[%d].digout_invert = 1\n", __FILE__, __FUNCTION__, i);
							setDigoutvalue = true;
						}
					}
					rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: setDigoutvalue = %d\n", __FILE__, __FUNCTION__, setDigoutvalue);
					if ((setDigoutvalue == true) && (dev->Pins[i].DigitalValueSet == 0)) {
						rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: dev->Pins[%d].DigitalValueSet = 1\n", __FILE__, __FUNCTION__, i);
						dev->Pins[i].DigitalValueSet = 1;
						DigitalIOSet = true;
					}
					else if ((setDigoutvalue == false) && (dev->Pins[i].DigitalValueSet == 1)) {
						dev->Pins[i].DigitalValueSet = 0;
						DigitalIOSet = true;
					}
					rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: DigitalValueSet[%d] = %d\n", __FILE__, __FUNCTION__, i, dev->Pins[i].DigitalValueSet);
				}
				else {
					rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: IO_data->Pin[%d].DigitalValueSet_ignore = true\n", __FILE__, __FUNCTION__, i);
				}
			}
			if (PK_CheckPinCapability(dev, i, PK_AllPinCap_digitalCounter) == 1) {
				// Pins_DigitalCounterAvailable(i)=dev->Pins[i].DigitalCounterAvailable;
				rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: DigitalCounterAvailable[%d] = %d\n", __FILE__, __FUNCTION__, i, dev->Pins[i].DigitalCounterAvailable);
				if (dev->Pins[i].DigitalCounterAvailable) {
					// Pins_DigitalCounterValue(i) = dev->Pins[i].DigitalCounterValue;
					rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s:counter value %d = %d\n", __FILE__, __FUNCTION__, i, dev->Pins[i].DigitalCounterValue);
					dev->Pins[i].DigitalCounterValue = dev->Pins[i].DigitalCounterValue;
					// Pins_CounterOptions(i) = dev->Pins[i].CounterOptions;
				}
			}
			// Pins_MappingType(i)=dev->Pins[i].MappingType;
			// Pins_KeyCodeMacroID(i)=dev->Pins[i].KeyCodeMacroID;
			// Pins_KeyModifier(i)=dev->Pins[i].KeyModifier;
			// Pins_downKeyCodeMacroID(i)=dev->Pins[i].downKeyCodeMacroID;
			// Pins_downKeyModifier(i)=dev->Pins[i].downKeyModifier;
			// Pins_upKeyCodeMacroID(i)=dev->Pins[i].upKeyCodeMacroID;
			// Pins_upKeyModifier(i)=dev->Pins[i].upKeyModifier;
			// dev->Pins[i].preventUpdate=Pins_preventUpdate(i);

			*(IO_data->deb_out) = 2330 + i;
		}


		*(IO_data->deb_out) = 236;
		rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_DigitalIOSet(dev)\n", __FILE__, __FUNCTION__);
		if (DigitalIOSet == true) {
			rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_DigitalIOSet(dev)\n", __FILE__, __FUNCTION__);
			if (PK_DigitalIOSet(dev) != PK_OK) {
				rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_DigitalIOSet(dev) failed\n", __FILE__, __FUNCTION__);
			}
			else {
				rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_DigitalIOSet(dev) passed\n", __FILE__, __FUNCTION__);
			}
			usleep(sleepdur);
			if (PK_DigitalIOSet(dev) != PK_OK) {
				rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_DigitalIOSet(dev) failed\n", __FILE__, __FUNCTION__);
			}
			else {
				rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_DigitalIOSet(dev) passed\n", __FILE__, __FUNCTION__);
			}
			usleep(sleepdur);
		}

		*(IO_data->deb_out) = 237;
	}
	else {
		*(IO_data->deb_out) = 250;
	}
}

void PKIO_Setup(sPoKeysDevice* dev, bool readonly) {
	bool PinConfigurationSet = false;
	//bool readonly = false;


	// Setting PinFunction
	if (PK_PinConfigurationGet(dev) == PK_OK) {
		rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_PinConfigurationGet(dev) OK\n", __FILE__, __FUNCTION__);
		//readonly = true;

		for (int i = 0; i < dev->info.iPinCount - 1; i++) {

			if (readonly || IO_data->Pin[i].PinFunction == 0) {
				IO_data->Pin[i].PinFunction = dev->Pins[i].PinFunction;
			}
			else {
				if (PK_CheckPinCapability(dev, i, IO_data->Pin[i].PinFunction) != 1) {
					IO_data->Pin[i].PinFunction = 0;
				}
				else if (dev->Pins[i].PinFunction != IO_data->Pin[i].PinFunction) {
					dev->Pins[i].PinFunction = IO_data->Pin[i].PinFunction;
					PinConfigurationSet = true;
				}

			}
		}


		if (PinConfigurationSet == true) {
			rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_PinConfigurationSet(dev)\n", __FILE__, __FUNCTION__);
			PK_PinConfigurationSet(dev);
			if (PK_PinConfigurationSet(dev) != PK_OK) {
				rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PK_PinConfigurationSet(dev) failed\n", __FILE__, __FUNCTION__);
				PK_PinConfigurationSet(dev);
			}
			else{
				rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_PinConfigurationSet(dev) passed\n", __FILE__, __FUNCTION__);
			}
			usleep(sleepdur);
		}
	}


	if (dev->info.iPWMCount) {

		*(IO_data->adcout_deb_outv) = 100;
		*(IO_data->adcout_deb_outv) = 100;
		uint32_t PWMperiod; // PWM period, shared among all channels
		bool doPwmConfig = false;

		uint32_t PWMduty[6];		   // PWM duty cycles (range between 0 and PWM period)
		uint8_t PWMenabledChannels[6]; // List of enabled PWM channels
		uint8_t PWMpinIDs[6];
		float PWM_SCale[6];
		float max_v[6];
		float high_limit[6];
		float low_limit[6];
		float PWM_value[6];
		float PWM_OffSet[6];
		rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_PWMConfigurationGet(dev)\n", __FILE__, __FUNCTION__);

		if (PK_PWMConfigurationGet(dev) == PK_OK) {

			rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_PWMConfigurationGet(dev) OK\n", __FILE__, __FUNCTION__);
			PWMperiod = IO_data->adcout_pwm_period;
			usleep(sleepdur);

			if (PWMperiod == 0) {
				PWMperiod = 2500;
			}
			PWMperiod = 2500;
			if (dev->PWM.PWMperiod != PWMperiod) {
				dev->PWM.PWMperiod = PWMperiod;
				doPwmConfig = true;
			}

			if (IO_data->adcout_pwm_period != PWMperiod) {
				IO_data->adcout_pwm_period = PWMperiod;
			}

			for (int i = 0; i < 6; i++) {
				int PwmId = 5 - i;
				*(IO_data->adcout[PwmId]).deb_setval = 100;
				//adcout_deb_out(PwmId) = 100;

				PWMduty[i] = dev->PWM.PWMduty[i];			  // PWM duty cycles (range between 0 and PWM period)
				PWMenabledChannels[i] = *(IO_data->adcout[PwmId]).enable; // List of enabled PWM channels
				PWMpinIDs[i] = dev->PWM.PWMpinIDs[i];
				IO_data->adcout[PwmId].PinId = PWMpinIDs[i];
				//adcout_PinId(PwmId) = PWMpinIDs[i];
				PWM_SCale[i] = 1;

				PWM_value[i] = *(IO_data->adcout[PwmId]).value;
				PWM_OffSet[i] = IO_data->adcout[PwmId].offset;
				max_v[i] = IO_data->adcout[PwmId].max_v;
				high_limit[i] = IO_data->adcout[PwmId].high_limit;
				low_limit[i] = IO_data->adcout[PwmId].low_limit;



				if (PWMenabledChannels[i] == true) {
					if (max_v[PwmId] == 0) {
						if ((dev->DeviceData.DeviceTypeID == PK_DeviceID_PoKeys57CNC) && (PWMpinIDs[i] = 17)) {
							max_v[PwmId] = 10.000; // Pin17 0-10V
						}
						else {
							max_v[PwmId] = 5.000; // usually pokeys57 provide 0..5V PWM
						}
					}

					if (high_limit[PwmId] = low_limit[PwmId]) {
						if (high_limit[PwmId] == 0) {
							high_limit[PwmId] = max_v[PwmId];
						}
					}

				}



				if (IO_data->adcout[PwmId].scale != PWM_SCale[i]) {
					IO_data->adcout[PwmId].scale = PWM_SCale[i];
				}

				if (IO_data->adcout[PwmId].max_v != max_v[i]) {
					IO_data->adcout[PwmId].max_v = max_v[i];
				}

				if (IO_data->adcout[PwmId].high_limit != high_limit[i]) {
					IO_data->adcout[PwmId].high_limit = high_limit[i];
				}

				if (IO_data->adcout[PwmId].low_limit != low_limit[i]) {
					IO_data->adcout[PwmId].low_limit = low_limit[i];

				}

				if (dev->PWM.PWMenabledChannels[i] != PWMenabledChannels[i]) {
					dev->PWM.PWMenabledChannels[i] = PWMenabledChannels[i];
					doPwmConfig = true;
				}

			}

			if (doPwmConfig == true) {
				rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_PWMConfigurationSet(dev)\n", __FILE__, __FUNCTION__);
				PK_PWMConfigurationSet(dev);
				if (PK_PWMConfigurationSet(dev) != PK_OK) {
					rtapi_print_msg(RTAPI_MSG_ERR, "PoKeys: %s:%s: PK_PWMConfigurationSet(dev) failed\n", __FILE__, __FUNCTION__);
					PK_PWMConfigurationSet(dev);
				}
				else{
					rtapi_print_msg(RTAPI_MSG_DBG, "PoKeys: %s:%s: PK_PWMConfigurationSet(dev) passed\n", __FILE__, __FUNCTION__);
				}
				usleep(sleepdur);
			}
		}
	}


}

#ifdef MODULE_INFO
MODULE_INFO(linuxcnc, "pin:adcout.#.deb.out:u32:6:out::None:None");
MODULE_INFO(linuxcnc, "pin:adcout.#.deb.setval:u32:6:out::None:None");
MODULE_INFO(linuxcnc, "pin:adcout.deb.outv:u32:0:out::None:None");

MODULE_INFO(linuxcnc, "pin:counter.#.value:u32:55:out::None:None");
MODULE_INFO(linuxcnc, "pin:adcin.#.value-raw:float:7:out::None:None");
MODULE_INFO(linuxcnc, "pin:adcin.#.value:float:7:out::None:None");
MODULE_INFO(linuxcnc, "param:adcin.#.scale:float:7:rw:The input voltage (or current) will be multiplied by scale before being output to value.:None:None");
MODULE_INFO(linuxcnc, "param:adcin.#.offset:float:7:rw:This will be subtracted from the hardware input voltage (or current) after the scale multiplier has been applied.:None:None");
MODULE_INFO(linuxcnc, "pin:digin.#.in:bit:55:out::None:None");
MODULE_INFO(linuxcnc, "pin:digin.#.in-not:bit:55:out::None:None");
MODULE_INFO(linuxcnc, "pin:digout.#.out:bit:55:in::None:None");
MODULE_INFO(linuxcnc, "param:digout.#.invert:bit:55:rw:If TRUE, out is inverted before writing to the hardware.:None:None");

MODULE_INFO(linuxcnc, "pin:adcout.#.value:float:6:in::None:None");
MODULE_INFO(linuxcnc, "pin:adcout.#.enable:bit:6:in::None:None");
MODULE_INFO(linuxcnc, "param:adcout.#.offset:float:6:rw:This will be added to the value before the hardware is updated:None:None");
MODULE_INFO(linuxcnc, "param:adcout.#.scale:float:6:rw:This should be set so that an input of 1 on the value pin will cause 1V:None:None");
MODULE_INFO(linuxcnc, "param:adcout.#.high_limit:float:6:rw:When calculating the value to output to the hardware, if value +offset is greater than high_limit, then high_limit will be used instead.:None:None");
MODULE_INFO(linuxcnc, "param:adcout.#.low_limit:float:6:rw:When calculating the value to output to the hardware, if value +offset is less than low_limit, then low_limit will be used instead:None:None");
MODULE_INFO(linuxcnc, "param:adcout.#.max_v:float:6:rw:max v:None:None");
MODULE_INFO(linuxcnc, "param:adcout.#.PinId:u32:6:r:max v:None:None");
MODULE_INFO(linuxcnc, "param:adcout.pwm.period:u32:0:rw:PWM period, shared among all channels:None:None");
#endif // MODULE_INFO