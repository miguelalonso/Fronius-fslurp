//
// fslurp, Copyright (c) 2012 David Stone <ds.fslurp@asif.com>
//
// $RCSfile: simIFCEasy.cpp,v $
// $Revision: 1.2 $
// $Date: 2012/10/08 00:56:02 $
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
	0x2, 0x1, 0x2, 0x2, 0xc 
};

static unsigned char GetActiveInverterNumbersHeader[] =
{
	0x80, 0x80, 0x80, 0x1, 0, 0, 0x4
};

static unsigned char GetActiveInverterNumbersData[] =
{
	0x1, 0x6 
};

static unsigned char GetDeviceTypeHeader[] =
{
	0x80, 0x80, 0x80, 0x1, 0x1, 0x1, 0x2
};

static unsigned char GetDeviceTypeData[] =
{
	0xd9, 0xde
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
	else
	{
		assert(deviceOption_ == FRONIUS_INVERTER);
		readInverterHeader(buffer, numBytesToRead);
	};
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
	case FRONIUS_CMD_GET_ACTIVE_INVERTER_NUMBERS:
		assert(numBytesToRead ==
			NUM_ELEMS(GetActiveInverterNumbersHeader));
		memcpy(buffer, GetActiveInverterNumbersHeader, numBytesToRead);
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
		assert(numBytesToRead == NUM_ELEMS(GetDeviceTypeHeader));
		memcpy(buffer, GetDeviceTypeHeader, numBytesToRead);
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
		assert(numBytesToRead == NUM_ELEMS(GetEnergyTotalHeader));
		memcpy(buffer, GetEnergyTotalHeader, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_MAXIMUM_POWER_TOTAL:
		assert(numBytesToRead == NUM_ELEMS(GetMaximumPowerTotalHeader));
		memcpy(buffer, GetMaximumPowerTotalHeader, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_MAXIMUM_AC_VOLTAGE_TOTAL:
		assert(numBytesToRead ==
			NUM_ELEMS(GetMaximumACVoltageTotalHeader));
		memcpy(buffer, GetMaximumACVoltageTotalHeader, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_MINIMUM_AC_VOLTAGE_TOTAL:
		assert(numBytesToRead ==
			NUM_ELEMS(GetMinimumACVoltageTotalHeader));
		memcpy(buffer, GetMinimumACVoltageTotalHeader, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_MAXIMUM_DC_VOLTAGE_TOTAL:
		assert(numBytesToRead ==
			NUM_ELEMS(GetMaximumDCVoltageTotalHeader));
		memcpy(buffer, GetMaximumDCVoltageTotalHeader, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_OPERATING_HOURS_TOTAL:
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
FroniusSimulator::readData(
	unsigned char* buffer,
	unsigned char numBytesToRead)
{
	if (deviceOption_ == FRONIUS_INTERFACE_CARD)
	{
		readInterfaceCardData(buffer, numBytesToRead);
	}
	else
	{
		assert(deviceOption_ == FRONIUS_INVERTER);
		readInverterData(buffer, numBytesToRead);
	};
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
	else if (command_ == FRONIUS_CMD_GET_ACTIVE_INVERTER_NUMBERS)
	{
		assert(numBytesToRead ==
			NUM_ELEMS(GetActiveInverterNumbersData));
		memcpy(buffer, GetActiveInverterNumbersData, numBytesToRead);
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
		assert(numBytesToRead == NUM_ELEMS(GetDeviceTypeData));
		memcpy(buffer, GetDeviceTypeData, numBytesToRead);
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
		assert(numBytesToRead == NUM_ELEMS(GetEnergyTotalData));
		memcpy(buffer, GetEnergyTotalData, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_MAXIMUM_POWER_TOTAL:
		assert(numBytesToRead == NUM_ELEMS(GetMaximumPowerTotalData));
		memcpy(buffer, GetMaximumPowerTotalData, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_MAXIMUM_AC_VOLTAGE_TOTAL:
		assert(numBytesToRead ==
			NUM_ELEMS(GetMaximumACVoltageTotalData));
		memcpy(buffer, GetMaximumACVoltageTotalData, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_MINIMUM_AC_VOLTAGE_TOTAL:
		assert(numBytesToRead ==
			NUM_ELEMS(GetMinimumACVoltageTotalData));
		memcpy(buffer, GetMinimumACVoltageTotalData, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_MAXIMUM_DC_VOLTAGE_TOTAL:
		assert(numBytesToRead ==
			NUM_ELEMS(GetMaximumDCVoltageTotalData));
		memcpy(buffer, GetMaximumDCVoltageTotalData, numBytesToRead);
		break;
	case FRONIUS_CMD_GET_OPERATING_HOURS_TOTAL:
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
