//
// fslurp, Copyright (c) 2012 David Stone <ds.fslurp@asif.com>
//
// $RCSfile: args.cpp,v $
// $Revision: 1.7 $
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

const char* Args::programName_;

Args::Args(int argc, char* argv[])
{
	// set defaults

	serialPortName_ = "/dev/ttyS0";
	baudRate_ = "9600";
	reportTypeName_ = "system";
	reportType_ = ReportTypeSystem;
	deviceType_ = DeviceTypeInverter;
	deviceTargeting_ = AllInstances;
	timeFormat_ = "%c";
	delimiter_ = NULL;
	showReportHeader_ = false;
	verbose_ = false;

	parseArgs(argc, argv);

	// initialize global timestamp

	timestamp_ = time(NULL);
}

const char*
Args::getSerialPortName(void) const
{
	return serialPortName_;
}

const char*
Args::getDelimiter(void) const
{
	return delimiter_;
}

const char*
Args::getTimeFormat(void) const
{
	return timeFormat_;
}

time_t
Args::getTimestamp(void) const
{
	return timestamp_;
}

bool
Args::showReportHeader(void) const
{
	return showReportHeader_;
}

void
Args::usage(void)
{
	fprintf(stderr, 

	"Usage: %s\n"
	"       [-b baud] [-D device_type] [-d delimiter] [-H] [-h]\n"
	"       [-n device_number] [-p serial_port_device] [-r report]\n"
	"       [-t format] [-v]\n"
	"\n"
	"-b baud              Set the baud rate.  Supported rates are:\n"
	"                     2400, 4800, 9600, and 19200. Defaults to %s.\n"
	"-D device_type       inverter or sensor. Defaults to inverter.\n"
	"-d delimiter         Set delimiter for CSV-style output. The\n"
	"                     default is no delimiter, human-readable.\n"
	"-H                   Include the delimited output header for the\n"
	"                     current reporting mode.  The -d option must\n"
	"                     also be set.\n"
	"-h                   Display this help message.\n"
	"-n device_number     Run the report for an individual inverter or\n"
	"                     sensor card. The device number is specified in\n"
	"                     decimal, and can be seen in the\n"
	"                     The default is to report on all\n"
	"                     devices of the current type (see the -D option)\n"
	"-p serial_device     Set the serial port device. Defaults to %s.\n"
	"-r report            Set report type. Supported types are:\n"
	"                     system, now, day, year, total, and all.\n"
	"                     Defaults to %s. Year is not supported for\n"
	"                     Interface Card Easy systems.\n"
	"-t format            Set the format for the timestamp field,\n"
	"                     using the Linux strftime(3) format string.\n"
	"                     The default is \"%%c\", which looks like:\n"
	"                             Wed May 26 10:43:25 2010\n"
	"-v                   Enable verbose, debugging output.\n"
	"\n"
	"Return codes:\n"
	"\n"
	"%d                   Success.\n"
	"%d                   Communications failure.\n"
	"%d                   Inverter not found.\n"
	"%d                   Sensor card not found.\n"
	"%d                   Usage error.\n"
	"%d                   Fronius protocol error.\n",

	programName_,
	baudRate_,
	serialPortName_,
	reportTypeName_,
	ExitNormal,
	ExitCommFailure,
	ExitInverterNotFound,
	ExitSensorCardNotFound,
	ExitUsageError,
	ExitProtocolError);
}

void
Args::parseArgs(int argc, char* argv[])
{
	int	c;

	CHECK_PTR(argv);
	assert(argc >= 1);

	programName_ = argv[0];

	while (1)
	{
		c = getopt(argc, argv, "b:D:d:Hhn:p:r:t:v");

		if (c == -1)
		{
			break;
		}

		switch (c)
		{
			case '?':
				usage();
				exit(ExitUsageError);
				break;
			case 'b':
				setBaudRate(optarg);
				break;
			case 'D':
				setDeviceType(optarg);
				break;
			case 'd':
				delimiter_ = optarg;
				break;
			case 'H':
				showReportHeader_ = true;
				break;
			case 'h':
				usage();
				exit(ExitNormal);
				break;
			case 'n':
				deviceNumber_ = atoi(optarg);
				deviceTargeting_ = SingleTarget;

				// FIX THIS, DS; audit against 0?
				break;
			case 'p':
				serialPortName_ = optarg;
				break;
			case 'r':
				setReportType(optarg);
				break;
			case 't':
				timeFormat_ = optarg;
				break;
			case 'v':
				verbose_ = 1;
				break;
			default:
				dieUsage("Unsupported option: %c\n", c);
				break;
		}
	}

	if (optind < argc)
	{
		dieUsage("Unsupported arguments.\n");
	}

	auditArgs();
}

void
Args::auditArgs(void)
{
	if (showReportHeader_ && (delimiter_ == NULL))
	{
		dieUsage("-H requires that the delimiter also be set (-d).\n");
	}

	if ((reportType_ == ReportTypeSystem)
		&& (deviceTargeting_ == SingleTarget))
	{
		dieUsage("The -n option is not allowed with "
			"the system report.\n");
	}
}

void
Args::dieUsage(const char* format, ...)
{
	va_list	ap;
	va_start(ap, format);
	fprintf(stderr, "%s ERROR: ", programName_);
	vfprintf(stderr, format, ap);
	va_end(ap);
	fprintf(stderr, "\n");
	usage();
	exit(ExitUsageError);
}

void
Args::dieMessage(int returnCode, const char* format, ...)
{
	va_list	ap;
	va_start(ap, format);
	fprintf(stderr, "%s ERROR: ", programName_);
	vfprintf(stderr, format, ap);
	va_end(ap);
	exit(returnCode);
}

void Args::setBaudRate(const char* baudRate)
{
	if ((strcmp(baudRate, "2400") == 0)
		|| (strcmp(baudRate, "4800") == 0)
		|| (strcmp(baudRate, "9600") == 0)
		|| (strcmp(baudRate, "19200") == 0))
	{
		baudRate_ = baudRate;
	}
	else
	{
		dieUsage("%s is not a supported baud rate\n.", baudRate);
	}
}

void Args::setReportType(const char* reportType)
{
	if (strcmp(reportType, "now") == 0)
	{
		reportType_ = ReportTypeNow;
	}
	else if (strcmp(reportType, "day") == 0)
	{
		reportType_ = ReportTypeDay;
	}
	else if (strcmp(reportType, "year") == 0)
	{
		reportType_ = ReportTypeYear;
	}
	else if (strcmp(reportType, "total") == 0)
	{
		reportType_ = ReportTypeTotal;
	}
	else if (strcmp(reportType, "all") == 0)
	{
		reportType_ = ReportTypeAll;
	}
	else if (strcmp(reportType, "system") == 0)
	{
		reportType_ = ReportTypeSystem;
	}
	else
	{
		dieUsage("%s is not a supported report type.\n", reportType);
	}
}

void Args::setDeviceType(const char* deviceType)
{
	if (strcmp(deviceType, "inverter") == 0)
	{
		deviceType_ = DeviceTypeInverter;
	}
	else if (strcmp(deviceType, "sensor") == 0)
	{
		deviceType_ = DeviceTypeSensorCard;
	}
	else
	{
		dieUsage("%s is not a supported device type.\n", deviceType);
	}
}

const char*
Args::getBaudRate(void) const
{
	return baudRate_;
}

bool
Args::getVerbose(void) const
{
	return verbose_;
}

Args::ReportType
Args::getReportType(void) const
{
	return reportType_;
}

Args::DeviceType
Args::getDeviceType(void) const
{
	return deviceType_;
}

unsigned char
Args::getDeviceNumber(void) const
{
	assert(deviceTargeting_ == SingleTarget);
	return deviceNumber_;
}

Args::DeviceTargeting
Args::getDeviceTargeting(void) const
{
	return deviceTargeting_;
}
