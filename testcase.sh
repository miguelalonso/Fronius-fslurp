#!/bin/bash
#
# fslurp, Copyright (c) 2012, 2013 David Stone <ds.fslurp@asif.com>
#
# $RCSfile: testcase.sh,v $
# $Revision: 1.5 $
# $Date: 2013/12/21 14:52:25 $
#
# Permission to use, copy, modify, distribute, and sell this software and its
# documentation for any purpose is hereby granted without fee, provided that
# the above copyright notice appear in all copies and that both the
# copyright notice and this permission notice appear in supporting
# documentation.  No representations are made about the suitability of this
# software for any purpose.  It is provided "as is" without express or 
# implied warranty.
#

. ./testcase.env

Fslurp=./fslurp
SimIFCEasy=./simIFCEasy
SimSolarNet2i1s=./simSolarNet2i1s
SimSolarNet1i0s=./simSolarNet1i0s
SimSolarNet0i0s=./simSolarNet0i0s
Sim422IGTL=./sim422IGTL


TestCases=
TestLog=test.log

Verbose=0

#
# fslurp exit codes
#
ExitNormal=0
ExitCommFailure=1
ExitInverterNotFound=2
ExitSensorCardNotFound=3
ExitUsageError=4
ExitProtocolError=5
ExitUnsupportedReportError=6
ExitSimulatorError=7

DeviceReportTypes="now day total all"

UsageTest()
{
	$SimIFCEasy -h
	[ $? -ne $ExitNormal ] && return 1

	return 0
}
TestCases="$TestCases UsageTest"

UnknownOptionTest()
{
	$SimIFCEasy -1
	[ $? -ne $ExitUsageError ] && return 1

	return 0
}
TestCases="$TestCases UnknownOptionTest"

IllegalComboTest()
{
	$SimIFCEasy -r system -n 1
	[ $? -ne $ExitUsageError ] && return 1

	return 0
}
TestCases="$TestCases IllegalComboTest"

HeaderWithoutDelimiterTest()
{
	$SimIFCEasy -H 
	[ $? -ne $ExitUsageError ] && return 1

	return 0
}
TestCases="$TestCases HeaderWithoutDelimiterTest"

YearReportNotSupported()
{
	$SimIFCEasy -r year

	[ $? -ne $ExitUnsupportedReportError ] && return 1

	return 0
}
TestCases="$TestCases YearReportNotSupported"

YearReportSupported()
{
	$SimSolarNet2i1s -r year

	[ $? -ne 0 ] && return 1

	return 0
}
TestCases="$TestCases YearReportSupported"

AllReportContainsNoYearData()
{
	$SimIFCEasy -r all > $TestLog

	[ $? -ne 0 ] && return 1

	grep -i year $TestLog && return 1

	return 0
}
TestCases="$TestCases AllReportContainsNoYearData"

HeaderOnlyTest()
{
	typeset failedCount=0

	for reportType in $DeviceReportTypes
	do
		HeaderOnly $SimSolarNet0i0s -D inverter -r $reportType
		failCount=$(($failCount + $?))

		HeaderOnly $SimSolarNet0i0s -D sensor -r $reportType
		failCount=$(($failCount + $?))
	done

	return $failCount
}
TestCases="$TestCases HeaderOnlyTest"

NoOutputTest()
{
	typeset failedCount=0

	for reportType in $DeviceReportTypes
	do
		NoOutput $SimSolarNet0i0s -D inverter -r $reportType
		failCount=$(($failCount + $?))

		NoOutput $SimSolarNet0i0s -D sensor -r $reportType
		failCount=$(($failCount + $?))
	done

	return $failCount
}
TestCases="$TestCases NoOutputTest"

InverterDelimitedReportsTest()
{
	typeset reportType=
	typeset failCount=0

	for reportType in $DeviceReportTypes
	do
		WellFormed $SimIFCEasy -r $reportType
		failCount=$(($failCount + $?))
		WellFormed $SimIFCEasy -r $reportType -D inverter
		failCount=$(($failCount + $?))

		WellFormed $SimSolarNet1i0s -r $reportType
		failCount=$(($failCount + $?))
		WellFormed $SimSolarNet1i0s -r $reportType -D inverter
		failCount=$(($failCount + $?))

		WellFormed $SimSolarNet2i1s -r $reportType
		failCount=$(($failCount + $?))
		WellFormed $SimSolarNet2i1s -r $reportType -D inverter
		failCount=$(($failCount + $?))

		WellFormed $Sim422IGTL -r $reportType
		failCount=$(($failCount + $?))
		WellFormed $Sim422IGTL -r $reportType -D inverter
		failCount=$(($failCount + $?))
	done

	return $failCount
}
TestCases="$TestCases InverterDelimitedReportsTest"

SensorCardDelimitedReportsTest()
{
	typeset reportType=
	typeset failCount=0

	for reportType in $DeviceReportTypes
	do
		WellFormed $SimSolarNet2i1s -r $reportType -D sensor
		failCount=$(($failCount + $?))
	done

	return $failCount
}
TestCases="$TestCases SensorCardDelimitedReportsTest"

ParseArgs()
{
	while getopts 'lt:v' option
	do
		case $option in
		l)
			for testCase in $TestCases
			do
				echo $testCase
			done
			exit 0
			;;
		t)
			TestCases=$OPTARG
			;;
		v)
			Verbose=1
			;;
		?)
			echo $ProgName: invalid option \"$option\"
			exit 1
			;;
		esac
	done
}

CheckEnv()
{
	:
}

RunTest()
{
	echo STARTING TEST CASE $testCase >> $TestLog

	if [ $Verbose -eq 0 ]
	then
		eval $1 >> $TestLog 2>&1
	else
		eval $1
	fi

	exitCode=$?

	if [ $exitCode -eq  0 ]
	then
		result=PASSED
	else
		result=FAILED
	fi

	echo TEST CASE $testCase $result >> $TestLog

	return $exitCode
}

Main()
{
	ParseArgs $*
	CheckEnv $*

	> $TestLog

	failedCount=0

	for testCase in $TestCases
	do
		echo -n $testCase .....

		RunTest $testCase

		if [ $? -eq 0 ]
		then
			echo PASSED
		else
			echo FAILED
			failedCount=$(($failedCount + 1))
		fi
	done
	
	[ $failedCount -ne 0 ] && echo See $TestLog for details

	return $failedCount
}

Main $*
