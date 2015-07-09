//
// fslurp, Copyright (c) 2012 David Stone <ds.fslurp@asif.com>
//
// $RCSfile: simSolarNet0i0s.cpp,v $
// $Revision: 1.1 $
// $Date: 2012/10/20 01:35:13 $
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
	0x80, 0x80, 0x80, 0x0, 0, 0, 0x4
};

static unsigned char GetActiveInverterNumbersData[] =
{
	0x4 
};

static unsigned char GetActiveSensorCardsHeader[] =
{
	0x80, 0x80, 0x80, 0, 0, 0, 0x5
};

static unsigned char GetActiveSensorCardsData[] =
{
	0x5
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
	default:
		Args::dieMessage(
			Args::ExitSimulatorError,
			"Simulator: unsupported Sensor Card Command: 0x%x\n",
			command_);
		break;
	};
}
