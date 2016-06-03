//
// fslurp, Copyright (c) 2012, 2013 David Stone <ds.fslurp@asif.com>
//
// $RCSfile: inverter.cpp,v $
// $Revision: 1.9 $
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

static Inverter::DeviceTypeMap deviceTypeMap[] =
{
	{
		FRONIUS_IG_15_INVERTER,
		"FRONIUS IG 15 1-phase inverter"
	},
	{
		FRONIUS_IG_20_INVERTER,
		"FRONIUS IG 20 1-phase inverter"
	},
	{
		FRONIUS_IG_30_INVERTER,
		"FRONIUS IG 30 1-phase inverter"
	},
	{
		FRONIUS_IG_40_DUMMY_INVERTER,
		"FRONIUS IG 30 Dummy, Dummy inverter"
	},
	{
		FRONIUS_IG_40_INVERTER,
		"FRONIUS IG 40 1-phase inverter"
	},
	{
		FRONIUS_IG_50_INVERTER,
		"FRONIUS IG 50 1-phase inverter"
	},
	{
		FRONIUS_IG_60_INVERTER,
		"FRONIUS IG 60 / IG 60 HV, 1-phase inverter"
	},
	{
		FRONIUS_IG_300_INVERTER,
		"FRONIUS IG 300 3-phase inverter"
	},
	{
		FRONIUS_IG_400_INVERTER,
		"FRONIUS IG 400 3-phase inverter"
	},
	{
		FRONIUS_IG_500_INVERTER,
		"FRONIUS IG 500 3-phase inverter"
	},
	{
		FRONIUS_IG_60_DUPLICATE_ID_INVERTER,
		"FRONIUS IG 60 / IG 60 HV, 1-phase inverter"
	},
	{
		FRONIUS_IG_2000_INVERTER,
		"FRONIUS IG 2000 1-phase inverter"
	},
	{
		FRONIUS_IG_3000_INVERTER,
		"FRONIUS IG 3000 1-phase inverter"
	},
	{
		FRONIUS_IG_4000_INVERTER,
		"FRONIUS IG 4000 1-phase inverter"
	},
	{
		FRONIUS_IG_5100_INVERTER,
		"FRONIUS IG 5100 1-phase inverter"
	},
	{
		FRONIUS_IG_2500_LV_INVERTER,
		"FRONIUS IG 2500-LV 1-phase inverter"
	},
	{
		FRONIUS_IG_4500_LV_INVERTER,
		"FRONIUS IG 4500-LV 1-phase inverter"
	},
	{
		FRONIUS_IG_PLUS_11_4_3_INVERTER,
		"Fronius IG Plus 11.4-3 Delta 3-phase inverter"
	},
	{
		FRONIUS_IG_PLUS_11_4_1_INVERTER,
		"Fronius IG Plus 11.4-1 UNI 1-phase inverter"
	},
	{
		FRONIUS_IG_PLUS_10_0_1_INVERTER,
		"Fronius IG Plus 10.0-1 UNI 1-phase inverter"
	},
	{
		FRONIUS_IG_PLUS_7_5_1_INVERTER,
		"Fronius IG Plus 7.5-1 UNI 1-phase inverter"
	},
	{
		FRONIUS_IG_PLUS_6_0_1_INVERTER,
		"Fronius IG Plus 6.0-1 UNI 1-phase inverter"
	},
	{
		FRONIUS_IG_PLUS_5_0_1_INVERTER,
		"Fronius IG Plus 5.0-1 UNI 1-phase inverter"
	},
	{
		FRONIUS_IG_PLUS_3_8_1_INVERTER,
		"Fronius IG Plus 3.8-1 UNI 1-phase inverter"
	},
	{
		FRONIUS_IG_PLUS_3_0_1_INVERTER,
		"Fronius IG Plus 3.0-1 UNI 1-phase inverter"
	},
	{
		FRONIUS_IG_PLUS_120_3_INVERTER,
		"Fronius IG Plus 120-3 3-phase inverter"
	},
	{
		FRONIUS_IG_PLUS_70_2_INVERTER,
		"Fronius IG Plus 70-2 2-phase inverter"
	},
	{
		FRONIUS_IG_PLUS_70_1_INVERTER,
		"Fronius IG Plus 70-1 1-phase inverter"
	},
	{
		FRONIUS_IG_PLUS_35_1_INVERTER,
		"Fronius IG Plus 35-1 1-phase inverter"
	},
	{
		FRONIUS_IG_PLUS_150_3_INVERTER,
		"Fronius IG Plus 150-3 3-phase inverter"
	},
	{
		FRONIUS_IG_PLUS_100_2_INVERTER,
		"Fronius IG Plus 100-2 2-phase inverter"
	},
	{
		FRONIUS_IG_PLUS_100_1_INVERTER,
		"Fronius IG Plus 100-1 1-phase inverter"
	},
	{
		FRONIUS_IG_PLUS_50_1_INVERTER,
		"Fronius IG Plus 50-1 1-phase inverter"
	},
	{
		FRONIUS_IG_PLUS_12_0_3_INVERTER,
		"Fronius IG Plus 12.0-3 WYE277 3-phase inverter"
	},
	{
		FRONIUS_IG_TL_3_6_INVERTER,
		"Fronius IG TL 3.6 1-phase inverter"
	},
	{
		FRONIUS_IG_TL_5_0_INVERTER,
		"Fronius IG TL 5.0 1-phase inverter"
	},
	{
		FRONIUS_IG_TL_4_0_INVERTER,
		"Fronius IG TL 4.0 1-phase inverter"
	},
	{
		FRONIUS_IG_TL_3_0_INVERTER,
		"Fronius IG TL 3.0 1-phase inverter"
	},
	{
		FRONIUS_IG_TL_4_6_INVERTER,
		"Fronius IG TL 4.6 1-phase inverter"
	},
	{
		FRONIUS_IG_TL_DUMMY_INVERTER,
		"Fronius IG TL Dummy, Dummy inverter"
	},
	{
		FRONIUS_AGILO_75_0_3_OUTDOOR_INVERTER,
		"Fronius Agilo 75.0-3 Outdoor 3-phase inverter"
	},
	{
		FRONIUS_AGILO_100_0_3_OUTDOOR_INVERTER,
		"Fronius Agilo 100.0-3 Outdoor 3-phase inverter"
	},
	{
		FRONIUS_IG_PLUS_55V_1_INVERTER,
		"Fronius IG Plus 55V-1 1-phase inverter"
	},
	{
		FRONIUS_IG_PLUS_55V_2_INVERTER,
		"Fronius IG Plus 55V-2 2-phase inverter"
	},
	{
		FRONIUS_IG_PLUS_60V_2_INVERTER,
		"Fronius IG Plus 60V-2 2-phase inverter"
	},
	{
		FRONIUS_IG_PLUS_60V_1_INVERTER,
		"Fronius IG Plus 60V-1 1-phase inverter"
	},
	{
		FRONIUS_IG_PLUS_25V_1_INVERTER,
		"Fronius IG Plus 25V-1 1-phase inverter"
	},
	{
		FRONIUS_IG_PLUS_35V_1_INVERTER,
		"Fronius IG Plus 35V-1 1-phase inverter"
	},
	{
		FRONIUS_IG_PLUS_50V_1_INVERTER,
		"Fronius IG Plus 50V-1 1-phase inverter"
	},
	{
		FRONIUS_IG_PLUS_70V_1_INVERTER,
		"Fronius IG Plus 70V-1 1-phase inverter"
	},
	{
		FRONIUS_IG_PLUS_70V_2_INVERTER,
		"Fronius IG Plus 70V-2 2-phase inverter"
	},
	{
		FRONIUS_IG_PLUS_100V_1_INVERTER,
		"Fronius IG Plus 100V-1 1-phase inverter"
	},
	{
		FRONIUS_IG_PLUS_100V_2_INVERTER,
		"Fronius IG Plus 100V-2 2-phase inverter"
	},
	{
		FRONIUS_IG_PLUS_100V_3_INVERTER,
		"Fronius IG Plus 100V-3 3-phase inverter"
	},
	{
		FRONIUS_AGILO_100_0_3_INVERTER,
		"Fronius Agilo 100.0-3 3-phase inverter"
	},
	{
		FRONIUS_IG_PLUS_120V_3_INVERTER,
		"Fronius IG Plus 120V-3 3-phase inverter"
	},
	{
		FRONIUS_IG_PLUS_150V_3_INVERTER,
		"Fronius IG Plus 150V-3 3-phase inverter"
	},
	{
		FRONIUS_IG_PLUS_V_3_0_1_INVERTER,
		"Fronius IG Plus V 3.0-1 UNI 1-phase inverter"
	},
	{
		FRONIUS_IG_PLUS_V_3_8_1_INVERTER,
		"Fronius IG Plus V 3.8-1 UNI 1-phase inverter"
	},
	{
		FRONIUS_IG_PLUS_V_5_0_1_INVERTER,
		"Fronius IG Plus V 5.0-1 UNI 1-phase inverter"
	},
	{
		FRONIUS_IG_PLUS_V_6_0_1_INVERTER,
		"Fronius IG Plus V 6.0-1 UNI 1-phase inverter"
	},
	{
		FRONIUS_IG_PLUS_V_7_5_1_INVERTER,
		"Fronius IG Plus V 7.5-1 UNI 1-phase inverter"
	},
	{
		FRONIUS_IG_PLUS_V_10_0_1_INVERTER,
		"Fronius IG Plus V 10.0-1 UNI 1-phase inverter"
	},
	{
		FRONIUS_IG_PLUS_V_10_0_3_INVERTER,
		"Fronius IG Plus V 10.0-3 Delta 3-phase inverter"
	},
	{
		FRONIUS_IG_PLUS_30V_1_INVERTER,
		"Fronius IG Plus 30V-1 1-phase inverter"
	},
	{
		FRONIUS_IG_PLUS_V_11_4_1_INVERTER,
		"Fronius IG Plus V 11.4-1 UNI 1-phase inverter"
	},
	{
		FRONIUS_IG_PLUS_V_11_4_3_INVERTER,
		"Fronius IG Plus V 11.4-3 DELTA 3-phase inverter"
	},
	{
		FRONIUS_IG_PLUS_V_12_0_3_INVERTER,
		"Fronius IG Plus V 12.0-3 WYE 3-phase inverter"
	},
	{
		FRONIUS_IG_PLUS_50V_1_DUMMY_INVERTER,
		"Fronius IG Plus 50V-1 Dummy inverter"
	},
	{
		FRONIUS_IG_PLUS_100V_2_DUMMY_INVERTER,
		"Fronius IG Plus 100V-2 Dummy inverter"
	},
	{
		FRONIUS_IG_PLUS_150V_3_DUMMY_INVERTER,
		"Fronius IG Plus 150V-3 Dummy inverter"
	},
	{
		FRONIUS_IG_PLUS_V_3_8_1_DUMMY_INVERTER,
		"Fronius IG Plus V 3.8-1 Dummy inverter"
	},
	{
		FRONIUS_IG_PLUS_V_7_5_1_DUMMY_INVERTER,
		"Fronius IG Plus V 7.5-1 Dummy inverter"
	},
	{
		FRONIUS_IG_PLUS_V_12_0_3_DUMMY_INVERTER,
		"Fronius IG Plus V 12.0-3 Dummy inverter"
	},
	{
		FRONIUS_CL_36_0_INVERTER,
		"Fronius CL 36.0 3-phase inverter"
	},
	{
		FRONIUS_CL_48_0_INVERTER,
		"Fronius CL 48.0 3-phase inverter"
	},
	{
		FRONIUS_CL_60_0_INVERTER,
		"Fronius CL 60.0 3-phase inverter"
	},
	{
		FRONIUS_CL_36_0_WYE277_INVERTER,
		"Fronius CL 36.0 WYE277 3-phase inverter"
	},
	{
		FRONIUS_CL_48_0_WYE277_INVERTER,
		"Fronius CL 48.0 WYE277 3-phase inverter"
	},
	{
		FRONIUS_CL_60_0_WYE277_INVERTER,
		"Fronius CL 60.0 WYE277 3-phase inverter"
	},
	{
		FRONIUS_CL_33_3_INVERTER,
		"Fronius CL 33.3 Delta 3-phase inverter"
	},
	{
		FRONIUS_CL_44_4_INVERTER,
		"Fronius CL 44.4 Delta 3-phase inverter"
	},
	{
		FRONIUS_CL_55_5_INVERTER,
		"Fronius CL 55.5 Delta 3-phase inverter"
	},
	{
		FRONIUS_CL_60_0_DUMMY_INVERTER,
		"Fronius CL 60.0 Dummy inverter"
	},
	{
		FRONIUS_CL_55_5_DUMMY_INVERTER,
		"Fronius CL 55.5 Delta Dummy inverter"
	},
	{
		FRONIUS_CL_60_0_WYE277_DUMMY_INVERTER,
		"Fronius CL 60.0 WYE277 Dummy inverter"
	},
	{
		FRONIUS_AGILO_75_0_3_INVERTER,
		"Fronius Agilo 75.0-3 3-phase inverter"
	},
	{
		FRONIUS_AGILO_100_0_3_DUMMY_INVERTER,
		"Fronius Agilo 100.0-3 Dummy inverter"
	},
	{
		FRONIUS_IG_PLUS_80V_3_INVERTER,
		"Fronius IG Plus 80V-3 3-phase inverter"
	},
	{
		FRONIUS_IG_PLUS_60V_3_INVERTER,
		"Fronius IG Plus 60V-3 3-phase inverter"
	},
	{
		FRONIUS_IG_PLUS_55V_3_INVERTER,
		"Fronius IG Plus 55V-3 3-phase inverter"
	},
	{
		SUNPOWER_SPR_3501F_2_INVERTER,
		"Sunpower SPR-3501F-2"
	},
};

Device::DeviceOption
Inverter::getDeviceOption(void) const
{
	return DeviceOptionInverter;
}

const char*
Inverter::getModel(void) const
{
	unsigned int	i;
	unsigned char	deviceType;

	deviceType = deviceTypeResponse_.getDeviceType();

	for (i = 0; i < NUM_ELEMS(deviceTypeMap); i++)
	{
		if (deviceTypeMap[i].id == deviceType)
		{
			return deviceTypeMap[i].model;
		}
	}

	return "unknown inverter type";
}

unsigned char
Inverter::maximumFieldLabelWidth(void) const
{
	// FIX THIS, DS:
	return 28;
}

void
Inverter::report(Args& args)
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
			displayCumulativeData(args, "this year", yearData);
			break;
		case Args::ReportTypeTotal:
			getTotalData(totalData);
			displayCumulativeData(args, "total", totalData);
			break;
		case Args::ReportTypeAll:
			getNowData(nowData);
			getDayData(dayData);
			if (supportsYearData())
			{
				getYearData(yearData);
			}
			getTotalData(totalData);
			displayNowData(args, nowData);
			displayCumulativeData(args, "today", dayData);
			if (supportsYearData())
			{
				displayCumulativeData(
					args, "this year", yearData);
			}
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
Inverter::getNowData(NowData& nowData)
{
	getMeasuredValue(
		FRONIUS_CMD_GET_POWER_NOW,
		nowData.power);
	getMeasuredValue(
		FRONIUS_CMD_GET_AC_CURRENT_NOW,
		nowData.acCurrent);
	getMeasuredValue(
		FRONIUS_CMD_GET_AC_VOLTAGE_NOW,
		nowData.acVoltage);
	getMeasuredValue(
		FRONIUS_CMD_GET_AC_FREQUENCY_NOW,
		nowData.acFrequency);
	getMeasuredValue(
		FRONIUS_CMD_GET_DC_CURRENT_NOW,
		nowData.dcCurrent);
	getMeasuredValue(
		FRONIUS_CMD_GET_DC_VOLTAGE_NOW,
		nowData.dcVoltage);

		if (getDeviceType()==0x84){ //corregido para que solo lea el trifásico, modificar para otros
			getMeasuredValue(
				FRONIUS_CMD_GET_PHASE_CURRENT_PHASE1,
				nowData.acCurrent1);
			getMeasuredValue(
				FRONIUS_CMD_GET_PHASE_CURRENT_PHASE2,
				nowData.acCurrent2);
			getMeasuredValue(
				FRONIUS_CMD_GET_PHASE_CURRENT_PHASE3,
				nowData.acCurrent3);

			getMeasuredValue(
				FRONIUS_CMD_GET_PHASE_VOLTAGE_PHASE1,
				nowData.acVoltage1);
			getMeasuredValue(
				FRONIUS_CMD_GET_PHASE_VOLTAGE_PHASE2,
				nowData.acVoltage2);
			getMeasuredValue(
				FRONIUS_CMD_GET_PHASE_VOLTAGE_PHASE3,
				nowData.acVoltage3);

		}
		else
		{
			struct MeasuredValue EmptyStruct={0};
			nowData.acCurrent1=nowData.acCurrent;
			nowData.acCurrent2=EmptyStruct;
			nowData.acCurrent3=EmptyStruct;
			nowData.acVoltage1=nowData.acVoltage;
			nowData.acVoltage2=EmptyStruct;
			nowData.acVoltage3=EmptyStruct;
		}


}

void
Inverter::getDayData(CumulativeData& dayData)
{
	getMeasuredValue(
		FRONIUS_CMD_GET_ENERGY_DAY,
		dayData.energy);
	getMeasuredValue(
		FRONIUS_CMD_GET_MAXIMUM_POWER_DAY,
		dayData.maximumPower);
	getMeasuredValue(
		FRONIUS_CMD_GET_MAXIMUM_AC_VOLTAGE_DAY,
		dayData.acVoltageMaximum);
	if (supportsMinimumACVoltageData())
	{
		getMeasuredValue(
			FRONIUS_CMD_GET_MINIMUM_AC_VOLTAGE_DAY,
			dayData.acVoltageMinimum);
	}
	getMeasuredValue(
		FRONIUS_CMD_GET_MAXIMUM_DC_VOLTAGE_DAY,
		dayData.dcVoltageMaximum);
	getMeasuredValue(
		FRONIUS_CMD_GET_OPERATING_HOURS_DAY,
		dayData.operatingHours);
}

void
Inverter::getYearData(CumulativeData& yearData)
{
	assert(supportsYearData());

	getMeasuredValue(
		FRONIUS_CMD_GET_ENERGY_YEAR,
		yearData.energy);
	getMeasuredValue(
		FRONIUS_CMD_GET_MAXIMUM_POWER_YEAR,
		yearData.maximumPower);
	getMeasuredValue(
		FRONIUS_CMD_GET_MAXIMUM_AC_VOLTAGE_YEAR,
		yearData.acVoltageMaximum);
	if (supportsMinimumACVoltageData())
	{
		getMeasuredValue(
			FRONIUS_CMD_GET_MINIMUM_AC_VOLTAGE_YEAR,
			yearData.acVoltageMinimum);
	}
	getMeasuredValue(
		FRONIUS_CMD_GET_MAXIMUM_DC_VOLTAGE_YEAR,
		yearData.dcVoltageMaximum);
	getMeasuredValue(
		FRONIUS_CMD_GET_OPERATING_HOURS_YEAR,
		yearData.operatingHours);
}

void
Inverter::getTotalData(CumulativeData& totalData)
{
	getMeasuredValue(
		FRONIUS_CMD_GET_ENERGY_TOTAL,
		totalData.energy);
	getMeasuredValue(
		FRONIUS_CMD_GET_MAXIMUM_POWER_TOTAL,
		totalData.maximumPower);
	getMeasuredValue(
		FRONIUS_CMD_GET_MAXIMUM_AC_VOLTAGE_TOTAL,
		totalData.acVoltageMaximum);
	if (supportsMinimumACVoltageData())
	{
		getMeasuredValue(
			FRONIUS_CMD_GET_MINIMUM_AC_VOLTAGE_TOTAL,
			totalData.acVoltageMinimum);
	}
	getMeasuredValue(
		FRONIUS_CMD_GET_MAXIMUM_DC_VOLTAGE_TOTAL,
		totalData.dcVoltageMaximum);
	getMeasuredValue(
		FRONIUS_CMD_GET_OPERATING_HOURS_TOTAL,
		totalData.operatingHours);
}

void
Inverter::displayNowData(Args& args, NowData& data)
{
	const char*	delimiter = args.getDelimiter();

	if (delimiter == NULL)
	{
		printf("%-*s : %s watts\n",
			maximumFieldLabelWidth(),
			"Power now",
			Utils::getValue(data.power));
		printf("%-*s : %s amps\n",
			maximumFieldLabelWidth(),
			"AC current now",
			Utils::getValue(data.acCurrent));
		printf("%-*s : %s volts\n",
			maximumFieldLabelWidth(),
			"AC voltage now",
			Utils::getValue(data.acVoltage));
		printf("%-*s : %s Hz\n",
			maximumFieldLabelWidth(),
			"AC frequency now",
			Utils::getValue(data.acFrequency));
		printf("%-*s : %s amps\n",
			maximumFieldLabelWidth(),
			"DC current now",
			Utils::getValue(data.dcCurrent));
		printf("%-*s : %s volts\n",
			maximumFieldLabelWidth(),
			"DC voltage now",
			Utils::getValue(data.dcVoltage));

		printf("%-*s : %s amps\n",
			maximumFieldLabelWidth(),
			"AC current PH1",
			Utils::getValue(data.acCurrent1));
		printf("%-*s : %s amps\n",
			maximumFieldLabelWidth(),
			"AC current PH2",
			Utils::getValue(data.acCurrent2));
		printf("%-*s : %s amps\n",
			maximumFieldLabelWidth(),
			"AC current PH3",
			Utils::getValue(data.acCurrent3));
		printf("%-*s : %s volts\n",
			maximumFieldLabelWidth(),
			"AC voltage PH1",
			Utils::getValue(data.acVoltage1));
		printf("%-*s : %s volts\n",
			maximumFieldLabelWidth(),
			"AC voltage PH2",
			Utils::getValue(data.acVoltage2));
		printf("%-*s : %s volts\n",
			maximumFieldLabelWidth(),
			"AC voltage PH3",
			Utils::getValue(data.acVoltage3));



	}
	else
	{
		printf("%s%s", Utils::getValue(data.power), delimiter);
		printf("%s%s", Utils::getValue(data.acCurrent), delimiter);
		printf("%s%s", Utils::getValue(data.acVoltage), delimiter);
		printf("%s%s", Utils::getValue(data.acFrequency), delimiter);
		printf("%s%s", Utils::getValue(data.dcCurrent), delimiter);
		printf("%s%s", Utils::getValue(data.dcVoltage), delimiter);
		printf("%s%s", Utils::getValue(data.acCurrent1), delimiter);
		printf("%s%s", Utils::getValue(data.acCurrent2), delimiter);
		printf("%s%s", Utils::getValue(data.acCurrent3), delimiter);
		printf("%s%s", Utils::getValue(data.acVoltage1), delimiter);
		printf("%s%s", Utils::getValue(data.acVoltage2), delimiter);
		printf("%s%s", Utils::getValue(data.acVoltage3), delimiter);



	}
}

void
Inverter::displayCumulativeData(
	Args& args,
	const char* timePeriod,
	CumulativeData& data)
{
	const char*	delimiter = args.getDelimiter();

	if (delimiter == NULL)
	{
		printf("%-*s : %s kilowatt hours\n",
			maximumFieldLabelWidth(),
			Utils::strCat("Energy ", timePeriod),
			Utils::getThousandsValue(data.energy));
		printf("%-*s : %s watts\n",
			maximumFieldLabelWidth(),
			Utils::strCat("Maximum power ", timePeriod),
			Utils::getValue(data.maximumPower));
		printf("%-*s : %s volts\n",
			maximumFieldLabelWidth(),
			Utils::strCat("Maximum AC voltage ", timePeriod),
			Utils::getValue(data.acVoltageMaximum));
		if (supportsMinimumACVoltageData())
		{
			printf("%-*s : %s volts\n",
				maximumFieldLabelWidth(),
				Utils::strCat(
					"Minimum AC voltage ", timePeriod),
				Utils::getValue(data.acVoltageMinimum));
		}
		printf("%-*s : %s volts\n",
			maximumFieldLabelWidth(),
			Utils::strCat("Maximum DC voltage ", timePeriod),
			Utils::getValue(data.dcVoltageMaximum));
		printf("%-*s : %s hours\n",
			maximumFieldLabelWidth(),
			Utils::strCat("Operating time ", timePeriod),
			Utils::getHoursValue(data.operatingHours));
	}
	else
	{
		printf("%s%s",
			Utils::getThousandsValue(data.energy), delimiter);
		printf("%s%s",
			Utils::getValue(data.maximumPower), delimiter);
		printf("%s%s",
			Utils::getValue(data.acVoltageMaximum), delimiter);
		if (supportsMinimumACVoltageData())
		{
			printf("%s%s", Utils::getValue(
				data.acVoltageMinimum), delimiter);
		}
		printf("%s%s",
			Utils::getValue(data.dcVoltageMaximum), delimiter);
		printf("%s%s",
			Utils::getHoursValue(data.operatingHours), delimiter);
	}
}

void
Inverter::displayNowHeader(Args& args)
{
	const char*	delimiter;

	delimiter = args.getDelimiter();
	CHECK_PTR(delimiter);

	printf("Power now%s", delimiter);
	printf("AC current now%s", delimiter);
	printf("AC voltage now%s", delimiter);
	printf("AC frequency now%s", delimiter);
	printf("DC current now%s", delimiter);
	printf("DC voltage now%s", delimiter);
}

void
Inverter::displayDayHeader(Args& args)
{
	const char*	delimiter;

	delimiter = args.getDelimiter();
	CHECK_PTR(delimiter);

	printf("Energy today (KWH)%s", delimiter);
	printf("Maximum power today (W)%s", delimiter);
	printf("Maximum AC voltage today (V)%s", delimiter);
	if (supportsMinimumACVoltageData())
	{
		printf("Minimum AC voltage today (V)%s", delimiter);
	}
	printf("Maximum DC voltage today (V)%s", delimiter);
	printf("Operating time today (H)%s", delimiter);
}

void
Inverter::displayYearHeader(Args& args)
{
	const char*	delimiter;

	delimiter = args.getDelimiter();
	CHECK_PTR(delimiter);

	printf("Energy this year (KWH)%s", delimiter);
	printf("Maximum power this year (W)%s", delimiter);
	printf("Maximum AC voltage this year (V)%s", delimiter);
	if (supportsMinimumACVoltageData())
	{
		printf("Minimum AC voltage this year (V)%s", delimiter);
	}
	printf("Maximum DC voltage this year (V)%s", delimiter);
	printf("Operating time this year (H)%s", delimiter);
}

void
Inverter::displayTotalHeader(Args& args)
{
	const char*	delimiter;

	delimiter = args.getDelimiter();
	CHECK_PTR(delimiter);

	printf("Energy total (KWH)%s", delimiter);
	printf("Maximum power total (W)%s", delimiter);
	printf("Maximum AC voltage total (V)%s", delimiter);
	if (supportsMinimumACVoltageData())
	{
		printf("Minimum AC voltage total (V)%s", delimiter);
	}
	printf("Maximum DC voltage total (V)%s", delimiter);
	printf("Operating time total (H)%s", delimiter);
}
