//
// fslurp, Copyright (c) 2012, 2013 David Stone <ds.fslurp@asif.com>
//
// $RCSfile: device.cpp,v $
// $Revision: 1.7 $
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

Device::Device(void)
{
	deviceID_ = ~0;
	comm_ = NULL;
	supportsYearData_ = false;
	supportsMinimumACVoltageData_ = true;
}

void
Device::initialize(
	Comm* comm,
	unsigned char interfaceType,
	unsigned char deviceID,
	bool supportsYearData)
{
	Request	request;

	CHECK_PTR(comm);
	comm_ = comm;

	request.setDeviceOption(getDeviceOption());
	request.setDeviceNumber(deviceID);
	request.setCommand(FRONIUS_CMD_GET_DEVICE_TYPE);

	request.send(*comm_);
	deviceTypeResponse_.receive(*comm_);

	deviceID_ = deviceID;
	interfaceType_ = interfaceType;
	supportsYearData_ = supportsYearData;

	switch (getDeviceType())
	{
	case FRONIUS_IG_TL_3_6_INVERTER:
	case FRONIUS_IG_TL_5_0_INVERTER:
	case FRONIUS_IG_TL_4_0_INVERTER:
	case FRONIUS_IG_TL_3_0_INVERTER:
	case FRONIUS_IG_TL_4_6_INVERTER:
	case FRONIUS_IG_TL_DUMMY_INVERTER:
	case FRONIUS_AGILO_75_0_3_OUTDOOR_INVERTER:
	case FRONIUS_AGILO_100_0_3_OUTDOOR_INVERTER:
	case FRONIUS_AGILO_100_0_3_INVERTER:
	case FRONIUS_AGILO_75_0_3_INVERTER:
	case FRONIUS_AGILO_100_0_3_DUMMY_INVERTER:
		if (interfaceType_ == FRONIUS_INTERFACE_RS_422)
		{
			// AC voltage DAY
			supportsMinimumACVoltageData_ = false;

			// yield, max AC, max DC, op hours, 
			supportsYearData_ = true;
		}
		break;

	case FRONIUS_CL_36_0_INVERTER:
	case FRONIUS_CL_48_0_INVERTER:
	case FRONIUS_CL_60_0_INVERTER:
	case FRONIUS_CL_36_0_WYE277_INVERTER:
	case FRONIUS_CL_48_0_WYE277_INVERTER:
	case FRONIUS_CL_60_0_WYE277_INVERTER:
	case FRONIUS_CL_33_3_INVERTER:
	case FRONIUS_CL_44_4_INVERTER:
	case FRONIUS_CL_55_5_INVERTER:
	case FRONIUS_CL_60_0_DUMMY_INVERTER:
	case FRONIUS_CL_55_5_DUMMY_INVERTER:
	case FRONIUS_CL_60_0_WYE277_DUMMY_INVERTER:
	case FRONIUS_IG_PLUS_11_4_3_INVERTER:
	case FRONIUS_IG_PLUS_11_4_1_INVERTER:
	case FRONIUS_IG_PLUS_10_0_1_INVERTER:
	case FRONIUS_IG_PLUS_7_5_1_INVERTER:
	case FRONIUS_IG_PLUS_6_0_1_INVERTER:
	case FRONIUS_IG_PLUS_5_0_1_INVERTER:
	case FRONIUS_IG_PLUS_3_8_1_INVERTER:
	case FRONIUS_IG_PLUS_3_0_1_INVERTER:
	case FRONIUS_IG_PLUS_120_3_INVERTER:
	case FRONIUS_IG_PLUS_70_2_INVERTER:
	case FRONIUS_IG_PLUS_70_1_INVERTER:
	case FRONIUS_IG_PLUS_35_1_INVERTER:
	case FRONIUS_IG_PLUS_150_3_INVERTER:
	case FRONIUS_IG_PLUS_100_2_INVERTER:
	case FRONIUS_IG_PLUS_100_1_INVERTER:
	case FRONIUS_IG_PLUS_50_1_INVERTER:
	case FRONIUS_IG_PLUS_12_0_3_INVERTER:
	case FRONIUS_IG_PLUS_55V_1_INVERTER:
	case FRONIUS_IG_PLUS_55V_2_INVERTER:
	case FRONIUS_IG_PLUS_60V_2_INVERTER:
	case FRONIUS_IG_PLUS_60V_1_INVERTER:
	case FRONIUS_IG_PLUS_25V_1_INVERTER:
	case FRONIUS_IG_PLUS_35V_1_INVERTER:
	case FRONIUS_IG_PLUS_50V_1_INVERTER:
	case FRONIUS_IG_PLUS_70V_1_INVERTER:
	case FRONIUS_IG_PLUS_70V_2_INVERTER:
	case FRONIUS_IG_PLUS_100V_1_INVERTER:
	case FRONIUS_IG_PLUS_100V_2_INVERTER:
	case FRONIUS_IG_PLUS_100V_3_INVERTER:
	case FRONIUS_IG_PLUS_120V_3_INVERTER:
	case FRONIUS_IG_PLUS_150V_3_INVERTER:
	case FRONIUS_IG_PLUS_V_3_0_1_INVERTER:
	case FRONIUS_IG_PLUS_V_3_8_1_INVERTER:
	case FRONIUS_IG_PLUS_V_5_0_1_INVERTER:
	case FRONIUS_IG_PLUS_V_6_0_1_INVERTER:
	case FRONIUS_IG_PLUS_V_7_5_1_INVERTER:
	case FRONIUS_IG_PLUS_V_10_0_1_INVERTER:
	case FRONIUS_IG_PLUS_V_10_0_3_INVERTER:
	case FRONIUS_IG_PLUS_30V_1_INVERTER:
	case FRONIUS_IG_PLUS_V_11_4_1_INVERTER:
	case FRONIUS_IG_PLUS_V_11_4_3_INVERTER:
	case FRONIUS_IG_PLUS_V_12_0_3_INVERTER:
	case FRONIUS_IG_PLUS_50V_1_DUMMY_INVERTER:
	case FRONIUS_IG_PLUS_100V_2_DUMMY_INVERTER:
	case FRONIUS_IG_PLUS_150V_3_DUMMY_INVERTER:
	case FRONIUS_IG_PLUS_V_3_8_1_DUMMY_INVERTER:
	case FRONIUS_IG_PLUS_V_7_5_1_DUMMY_INVERTER:
	case FRONIUS_IG_PLUS_V_12_0_3_DUMMY_INVERTER:
	case FRONIUS_IG_PLUS_80V_3_INVERTER:
	case FRONIUS_IG_PLUS_60V_3_INVERTER:
	case FRONIUS_IG_PLUS_55V_3_INVERTER:
		if (interfaceType_ == FRONIUS_INTERFACE_RS_422)
		{
			supportsYearData_ = false;
		}
		break;
	}
}

unsigned char
Device::getDeviceID(void) const
{
	return deviceID_;
}

bool
Device::supportsYearData(void) const
{
	return supportsYearData_;
}

bool
Device::supportsMinimumACVoltageData(void) const
{
	return supportsMinimumACVoltageData_;
}

unsigned char
Device::getDeviceType(void) const
{
	return deviceTypeResponse_.getDeviceType();
}

void
Device::getMeasuredValue(
	unsigned char command,
	MeasuredValue& measuredValue)
{
	Request			request;

	// FIX THIS, DS: these are fixed length.
	// provide the buffer from the specialization
	// and compile it in.
	MeasuredValueResponse	response;

	request.setDeviceOption(getDeviceOption());
	request.setDeviceNumber(getDeviceID());
	request.setCommand(command);

	CHECK_PTR(comm_);
	request.send(*comm_);
	response.receive(*comm_);

	measuredValue.lsb = response.getLSB();
	measuredValue.msb = response.getMSB();
	measuredValue.exponent = response.getExponent();
}

void
Device::reportHeader(Args& args)
{
	reportHeader(args, supportsYearData_);
}

// For use by iterators, where we there might not
// be a device instance to ask.
void
Device::reportHeader(Args& args, bool supportsYearData)
{
	const char*		delimiter;
	Args::ReportType	reportType;

	reportType = args.getReportType();
	assert(reportType != Args::ReportTypeSystem);

	delimiter = args.getDelimiter();
	CHECK_PTR(delimiter);

	// FIX THIS, DS: this block is repeated in Interface::reportHeader
	printf("Time%s", delimiter);
	printf("Device Number%s", delimiter);
	printf("Device Type%s", delimiter);
	printf("Description%s", delimiter);

	switch (args.getReportType())
	{
		case Args::ReportTypeNow:
			displayNowHeader(args);
			break;

		case Args::ReportTypeDay:
			displayDayHeader(args);
			break;

		case Args::ReportTypeYear:
			displayYearHeader(args);
			break;

		case Args::ReportTypeTotal:
			displayTotalHeader(args);
			break;

		case Args::ReportTypeAll:
			displayNowHeader(args);
			displayDayHeader(args);
			if (supportsYearData)
			{
				displayYearHeader(args);
			}
			displayTotalHeader(args);
			break;

		default:
			assert(0);
	}

	printf("\n");
}

void
Device::deviceInfoReport(
	unsigned char fieldLabelWidth,
	Args& args)
{
	const char*	delimiter;

	delimiter = args.getDelimiter();

	if (delimiter == NULL)
	{
		printf("%-*s : %d\n",
			fieldLabelWidth,
			"Device number",
			getDeviceID());
		printf("%-*s : 0x%02X\n",
			fieldLabelWidth,
			"Device type",
			getDeviceType());
		printf("%-*s : %s\n",
			fieldLabelWidth,
			"Device description",
			getModel());
	}
	else
	{
		Utils::displayTimestamp(maximumFieldLabelWidth(), args);
		printf("%d%s", getDeviceID(), delimiter);
		printf("0x%02X%s", getDeviceType(), delimiter);
		printf("%s%s", getModel(), delimiter);

		if (args.getReportType() == Args::ReportTypeSystem)
		{
			printf("\n");
		}
	}
}
