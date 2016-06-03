//
// fslurp, Copyright (c) 2012 David Stone <ds.fslurp@asif.com>
//
// $RCSfile: simSolarNet1i0s.cpp,v $
// $Revision: 1.1 $
// $Date: 2012/10/20 01:35:50 $
//
// Permission to use, copy, modify, distribute, and sell this software and its
// documentation for any purpose is hereby granted without fee, provided that
// the above copyright notice appear in all copies and that both the
// copyright notice and this permission notice appear in supporting
// documentation.  No representations are made about the suitability of this
// software for any purpose.  It is provided "as is" without express or 
// implied warranty.
//

#include "simulator.h"

static unsigned char GetVersionHeader[] =
{
	0x80, 0x80, 0x80, 0x4, 0, 0, 0x1
};

static unsigned char GetVersionData[] =
{
	0x1, 0x1, 0x2, 0x2, 0xb 
};

static unsigned char GetSolarNetStatusHeader[] =
{
	0x80, 0x80, 0x80, 0x1, 0, 0, 0x06, 
};

static unsigned char GetSolarNetStatusData[] =
{
	1, 0x8
};

static unsigned char GetActiveInverterNumbersHeader[] =
{
	0x80, 0x80, 0x80, 0x2, 0, 0, 0x4
};

static unsigned char GetActiveInverterNumbersData[] =
{
	0x1, 0x2, 0x9 
};

static unsigned char GetActiveSensorCardsHeader[] =
{
	0x80, 0x80, 0x80, 0, 0, 0, 0x5
};

static unsigned char GetActiveSensorCardsData[] =
{
	0x5
};

static unsigned char GetInverterDeviceTypeHeader[] =
{
	0x80, 0x80, 0x80, 0x1, 0x1, 0x1, 0x2
};

static unsigned char GetInverterDeviceTypeData[] =
{
	0xd9, 0xde
};

static unsigned char GetSensorCardDeviceTypeHeader[] =
{
	0x80, 0x80, 0x80, 0x1, 0x1, 0x1, 0x2
};

static unsigned char GetSensorCardDeviceTypeData[] =
{
	0xfe, 0x3
};

static unsigned char GetPowerNowHeader[] =
{
	0x80, 0x80, 0x80, 0x3, 0x1, 0x1, 0x10
};

static unsigned char GetPowerNowData[] =
{
	0, 0x4b, 0, 0x60
};

static unsigned char GetACCurrentNowHeader[] =
{
	0x80, 0x80, 0x80, 0x3, 0x1, 0x1, 0x14
};

static unsigned char GetACCurrentNowData[] =
{
	0, 0x1f, 0xfe, 0x36, 
};

static unsigned char GetACVoltageNowHeader[] =
{
	0x80, 0x80, 0x80, 0x3, 0x1, 0x1, 0x15
};

static unsigned char GetACVoltageNowData[] =
{
	0, 0xf1, 0, 0xb
};

static unsigned char GetACFrequencyNowHeader[] =
{
	0x80, 0x80, 0x80, 0x3, 0x1, 0x1, 0x16
};

static unsigned char GetACFrequencyNowData[] =
{
	0x17, 0x71, 0xfe, 0xa1
};

static unsigned char GetDCCurrentNowHeader[] =
{
	0x80, 0x80, 0x80, 0x3, 0x1, 0x1, 0x17
};

static unsigned char GetDCCurrentNowData[] =
{
	0, 0x14, 0xfe, 0x2e
};

static unsigned char GetDCVoltageNowHeader[] =
{
	0x80, 0x80, 0x80, 0x3, 0x1, 0x1, 0x18
};

static unsigned char GetDCVoltageNowData[] =
{
	0x1, 0x4c, 0, 0x6a
};

static unsigned char GetEnergyDayHeader[] =
{
	0x80, 0x80, 0x80, 0x3, 0x1, 0x1, 0x12
};

static unsigned char GetEnergyDayData[] =
{
	0, 0x16, 0x3, 0x30
};

static unsigned char GetMaximumPowerDayHeader[] =
{
	0x80, 0x80, 0x80, 0x3, 0x1, 0x1, 0x1a
};

static unsigned char GetMaximumPowerDayData[] =
{
	0xc, 0xde, 0, 0x9
};

static unsigned char GetMaximumACVoltageDayHeader[] =
{
	0x80, 0x80, 0x80, 0x3, 0x1, 0x1, 0x1b
};

static unsigned char GetMaximumACVoltageDayData[] =
{
	0x1, 0xf, 0, 0x30
};

static unsigned char GetMinimumACVoltageDayHeader[] =
{
	0x80, 0x80, 0x80, 0x3, 0x1, 0x1, 0x1c
};

static unsigned char GetMinimumACVoltageDayData[] =
{
	0, 0xf0, 0, 0x11
};

static unsigned char GetMaximumDCVoltageDayHeader[] =
{
	0x80, 0x80, 0x80, 0x3, 0x1, 0x1, 0x1d
};

static unsigned char GetMaximumDCVoltageDayData[] =
{
	0x1, 0xc8, 0, 0xeb
};

static unsigned char GetOperatingHoursDayHeader[] =
{
	0x80, 0x80, 0x80, 0x3, 0x1, 0x1, 0x1e
};

static unsigned char GetOperatingHoursDayData[] =
{
	0x2, 0xaa, 0, 0xcf
};

static unsigned char GetEnergyTotalHeader[] =
{
	0x80, 0x80, 0x80, 0x3, 0x1, 0x1, 0x11
};

static unsigned char GetEnergyTotalData[] =
{
	0x42, 0x2, 0x3, 0x5d
};

static unsigned char GetMaximumPowerTotalHeader[] =
{
	0x80, 0x80, 0x80, 0x3, 0x1, 0x1, 0x26
};

static unsigned char GetMaximumPowerTotalData[] =
{
	0x10, 0xed, 0, 0x28
};

static unsigned char GetMaximumACVoltageTotalHeader[] =
{
	0x80, 0x80, 0x80, 0x3, 0x1, 0x1, 0x27

};
static unsigned char GetMaximumACVoltageTotalData[] =
{
	0x1, 0x1b, 0, 0x48
};

static unsigned char GetMinimumACVoltageTotalHeader[] =
{
	0x80, 0x80, 0x80, 0x3, 0x1, 0x1, 0x28
};

static unsigned char GetMinimumACVoltageTotalData[] =
{
	0, 0xc8, 0, 0xf5
};

static unsigned char GetMaximumDCVoltageTotalHeader[] =
{
	0x80, 0x80, 0x80, 0x3, 0x1, 0x1, 0x29
};

static unsigned char GetMaximumDCVoltageTotalData[] =
{
	0x2, 0x18, 0, 0x48
};

static unsigned char GetOperatingHoursTotalHeader[] =
{
	0x80, 0x80, 0x80, 0x3, 0x1, 0x1, 0x2a
};

static unsigned char GetOperatingHoursTotalData[] =
{
	0x1c, 0x97, 0x2, 0xe4
};

static unsigned char GetTemperatureCh1NowHeader[] =
{
	0x80, 0x80, 0x80, 0x3, 0x2, 0x4, 0xe0
};

static unsigned char GetTemperatureCh1NowData[] =
{
	0x7, 0x41, 0xfe, 0x2f
};

static unsigned char GetTemperatureCh2NowHeader[] =
{
	0x80, 0x80, 0x80, 0x3, 0x2, 0x4, 0xe1
};

static unsigned char GetTemperatureCh2NowData[] =
{
	0x7, 0x42, 0xfe, 0x31
};

static unsigned char GetIrradianceNowHeader[] =
{
	0x80, 0x80, 0x80, 0x3, 0x2, 0x4, 0xe2
};

static unsigned char GetIrradianceNowData[] =
{
	0x1, 0x1, 0x0, 0xed
};

static unsigned char GetDigitalCh1NowHeader[] =
{
	0x80, 0x80, 0x80, 0x3, 0x2, 0x4, 0xf2
};

static unsigned char GetDigitalCh1NowData[] =
{
	1, 0, 0x0, 0xfc
};

static unsigned char GetDigitalCh2NowHeader[] =
{
	0x80, 0x80, 0x80, 0x3, 0x2, 0x4, 0xf3
};

static unsigned char GetDigitalCh2NowData[] =
{
	1, 1, 0, 0xfe
};

static unsigned char GetMinimumTemperatureCh1DayHeader[] =
{
	0x80, 0x80, 0x80, 0x3, 0x2, 0x4, 0xe3
};

static unsigned char GetMinimumTemperatureCh1DayData[] =
{
	0x0, 0x5, 0, 0xf1
};

static unsigned char GetMaximumTemperatureCh1DayHeader[] =
{
	0x80, 0x80, 0x80, 0x3, 0x2, 0x4, 0xe4
};

static unsigned char GetMaximumTemperatureCh1DayData[] =
{
	0x0, 0x20, 0, 0x0d
};

static unsigned char GetMinimumTemperatureCh2DayHeader[] =
{
	0x80, 0x80, 0x80, 0x3, 0x2, 0x4, 0xe9
};

static unsigned char GetMinimumTemperatureCh2DayData[] =
{
	0x0, 0x6, 0, 0xf8
};

static unsigned char GetMaximumTemperatureCh2DayHeader[] =
{
	0x80, 0x80, 0x80, 0x3, 0x2, 0x4, 0xea
};

static unsigned char GetMaximumTemperatureCh2DayData[] =
{
	0x0, 0x21, 0, 0x14
};

static unsigned char GetMaximumIrradianceDayHeader[] =
{
	0x80, 0x80, 0x80, 0x3, 0x2, 0x4, 0xef
};

static unsigned char GetMaximumIrradianceDayData[] =
{
	0x1, 0x2, 0x0, 0xfb
};

static unsigned char GetMaximumDigitalCh1DayHeader[] =
{
	0x80, 0x80, 0x80, 0x3, 0x2, 0x4, 0xf4
};

static unsigned char GetMaximumDigitalCh1DayData[] =
{
	1, 8, 0x0, 0x6
};

static unsigned char GetMaximumDigitalCh2DayHeader[] =
{
	0x80, 0x80, 0x80, 0x3, 0x2, 0x4, 0xf7
};

static unsigned char GetMaximumDigitalCh2DayData[] =
{
	1, 9, 0x0, 0xa
};

static unsigned char GetMinimumTemperatureCh1TotalHeader[] =
{
	0x80, 0x80, 0x80, 0x3, 0x2, 0x4, 0xe7
};

static unsigned char GetMinimumTemperatureCh1TotalData[] =
{
	0x0, 0x1, 0, 0xf1
};

static unsigned char GetMaximumTemperatureCh1TotalHeader[] =
{
	0x80, 0x80, 0x80, 0x3, 0x2, 0x4, 0xe8
};

static unsigned char GetMaximumTemperatureCh1TotalData[] =
{
	0x0, 0x30, 0, 0x21
};

static unsigned char GetMinimumTemperatureCh2TotalHeader[] =
{
	0x80, 0x80, 0x80, 0x3, 0x2, 0x4, 0xed
};

static unsigned char GetMinimumTemperatureCh2TotalData[] =
{
	0x0, 0x2, 0, 0xf8
};

static unsigned char GetMaximumTemperatureCh2TotalHeader[] =
{
	0x80, 0x80, 0x80, 0x3, 0x2, 0x4, 0xee
};

static unsigned char GetMaximumTemperatureCh2TotalData[] =
{
	0x0, 0x31, 0, 0x28
};

static unsigned char GetMaximumIrradianceTotalHeader[] =
{
	0x80, 0x80, 0x80, 0x3, 0x2, 0x4, 0xf1
};

static unsigned char GetMaximumIrradianceTotalData[] =
{
	0x2, 0x0, 0x0, 0xfc
};

static unsigned char GetMaximumDigitalCh1TotalHeader[] =
{
	0x80, 0x80, 0x80, 0x3, 0x2, 0x4, 0xf6
};

static unsigned char GetMaximumDigitalCh1TotalData[] =
{
	2, 0, 0x0, 0x1
};

static unsigned char GetMaximumDigitalCh2TotalHeader[] =
{
	0x80, 0x80, 0x80, 0x3, 0x2, 0x4, 0xf9
};

static unsigned char GetMaximumDigitalCh2TotalData[] =
{
	2, 1, 0x0, 0x5
};

static FroniusSimulator	simulator;

Comm::Comm(Args& args)
{
	verbose_ = args.getVerbose();
	simulator.initialize();
}

void
Comm::writeBytes(
	unsigned char* buffer,
	unsigned char numBytesToWrite)
{
	if (verbose_)
	{
		Utils::hexDump(buffer, numBytesToWrite);
	}

	simulator.writeBytes((FroniusRequest*)buffer);
}

void
Comm::readBytes(
	unsigned char* buffer,
	unsigned char numBytesToRead)
{
	simulator.readBytes(buffer, numBytesToRead);

	if (verbose_)
	{
		Utils::hexDump(buffer, numBytesToRead);
	}
}

void
Comm::flush(void)
{
	simulator.flush();
}

bool
Comm::verbose(void)
{
	return verbose_;
}

void
FroniusSimulator::initialize(void)
{
	flush();
}

void
FroniusSimulator::flush(void)
{
	command_ = ~0;
	deviceOption_ = ~0;
	deviceNumber_ = ~0;
	state_ = WriteRequest;
}

void
FroniusSimulator::writeBytes(FroniusRequest* request)
{
	assert(state_ == WriteRequest);
	command_ = request->command;
	deviceOption_ = request->deviceOption;
	deviceNumber_ = request->deviceNumber;
	state_ = ReadHeader;
}

void
FroniusSimulator::readBytes(
	unsigned char* buffer,
	unsigned char numBytesToRead)
{
	if (state_ == ReadHeader)
	{
		assert(numBytesToRead == sizeof (FroniusResponseHeader));
		readHeader(buffer, numBytesToRead);
		state_ = ReadData;
	}
	else
	{
		assert(state_ == ReadData);
		readData(buffer, numBytesToRead);
		state_ = WriteRequest;
	}
}

void
FroniusSimulator::readHeader(
	unsigned char* buffer,
	unsigned char numBytesToRead)
{
	if (deviceOption_ == FRONIUS_INTERFACE_CARD)
	{
		readInterfaceCardHeader(buffer, numBytesToRead);
	}
	else if (deviceOption_ == FRONIUS_INVERTER)
	{
		readInverterHeader(buffer, numBytesToRead);
	}
	else
	{
		assert(deviceOption_ == FRONIUS_SENSOR_CARD);
		readSensorCardHeader(buffer, numBytesToRead);
	}
}

void
FroniusSimulator::readInterfaceCardHeader(
	unsigned char* buffer,
	unsigned char numBytesToRead)
{
	CHECK_PTR(buffer);

	switch (command_)
	{
	case FRONIUS_CMD_GET_VERSION:
		assert(numBytesToRead == NUM_ELEMS(GetVersionHeader));
		memcpy(buffer, GetVersionHeader, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_SOLAR_NET_STATUS:
		assert(numBytesToRead == NUM_ELEMS(GetSolarNetStatusHeader));
		memcpy(buffer, GetSolarNetStatusHeader, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_ACTIVE_INVERTER_NUMBERS:
		assert(numBytesToRead ==
			NUM_ELEMS(GetActiveInverterNumbersHeader));
		memcpy(buffer, GetActiveInverterNumbersHeader, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_ACTIVE_SENSOR_CARDS:
		assert(numBytesToRead == NUM_ELEMS(GetActiveSensorCardsHeader));
		memcpy(buffer, GetActiveSensorCardsHeader, numBytesToRead);
		break;
	default:
		Args::dieMessage(
			Args::ExitSimulatorError,
			"Simulator: unsupported Interface Card Command: 0x%x\n",
			command_);
		break;
	}
}

void
FroniusSimulator::readInverterHeader(
	unsigned char* buffer,
	unsigned char numBytesToRead)
{
	switch (command_)
	{
	case FRONIUS_CMD_GET_DEVICE_TYPE:
		assert(numBytesToRead ==
			NUM_ELEMS(GetInverterDeviceTypeHeader));
		memcpy(buffer, GetInverterDeviceTypeHeader, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_POWER_NOW:
		assert(numBytesToRead == NUM_ELEMS(GetPowerNowHeader));
		memcpy(buffer, GetPowerNowHeader, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_AC_CURRENT_NOW:
		assert(numBytesToRead == NUM_ELEMS(GetACCurrentNowHeader));
		memcpy(buffer, GetACCurrentNowHeader, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_AC_VOLTAGE_NOW:
		assert(numBytesToRead == NUM_ELEMS(GetACVoltageNowHeader));
		memcpy(buffer, GetACVoltageNowHeader, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_AC_FREQUENCY_NOW:
		assert(numBytesToRead == NUM_ELEMS(GetACFrequencyNowHeader));
		memcpy(buffer, GetACFrequencyNowHeader, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_DC_CURRENT_NOW:
		assert(numBytesToRead == NUM_ELEMS(GetDCCurrentNowHeader));
		memcpy(buffer, GetDCCurrentNowHeader, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_DC_VOLTAGE_NOW:
		assert(numBytesToRead == NUM_ELEMS(GetDCVoltageNowHeader));
		memcpy(buffer, GetDCVoltageNowHeader, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_ENERGY_DAY:
		assert(numBytesToRead == NUM_ELEMS(GetEnergyDayHeader));
		memcpy(buffer, GetEnergyDayHeader, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_MAXIMUM_POWER_DAY:
		assert(numBytesToRead == NUM_ELEMS(GetMaximumPowerDayHeader));
		memcpy(buffer, GetMaximumPowerDayHeader, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_MAXIMUM_AC_VOLTAGE_DAY:
		assert(numBytesToRead ==
			NUM_ELEMS(GetMaximumACVoltageDayHeader));
		memcpy(buffer, GetMaximumACVoltageDayHeader, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_MINIMUM_AC_VOLTAGE_DAY:
		assert(numBytesToRead ==
			NUM_ELEMS(GetMinimumACVoltageDayHeader));
		memcpy(buffer, GetMinimumACVoltageDayHeader, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_MAXIMUM_DC_VOLTAGE_DAY:
		assert(numBytesToRead ==
			NUM_ELEMS(GetMaximumDCVoltageDayHeader));
		memcpy(buffer, GetMaximumDCVoltageDayHeader, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_OPERATING_HOURS_DAY:
		assert(numBytesToRead == NUM_ELEMS(GetOperatingHoursDayHeader));
		memcpy(buffer, GetOperatingHoursDayHeader, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_ENERGY_TOTAL:
	case FRONIUS_CMD_GET_ENERGY_YEAR:
		assert(numBytesToRead == NUM_ELEMS(GetEnergyTotalHeader));
		memcpy(buffer, GetEnergyTotalHeader, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_MAXIMUM_POWER_TOTAL:
	case FRONIUS_CMD_GET_MAXIMUM_POWER_YEAR:
		assert(numBytesToRead == NUM_ELEMS(GetMaximumPowerTotalHeader));
		memcpy(buffer, GetMaximumPowerTotalHeader, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_MAXIMUM_AC_VOLTAGE_TOTAL:
	case FRONIUS_CMD_GET_MAXIMUM_AC_VOLTAGE_YEAR:
		assert(numBytesToRead ==
			NUM_ELEMS(GetMaximumACVoltageTotalHeader));
		memcpy(buffer, GetMaximumACVoltageTotalHeader, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_MINIMUM_AC_VOLTAGE_TOTAL:
	case FRONIUS_CMD_GET_MINIMUM_AC_VOLTAGE_YEAR:
		assert(numBytesToRead ==
			NUM_ELEMS(GetMinimumACVoltageTotalHeader));
		memcpy(buffer, GetMinimumACVoltageTotalHeader, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_MAXIMUM_DC_VOLTAGE_TOTAL:
	case FRONIUS_CMD_GET_MAXIMUM_DC_VOLTAGE_YEAR:
		assert(numBytesToRead ==
			NUM_ELEMS(GetMaximumDCVoltageTotalHeader));
		memcpy(buffer, GetMaximumDCVoltageTotalHeader, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_OPERATING_HOURS_TOTAL:
	case FRONIUS_CMD_GET_OPERATING_HOURS_YEAR:
		assert(numBytesToRead ==
			NUM_ELEMS(GetOperatingHoursTotalHeader));
		memcpy(buffer, GetOperatingHoursTotalHeader, numBytesToRead);
		break;
	default:
		Args::dieMessage(
			Args::ExitSimulatorError,
			"Simulator: unsupported Inverter Command: 0x%x\n",
			command_);
		break;
	};
}

void
FroniusSimulator::readSensorCardHeader(
	unsigned char* buffer,
	unsigned char numBytesToRead)
{
	switch (command_)
	{
	case FRONIUS_CMD_GET_DEVICE_TYPE:
		assert(numBytesToRead ==
			NUM_ELEMS(GetSensorCardDeviceTypeHeader));
		memcpy(buffer, GetSensorCardDeviceTypeHeader, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_TEMPERATURE_CH1_NOW:
		assert(numBytesToRead == NUM_ELEMS(GetTemperatureCh1NowHeader));
		memcpy(buffer, GetTemperatureCh1NowHeader, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_TEMPERATURE_CH2_NOW:
		assert(numBytesToRead == NUM_ELEMS(GetTemperatureCh2NowHeader));
		memcpy(buffer, GetTemperatureCh2NowHeader, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_IRRADIANCE_NOW:
		assert(numBytesToRead == NUM_ELEMS(GetIrradianceNowHeader));
		memcpy(buffer, GetIrradianceNowHeader, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_DIGITAL_CH1_NOW:
		assert(numBytesToRead == NUM_ELEMS(GetDigitalCh1NowHeader));
		memcpy(buffer, GetDigitalCh1NowHeader, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_DIGITAL_CH2_NOW:
		assert(numBytesToRead == NUM_ELEMS(GetDigitalCh2NowHeader));
		memcpy(buffer, GetDigitalCh2NowHeader, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_MINIMUM_TEMPERATURE_CH1_DAY:
		assert(numBytesToRead
			== NUM_ELEMS(GetMinimumTemperatureCh1DayHeader));
		memcpy(buffer,
			GetMinimumTemperatureCh1DayHeader, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_MAXIMUM_TEMPERATURE_CH1_DAY:
		assert(numBytesToRead
			== NUM_ELEMS(GetMaximumTemperatureCh1DayHeader));
		memcpy(buffer,
			GetMaximumTemperatureCh1DayHeader, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_MINIMUM_TEMPERATURE_CH2_DAY:
		assert(numBytesToRead
			== NUM_ELEMS(GetMinimumTemperatureCh2DayHeader));
		memcpy(buffer,
			GetMinimumTemperatureCh2DayHeader, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_MAXIMUM_TEMPERATURE_CH2_DAY:
		assert(numBytesToRead
			== NUM_ELEMS(GetMaximumTemperatureCh2DayHeader));
		memcpy(buffer,
			GetMaximumTemperatureCh2DayHeader, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_MAXIMUM_IRRADIANCE_DAY:
		assert(numBytesToRead
			== NUM_ELEMS(GetMaximumIrradianceDayHeader));
		memcpy(buffer, GetMaximumIrradianceDayHeader, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_MAXIMUM_DIGITAL_CH1_DAY:
		assert(numBytesToRead
			== NUM_ELEMS(GetMaximumDigitalCh1DayHeader));
		memcpy(buffer, GetMaximumDigitalCh1DayHeader, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_MAXIMUM_DIGITAL_CH2_DAY:
		assert(numBytesToRead
			== NUM_ELEMS(GetMaximumDigitalCh2DayHeader));
		memcpy(buffer, GetMaximumDigitalCh2DayHeader, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_MINIMUM_TEMPERATURE_CH1_TOTAL:
	case FRONIUS_CMD_GET_MINIMUM_TEMPERATURE_CH1_YEAR:
		assert(numBytesToRead
			== NUM_ELEMS(GetMinimumTemperatureCh1TotalHeader));
		memcpy(buffer,
			GetMinimumTemperatureCh1TotalHeader, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_MAXIMUM_TEMPERATURE_CH1_TOTAL:
	case FRONIUS_CMD_GET_MAXIMUM_TEMPERATURE_CH1_YEAR:
		assert(numBytesToRead
			== NUM_ELEMS(GetMaximumTemperatureCh1TotalHeader));
		memcpy(buffer,
			GetMaximumTemperatureCh1TotalHeader, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_MINIMUM_TEMPERATURE_CH2_TOTAL:
	case FRONIUS_CMD_GET_MINIMUM_TEMPERATURE_CH2_YEAR:
		assert(numBytesToRead
			== NUM_ELEMS(GetMinimumTemperatureCh2TotalHeader));
		memcpy(buffer,
			GetMinimumTemperatureCh2TotalHeader, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_MAXIMUM_TEMPERATURE_CH2_TOTAL:
	case FRONIUS_CMD_GET_MAXIMUM_TEMPERATURE_CH2_YEAR:
		assert(numBytesToRead
			== NUM_ELEMS(GetMaximumTemperatureCh2TotalHeader));
		memcpy(buffer,
			GetMaximumTemperatureCh2TotalHeader, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_MAXIMUM_IRRADIANCE_TOTAL:
	case FRONIUS_CMD_GET_MAXIMUM_IRRADIANCE_YEAR:
		assert(numBytesToRead
			== NUM_ELEMS(GetMaximumIrradianceTotalHeader));
		memcpy(buffer, GetMaximumIrradianceTotalHeader, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_MAXIMUM_DIGITAL_CH1_TOTAL:
	case FRONIUS_CMD_GET_MAXIMUM_DIGITAL_CH1_YEAR:
		assert(numBytesToRead
			== NUM_ELEMS(GetMaximumDigitalCh1TotalHeader));
		memcpy(buffer, GetMaximumDigitalCh1TotalHeader, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_MAXIMUM_DIGITAL_CH2_TOTAL:
	case FRONIUS_CMD_GET_MAXIMUM_DIGITAL_CH2_YEAR:
		assert(numBytesToRead
			== NUM_ELEMS(GetMaximumDigitalCh2TotalHeader));
		memcpy(buffer, GetMaximumDigitalCh2TotalHeader, numBytesToRead);
		break;
	default:
		Args::dieMessage(
			Args::ExitSimulatorError,
			"Simulator: unsupported Sensor Card Command: 0x%x\n",
			command_);
		break;
	}
}

void
FroniusSimulator::readData(
	unsigned char* buffer,
	unsigned char numBytesToRead)
{
	if (deviceOption_ == FRONIUS_INTERFACE_CARD)
	{
		readInterfaceCardData(buffer, numBytesToRead);
	}
	else if (deviceOption_ == FRONIUS_INVERTER)
	{
		readInverterData(buffer, numBytesToRead);
	}
	else
	{
		assert(deviceOption_ == FRONIUS_SENSOR_CARD);
		readSensorCardData(buffer, numBytesToRead);
	}
}

void
FroniusSimulator::readInterfaceCardData(
	unsigned char* buffer,
	unsigned char numBytesToRead)
{
	CHECK_PTR(buffer);

	if (command_ == FRONIUS_CMD_GET_VERSION)
	{
		assert(numBytesToRead == NUM_ELEMS(GetVersionData));
		memcpy(buffer, GetVersionData, numBytesToRead);

	}
	else if (command_ == FRONIUS_CMD_GET_SOLAR_NET_STATUS)
	{
		assert(numBytesToRead == NUM_ELEMS(GetSolarNetStatusData));
		memcpy(buffer, GetSolarNetStatusData, numBytesToRead);
	}
	else if (command_ == FRONIUS_CMD_GET_ACTIVE_INVERTER_NUMBERS)
	{
		assert(numBytesToRead ==
			NUM_ELEMS(GetActiveInverterNumbersData));
		memcpy(buffer, GetActiveInverterNumbersData, numBytesToRead);
	}
	else if (command_ == FRONIUS_CMD_GET_ACTIVE_SENSOR_CARDS)
	{
		assert(numBytesToRead == NUM_ELEMS(GetActiveSensorCardsData));
		memcpy(buffer, GetActiveSensorCardsData, numBytesToRead);
	}
	else
	{
		Args::dieMessage(
			Args::ExitSimulatorError,
			"Simulator: unsupported Interface Card Command: 0x%x\n",
			command_);
	}
}

void
FroniusSimulator::readInverterData(
	unsigned char* buffer,
	unsigned char numBytesToRead)
{
	switch (command_)
	{
	case FRONIUS_CMD_GET_DEVICE_TYPE:
		assert(numBytesToRead == NUM_ELEMS(GetInverterDeviceTypeData));
		memcpy(buffer, GetInverterDeviceTypeData, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_POWER_NOW:
		assert(numBytesToRead == NUM_ELEMS(GetPowerNowData));
		memcpy(buffer, GetPowerNowData, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_AC_CURRENT_NOW:
		assert(numBytesToRead == NUM_ELEMS(GetACCurrentNowData));
		memcpy(buffer, GetACCurrentNowData, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_AC_VOLTAGE_NOW:
		assert(numBytesToRead == NUM_ELEMS(GetACVoltageNowData));
		memcpy(buffer, GetACVoltageNowData, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_AC_FREQUENCY_NOW:
		assert(numBytesToRead == NUM_ELEMS(GetACFrequencyNowData));
		memcpy(buffer, GetACFrequencyNowData, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_DC_CURRENT_NOW:
		assert(numBytesToRead == NUM_ELEMS(GetDCCurrentNowData));
		memcpy(buffer, GetDCCurrentNowData, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_DC_VOLTAGE_NOW:
		assert(numBytesToRead == NUM_ELEMS(GetDCVoltageNowData));
		memcpy(buffer, GetDCVoltageNowData, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_ENERGY_DAY:
		assert(numBytesToRead == NUM_ELEMS(GetEnergyDayData));
		memcpy(buffer, GetEnergyDayData, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_MAXIMUM_POWER_DAY:
		assert(numBytesToRead == NUM_ELEMS(GetMaximumPowerDayData));
		memcpy(buffer, GetMaximumPowerDayData, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_MAXIMUM_AC_VOLTAGE_DAY:
		assert(numBytesToRead ==
			NUM_ELEMS(GetMaximumACVoltageDayData));
		memcpy(buffer, GetMaximumACVoltageDayData, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_MINIMUM_AC_VOLTAGE_DAY:
		assert(numBytesToRead ==
			NUM_ELEMS(GetMinimumACVoltageDayData));
		memcpy(buffer, GetMinimumACVoltageDayData, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_MAXIMUM_DC_VOLTAGE_DAY:
		assert(numBytesToRead ==
			NUM_ELEMS(GetMaximumDCVoltageDayData));
		memcpy(buffer, GetMaximumDCVoltageDayData, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_OPERATING_HOURS_DAY:
		assert(numBytesToRead == NUM_ELEMS(GetOperatingHoursDayData));
		memcpy(buffer, GetOperatingHoursDayData, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_ENERGY_TOTAL:
	case FRONIUS_CMD_GET_ENERGY_YEAR:
		assert(numBytesToRead == NUM_ELEMS(GetEnergyTotalData));
		memcpy(buffer, GetEnergyTotalData, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_MAXIMUM_POWER_TOTAL:
	case FRONIUS_CMD_GET_MAXIMUM_POWER_YEAR:
		assert(numBytesToRead == NUM_ELEMS(GetMaximumPowerTotalData));
		memcpy(buffer, GetMaximumPowerTotalData, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_MAXIMUM_AC_VOLTAGE_TOTAL:
	case FRONIUS_CMD_GET_MAXIMUM_AC_VOLTAGE_YEAR:
		assert(numBytesToRead ==
			NUM_ELEMS(GetMaximumACVoltageTotalData));
		memcpy(buffer, GetMaximumACVoltageTotalData, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_MINIMUM_AC_VOLTAGE_TOTAL:
	case FRONIUS_CMD_GET_MINIMUM_AC_VOLTAGE_YEAR:
		assert(numBytesToRead ==
			NUM_ELEMS(GetMinimumACVoltageTotalData));
		memcpy(buffer, GetMinimumACVoltageTotalData, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_MAXIMUM_DC_VOLTAGE_TOTAL:
	case FRONIUS_CMD_GET_MAXIMUM_DC_VOLTAGE_YEAR:
		assert(numBytesToRead ==
			NUM_ELEMS(GetMaximumDCVoltageTotalData));
		memcpy(buffer, GetMaximumDCVoltageTotalData, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_OPERATING_HOURS_TOTAL:
	case FRONIUS_CMD_GET_OPERATING_HOURS_YEAR:
		assert(numBytesToRead ==
			NUM_ELEMS(GetOperatingHoursTotalData));
		memcpy(buffer, GetOperatingHoursTotalData, numBytesToRead);
		break;
	default:
		Args::dieMessage(
			Args::ExitSimulatorError,
			"Simulator: unsupported Inverter Command: 0x%x\n",
			command_);
		break;
	};
}

void
FroniusSimulator::readSensorCardData(
	unsigned char* buffer,
	unsigned char numBytesToRead)
{
	switch (command_)
	{
	case FRONIUS_CMD_GET_DEVICE_TYPE:
		assert(numBytesToRead ==
			NUM_ELEMS(GetSensorCardDeviceTypeData));
		memcpy(buffer, GetSensorCardDeviceTypeData, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_TEMPERATURE_CH1_NOW:
		assert(numBytesToRead == NUM_ELEMS(GetTemperatureCh1NowData));
		memcpy(buffer, GetTemperatureCh1NowData, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_TEMPERATURE_CH2_NOW:
		assert(numBytesToRead == NUM_ELEMS(GetTemperatureCh2NowData));
		memcpy(buffer, GetTemperatureCh2NowData, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_IRRADIANCE_NOW:
		assert(numBytesToRead == NUM_ELEMS(GetIrradianceNowData));
		memcpy(buffer, GetIrradianceNowData, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_DIGITAL_CH1_NOW:
		assert(numBytesToRead == NUM_ELEMS(GetDigitalCh1NowData));
		memcpy(buffer, GetDigitalCh1NowData, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_DIGITAL_CH2_NOW:
		assert(numBytesToRead == NUM_ELEMS(GetDigitalCh2NowData));
		memcpy(buffer, GetDigitalCh2NowData, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_MINIMUM_TEMPERATURE_CH1_DAY:
		assert(numBytesToRead
			== NUM_ELEMS(GetMinimumTemperatureCh1DayData));
		memcpy(buffer, GetMinimumTemperatureCh1DayData, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_MAXIMUM_TEMPERATURE_CH1_DAY:
		assert(numBytesToRead
			== NUM_ELEMS(GetMaximumTemperatureCh1DayData));
		memcpy(buffer, GetMaximumTemperatureCh1DayData, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_MINIMUM_TEMPERATURE_CH2_DAY:
		assert(numBytesToRead
			== NUM_ELEMS(GetMinimumTemperatureCh2DayData));
		memcpy(buffer, GetMinimumTemperatureCh2DayData, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_MAXIMUM_TEMPERATURE_CH2_DAY:
		assert(numBytesToRead
			== NUM_ELEMS(GetMaximumTemperatureCh2DayData));
		memcpy(buffer, GetMaximumTemperatureCh2DayData, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_MAXIMUM_IRRADIANCE_DAY:
		assert(numBytesToRead
			== NUM_ELEMS(GetMaximumIrradianceDayData));
		memcpy(buffer, GetMaximumIrradianceDayData, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_MAXIMUM_DIGITAL_CH1_DAY:
		assert(numBytesToRead
			== NUM_ELEMS(GetMaximumDigitalCh1DayData));
		memcpy(buffer, GetMaximumDigitalCh1DayData, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_MAXIMUM_DIGITAL_CH2_DAY:
		assert(numBytesToRead
			== NUM_ELEMS(GetMaximumDigitalCh2DayData));
		memcpy(buffer, GetMaximumDigitalCh2DayData, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_MINIMUM_TEMPERATURE_CH1_TOTAL:
	case FRONIUS_CMD_GET_MINIMUM_TEMPERATURE_CH1_YEAR:
		assert(numBytesToRead
			== NUM_ELEMS(GetMinimumTemperatureCh1TotalData));
		memcpy(buffer,
			GetMinimumTemperatureCh1TotalData, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_MAXIMUM_TEMPERATURE_CH1_TOTAL:
	case FRONIUS_CMD_GET_MAXIMUM_TEMPERATURE_CH1_YEAR:
		assert(numBytesToRead
			== NUM_ELEMS(GetMaximumTemperatureCh1TotalData));
		memcpy(buffer,
			GetMaximumTemperatureCh1TotalData, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_MINIMUM_TEMPERATURE_CH2_TOTAL:
	case FRONIUS_CMD_GET_MINIMUM_TEMPERATURE_CH2_YEAR:
		assert(numBytesToRead
			== NUM_ELEMS(GetMinimumTemperatureCh2TotalData));
		memcpy(buffer,
			GetMinimumTemperatureCh2TotalData, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_MAXIMUM_TEMPERATURE_CH2_TOTAL:
	case FRONIUS_CMD_GET_MAXIMUM_TEMPERATURE_CH2_YEAR:
		assert(numBytesToRead
			== NUM_ELEMS(GetMaximumTemperatureCh2TotalData));
		memcpy(buffer,
			GetMaximumTemperatureCh2TotalData, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_MAXIMUM_IRRADIANCE_TOTAL:
	case FRONIUS_CMD_GET_MAXIMUM_IRRADIANCE_YEAR:
		assert(numBytesToRead
			== NUM_ELEMS(GetMaximumIrradianceTotalData));
		memcpy(buffer, GetMaximumIrradianceTotalData, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_MAXIMUM_DIGITAL_CH1_TOTAL:
	case FRONIUS_CMD_GET_MAXIMUM_DIGITAL_CH1_YEAR:
		assert(numBytesToRead
			== NUM_ELEMS(GetMaximumDigitalCh1TotalData));
		memcpy(buffer, GetMaximumDigitalCh1TotalData, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_MAXIMUM_DIGITAL_CH2_TOTAL:
	case FRONIUS_CMD_GET_MAXIMUM_DIGITAL_CH2_YEAR:
		assert(numBytesToRead
			== NUM_ELEMS(GetMaximumDigitalCh2TotalData));
		memcpy(buffer, GetMaximumDigitalCh2TotalData, numBytesToRead);
		break;
	default:
		Args::dieMessage(
			Args::ExitSimulatorError,
			"Simulator: unsupported Sensor Card Command: 0x%x\n",
			command_);
		break;
	};
}
