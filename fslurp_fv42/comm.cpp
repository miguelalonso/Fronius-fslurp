//
// fslurp, Copyright (c) 2012 David Stone <ds.fslurp@asif.com>
//
// $RCSfile: comm.cpp,v $
// $Revision: 1.2 $
// $Date: 2012/10/08 00:56:41 $
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

Comm::Comm(Args& args)
{
	fd_ = -1;
	verbose_ = args.getVerbose();
	serialDeviceInit(args);
}

void
Comm::serialDeviceInit(Args& args)
{
	struct termios	termio;
	const char*	deviceName = args.getSerialPortName();
	const char*	baudRate = args.getBaudRate();

	if ((fd_ = open(deviceName, O_RDWR|O_NOCTTY)) == -1)
	{
		Args::dieMessage(
			Args::ExitCommFailure,
			"open of %s failed: %s\n",
			deviceName,
			strerror(errno));
	}

	if (tcflush(fd_, TCIFLUSH) == -1)
	{
		Args::dieMessage(
			Args::ExitCommFailure,
			"tcflush failed: %s\n",
			strerror(errno));
	}

	memset(&termio, 0, sizeof (termio));

	termio.c_cflag = getBaudRateFlag(baudRate) | CS8 | CLOCAL | CREAD;
	termio.c_iflag = IGNPAR;
	termio.c_oflag = 0;
	termio.c_lflag = 0;
	termio.c_cc[VMIN] = 0;
	termio.c_cc[VTIME] = 1;

	if (tcsetattr(fd_, TCSANOW, &termio) == -1)
	{
		Args::dieMessage(
			Args::ExitCommFailure,
			"tcsetattr failed: %s\n",
			strerror(errno));
	}
}

unsigned int
Comm::getBaudRateFlag(const char* baudRate)
{
	unsigned int	baudRateFlag;

	if (strcmp(baudRate, "2400") == 0)
	{
		baudRateFlag = B2400;
	}
	else if (strcmp(baudRate, "4800") == 0)
	{
		baudRateFlag = B4800;
	}
	else if (strcmp(baudRate, "9600") == 0)
	{
		baudRateFlag = B9600;
	}
	else
	{
		assert(strcmp(baudRate, "19200") == 0);
		baudRateFlag = B19200;
	}

	return baudRateFlag;
}

void
Comm::writeBytes(
	unsigned char* buffer,
	unsigned char numBytesToWrite)
{
	int	bytesWritten;

	CHECK_PTR( buffer );

	if (verbose_)
	{
		Utils::hexDump(buffer, numBytesToWrite);
	}

	bytesWritten = write(fd_, buffer, numBytesToWrite);

	if (bytesWritten == -1)
	{
		Args::dieMessage(
			Args::ExitCommFailure,
			"write failed: %s\n",
			strerror(errno));
	}

	if (bytesWritten != numBytesToWrite)
	{
		Args::dieMessage(
			Args::ExitCommFailure,
			"writeBytes(%d) only wrote %d bytes\n",
			numBytesToWrite,
			bytesWritten);
	}
}

void
Comm::readBytes(
	unsigned char* buffer,
	unsigned char numBytesToRead)
{
	int	totalBytesRead;
	int	bytesRead;
	int	numRetries;

	for (numRetries = 0, totalBytesRead = 0;
		totalBytesRead < numBytesToRead; )
	{
		bytesRead = read(
			fd_,
			buffer + totalBytesRead,
			numBytesToRead - totalBytesRead);

		if (bytesRead == 0)
		{
			numRetries++;
			if (numRetries > numBytesToRead)
			{
				Args::dieMessage(
					Args::ExitCommFailure,
					"readBytes timeout\n");
			}
			else if (verbose_)
			{
				fprintf(stderr, "readBytes: retrying read\n");
			}
		}
		else if (bytesRead == -1)
		{
			Args::dieMessage(
				Args::ExitCommFailure,
				"readBytes failed: %s\n",
				strerror(errno));
		}
		else
		{
			totalBytesRead += bytesRead;
		}
	}

	if (verbose_)
	{
		Utils::hexDump(buffer, totalBytesRead);
	}
}

void
Comm::flush(void)
{
	assert(fd_ != -1);

	if (tcflush(fd_, TCIOFLUSH) == -1)
	{
		Args::dieMessage(
			Args::ExitCommFailure,
			"tcflush(TCIOFLUSH) failed: %s\n",
			strerror(errno));
	}
}

bool
Comm::verbose(void)
{
	return verbose_;
}
