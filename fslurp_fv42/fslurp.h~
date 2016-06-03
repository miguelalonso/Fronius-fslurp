/*
 * fslurp, Copyright (c) 2012, 2013 David Stone <ds.fslurp@asif.com>
 *
 * $RCSfile: fslurp.h,v $
 * $Revision: 1.11 $
 * $Date: 2013/12/21 14:55:35 $
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both the
 * copyright notice and this permission notice appear in supporting
 * documentation.  No representations are made about the suitability of this
 * software for any purpose.  It is provided "as is" without express or 
 * implied warranty.
 */

#ifndef _FSLURP_H
#define _FSLURP_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <stdarg.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <termios.h>
#include <math.h>
#include <time.h>

#include "fronius.h"

#define	CHECK_PTR(x)		{ assert((x) != NULL); }
#define NUM_ELEMS(arrayName)	(sizeof((arrayName)) / sizeof(*(arrayName)))

class Args
{
	public:
		enum ExitCodes
		{
			ExitNormal			= 0,
			ExitCommFailure			= 1,
			ExitInverterNotFound		= 2,
			ExitSensorCardNotFound		= 3,
			ExitUsageError			= 4,
			ExitProtocolError		= 5,
			ExitUnsupportedReport		= 6,
			ExitSimulatorError		= 7
		};

		enum ReportType
		{
			ReportTypeNow,
			ReportTypeDay,
			ReportTypeYear,
			ReportTypeTotal,
			ReportTypeAll,
			ReportTypeSystem,
		};

		enum DeviceType
		{
			DeviceTypeInverter = FRONIUS_INVERTER,
			DeviceTypeSensorCard = FRONIUS_SENSOR_CARD
		};

		enum DeviceTargeting
		{
			AllInstances,
			SingleTarget
		};

		Args(int argc, char* argv[]);
		virtual ~Args(void) { }

		const char* getSerialPortName(void) const;
		const char* getBaudRate(void) const;
		const char* getDelimiter(void) const;
		const char* getTimeFormat(void) const;
		bool getVerbose(void) const;
		bool showReportHeader(void) const;
		ReportType getReportType(void) const;
		DeviceType getDeviceType(void) const;
		DeviceTargeting getDeviceTargeting(void) const;
		unsigned char getDeviceNumber(void) const;
		time_t getTimestamp(void) const;
		static void dieMessage(int returnCode, const char* format, ...);

	private:
		static const char*	programName_;
		const char*		serialPortName_;
		const char*		baudRate_;
		const char*		delimiter_;
		const char*		reportTypeName_;
		unsigned char		deviceNumber_;
		ReportType		reportType_;
		DeviceType		deviceType_;
		DeviceTargeting		deviceTargeting_;
		const char*		timeFormat_;
		time_t			timestamp_;
		bool			showReportHeader_;
		bool			verbose_;

		Args(void) { }

		void auditArgs(void);
		void dieUsage(const char* format, ...);
		void parseArgs(int argc, char* argv[]);
		void setBaudRate(const char* baudRate);
		void setReportType(const char* reportType);
		void setDeviceType(const char* deviceType);
		void usage(void);
};

class Comm
{
	public:
		Comm(Args& args);
		virtual ~Comm(void) { }

		void writeBytes(
			unsigned char* buffer,
			unsigned char bytesToWrite);
		void readBytes(
			unsigned char* buffer,
			unsigned char bytesToRead);
		void flush(void);
		bool verbose(void);

	private:
		int		fd_;
		const char*	baudRate_;
		bool		verbose_;

		Comm(void) { }

		unsigned int getBaudRateFlag(const char* baudRate);
		void serialDeviceInit(Args& args);
};

class Request
{
	public:
		Request(void);
		virtual ~Request(void) { }

		void setDeviceOption(unsigned char deviceOption);
		void setDeviceNumber(unsigned char deviceNumber);
		void setCommand(unsigned char command);
		unsigned char length(void);
		const void* requestBytes(void);
		void send(Comm& comm);

	private:

		FroniusRequest	request_;

		void calculateChecksum(void);
		const char* decodeCommand(void);
};

class Response
{
	public:
		Response(void);
		virtual ~Response(void);

		void receive(Comm& comm);
		bool responseValid(void) const;

	protected:
		void readResponseBytes(Comm& comm);
		const void* getResponseData(void) const;
		unsigned char getResponseDataLength(void) const;

	private:

		struct ErrorResponse
		{
			unsigned char	failedCommand;
			unsigned char	errorInformation;
			unsigned char	checksum;
		};

		FroniusResponseHeader	header_;
		unsigned char*		data_;
		bool			valid_;

		void validateHeader(void);
		void validateChecksum(void);
		void handleError(void);
};

class VersionResponse : public Response
{
	public:

		VersionResponse(void) { }
		virtual ~VersionResponse(void) { }

		unsigned char getInterfaceType(void) const;
		unsigned char getVersionMajor(void) const;
		unsigned char getVersionMinor(void) const;
		unsigned char getVersionRelease(void) const;

	private:
		struct VersionData
		{
			unsigned char	interfaceType;
			unsigned char	versionMajor;
			unsigned char	versionMinor;
			unsigned char	versionRelease;
			unsigned char	checksum;
		};
};

class ActiveInverterNumbersResponse : public Response
{
	public:

		ActiveInverterNumbersResponse(void) { }
		virtual ~ActiveInverterNumbersResponse(void) { }

		unsigned char numActiveInverters(void) const;
		unsigned char getActiveInverterID(unsigned char index) const;

	private:
};

class ActiveSensorCardsResponse : public Response
{
	public:

		ActiveSensorCardsResponse(void) { }
		virtual ~ActiveSensorCardsResponse(void) { }

		unsigned char numActiveSensorCards(void) const;
		unsigned char getActiveSensorCardID(unsigned char index) const;

	private:
};

class DeviceTypeResponse : public Response
{
	public:
		DeviceTypeResponse(void) { }
		virtual ~DeviceTypeResponse(void) { }

		unsigned char getDeviceType(void) const;
	private:
		struct DeviceTypeData
		{
			unsigned char	deviceType;
		};
};

class SolarNetStatusResponse : public Response
{
	public:
		SolarNetStatusResponse(void) { }
		virtual ~SolarNetStatusResponse(void) { }

		unsigned char getSolarNetStatus(void) const;
	private:
		struct SolarNetStatusData
		{
			unsigned char	status;
		};
};

struct MeasuredValue
{
	unsigned char	msb;
	unsigned char	lsb;
	signed char	exponent;
};

class MeasuredValueResponse : public Response
{
	public:
		MeasuredValueResponse(void) { }
		virtual ~MeasuredValueResponse(void) { }

		unsigned char getLSB(void) const;
		unsigned char getMSB(void) const;
		signed char getExponent(void) const;
	private:
};

class Device
{
	public:
		enum DeviceOption
		{
			DeviceOptionInterfaceCard = FRONIUS_INTERFACE_CARD,
			DeviceOptionInverter = FRONIUS_INVERTER,
			DeviceOptionSensorCard = FRONIUS_SENSOR_CARD
		};

		Device(void);
		virtual ~Device(void) { }

		void initialize(
			Comm* comm,
			unsigned char interfaceType,
			unsigned char deviceID,
			bool supportsYearData);

		unsigned char getDeviceID(void) const;
		unsigned char getDeviceType(void) const;
		void deviceInfoReport(
			unsigned char fieldLabelWdith,
			Args& args);
		void reportHeader(Args& args);
		void reportHeader(Args& args, bool supportsYearData);
		bool supportsYearData(void) const;
		bool supportsMinimumACVoltageData(void) const;

		virtual const char* getModel(void) const = 0;
		virtual DeviceOption getDeviceOption(void) const = 0;
		virtual void report(Args& args) = 0;
		virtual void displayNowHeader(Args& args) = 0;
		virtual void displayDayHeader(Args& args) = 0;
		virtual void displayYearHeader(Args& args) = 0;
		virtual void displayTotalHeader(Args& args) = 0;
		virtual unsigned char maximumFieldLabelWidth(void) const = 0;


	protected:
		Comm*			comm_;
		DeviceTypeResponse	deviceTypeResponse_;
		unsigned char		deviceID_;
		unsigned char		interfaceType_;
		bool			supportsYearData_;
		bool			supportsMinimumACVoltageData_;

		void getMeasuredValue(
			unsigned char command,
			MeasuredValue& measuredValue);

	private:
};

class Inverter : public Device
{
	public:
		struct DeviceTypeMap
		{
			unsigned char	id;
			const char*	model;
		};

		Inverter(void) { }
		virtual ~Inverter(void) { }

		DeviceOption getDeviceOption(void) const;
		void report(Args& args);
		const char* getModel(void) const;
		void displayNowHeader(Args& args);
		void displayDayHeader(Args& args);
		void displayYearHeader(Args& args);
		void displayTotalHeader(Args& args);
		unsigned char maximumFieldLabelWidth(void) const;

	private:
		struct NowData
		{
			MeasuredValue	power;
			MeasuredValue	acCurrent;
			MeasuredValue	acVoltage;
			MeasuredValue	acFrequency;
			MeasuredValue	dcCurrent;
			MeasuredValue	dcVoltage;
			MeasuredValue	acCurrent1;
			MeasuredValue	acCurrent2;
			MeasuredValue	acCurrent3;
			MeasuredValue	acVoltage1;
			MeasuredValue	acVoltage2;
			MeasuredValue	acVoltage3;

		};

		struct CumulativeData
		{
			struct MeasuredValue	energy;
			struct MeasuredValue	maximumPower;
			struct MeasuredValue	acVoltageMaximum;
			struct MeasuredValue	acVoltageMinimum;
			struct MeasuredValue	dcVoltageMaximum;
			struct MeasuredValue	operatingHours;
		};

		void getNowData(NowData& nowData);
		void getYearData(CumulativeData& yearData);
		void getDayData(CumulativeData& dayData);
		void getTotalData(CumulativeData& totalData);

		void displayNowData(Args& args, NowData& nowdata);
		void displayCumulativeData(
			Args& args, 
			const char* timePeriod,
			CumulativeData& data);
};

class SensorCard : public Device
{
	public:
		SensorCard(void) { }
		virtual ~SensorCard(void) { }

		DeviceOption getDeviceOption(void) const;
		const char* getModel(void) const;
		void report(Args& args);
		void displayNowHeader(Args& args);
		void displayDayHeader(Args& args);
		void displayYearHeader(Args& args);
		void displayTotalHeader(Args& args);
		unsigned char maximumFieldLabelWidth(void) const;

	private:

		struct NowData
		{
			MeasuredValue	temperatureChan1;
			MeasuredValue	temperatureChan2;
			MeasuredValue	irradiance;
			MeasuredValue	digitalChan1Value;
			MeasuredValue	digitalChan2Value;
		};

		struct CumulativeData
		{
			MeasuredValue	temperatureChan1Minimum;
			MeasuredValue	temperatureChan1Maximum;
			MeasuredValue	temperatureChan2Minimum;
			MeasuredValue	temperatureChan2Maximum;
			MeasuredValue	irradianceMaximum;
			MeasuredValue	digitalChan1ValueMaximum;
			MeasuredValue	digitalChan2ValueMaximum;
		};

		void getNowData(NowData& nowData);
		void getDayData(CumulativeData& dayData);
		void getYearData(CumulativeData& yearData);
		void getTotalData(CumulativeData& totalData);

		void displayNowData(Args& args, NowData& nowdata);
		void displayCumulativeData(
			Args& args, 
			const char* timePeriod,
			CumulativeData& data);
};

class Interface
{
	public:

		Interface(Comm* comm);
		virtual ~Interface(void);

		const char* getModel(void) const;

		void initialize(void);
		void discoverDevices(void);
		void report(Args& args);
		void reportHeader(Args& args);
		unsigned char maximumFieldLabelWidth(void) const;
		bool supportsYearData(void) const;
		bool supportsMinimumACVoltageData(void) const;
		unsigned char getInterfaceType(void) const;

		unsigned char getNumActiveInverters(void) const;
		unsigned char getActiveInverterID(unsigned char index) const;
		Inverter* getInverter(unsigned char index);
		Inverter* getInverterByID(unsigned char deviceID);

		unsigned char getNumActiveSensorCards(void) const;
		unsigned char getActiveSensorCardID(unsigned char index) const;
		SensorCard* getSensorCard(unsigned char index);
		SensorCard* getSensorCardByID(unsigned char deviceID);

	private:
		VersionResponse			versionResponse_;
		SolarNetStatusResponse		solarNetStatusResponse_;
		ActiveInverterNumbersResponse	activeInverterNumbersResponse_;
		ActiveSensorCardsResponse	activeSensorCardsResponse_;
		Inverter*			inverters_;
		SensorCard*			sensorCards_;
		Comm*				comm_;
		bool				hasSolarNet_;
		unsigned char			solarNetStatus_;
		bool				supportsYearData_;
		bool				supportsMinimumACVoltageData_;

		// shouldn't happen
		Interface(void)
		{
			comm_ = NULL;
			assert(0);
		}

		void getVersion(void);
		void getInverters(void);
		void getActiveInverters(void);
		void getSolarNetStatus(void);
		void getSensorCards(void);
		void getActiveSensorCards(void);
		const char* decodeSolarNetStatus(void) const;
		void interfaceReport(Args& args);
};

class DeviceIterator
{
	public:
		DeviceIterator(Interface* interface);
		virtual ~DeviceIterator(void) { }

		void reset(void);
		unsigned char numDevices(void);
		virtual Device* next(void) = 0;
		virtual void reportHeader(Args& args) = 0;
		virtual unsigned char maximumFieldLabelWidth(void) = 0;

	protected:
		Interface*	interface_;
		unsigned char	numDevices_;
		unsigned char	currentIndex_;

		DeviceIterator(void) { }

	private:
};

class InverterIterator : public DeviceIterator
{
	public:
		InverterIterator(Interface* interface);
		~InverterIterator(void) { }

		Device* next(void);
		void reportHeader(Args& args);
		unsigned char maximumFieldLabelWidth(void);

	private:
};

class InverterSingleTargetIterator : public DeviceIterator
{
	public:
		InverterSingleTargetIterator(
			Interface* interface,
			unsigned char deviceNumber);
		~InverterSingleTargetIterator(void) { }

		Device* next(void);
		void reportHeader(Args& args);
		unsigned char maximumFieldLabelWidth(void);

	private:
		Inverter*	inverter_;
};

class SensorCardIterator : public DeviceIterator
{
	public:
		SensorCardIterator(Interface* interface);
		~SensorCardIterator(void) { }

		Device* next(void);
		void reportHeader(Args& args);
		unsigned char maximumFieldLabelWidth(void);

	private:
};

class SensorCardSingleTargetIterator : public DeviceIterator
{
	public:
		SensorCardSingleTargetIterator(
			Interface* interface,
			unsigned char deviceNumber);
		~SensorCardSingleTargetIterator(void) { }

		Device* next(void);
		void reportHeader(Args& args);
		unsigned char maximumFieldLabelWidth(void);

	private:
		SensorCard*	sensorCard_;
};

class Report
{
	public:
		Report(Args* args, Interface* interface);
		virtual ~Report(void) { }

		void report(void);
	private:
		Args*		args_;
		Interface*	interface_;
		DeviceIterator*	deviceIterator_;

		// shouldn't happen
		Report(void)
		{
			args_ = NULL;
			interface_ = NULL;
			assert(0);
		};

		void systemReport(void);
		void deviceReport(void);
		void initializeIterator(void);
};


class Utils
{
	public:
		static time_t	timestamp_;
		static char	ctimeBuf_[4096];

		Utils(void) { }
		virtual ~Utils(void) { }

		static void hexDump(
			unsigned char* buffer,
			unsigned int length);
		static const char* decodeCommand(
			unsigned char command);
		static const char* decodeDeviceOption(
			unsigned char deviceOption);
		static const char* decodeErrorInformation(
			unsigned char errorInfo);
		static const char* getValue(
			struct MeasuredValue measuredValue);
		static const char* getHoursValue(
			struct MeasuredValue measuredValue);
		static const char* getThousandsValue(
			struct MeasuredValue measuredValue);
		static const char* getTimestamp(Args& args);
		static void displayTimestamp(
			unsigned char fieldWidth,
			Args& args);
		static void displaySeparator(Args& args);
		static const char* strCat(
			const char* str1,
			const char* str2);
	private:
};
#endif /* _FSLURP_H */
