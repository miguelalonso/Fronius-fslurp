//
// fslurp, Copyright (c) 2012, 2013 David Stone <ds.fslurp@asif.com>
//
// $RCSfile: interface.cpp,v $
// $Revision: 1.10 $
// $Date: 2013/12/21 14:55:35 $
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

static Inverter::DeviceTypeMap interfaceTypeMap[] =
{
	{ FRONIUS_INTERFACE_RS_232, "RS232 interface card/box" },
	{ FRONIUS_INTERFACE_CARD_EASY, "RS232 Interface Card easy" },
	{ FRONIUS_INTERFACE_RS_422, "RS422 virtual interface card" }
};

Interface::Interface(Comm* comm)
{
	inverters_ = NULL;
	comm_ = comm;
	CHECK_PTR( comm_ );
	hasSolarNet_ = false;
	solarNetStatus_ = FRONIUS_SOLAR_NET_ERROR;
	supportsYearData_ = false;
	supportsMinimumACVoltageData_ = false;
}

Interface::~Interface(void)
{
	if (inverters_ != NULL)
	{
		delete [] inverters_;
		inverters_ = NULL;
	}

	comm_ = NULL;
}

void
Interface::initialize(void)
{
	getVersion();

	switch(getInterfaceType())
	{
	case FRONIUS_INTERFACE_RS_232:
		getSolarNetStatus();
		hasSolarNet_ = true;
		supportsYearData_ = true;
		supportsMinimumACVoltageData_ = true;
		break;
	case FRONIUS_INTERFACE_CARD_EASY:
		supportsMinimumACVoltageData_ = true;
		break;
	case FRONIUS_INTERFACE_RS_422:
		supportsYearData_ = true;
		break;
	}
}

unsigned char
Interface::maximumFieldLabelWidth(void) const
{
	// FIX THIS, DS:
	return 24;
}

void
Interface::getVersion(void)
{
	Request		request;

	request.setDeviceOption(FRONIUS_INTERFACE_CARD);
	request.setDeviceNumber(0);
	request.setCommand(FRONIUS_CMD_GET_VERSION);

	// FIX THIS, DS: move this to its own startup command?
	//
	// Do an initial write and flush to give the Fronius
	// serial port a chance to auto detect the baud rate.

	CHECK_PTR(comm_);
	request.send(*comm_);
	usleep(100000);
	comm_->flush();

	request.send(*comm_);
	versionResponse_.receive(*comm_);
}

unsigned char
Interface::getInterfaceType(void) const
{
	return versionResponse_.getInterfaceType();
}

void
Interface::getSolarNetStatus(void)
{
	Request			request;

	request.setDeviceOption(FRONIUS_INTERFACE_CARD);
	request.setDeviceNumber(0);
	request.setCommand(FRONIUS_CMD_GET_SOLAR_NET_STATUS);

	CHECK_PTR(comm_);
	request.send(*comm_);
	solarNetStatusResponse_.receive(*comm_);
}

bool
Interface::supportsYearData(void) const
{
	return supportsYearData_;
}

bool
Interface::supportsMinimumACVoltageData(void) const
{
	return supportsMinimumACVoltageData_;
}

void
Interface::discoverDevices(void)
{
	getInverters();

	if (getInterfaceType() == FRONIUS_INTERFACE_RS_232)
	{
		getSensorCards();
	}
}

const char*
Interface::getModel(void) const
{
	unsigned int	i;
	unsigned char	interfaceType;

	interfaceType = getInterfaceType();

	for (i = 0; i < NUM_ELEMS(interfaceTypeMap); i++)
	{
		if (interfaceTypeMap[i].id == interfaceType)
		{
			return interfaceTypeMap[i].model;
		}
	}

	return "unknown interface type";
}

const char*
Interface::decodeSolarNetStatus(void) const
{
	unsigned char	solarNetStatus;

	if (hasSolarNet_ == false)
	{
		return "N/A";
	}

	solarNetStatus = solarNetStatusResponse_.getSolarNetStatus();

	if (solarNetStatus == FRONIUS_SOLAR_NET_OK)
	{
		return "Solar Net OK";
	}

	assert(solarNetStatus == FRONIUS_SOLAR_NET_ERROR);

	return "Solar Net Error";
}

// Display the header for the sytem report

void
Interface::reportHeader(Args& args)
{
	const char*	delimiter;

	delimiter = args.getDelimiter();

	assert(delimiter != NULL);

	printf("Time%s", delimiter);
	printf("Device Number%s", delimiter);
	printf("Device Type%s", delimiter);
	printf("Description%s", delimiter);
	printf("\n");
}

void
Interface::report(Args& args)
{
	InverterIterator	inverterIterator(this);
	SensorCardIterator	sensorCardIterator(this);
	Device*			device;

	interfaceReport(args);

	Utils::displaySeparator(args);

	while ((device = inverterIterator.next()) != NULL)
	{
		device->deviceInfoReport(maximumFieldLabelWidth(), args);
		Utils::displaySeparator(args);
	}

	while ((device = sensorCardIterator.next()) != NULL)
	{
		device->deviceInfoReport(maximumFieldLabelWidth(), args);
		Utils::displaySeparator(args);
	}
}

void
Interface::interfaceReport(Args& args)
{
	const char*		delimiter;
	const char*		timestamp;

	delimiter = args.getDelimiter();

	if (delimiter == NULL)
	{
		printf("%-*s : %d\n",
			maximumFieldLabelWidth(),
			"Device number",
			0);
		printf("%-*s : 0x%02X\n",
			maximumFieldLabelWidth(),
			"Device type",
			getInterfaceType());

		printf("%-*s : %s, %d.%d.%d",
			maximumFieldLabelWidth(),
			"Device description",
			getModel(),
			versionResponse_.getVersionMajor(),
			versionResponse_.getVersionMinor(),
			versionResponse_.getVersionRelease());

		if (getInterfaceType() == FRONIUS_INTERFACE_RS_232)
		{
			printf(", %s", decodeSolarNetStatus());
		}
		printf("\n");
	}
	else
	{
		timestamp = Utils::getTimestamp(args);
		delimiter = args.getDelimiter();
		CHECK_PTR(delimiter);

		printf("%s%s", timestamp, delimiter);
		printf("0%s", delimiter);
		printf("0x%02X%s", getInterfaceType(),
			delimiter);
		printf("%s %d.%d.%d",
			getModel(),
			versionResponse_.getVersionMajor(),
			versionResponse_.getVersionMinor(),
			versionResponse_.getVersionRelease());
		if (getInterfaceType() == FRONIUS_INTERFACE_RS_232)
		{
			printf(" %s", decodeSolarNetStatus());
		}
		printf("%s", delimiter);
		printf("\n");
	}
}

void
Interface::getInverters(void)
{
	unsigned char	numActiveInverters;
	unsigned char	i;

	getActiveInverters();
	numActiveInverters = getNumActiveInverters();

	CHECK_PTR(comm_);
	if (comm_->verbose())
	{
		fprintf(stderr, "%d inverter%s found\n",
			numActiveInverters,
			(numActiveInverters == 1) ? "" : "s");
	}

	if (numActiveInverters == 0)
	{
		return;
	}

	inverters_ = new Inverter[numActiveInverters];
	CHECK_PTR(inverters_);

	for (i = 0; i < numActiveInverters; i++)
	{
		inverters_[i].initialize(
			comm_,
			getInterfaceType(),
			getActiveInverterID(i),
			supportsYearData_);
	}
}

void
Interface::getActiveInverters(void)
{
	Request		request;

	request.setDeviceOption(FRONIUS_INTERFACE_CARD);
	request.setDeviceNumber(0);
	request.setCommand(FRONIUS_CMD_GET_ACTIVE_INVERTER_NUMBERS);

	CHECK_PTR(comm_);
	request.send(*comm_);
	activeInverterNumbersResponse_.receive(*comm_);
}

unsigned char
Interface::getNumActiveInverters(void) const
{
	assert(activeInverterNumbersResponse_.responseValid());
	return activeInverterNumbersResponse_.numActiveInverters();
}

unsigned char
Interface::getActiveInverterID(unsigned char index) const
{
	assert(activeInverterNumbersResponse_.responseValid());
	return activeInverterNumbersResponse_.getActiveInverterID(index);
}

Inverter*
Interface::getInverter(unsigned char index)
{
	assert(index < getNumActiveInverters());
	CHECK_PTR(inverters_);
	return &inverters_[index];
}

Inverter*
Interface::getInverterByID(unsigned char inverterID)
{
	unsigned char	i;

	for (i = 0; i < getNumActiveInverters(); i++)
	{
		CHECK_PTR(inverters_);
		if (inverters_[i].getDeviceID() == inverterID)
		{
			return &inverters_[i];
		}
	}

	return NULL;
}

void
Interface::getSensorCards(void)
{
	unsigned char	numActiveSensorCards;
	unsigned char	i;

	getActiveSensorCards();
	numActiveSensorCards = getNumActiveSensorCards();

	CHECK_PTR(comm_);
	if (comm_->verbose())
	{
		fprintf(stderr, "%d sensor card%s found\n",
			numActiveSensorCards,
			(numActiveSensorCards == 1) ? "" : "s");
	}

	if (numActiveSensorCards == 0)
	{
		return;
	}

	sensorCards_ = new SensorCard[numActiveSensorCards];
	CHECK_PTR(sensorCards_);

	for (i = 0; i < numActiveSensorCards; i++)
	{
		sensorCards_[i].initialize(
			comm_,
			getInterfaceType(),
			getActiveSensorCardID(i),
			supportsYearData_);
	}
}

void
Interface::getActiveSensorCards(void)
{
	Request		request;

	request.setDeviceOption(FRONIUS_INTERFACE_CARD);
	request.setDeviceNumber(0);
	request.setCommand(FRONIUS_CMD_GET_ACTIVE_SENSOR_CARDS);

	CHECK_PTR(comm_);
	request.send(*comm_);
	activeSensorCardsResponse_.receive(*comm_);
}

unsigned char
Interface::getNumActiveSensorCards(void) const
{
	if (getInterfaceType() != FRONIUS_INTERFACE_RS_232)
	{
		return 0;
	}

	assert(activeSensorCardsResponse_.responseValid());
	return activeSensorCardsResponse_.numActiveSensorCards();
}

unsigned char
Interface::getActiveSensorCardID(unsigned char index) const
{
	assert(activeSensorCardsResponse_.responseValid());
	return activeSensorCardsResponse_.getActiveSensorCardID(index);
}

SensorCard*
Interface::getSensorCard(unsigned char index)
{
	assert(index < getNumActiveSensorCards());
	CHECK_PTR(sensorCards_);
	return &sensorCards_[index];
}

SensorCard*
Interface::getSensorCardByID(unsigned char sensorCardID)
{
	unsigned char	i;

	for (i = 0; i < getNumActiveSensorCards(); i++)
	{
		CHECK_PTR(sensorCards_);
		if (sensorCards_[i].getDeviceID() == sensorCardID)
		{
			return &sensorCards_[i];
		}
	}

	return NULL;
}
