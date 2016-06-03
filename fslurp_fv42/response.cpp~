//
// fslurp, Copyright (c) 2012 David Stone <ds.fslurp@asif.com>
//
// $RCSfile: response.cpp,v $
// $Revision: 1.4 $
// $Date: 2012/10/08 00:56:31 $
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

Response::Response(void)
{
	valid_ = false;
	data_ = NULL;
}

Response::~Response(void)
{
	if (data_ != NULL)
	{
		delete [] data_;
		data_ = NULL;
	}
}

bool
Response::responseValid(void) const
{
	return valid_;
}

void
Response::receive(Comm& comm)
{
	readResponseBytes(comm);

	if (header_.command == FRONIUS_ERROR)
	{
		handleError();
	}

	valid_ = true;
}

void
Response::readResponseBytes(Comm& comm)
{
	unsigned char	length;

	if (comm.verbose())
	{
		fprintf(stderr, "readBytes(header)\n");
	}

	comm.readBytes((unsigned char*)&header_, sizeof (header_));
	validateHeader();

	length = header_.length;

	// add checksum to length
	length++;

	assert(data_ == NULL);
	data_ = new unsigned char[length];

	if (comm.verbose())
	{
		fprintf(stderr, "readBytes(data)\n");
	}

	// read the response data and the checksum
	comm.readBytes(data_, length);

	validateChecksum();

}

void
Response::validateHeader(void)
{
	unsigned char	i;

	for (i = 0; i < FRONIUS_NUM_START_BYTES; i++)
	{
		if (header_.startBytes[i] != FRONIUS_START_VALUE)
		{
			Args::dieMessage(
				Args::ExitCommFailure,
				"byte %d of the start field invalid: %x\n",
				i, header_.startBytes[i]);
		}
	}
}

void
Response::validateChecksum(void)
{
	unsigned char	checksum;
	unsigned char	i;

	checksum = 0;

	checksum += header_.length;
	checksum += header_.deviceOption;
	checksum += header_.deviceNumber;
	checksum += header_.command;

	for (i = 0; i < header_.length; i++)
	{
		CHECK_PTR( data_ );
		checksum += data_[i];
	}

	if (checksum != data_[header_.length])
	{
		Args::dieMessage(
			Args::ExitProtocolError,
			"response checksum mismatch, got %d, expected %d\n",
			data_[header_.length],
			checksum);
	}
}

void
Response::handleError(void)
{
	struct ErrorResponse*	response;

	CHECK_PTR( data_ );
	response = (ErrorResponse*)data_;

	Args::dieMessage(
		Args::ExitProtocolError,
		"%s:%d command '%s' failed: %s\n",
		Utils::decodeDeviceOption(header_.deviceOption),
		header_.deviceNumber,
		Utils::decodeCommand(response->failedCommand),
		Utils::decodeErrorInformation(response->errorInformation));
}

const void*
Response::getResponseData(void) const
{
	assert(valid_);
	return data_;
}

unsigned char
Response::getResponseDataLength(void) const
{
	assert(valid_);
	return header_.length;
}

unsigned char
VersionResponse::getInterfaceType(void) const
{
	VersionData	*versionData;

	versionData = (VersionData*)getResponseData();

	CHECK_PTR( versionData );
	return versionData->interfaceType;
}

unsigned char
VersionResponse::getVersionMajor(void) const
{
	VersionData	*versionData;

	versionData = (VersionData*)getResponseData();

	CHECK_PTR( versionData );
	return versionData->versionMajor;
}

unsigned char
VersionResponse::getVersionMinor(void) const
{
	VersionData	*versionData;

	versionData = (VersionData*)getResponseData();

	CHECK_PTR( versionData );
	return versionData->versionMinor;
}

unsigned char
VersionResponse::getVersionRelease(void) const
{
	VersionData	*versionData;

	versionData = (VersionData*)getResponseData();

	CHECK_PTR( versionData );
	return versionData->versionRelease;
}

unsigned char
ActiveInverterNumbersResponse::numActiveInverters(void) const
{
	//
	// Get Active Inverter Numbers command returns a variable
	// length array of active inverter IDs
	//

	return getResponseDataLength();
}

unsigned char
ActiveInverterNumbersResponse::getActiveInverterID(unsigned char index) const
{
	unsigned char*	activeInverters;

	activeInverters = (unsigned char*)getResponseData();

	CHECK_PTR(activeInverters);
	assert(index < getResponseDataLength());
	return activeInverters[index];
}

unsigned char
ActiveSensorCardsResponse::numActiveSensorCards(void) const
{
	//
	// Get Active Sensor Cards command returns a variable
	// length array of active sensor card IDs
	//

	return getResponseDataLength();
}

unsigned char
ActiveSensorCardsResponse::getActiveSensorCardID(unsigned char index) const
{
	unsigned char*	activeSensorCards;

	activeSensorCards = (unsigned char*)getResponseData();

	CHECK_PTR(activeSensorCards);
	assert(index < getResponseDataLength());
	return activeSensorCards[index];
}

unsigned char
DeviceTypeResponse::getDeviceType(void) const
{
	DeviceTypeData	*deviceTypeData;

	deviceTypeData = (DeviceTypeData*)getResponseData();

	CHECK_PTR( deviceTypeData );
	return deviceTypeData->deviceType;
}

unsigned char
SolarNetStatusResponse::getSolarNetStatus(void) const
{
	SolarNetStatusData	*solarNetStatusData;

	solarNetStatusData = (SolarNetStatusData*)getResponseData();

	CHECK_PTR( solarNetStatusData );
	return solarNetStatusData->status;
}

unsigned char
MeasuredValueResponse::getLSB(void) const
{
	MeasuredValue	*measuredValue;

	measuredValue = (MeasuredValue*)getResponseData();

	CHECK_PTR( measuredValue );
	return measuredValue->lsb;
}

unsigned char
MeasuredValueResponse::getMSB(void) const
{
	MeasuredValue	*measuredValue;

	measuredValue = (MeasuredValue*)getResponseData();

	CHECK_PTR( measuredValue );
	return measuredValue->msb;
}

signed char
MeasuredValueResponse::getExponent(void) const
{
	MeasuredValue	*measuredValue;

	measuredValue = (MeasuredValue*)getResponseData();

	CHECK_PTR( measuredValue );
	return measuredValue->exponent;
}
