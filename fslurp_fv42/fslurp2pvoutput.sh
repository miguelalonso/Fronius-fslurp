#!/bin/sh
#
# fslurp, Copyright (c) 2012-2013 David Stone <ds.fslurp@asif.com>
#
# $RCSfile: fslurp2pvoutput.sh,v $
# $Revision: 1.2 $
# $Date: 2013/12/21 14:47:55 $
#
# Permission to use, copy, modify, distribute, and sell this software and its
# documentation for any purpose is hereby granted without fee, provided that
# the above copyright notice appear in all copies and that both the
# copyright notice and this permission notice appear in supporting
# documentation.  No representations are made about the suitability of this
# software for any purpose.  It is provided "as is" without express or 
# implied warranty.
#
#
# Sample script to fetch current daily total KWH production and send it
# to pvoutput.org.
#
# To use, update the PVAPIKey, PVSystemID, FSlurp, and CURL variables below

PVAPIKey=Your-PVOutput-API-Key
PVSystemID=Your-PVOutput-System-ID

# Location of fslurp
FSlurp=./fslurp

# Location of curl
CURL=/usr/bin/curl

#######################################

ServiceURL=http://pvoutput.org/service/r2/addoutput.jsp
Header1="X-Pvoutput-Apikey: $PVAPIKey"
Header2="X-Pvoutput-SystemId: $PVSystemID"

# Options for the report from fslurp
Delimiter=,					# fslurp report field delimiter
FSlurpOptions=
FSlurpOptions="${FSlurpOptions} -t %Y%m%d"	# format date as YYYYMMDD
FSlurpOptions="${FSlurpOptions} -r day"		# Use day report
FSlurpOptions="${FSlurpOptions} -d $Delimiter"	# Use delimited report


# Get the data from fslurp
reportOutput=`$FSlurp $FSlurpOptions | cut -d$Delimiter -f1,5`

Date=`echo $reportOutput | cut -d$Delimiter -f1`
Generated=`echo $reportOutput | cut -d$Delimiter -f2`
# Convert KilowattHour data from Inverter to WattHour data.
Generated=`echo ${Generated} \* 1000 | bc`

CURLData=
CURLData="${CURLData} -d d=$Date"
CURLData="${CURLData} -d g=$Generated"

$CURL -H "$Header1" -H "$Header2" $CURLData $ServiceURL
