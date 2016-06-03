//
// fslurp, Copyright (c) 2012 David Stone <ds.fslurp@asif.com>
//
// $RCSfile: utils.cpp,v $
// $Revision: 1.6 $
// $Date: 2012/10/15 15:44:37 $
//
// Permission to use, copy, modify, distribute, and sell this software and its
// documentation for any purpose is hereby granted without fee, provided that
// the above copyright notice appear in all copies and that both the
// copyright notice and this permission notice appear in supporting
// documentation.  No representations are made about the suitability of this
// software for any purpose.  It is provided "as is" without express or 
// implied warranty.
//

#include "fslurp.h"

static const char* FroniusDeviceOptionTable[] =
{
	"Interface Card",		// 0
	"Inverter",			// 1
	"Sensor Card",			// 2
	"Data Logger",			// 3
	"Option ID Reserved"		// 4
	"String Control"		// 5
};

time_t	Utils::timestamp_;
char	Utils::ctimeBuf_[4096];

const char*
Utils::decodeCommand(unsigned char command)
{
	const char*	decodedCommand;

	switch (command)
	{
		case FRONIUS_CMD_GET_VERSION:
			decodedCommand = "getVersion";
			break;
		case FRONIUS_CMD_GET_DEVICE_TYPE:
			decodedCommand = "getDeviceType";
			break;
		case FRONIUS_CMD_GET_DATE_TIME:
			decodedCommand = "getDateTime";
			break;
		case FRONIUS_CMD_GET_ACTIVE_INVERTER_NUMBERS:
			decodedCommand = "getActiveInverterNumbers";
			break;
		case FRONIUS_CMD_GET_ACTIVE_SENSOR_CARDS:
			decodedCommand = "getActiveSensorCards";
			break;
		case FRONIUS_CMD_GET_SOLAR_NET_STATUS:
			decodedCommand = "getSolarNetStatus";
			break;
		case FRONIUS_CMD_GET_POWER_NOW:
			decodedCommand = "getPowerNow";
			break;
		case FRONIUS_CMD_GET_ENERGY_TOTAL:
			decodedCommand = "getEnergyTotal";
			break;
		case FRONIUS_CMD_GET_ENERGY_DAY:
			decodedCommand = "getEnergyDay";
			break;
		case FRONIUS_CMD_GET_ENERGY_YEAR:
			decodedCommand = "getEnergyYear";
			break;
		case FRONIUS_CMD_GET_AC_CURRENT_NOW:
			decodedCommand = "getACCurrentNow";
			break;
		case FRONIUS_CMD_GET_AC_VOLTAGE_NOW:
			decodedCommand = "getACVoltageNow";
			break;
		case FRONIUS_CMD_GET_AC_FREQUENCY_NOW:
			decodedCommand = "getACFrequencyNow";
			break;
		case FRONIUS_CMD_GET_DC_CURRENT_NOW:
			decodedCommand = "getDCCurrentNow";
			break;
		case FRONIUS_CMD_GET_DC_VOLTAGE_NOW:
			decodedCommand = "getDCVoltageNow";
			break;
		case FRONIUS_CMD_GET_YIELD_DAY:
			decodedCommand = "getYieldDay";
			break;
		case FRONIUS_CMD_GET_MAXIMUM_POWER_DAY:
			decodedCommand = "getMaximumPowerDay";
			break;
		case FRONIUS_CMD_GET_MAXIMUM_AC_VOLTAGE_DAY:
			decodedCommand = "getMaximumACVoltageDay";
			break;
		case FRONIUS_CMD_GET_MINIMUM_AC_VOLTAGE_DAY:
			decodedCommand = "getMinimumACVoltageDay";
			break;
		case FRONIUS_CMD_GET_MAXIMUM_DC_VOLTAGE_DAY:
			decodedCommand = "getMaximumDCVoltageDay";
			break;
		case FRONIUS_CMD_GET_OPERATING_HOURS_DAY:
			decodedCommand = "getOperatingHoursDay";
			break;
		case FRONIUS_CMD_GET_YIELD_YEAR:
			decodedCommand = "getYieldYear";
			break;
		case FRONIUS_CMD_GET_MAXIMUM_POWER_YEAR:
			decodedCommand = "getMaximumPowerYear";
			break;
		case FRONIUS_CMD_GET_MAXIMUM_AC_VOLTAGE_YEAR:
			decodedCommand = "getMaximumACVoltageYear";
			break;
		case FRONIUS_CMD_GET_MINIMUM_AC_VOLTAGE_YEAR:
			decodedCommand = "getMinimumACVoltageYear";
			break;
		case FRONIUS_CMD_GET_MAXIMUM_DC_VOLTAGE_YEAR:
			decodedCommand = "getMaximumDCVoltageYear";
			break;
		case FRONIUS_CMD_GET_OPERATING_HOURS_YEAR:
			decodedCommand = "getOperatingHoursYear";
			break;
		case FRONIUS_CMD_GET_MAXIMUM_POWER_TOTAL:
			decodedCommand = "getMaximumPowerTotal";
			break;
		case FRONIUS_CMD_GET_MAXIMUM_AC_VOLTAGE_TOTAL:
			decodedCommand = "getMaximumACVoltageTotal";
			break;
		case FRONIUS_CMD_GET_MINIMUM_AC_VOLTAGE_TOTAL:
			decodedCommand = "getMinimumACVoltageTotal";
			break;
		case FRONIUS_CMD_GET_MAXIMUM_DC_VOLTAGE_TOTAL:
			decodedCommand = "getMaximumDCVoltageTotal";
			break;
		case FRONIUS_CMD_GET_OPERATING_HOURS_TOTAL:
			decodedCommand = "getOperatingHoursTotal";
			break;
		case FRONIUS_CMD_GET_TEMPERATURE_CH1_NOW:
			decodedCommand = "getTemperatureCh1Now";
			break;
		case FRONIUS_CMD_GET_TEMPERATURE_CH2_NOW:
			decodedCommand = "getTemperatureCh2Now";
			break;
		case FRONIUS_CMD_GET_IRRADIANCE_NOW:
			decodedCommand = "getIrradianceNow";
			break;
		case FRONIUS_CMD_GET_DIGITAL_CH1_NOW:
			decodedCommand = "getDigitalCh1Now";
			break;
		case FRONIUS_CMD_GET_DIGITAL_CH2_NOW:
			decodedCommand = "getDigitalCh2Now";
			break;
		case FRONIUS_CMD_GET_MINIMUM_TEMPERATURE_CH1_DAY:
			decodedCommand = "getMinimumTemperatureCh1Day";
			break;
		case FRONIUS_CMD_GET_MAXIMUM_TEMPERATURE_CH1_DAY:
			decodedCommand = "getMaximumTemperatureCh1Day";
			break;
		case FRONIUS_CMD_GET_MINIMUM_TEMPERATURE_CH2_DAY:
			decodedCommand = "getMinimumTemperatureCh2Day";
			break;
		case FRONIUS_CMD_GET_MAXIMUM_TEMPERATURE_CH2_DAY:
			decodedCommand = "getMaximumTemperatureCh2Day";
			break;
		case FRONIUS_CMD_GET_MAXIMUM_IRRADIANCE_DAY:
			decodedCommand = "getMaximumIrradianceDay";
			break;
		case FRONIUS_CMD_GET_MAXIMUM_DIGITAL_CH1_DAY:
			decodedCommand = "getMaximumDigitalCh1Day";
			break;
		case FRONIUS_CMD_GET_MAXIMUM_DIGITAL_CH2_DAY:
			decodedCommand = "getMaximumDigitalCh2Day";
			break;
		default:
			// FIX THIS, DS: display value
			decodedCommand = "Unknown command";
			break;
	}

	return decodedCommand;
}

const char*
Utils::decodeDeviceOption(unsigned char deviceOption)
{
	assert(deviceOption < NUM_ELEMS(FroniusDeviceOptionTable));
	return FroniusDeviceOptionTable[deviceOption];
}

const char*
Utils::decodeErrorInformation(unsigned char errorInfo)
{
	const char*	decodedError;

	switch (errorInfo)
	{
		case FRONIUS_PROTOCOL_ERROR_UNKNOWN_COMMAND:
			decodedError = "unknown command";
			break;
		case FRONIUS_PROTOCOL_ERROR_TIMEOUT:
			decodedError = "timeout";
			break;
		case FRONIUS_PROTOCOL_ERROR_INCORRECT_DATA_STRUCTURE:
			decodedError = "incorrect data structure";
			break;
		case FRONIUS_PROTOCOL_ERROR_COMMAND_QUEUE_FULL:
			decodedError = "command queue full";
			break;
		case FRONIUS_PROTOCOL_ERROR_DEVICE_OR_OPTION_NA:
			decodedError = "device or option not available";
			break;
		case FRONIUS_PROTOCOL_ERROR_NO_RESPONSE:
			decodedError = "no response";
			break;
		case FRONIUS_PROTOCOL_ERROR_SENSOR_ERROR:
			decodedError = "sensor error";
			break;
		case FRONIUS_PROTOCOL_ERROR_SENSOR_NOT_ACTIVE:
			decodedError = "sensor not active";
			break;
		case FRONIUS_PROTOCOL_ERROR_INCORRECT_COMMAND:
			decodedError = "incorrect command";
			break;
		case FRONIUS_PROTOCOL_ERROR_DEVICE_ID_COLLISION:
			decodedError = "device id collision";
			break;
		default:
			// FIX THIS, DS: encode value
			decodedError = "unknown error information";
			break;
	}

	return decodedError;
}

void
Utils::hexDump(unsigned char* buffer, unsigned int length)
{
	unsigned int	i;

	assert(buffer != NULL);

	if (length == 0)
	{
		return;
	}

	for (i = 0; i < length; i++)
	{
		if ((i % 16) == 0)
		{
			fprintf(stderr, "\t");
		}

		fprintf(stderr, "%#02x ", buffer[i]);

		if ((i != 0) && ((i % 16) == 0))
		{
			fprintf(stderr, "\n");
		}
	}

	if ((i % 16) != 0)
	{
		fprintf(stderr, "\n");
	}
}

// FIX THIS, DS: clean up and condense
const char*
Utils::getValue(struct MeasuredValue measuredValue)
{
	unsigned int	value;
	static char	valueString[64];
	signed char	exponent;

	exponent = measuredValue.exponent;

	if (exponent == 0xb)
	{
		return ("overflow");
	}
	else if (exponent == -4)
	{
		return ("underflow");
	}

	valueString[0] = 0;

	value = measuredValue.lsb;
	value += ((measuredValue.msb) << 8);

	if (exponent == 0)
	{
		sprintf(valueString, "%d", value);
		return valueString;
	}
	else if (exponent > 0)
	{
		sprintf(valueString, "%d", value * (int)(pow(10, exponent)));
	}
	else
	{
		assert(exponent < 0);

		double	gValue;

		gValue = value;
		gValue *= pow(10, exponent);

		sprintf(valueString, "%.2f", gValue);
	}

	return valueString;
}

const char*
Utils::getThousandsValue(struct MeasuredValue measuredValue)
{
	unsigned int	value;
	static char	valueString[64];
	signed char	exponent;

	exponent = measuredValue.exponent;

	if (exponent == 0xb)
	{
		return ("overflow");
	}
	else if (exponent == -4)
	{
		return ("underflow");
	}

	valueString[0] = 0;

	value = measuredValue.lsb;
	value += ((measuredValue.msb) << 8);

	exponent -= 3;

	if (exponent == 0)
	{
		sprintf(valueString, "%d", value);
		return valueString;
	}
	else if (exponent > 0)
	{
		sprintf(valueString, "%d", value * (int)(pow(10, exponent)));
	}
	else
	{
		assert(exponent < 0);

		double	gValue;

		gValue = value;
		gValue *= pow(10, exponent);

		sprintf(valueString, "%.2f", gValue);
	}

	return valueString;
}

const char*
Utils::getHoursValue(struct MeasuredValue measuredValue)
{
	double		gValue;
	static char	valueString[64];
	signed char	exponent;

	exponent = measuredValue.exponent;

	if (exponent == 0xb)
	{
		return ("overflow");
	}
	else if (exponent == -4)
	{
		return ("underflow");
	}

	valueString[0] = 0;

	gValue = measuredValue.lsb;
	gValue += ((measuredValue.msb) << 8);
	gValue /= 60;
	gValue *= pow(10, exponent);
	sprintf(valueString, "%.2f", gValue);

	return valueString;
}

const char*
Utils::getTimestamp(Args& args)
{
	struct tm	*tmResult;
	time_t		timestamp;

	timestamp = args.getTimestamp();

	tmResult = localtime(&timestamp);
	strftime(ctimeBuf_, sizeof(ctimeBuf_), args.getTimeFormat(), tmResult);

	return ctimeBuf_;
}

void
Utils::displayTimestamp(unsigned char fieldLabelWidth, Args& args)
{
	if (args.getDelimiter() == NULL)
	{
		printf("%-*s : %s\n",
			fieldLabelWidth,
			"Time",
			Utils::getTimestamp(args));
	}
	else
	{
		printf("%s%s", Utils::getTimestamp(args), args.getDelimiter());
	}
}

void
Utils::displaySeparator(Args& args)
{
	if (args.getDelimiter() == NULL)
	{
		printf("--------------------------------------------------\n");
	}
}

const char*
Utils::strCat(const char* str1, const char* str2)
{
	enum { BufferSize = 48 };
	static char strBuf[BufferSize];

	assert((strlen(str1) + strlen(str2)) < BufferSize);
	strcpy(strBuf, str1);
	strcat(strBuf, str2);

	return strBuf;
}
