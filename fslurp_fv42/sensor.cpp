//
// fslurp, Copyright (c) 2012 David Stone <ds.fslurp@asif.com>
//
// $RCSfile: sensor.cpp,v $
// $Revision: 1.5 $
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

Device::DeviceOption
SensorCard::getDeviceOption(void) const
{
	return DeviceOptionSensorCard;
}

const char*
SensorCard::getModel(void) const
{
	return "Sensor Card";
}

unsigned char
SensorCard::maximumFieldLabelWidth(void) const
{
	// FIX THIS, DS:
	return 36;
}

void
SensorCard::report(Args& args)
{
	NowData		nowData;
	CumulativeData	dayData;
	CumulativeData	yearData;
	CumulativeData	totalData;

	deviceInfoReport(maximumFieldLabelWidth(), args);

	switch(args.getReportType())
	{
		case Args::ReportTypeNow:
			getNowData(nowData);
			displayNowData(args, nowData);
			break;
		case Args::ReportTypeDay:
			getDayData(dayData);
			displayCumulativeData(args, "today", dayData);
			break;
		case Args::ReportTypeYear:
			getYearData(yearData);
			displayCumulativeData(args, "year", yearData);
			break;
		case Args::ReportTypeTotal:
			getTotalData(totalData);
			displayCumulativeData(args, "total", totalData);
			break;
		case Args::ReportTypeAll:
			getNowData(nowData);
			getDayData(dayData);
			getYearData(yearData);
			getTotalData(totalData);
			displayNowData(args, nowData);
			displayCumulativeData(args, "today", dayData);
			displayCumulativeData(args, "year", yearData);
			displayCumulativeData(args, "total", totalData);
			break;
		default:
			assert(0);
	}

	if (args.getDelimiter() != NULL)
	{
		printf("\n");
	}
}

void
SensorCard::getNowData(NowData& nowData)
{
	getMeasuredValue(
		FRONIUS_CMD_GET_TEMPERATURE_CH1_NOW,
		nowData.temperatureChan1);
	getMeasuredValue(
		FRONIUS_CMD_GET_TEMPERATURE_CH2_NOW,
		nowData.temperatureChan2);
	getMeasuredValue(
		FRONIUS_CMD_GET_IRRADIANCE_NOW,
		nowData.irradiance);
	getMeasuredValue(
		FRONIUS_CMD_GET_DIGITAL_CH1_NOW,
		nowData.digitalChan1Value);
	getMeasuredValue(
		FRONIUS_CMD_GET_DIGITAL_CH2_NOW,
		nowData.digitalChan2Value);
}

void
SensorCard::getDayData(CumulativeData& dayData)
{
	getMeasuredValue(
		FRONIUS_CMD_GET_MINIMUM_TEMPERATURE_CH1_DAY,
		dayData.temperatureChan1Minimum);
	getMeasuredValue(
		FRONIUS_CMD_GET_MAXIMUM_TEMPERATURE_CH1_DAY,
		dayData.temperatureChan1Maximum);
	getMeasuredValue(
		FRONIUS_CMD_GET_MINIMUM_TEMPERATURE_CH2_DAY,
		dayData.temperatureChan2Minimum);
	getMeasuredValue(
		FRONIUS_CMD_GET_MAXIMUM_TEMPERATURE_CH2_DAY,
		dayData.temperatureChan2Maximum);
	getMeasuredValue(
		FRONIUS_CMD_GET_MAXIMUM_IRRADIANCE_DAY,
		dayData.irradianceMaximum);
	getMeasuredValue(
		FRONIUS_CMD_GET_MAXIMUM_DIGITAL_CH1_DAY,
		dayData.digitalChan1ValueMaximum);
	getMeasuredValue(
		FRONIUS_CMD_GET_MAXIMUM_DIGITAL_CH2_DAY,
		dayData.digitalChan2ValueMaximum);
}

void
SensorCard::getYearData(CumulativeData& yearData)
{
	assert(supportsYearData());

	getMeasuredValue(
		FRONIUS_CMD_GET_MINIMUM_TEMPERATURE_CH1_YEAR,
		yearData.temperatureChan1Minimum);
	getMeasuredValue(
		FRONIUS_CMD_GET_MAXIMUM_TEMPERATURE_CH1_YEAR,
		yearData.temperatureChan1Maximum);
	getMeasuredValue(
		FRONIUS_CMD_GET_MINIMUM_TEMPERATURE_CH2_YEAR,
		yearData.temperatureChan2Minimum);
	getMeasuredValue(
		FRONIUS_CMD_GET_MAXIMUM_TEMPERATURE_CH2_YEAR,
		yearData.temperatureChan2Maximum);
	getMeasuredValue(
		FRONIUS_CMD_GET_MAXIMUM_IRRADIANCE_YEAR,
		yearData.irradianceMaximum);
	getMeasuredValue(
		FRONIUS_CMD_GET_MAXIMUM_DIGITAL_CH1_YEAR,
		yearData.digitalChan1ValueMaximum);
	getMeasuredValue(
		FRONIUS_CMD_GET_MAXIMUM_DIGITAL_CH2_YEAR,
		yearData.digitalChan2ValueMaximum);
}

void
SensorCard::getTotalData(CumulativeData& totalData)
{
	getMeasuredValue(
		FRONIUS_CMD_GET_MINIMUM_TEMPERATURE_CH1_TOTAL,
		totalData.temperatureChan1Minimum);
	getMeasuredValue(
		FRONIUS_CMD_GET_MAXIMUM_TEMPERATURE_CH1_TOTAL,
		totalData.temperatureChan1Maximum);
	getMeasuredValue(
		FRONIUS_CMD_GET_MINIMUM_TEMPERATURE_CH2_TOTAL,
		totalData.temperatureChan2Minimum);
	getMeasuredValue(
		FRONIUS_CMD_GET_MAXIMUM_TEMPERATURE_CH2_TOTAL,
		totalData.temperatureChan2Maximum);
	getMeasuredValue(
		FRONIUS_CMD_GET_MAXIMUM_IRRADIANCE_TOTAL,
		totalData.irradianceMaximum);
	getMeasuredValue(
		FRONIUS_CMD_GET_MAXIMUM_DIGITAL_CH1_TOTAL,
		totalData.digitalChan1ValueMaximum);
	getMeasuredValue(
		FRONIUS_CMD_GET_MAXIMUM_DIGITAL_CH2_TOTAL,
		totalData.digitalChan2ValueMaximum);
}

void
SensorCard::displayNowData(Args& args, NowData& data)
{
	const char*	delimiter = args.getDelimiter();

	if (delimiter == NULL)
	{
		printf("%-*s : %s degrees\n",
			maximumFieldLabelWidth(),
			"Chan. 1 Temperature now",
			Utils::getValue(data.temperatureChan1));
		printf("%-*s : %s degrees\n",
			maximumFieldLabelWidth(),
			"Chan. 2 Temperature now",
			Utils::getValue(data.temperatureChan2));
		printf("%-*s : %s watts/meter2\n",
			maximumFieldLabelWidth(),
			"Irradiance now",
			Utils::getValue(data.irradiance));
		printf("%-*s : %s\n",
			maximumFieldLabelWidth(),
			"Chan. 1 Value now",
			Utils::getValue(data.digitalChan1Value));
		printf("%-*s : %s\n",
			maximumFieldLabelWidth(),
			"Chan. 2 Value now",
			Utils::getValue(data.digitalChan2Value));
	}
	else
	{
		printf("%s%s",
			Utils::getValue(data.temperatureChan1), delimiter);
		printf("%s%s",
			Utils::getValue(data.temperatureChan2), delimiter);
		printf("%s%s",
			Utils::getValue(data.irradiance), delimiter);
		printf("%s%s",
			Utils::getValue(data.digitalChan1Value), delimiter);
		printf("%s%s",
			Utils::getValue(data.digitalChan2Value), delimiter);
	}
}

void
SensorCard::displayCumulativeData(
	Args& args,
	const char* timePeriod,
	CumulativeData& data)
{
	const char*	delimiter = args.getDelimiter();

	if (delimiter == NULL)
	{
		printf("%-*s : %s degrees\n",
			maximumFieldLabelWidth(),
			Utils::strCat(
				"Chan. 1 Minimum Temperature ", timePeriod),
			Utils::getValue(data.temperatureChan1Minimum));
		printf("%-*s : %s degrees\n",
			maximumFieldLabelWidth(),
			Utils::strCat(
				"Chan. 1 Maximum Temperature ", timePeriod),
			Utils::getValue(data.temperatureChan1Maximum));
		printf("%-*s : %s degrees\n",
			maximumFieldLabelWidth(),
			Utils::strCat(
				"Chan. 2 Minimum Temperature ", timePeriod),
			Utils::getValue(data.temperatureChan2Minimum));
		printf("%-*s : %s degrees\n",
			maximumFieldLabelWidth(),
			Utils::strCat(
				"Chan. 2 Maximum Temperature ", timePeriod),
			Utils::getValue(data.temperatureChan2Maximum));
		printf("%-*s : %s watts/meter2\n",
			maximumFieldLabelWidth(),
			Utils::strCat("Irradiance Maximum ", timePeriod),
			Utils::getValue(data.temperatureChan2Maximum));
		printf("%-*s : %s\n",
			maximumFieldLabelWidth(),
			Utils::strCat("Chan. 1 Maximum value ", timePeriod),
			Utils::getValue(data.digitalChan1ValueMaximum));
		printf("%-*s : %s\n",
			maximumFieldLabelWidth(),
			Utils::strCat("Chan. 2 Maximum value ", timePeriod),
			Utils::getValue(data.digitalChan2ValueMaximum));
	}
	else
	{
		printf("%s%s",
			Utils::getValue(data.temperatureChan1Minimum),
			delimiter);
		printf("%s%s",
			Utils::getValue(data.temperatureChan1Maximum),
			delimiter);
		printf("%s%s",
			Utils::getValue(data.temperatureChan2Minimum),
			delimiter);
		printf("%s%s",
			Utils::getValue(data.temperatureChan2Maximum),
			delimiter);
		printf("%s%s",
			Utils::getValue(data.irradianceMaximum),
			delimiter);
		printf("%s%s",
			Utils::getValue(data.digitalChan1ValueMaximum),
			delimiter);
		printf("%s%s",
			Utils::getValue(data.digitalChan2ValueMaximum),
			delimiter);
	}
}

void
SensorCard::displayNowHeader(Args& args)
{
	const char*	delimiter;

	delimiter = args.getDelimiter();
	CHECK_PTR(delimiter);

	printf("Chan. 1 Temperature now%s", delimiter);
	printf("Chan. 2 Temperature now%s", delimiter);
	printf("Irradiance now%s", delimiter);
	printf("Chan. 1 Value now%s", delimiter);
	printf("Chan. 2 Value now%s", delimiter);
}

void
SensorCard::displayDayHeader(Args& args)
{
	const char*	delimiter;

	delimiter = args.getDelimiter();
	CHECK_PTR(delimiter);

	printf("Chan. 1 Minimum Temperature today (F or C)%s", delimiter);
	printf("Chan. 1 Maximum Temperature today (F or C)%s", delimiter);
	printf("Chan. 2 Minimum Temperature today (F or C)%s", delimiter);
	printf("Chan. 2 Maximum Temperature today (F or C)%s", delimiter);
	printf("Irradiance Maximum today (Watts/meter2)%s", delimiter);
	printf("Chan. 1 Maximum value today%s", delimiter);
	printf("Chan. 2 Maximum value today%s", delimiter);
}

void
SensorCard::displayYearHeader(Args& args)
{
	const char*	delimiter;

	delimiter = args.getDelimiter();
	CHECK_PTR(delimiter);

	printf("Chan. 1 Minimum Temperature this year (F or C)%s", delimiter);
	printf("Chan. 1 Maximum Temperature this year (F or C)%s", delimiter);
	printf("Chan. 2 Minimum Temperature this year (F or C)%s", delimiter);
	printf("Chan. 2 Maximum Temperature this year (F or C)%s", delimiter);
	printf("Irradiance Maximum this year (Watts/meter2)%s", delimiter);
	printf("Chan. 1 Maximum value this year%s", delimiter);
	printf("Chan. 2 Maximum value this year%s", delimiter);
}

void
SensorCard::displayTotalHeader(Args& args)
{
	const char*	delimiter;

	delimiter = args.getDelimiter();
	CHECK_PTR(delimiter);

	printf("Chan. 1 Minimum Temperature total (F or C)%s", delimiter);
	printf("Chan. 1 Maximum Temperature total (F or C)%s", delimiter);
	printf("Chan. 2 Minimum Temperature total (F or C)%s", delimiter);
	printf("Chan. 2 Maximum Temperature total (F or C)%s", delimiter);
	printf("Irradiance Maximum total (Watts/meter2)%s", delimiter);
	printf("Chan. 1 Maximum value total%s", delimiter);
	printf("Chan. 2 Maximum value total%s", delimiter);
}
