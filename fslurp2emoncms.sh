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

# tipico comando de solicitud de datos: 
# ./fslurp -b 19200 -r all -d , -p /dev/ttyS1 

ServiceURL=http://163.117.157.189/emoncms_spectrum/input/post.json
apikey=66999c52a47cec0a579ec0a81e98ba23

# Location of fslurp
FSlurp=/home/u4477/fslurp/fslurp

# Location of curl
CURL=/usr/bin/curl 
# curl_setopt($CURL, CURLOPT_RETURNTRANSFER, 1)
#######################################

# Options for the report from fslurp
Delimiter=,					# fslurp report field delimiter
FSlurpOptions="-p /dev/ttyS1"       		# baudios
FSlurpOptions="${FSlurpOptions} -b 19200"      	# format date as YYYYMMDD
FSlurpOptions="${FSlurpOptions} -t %Y%m%d" # format date as YYYYMMDD
FSlurpOptions="${FSlurpOptions} -r all"		# Use day report
FSlurpOptions="${FSlurpOptions} -d $Delimiter"	# Use delimited report

# Get the data from fslurp
# reportOutput=`$FSlurp $FSlurpOptions | cut -d$Delimiter -f1,5`

#echo "Espere, reclamando datos..."

reportOutput=`$FSlurp $FSlurpOptions`
echo $reportOutput >> /home/u4477/fslurp/datos/datos_fronius_ciemat.log

#echo $reportOutput

#echo "Este ha sido el report de los inversores"

Date=`echo $reportOutput | cut -d$Delimiter -f1`
INV=`echo $reportOutput | cut -d$Delimiter -f2`
# Convert KilowattHour data from Inverter to WattHour data.
# Generated=`echo ${Generated} \* 1000 | bc`
POWER=`echo $reportOutput | cut -d$Delimiter -f5`
AC_CURRENT=`echo $reportOutput | cut -d$Delimiter -f6`
AC_VOLTAGE=`echo $reportOutput | cut -d$Delimiter -f7`
AC_FREQUENCY=`echo $reportOutput | cut -d$Delimiter -f8`
DC_CURRENT=`echo $reportOutput | cut -d$Delimiter -f9`
DC_VOLTAGE=`echo $reportOutput | cut -d$Delimiter -f10`

ENERGY_DAY=`echo $reportOutput | cut -d$Delimiter -f11`
MAXIMUM_POWER_DAY=`echo $reportOutput | cut -d$Delimiter -f12`
MAXIMUM_AC_VOLTAGE_DAY=`echo $reportOutput | cut -d$Delimiter -f13`
MINIMUM_AC_VOLTAGE_DAY=`echo $reportOutput | cut -d$Delimiter -f14`
MAXIMUM_DC_VOLTAGE_DAY=`echo $reportOutput | cut -d$Delimiter -f15`
OPERATING_HOURS_DAY=`echo $reportOutput | cut -d$Delimiter -f16`

ENERGY_TOTAL=`echo $reportOutput | cut -d$Delimiter -f17`
MAXIMUM_POWER_TOTAL=`echo $reportOutput | cut -d$Delimiter -f18`
MAXIMUM_AC_VOLTAGE_TOTAL=`echo $reportOutput | cut -d$Delimiter -f19`
MINIMUM_AC_VOLTAGE_TOTAL=`echo $reportOutput | cut -d$Delimiter -f20`
MAXIMUM_DC_VOLTAGE_TOTAL=`echo $reportOutput | cut -d$Delimiter -f21`
OPERATING_HOURS_TOTAL=`echo $reportOutput | cut -d$Delimiter -f22`

# Numero de inversor entre 1 y 5, potencia >-87

if [ $INV -lt 6 ] && [ $POWER -gt -87 ] && [ $INV -gt 0 ] 
then

var0="DATE_"$INV":"
var1=",POWER_"$INV":"
var2=",AC_CURRENT_"$INV":"
var3=",AC_VOLTAGE_"$INV":"
var4=",AC_FREQUENCY_"$INV":"
var5=",DC_CURRENT_"$INV":"
var6=",DC_VOLTAGE_"$INV":"


a="?json={"
$CURL $ServiceURL$a\
$var0$Date\
$var1$POWER\
$var2$AC_CURRENT\
$var3$AC_VOLTAGE\
$var4$AC_FREQUENCY\
$var5$DC_CURRENT\
$var6$DC_VOLTAGE\
"}&apikey="$apikey > /dev/null 2>&1

var0="ENERGY_DAY_"$INV":"
var1=",MAXIMUM_POWER_DAY_"$INV":"
var2=",MAXIMUM_AC_VOLTAGE_DAY_"$INV":"
var3=",MINIMUM_AC_VOLTAGE_DAY_"$INV":"
var4=",MAXIMUM_DC_VOLTAGE_DAY_"$INV":"
var5=",OPERATING_HOURS_DAY_"$INV":"


$CURL $ServiceURL$a\
$var0$ENERGY_DAY\
$var1$POWER\
$var2$AC_CURRENT\
$var3$AC_VOLTAGE\
$var4$AC_FREQUENCY\
$var5$DC_CURRENT\
"}&apikey="$apikey > /dev/null 2>&1

var0="ENERGY_TOTAL_"$INV":"
var1=",MAXIMUM_POWER_TOTAL_"$INV":"
var2=",MAXIMUM_AC_VOLTAGE_TOTAL_"$INV":"
var3=",MINIMUM_AC_VOLTAGE_TOTAL_"$INV":"
var4=",MAXIMUM_DC_VOLTAGE_TOTAL_"$INV":"
var5=",OPERATING_HOURS_TOTAL_"$INV":"


$CURL $ServiceURL$a\
$var0$ENERGY_TOTAL\
$var1$POWER\
$var2$AC_CURRENT\
$var3$AC_VOLTAGE\
$var4$AC_FREQUENCY\
$var5$DC_CURRENT\
"}&apikey="$apikey > /dev/null 2>&1

fi

#########Inversor 4
#Date=`echo $reportOutput | cut -d$Delimiter -f23`
# Se comenta Date ya que sale con espacios en blanco y no se puede enviar por curl. hay que
# quitarlos en la proxima versión
INV=`echo $reportOutput | cut -d$Delimiter -f24`
# Convert KilowattHour data from Inverter to WattHour data.
# Generated=`echo ${Generated} \* 1000 | bc`
POWER=`echo $reportOutput | cut -d$Delimiter -f27`
AC_CURRENT=`echo $reportOutput | cut -d$Delimiter -f28`
AC_VOLTAGE=`echo $reportOutput | cut -d$Delimiter -f29`
AC_FREQUENCY=`echo $reportOutput | cut -d$Delimiter -f30`
DC_CURRENT=`echo $reportOutput | cut -d$Delimiter -f31`
DC_VOLTAGE=`echo $reportOutput | cut -d$Delimiter -f32`

ENERGY_DAY=`echo $reportOutput | cut -d$Delimiter -f33`
MAXIMUM_POWER_DAY=`echo $reportOutput | cut -d$Delimiter -f34`
MAXIMUM_AC_VOLTAGE_DAY=`echo $reportOutput | cut -d$Delimiter -f35`
MINIMUM_AC_VOLTAGE_DAY=`echo $reportOutput | cut -d$Delimiter -f36`
MAXIMUM_DC_VOLTAGE_DAY=`echo $reportOutput | cut -d$Delimiter -f37`
OPERATING_HOURS_DAY=`echo $reportOutput | cut -d$Delimiter -f38`

ENERGY_TOTAL=`echo $reportOutput | cut -d$Delimiter -f39`
MAXIMUM_POWER_TOTAL=`echo $reportOutput | cut -d$Delimiter -f40`
MAXIMUM_AC_VOLTAGE_TOTAL=`echo $reportOutput | cut -d$Delimiter -f41`
MINIMUM_AC_VOLTAGE_TOTAL=`echo $reportOutput | cut -d$Delimiter -f42`
MAXIMUM_DC_VOLTAGE_TOTAL=`echo $reportOutput | cut -d$Delimiter -f43`
OPERATING_HOURS_TOTAL=`echo $reportOutput | cut -d$Delimiter -f44`

if [ $INV -lt 6 ] && [ $POWER -gt -87 ] && [ $INV -gt 0 ]
	then
var0="DATE_"$INV":"
var1=",POWER_"$INV":"
var2=",AC_CURRENT_"$INV":"
var3=",AC_VOLTAGE_"$INV":"
var4=",AC_FREQUENCY_"$INV":"
var5=",DC_CURRENT_"$INV":"
var6=",DC_VOLTAGE_"$INV":"


a="?json={"
$CURL $ServiceURL$a\
$var0$Date\
$var1$POWER\
$var2$AC_CURRENT\
$var3$AC_VOLTAGE\
$var4$AC_FREQUENCY\
$var5$DC_CURRENT\
$var6$DC_VOLTAGE\
"}&apikey="$apikey > /dev/null 2>&1

var0="ENERGY_DAY_"$INV":"
var1=",MAXIMUM_POWER_DAY_"$INV":"
var2=",MAXIMUM_AC_VOLTAGE_DAY_"$INV":"
var3=",MINIMUM_AC_VOLTAGE_DAY_"$INV":"
var4=",MAXIMUM_DC_VOLTAGE_DAY_"$INV":"
var5=",OPERATING_HOURS_DAY_"$INV":"


$CURL $ServiceURL$a\
$var0$ENERGY_DAY\
$var1$POWER\
$var2$AC_CURRENT\
$var3$AC_VOLTAGE\
$var4$AC_FREQUENCY\
$var5$DC_CURRENT\
"}&apikey="$apikey > /dev/null 2>&1

var0="ENERGY_TOTAL_"$INV":"
var1=",MAXIMUM_POWER_TOTAL_"$INV":"
var2=",MAXIMUM_AC_VOLTAGE_TOTAL_"$INV":"
var3=",MINIMUM_AC_VOLTAGE_TOTAL_"$INV":"
var4=",MAXIMUM_DC_VOLTAGE_TOTAL_"$INV":"
var5=",OPERATING_HOURS_TOTAL_"$INV":"


$CURL $ServiceURL$a\
$var0$ENERGY_TOTAL\
$var1$POWER\
$var2$AC_CURRENT\
$var3$AC_VOLTAGE\
$var4$AC_FREQUENCY\
$var5$DC_CURRENT\
"}&apikey="$apikey > /dev/null 2>&1

fi

#########Inversor 3
#Date=`echo $reportOutput | cut -d$Delimiter -f45`
INV=`echo $reportOutput | cut -d$Delimiter -f46`
# Convert KilowattHour data from Inverter to WattHour data.
# Generated=`echo ${Generated} \* 1000 | bc`
POWER=`echo $reportOutput | cut -d$Delimiter -f49`
AC_CURRENT=`echo $reportOutput | cut -d$Delimiter -f50`
AC_VOLTAGE=`echo $reportOutput | cut -d$Delimiter -f51`
AC_FREQUENCY=`echo $reportOutput | cut -d$Delimiter -f52`
DC_CURRENT=`echo $reportOutput | cut -d$Delimiter -f53`
DC_VOLTAGE=`echo $reportOutput | cut -d$Delimiter -f54`

ENERGY_DAY=`echo $reportOutput | cut -d$Delimiter -f55`
MAXIMUM_POWER_DAY=`echo $reportOutput | cut -d$Delimiter -f56`
MAXIMUM_AC_VOLTAGE_DAY=`echo $reportOutput | cut -d$Delimiter -f57`
MINIMUM_AC_VOLTAGE_DAY=`echo $reportOutput | cut -d$Delimiter -f58`
MAXIMUM_DC_VOLTAGE_DAY=`echo $reportOutput | cut -d$Delimiter -f59`
OPERATING_HOURS_DAY=`echo $reportOutput | cut -d$Delimiter -f60`

ENERGY_TOTAL=`echo $reportOutput | cut -d$Delimiter -f61`
MAXIMUM_POWER_TOTAL=`echo $reportOutput | cut -d$Delimiter -f62`
MAXIMUM_AC_VOLTAGE_TOTAL=`echo $reportOutput | cut -d$Delimiter -f63`
MINIMUM_AC_VOLTAGE_TOTAL=`echo $reportOutput | cut -d$Delimiter -f64`
MAXIMUM_DC_VOLTAGE_TOTAL=`echo $reportOutput | cut -d$Delimiter -f65`
OPERATING_HOURS_TOTAL=`echo $reportOutput | cut -d$Delimiter -f66`

if [ $INV -lt 6 ] && [ $POWER -gt -87 ] && [ $INV -gt 0 ]
	then

var0="DATE_"$INV":"
var1=",POWER_"$INV":"
var2=",AC_CURRENT_"$INV":"
var3=",AC_VOLTAGE_"$INV":"
var4=",AC_FREQUENCY_"$INV":"
var5=",DC_CURRENT_"$INV":"
var6=",DC_VOLTAGE_"$INV":"


a="?json={"
$CURL $ServiceURL$a\
$var0$Date\
$var1$POWER\
$var2$AC_CURRENT\
$var3$AC_VOLTAGE\
$var4$AC_FREQUENCY\
$var5$DC_CURRENT\
$var6$DC_VOLTAGE\
"}&apikey="$apikey > /dev/null 2>&1

var0="ENERGY_DAY_"$INV":"
var1=",MAXIMUM_POWER_DAY_"$INV":"
var2=",MAXIMUM_AC_VOLTAGE_DAY_"$INV":"
var3=",MINIMUM_AC_VOLTAGE_DAY_"$INV":"
var4=",MAXIMUM_DC_VOLTAGE_DAY_"$INV":"
var5=",OPERATING_HOURS_DAY_"$INV":"


$CURL $ServiceURL$a\
$var0$ENERGY_DAY\
$var1$POWER\
$var2$AC_CURRENT\
$var3$AC_VOLTAGE\
$var4$AC_FREQUENCY\
$var5$DC_CURRENT\
"}&apikey="$apikey > /dev/null 2>&1

var0="ENERGY_TOTAL_"$INV":"
var1=",MAXIMUM_POWER_TOTAL_"$INV":"
var2=",MAXIMUM_AC_VOLTAGE_TOTAL_"$INV":"
var3=",MINIMUM_AC_VOLTAGE_TOTAL_"$INV":"
var4=",MAXIMUM_DC_VOLTAGE_TOTAL_"$INV":"
var5=",OPERATING_HOURS_TOTAL_"$INV":"


$CURL $ServiceURL$a\
$var0$ENERGY_TOTAL\
$var1$POWER\
$var2$AC_CURRENT\
$var3$AC_VOLTAGE\
$var4$AC_FREQUENCY\
$var5$DC_CURRENT\
"}&apikey="$apikey > /dev/null 2>&1

fi

#########Inversor 2
#Date=`echo $reportOutput | cut -d$Delimiter -f67`
INV=`echo $reportOutput | cut -d$Delimiter -f68`
# Convert KilowattHour data from Inverter to WattHour data.
# Generated=`echo ${Generated} \* 1000 | bc`
POWER=`echo $reportOutput | cut -d$Delimiter -f71`
AC_CURRENT=`echo $reportOutput | cut -d$Delimiter -f72`
AC_VOLTAGE=`echo $reportOutput | cut -d$Delimiter -f73`
AC_FREQUENCY=`echo $reportOutput | cut -d$Delimiter -f74`
DC_CURRENT=`echo $reportOutput | cut -d$Delimiter -f75`
DC_VOLTAGE=`echo $reportOutput | cut -d$Delimiter -f76`

ENERGY_DAY=`echo $reportOutput | cut -d$Delimiter -f77`
MAXIMUM_POWER_DAY=`echo $reportOutput | cut -d$Delimiter -f78`
MAXIMUM_AC_VOLTAGE_DAY=`echo $reportOutput | cut -d$Delimiter -f79`
MINIMUM_AC_VOLTAGE_DAY=`echo $reportOutput | cut -d$Delimiter -f80`
MAXIMUM_DC_VOLTAGE_DAY=`echo $reportOutput | cut -d$Delimiter -f81`
OPERATING_HOURS_DAY=`echo $reportOutput | cut -d$Delimiter -f82`

ENERGY_TOTAL=`echo $reportOutput | cut -d$Delimiter -f83`
MAXIMUM_POWER_TOTAL=`echo $reportOutput | cut -d$Delimiter -f84`
MAXIMUM_AC_VOLTAGE_TOTAL=`echo $reportOutput | cut -d$Delimiter -f85`
MINIMUM_AC_VOLTAGE_TOTAL=`echo $reportOutput | cut -d$Delimiter -f86`
MAXIMUM_DC_VOLTAGE_TOTAL=`echo $reportOutput | cut -d$Delimiter -f87`
OPERATING_HOURS_TOTAL=`echo $reportOutput | cut -d$Delimiter -f88`

if [ $INV -lt 6 ] && [ $POWER -gt -87 ] && [ $INV -gt 0 ]
	then

var0="DATE_"$INV":"
var1=",POWER_"$INV":"
var2=",AC_CURRENT_"$INV":"
var3=",AC_VOLTAGE_"$INV":"
var4=",AC_FREQUENCY_"$INV":"
var5=",DC_CURRENT_"$INV":"
var6=",DC_VOLTAGE_"$INV":"


a="?json={"
$CURL $ServiceURL$a\
$var0$Date\
$var1$POWER\
$var2$AC_CURRENT\
$var3$AC_VOLTAGE\
$var4$AC_FREQUENCY\
$var5$DC_CURRENT\
$var6$DC_VOLTAGE\
"}&apikey="$apikey > /dev/null 2>&1

var0="ENERGY_DAY_"$INV":"
var1=",MAXIMUM_POWER_DAY_"$INV":"
var2=",MAXIMUM_AC_VOLTAGE_DAY_"$INV":"
var3=",MINIMUM_AC_VOLTAGE_DAY_"$INV":"
var4=",MAXIMUM_DC_VOLTAGE_DAY_"$INV":"
var5=",OPERATING_HOURS_DAY_"$INV":"


$CURL $ServiceURL$a\
$var0$ENERGY_DAY\
$var1$POWER\
$var2$AC_CURRENT\
$var3$AC_VOLTAGE\
$var4$AC_FREQUENCY\
$var5$DC_CURRENT\
"}&apikey="$apikey > /dev/null 2>&1

var0="ENERGY_TOTAL_"$INV":"
var1=",MAXIMUM_POWER_TOTAL_"$INV":"
var2=",MAXIMUM_AC_VOLTAGE_TOTAL_"$INV":"
var3=",MINIMUM_AC_VOLTAGE_TOTAL_"$INV":"
var4=",MAXIMUM_DC_VOLTAGE_TOTAL_"$INV":"
var5=",OPERATING_HOURS_TOTAL_"$INV":"


$CURL $ServiceURL$a\
$var0$ENERGY_TOTAL\
$var1$POWER\
$var2$AC_CURRENT\
$var3$AC_VOLTAGE\
$var4$AC_FREQUENCY\
$var5$DC_CURRENT\
"}&apikey="$apikey > /dev/null 2>&1

fi

#########Inversor 1

#Date=`echo $reportOutput | cut -d$Delimiter -f89`
INV=`echo $reportOutput | cut -d$Delimiter -f90`
# Convert KilowattHour data from Inverter to WattHour data.
# Generated=`echo ${Generated} \* 1000 | bc`
POWER=`echo $reportOutput | cut -d$Delimiter -f93`
AC_CURRENT=`echo $reportOutput | cut -d$Delimiter -f94`
AC_VOLTAGE=`echo $reportOutput | cut -d$Delimiter -f95`
AC_FREQUENCY=`echo $reportOutput | cut -d$Delimiter -f96`
DC_CURRENT=`echo $reportOutput | cut -d$Delimiter -f97`
DC_VOLTAGE=`echo $reportOutput | cut -d$Delimiter -f98`

ENERGY_DAY=`echo $reportOutput | cut -d$Delimiter -f99`
MAXIMUM_POWER_DAY=`echo $reportOutput | cut -d$Delimiter -f100`
MAXIMUM_AC_VOLTAGE_DAY=`echo $reportOutput | cut -d$Delimiter -f101`
MINIMUM_AC_VOLTAGE_DAY=`echo $reportOutput | cut -d$Delimiter -f102`
MAXIMUM_DC_VOLTAGE_DAY=`echo $reportOutput | cut -d$Delimiter -f103`
OPERATING_HOURS_DAY=`echo $reportOutput | cut -d$Delimiter -f104`

ENERGY_TOTAL=`echo $reportOutput | cut -d$Delimiter -f105`
MAXIMUM_POWER_TOTAL=`echo $reportOutput | cut -d$Delimiter -f106`
MAXIMUM_AC_VOLTAGE_TOTAL=`echo $reportOutput | cut -d$Delimiter -f107`
MINIMUM_AC_VOLTAGE_TOTAL=`echo $reportOutput | cut -d$Delimiter -f108`
MAXIMUM_DC_VOLTAGE_TOTAL=`echo $reportOutput | cut -d$Delimiter -f109`
OPERATING_HOURS_TOTAL=`echo $reportOutput | cut -d$Delimiter -f110`

if [ $INV -lt 6 ] && [ $POWER -gt -87 ] && [ $INV -gt 0 ]
	then

var0="DATE_"$INV":"
var1=",POWER_"$INV":"
var2=",AC_CURRENT_"$INV":"
var3=",AC_VOLTAGE_"$INV":"
var4=",AC_FREQUENCY_"$INV":"
var5=",DC_CURRENT_"$INV":"
var6=",DC_VOLTAGE_"$INV":"


a="?json={"
$CURL $ServiceURL$a\
$var0$Date\
$var1$POWER\
$var2$AC_CURRENT\
$var3$AC_VOLTAGE\
$var4$AC_FREQUENCY\
$var5$DC_CURRENT\
$var6$DC_VOLTAGE\
"}&apikey="$apikey > /dev/null 2>&1

var0="ENERGY_DAY_"$INV":"
var1=",MAXIMUM_POWER_DAY_"$INV":"
var2=",MAXIMUM_AC_VOLTAGE_DAY_"$INV":"
var3=",MINIMUM_AC_VOLTAGE_DAY_"$INV":"
var4=",MAXIMUM_DC_VOLTAGE_DAY_"$INV":"
var5=",OPERATING_HOURS_DAY_"$INV":"


$CURL $ServiceURL$a\
$var0$ENERGY_DAY\
$var1$POWER\
$var2$AC_CURRENT\
$var3$AC_VOLTAGE\
$var4$AC_FREQUENCY\
$var5$DC_CURRENT\
"}&apikey="$apikey > /dev/null 2>&1

var0="ENERGY_TOTAL_"$INV":"
var1=",MAXIMUM_POWER_TOTAL_"$INV":"
var2=",MAXIMUM_AC_VOLTAGE_TOTAL_"$INV":"
var3=",MINIMUM_AC_VOLTAGE_TOTAL_"$INV":"
var4=",MAXIMUM_DC_VOLTAGE_TOTAL_"$INV":"
var5=",OPERATING_HOURS_TOTAL_"$INV":"


$CURL $ServiceURL$a\
$var0$ENERGY_TOTAL\
$var1$POWER\
$var2$AC_CURRENT\
$var3$AC_VOLTAGE\
$var4$AC_FREQUENCY\
$var5$DC_CURRENT\
"}&apikey="$apikey > /dev/null 2>&1

fi

# echo "Datos enviados a emoncms"

