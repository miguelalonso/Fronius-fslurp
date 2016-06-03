//
// fslurp, Copyright (c) 2012 David Stone <ds.fslurp@asif.com>
//
// $RCSfile: report.cpp,v $
// $Revision: 1.9 $
// $Date: 2012/10/20 01:37:07 $
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

Report::Report(Args* args, Interface* interface)
{
	args_ = args;
	interface_ = interface;
	deviceIterator_ = NULL;
	initializeIterator();
}

void
Report::initializeIterator(void)
{
	CHECK_PTR(args_);

	Args::DeviceType	deviceType;
	Args::DeviceTargeting	deviceTargeting;

	if (args_->getReportType() == Args::ReportTypeSystem)
	{
		return;
	}

	deviceType = args_->getDeviceType();
	deviceTargeting = args_->getDeviceTargeting();

	if (deviceType == Args::DeviceTypeInverter)
	{
		if (deviceTargeting == Args::AllInstances)
		{
			deviceIterator_ = new InverterIterator(interface_);
		}
		else
		{
			assert(deviceTargeting == Args::SingleTarget);
			deviceIterator_ = new InverterSingleTargetIterator(
				interface_, args_->getDeviceNumber());
		}
	}
	else
	{
		assert(deviceType == Args::DeviceTypeSensorCard);

		if (deviceTargeting == Args::AllInstances)
		{
			deviceIterator_ = new SensorCardIterator(interface_);
		}
		else
		{
			assert(deviceTargeting == Args::SingleTarget);
			deviceIterator_ = new SensorCardSingleTargetIterator(
				interface_, args_->getDeviceNumber());
		}
	}

	CHECK_PTR(deviceIterator_);
}

void
Report::report(void)
{
	CHECK_PTR(args_);
	CHECK_PTR(interface_);

	switch (args_->getReportType())
	{
		case Args::ReportTypeSystem:
			systemReport();
			break;

		case Args::ReportTypeYear:
			if (! interface_->supportsYearData())
			{
				Args::dieMessage(
					Args::ExitUnsupportedReport,
					"The year report is not "
					"supported for %s\n",
					interface_->getModel());
			}

			// fall through
		case Args::ReportTypeAll:
		case Args::ReportTypeNow:
		case Args::ReportTypeDay:
		case Args::ReportTypeTotal:
			deviceReport();
			break;
		default:
			assert(0);
	}
}

void
Report::systemReport(void)
{
	CHECK_PTR(args_);
	CHECK_PTR(interface_);

	if (args_->showReportHeader())
	{
		interface_->reportHeader(*args_);
	}

	if (args_->getDelimiter() == NULL)
	{
		Utils::displayTimestamp(
			interface_->maximumFieldLabelWidth(),
			*args_);
	}

	Utils::displaySeparator(*args_);

	interface_->report(*args_);
}

void
Report::deviceReport(void)
{
	Device*	device;

	CHECK_PTR(args_);
	CHECK_PTR(deviceIterator_);

	if (args_->showReportHeader())
	{
		deviceIterator_->reportHeader(*args_);
	}

	if (deviceIterator_->numDevices() != 0)
	{
		if (args_->getDelimiter() == NULL)
		{
			Utils::displayTimestamp(
				deviceIterator_->maximumFieldLabelWidth(),
				*args_);
		}

		Utils::displaySeparator(*args_);
	}

	while ((device = deviceIterator_->next()) != NULL)
	{
		CHECK_PTR(device);
		device->report(*args_);
		Utils::displaySeparator(*args_);
	}
}
