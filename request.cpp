//
// fslurp, Copyright (c) 2012 David Stone <ds.fslurp@asif.com>
//
// $RCSfile: request.cpp,v $
// $Revision: 1.1 $
// $Date: 2012/09/10 07:01:11 $
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

Request::Request(void)
{
	unsigned char	i;

	for (i = 0; i < FRONIUS_NUM_START_BYTES; i++)
	{
		request_.startBytes[i] = FRONIUS_START_VALUE;
	}
	request_.length = 0;
}

void
Request::setDeviceOption(unsigned char deviceOption)
{
	request_.deviceOption = deviceOption;
}

void
Request::setDeviceNumber(unsigned char deviceNumber)
{
	request_.deviceNumber = deviceNumber;
}

void
Request::setCommand(unsigned char command)
{
	request_.command = command;
}

const void*
Request::requestBytes(void)
{
	return (void*)&request_;
}

void
Request::send(Comm& comm)
{
	calculateChecksum();

	if (comm.verbose())
	{
		fprintf(stderr, "writeBytes(%s)\n", decodeCommand());
	}

	comm.writeBytes((unsigned char*)&request_, length());
}

const char*
Request::decodeCommand(void)
{
	return Utils::decodeCommand(request_.command);
}

void
Request::calculateChecksum(void)
{
	unsigned char*	data = &request_.length;
	unsigned char	length = &request_.checksum - &request_.length;
	unsigned char	index;

	request_.checksum = 0;
	for (index = 0; index < length; index++)
	{
		request_.checksum += data[index];
	}
}

unsigned char
Request::length(void)
{
	return sizeof(request_);
}
