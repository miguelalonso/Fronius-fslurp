//
// fslurp, Copyright (c) 2012, 2013 David Stone <ds.fslurp@asif.com>
//
// $RCSfile: iterator.cpp,v $
// $Revision: 1.5 $
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

void
DeviceIterator::reset(void)
{
	currentIndex_ = 0;
}

DeviceIterator::DeviceIterator(Interface* interface)
{
	CHECK_PTR(interface);
	interface_ = interface;
	currentIndex_ = 0;
}

unsigned char
DeviceIterator::numDevices(void)
{
	return numDevices_;
}

InverterIterator::InverterIterator(Interface* interface)
	: DeviceIterator(interface)
{
	CHECK_PTR(interface_);
	numDevices_ = interface->getNumActiveInverters();
}

Device*
InverterIterator::next(void)
{
	if ((numDevices_ != 0) &&(currentIndex_ < numDevices_))
	{
		CHECK_PTR(interface_);
		return interface_->getInverter(currentIndex_++);
	}

	return NULL;
}

void
InverterIterator::reportHeader(Args& args)
{
	CHECK_PTR(interface_);

	if (numDevices_ == 0)
	{
		Inverter	inverter;

		inverter.reportHeader(args, interface_->supportsYearData());
	}
	else
	{
		Device*		device;

		device = interface_->getInverter(0);

		CHECK_PTR(device);
		device->reportHeader(args, interface_->supportsYearData());
	}
}

unsigned char
InverterIterator::maximumFieldLabelWidth(void)
{
	Inverter	inverter;

	return inverter.maximumFieldLabelWidth();
}

InverterSingleTargetIterator::InverterSingleTargetIterator(
	Interface* interface,
	unsigned char deviceNumber) : DeviceIterator(interface)
{
	CHECK_PTR(interface_);
	inverter_ = interface->getInverterByID(deviceNumber);

	if (inverter_ == NULL)
	{
		Args::dieMessage(
			Args::ExitInverterNotFound,
			"Inverter Number %d not found\n",
			deviceNumber);
	}

	numDevices_ = 1;
}

Device*
InverterSingleTargetIterator::next(void)
{
	if (currentIndex_++ == 0)
	{
		return inverter_;
	}

	return NULL;
}

void
InverterSingleTargetIterator::reportHeader(Args& args)
{
	Inverter	inverter;

	CHECK_PTR(interface_);
	inverter.reportHeader(args, interface_->supportsYearData());
}

unsigned char
InverterSingleTargetIterator::maximumFieldLabelWidth(void)
{
	Inverter	inverter;

	return inverter.maximumFieldLabelWidth();
}

SensorCardIterator::SensorCardIterator(Interface* interface)
	: DeviceIterator(interface)
{
	CHECK_PTR(interface_);
	numDevices_ = interface->getNumActiveSensorCards();
}

Device*
SensorCardIterator::next(void)
{
	if ((numDevices_ != 0) &&(currentIndex_ < numDevices_))
	{
		CHECK_PTR(interface_);
		return interface_->getSensorCard(currentIndex_++);
	}

	return NULL;
}

void
SensorCardIterator::reportHeader(Args& args)
{
	SensorCard	sensorCard;

	CHECK_PTR(interface_);
	sensorCard.reportHeader(args, interface_->supportsYearData());
}

unsigned char
SensorCardIterator::maximumFieldLabelWidth(void)
{
	SensorCard	sensorCard;

	return sensorCard.maximumFieldLabelWidth();
}

SensorCardSingleTargetIterator::SensorCardSingleTargetIterator(
	Interface* interface,
	unsigned char deviceNumber) : DeviceIterator(interface)
{
	CHECK_PTR(interface_);
	sensorCard_ = interface->getSensorCardByID(deviceNumber);

	if (sensorCard_ == NULL)
	{
		Args::dieMessage(
			Args::ExitSensorCardNotFound,
			"Sensor Card Number %d not found\n",
			deviceNumber);
	}

	numDevices_ = 1;
}

Device*
SensorCardSingleTargetIterator::next(void)
{
	if (currentIndex_++ == 0)
	{
		return sensorCard_;
	}

	return NULL;
}

void
SensorCardSingleTargetIterator::reportHeader(Args& args)
{
	SensorCard	sensorCard;

	CHECK_PTR(interface_);
	sensorCard.reportHeader(args, interface_->supportsYearData());
}

unsigned char
SensorCardSingleTargetIterator::maximumFieldLabelWidth(void)
{
	SensorCard	sensorCard;

	return sensorCard.maximumFieldLabelWidth();
}
