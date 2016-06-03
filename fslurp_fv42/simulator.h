/*
 * fslurp, Copyright (c) 2012 David Stone <ds.fslurp@asif.com>
 *
 * $RCSfile: simulator.h,v $
 * $Revision: 1.2 $
 * $Date: 2012/09/24 06:41:02 $
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both the
 * copyright notice and this permission notice appear in supporting
 * documentation.  No representations are made about the suitability of this
 * software for any purpose.  It is provided "as is" without express or 
 * implied warranty.
 */

#ifndef _SIMULATOR_H
#define _SIMULATOR_H

#include "fslurp.h"

class FroniusSimulator
{
	public:
		FroniusSimulator(void) { }
		virtual ~FroniusSimulator(void) { }

		void initialize(void);
		void flush(void);
		void writeBytes(FroniusRequest* request);
		void readBytes(
			unsigned char* buffer,
			unsigned char numBytesToRead);

	private:
		enum State
		{
			BogusState = 0,
			WriteRequest,
			ReadHeader,
			ReadData
		};

		unsigned char	command_;
		unsigned char	deviceOption_;
		unsigned char	deviceNumber_;
		State		state_;

		void readHeader(
			unsigned char* buffer,
			unsigned char numBytesToRead);
		void readData(
			unsigned char* buffer,
			unsigned char numBytesToRead);

		void readInterfaceCardHeader(
			unsigned char* buffer,
			unsigned char numBytesToRead);
		void readInverterHeader(
			unsigned char* buffer,
			unsigned char numBytesToRead);
		void readSensorCardHeader(
			unsigned char* buffer,
			unsigned char numBytesToRead);

		void readInterfaceCardData(
			unsigned char* buffer,
			unsigned char numBytesToRead);
		void readInverterData(
			unsigned char* buffer,
			unsigned char numBytesToRead);
		void readSensorCardData(
			unsigned char* buffer,
			unsigned char numBytesToRead);
};

#endif /* _SIMULATOR_H */

