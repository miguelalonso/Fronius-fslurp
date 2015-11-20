#!/bin/bash
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
# modificado por Miguel Alonso Abella 2015
#Se hace un cron: sudo crontab -e
#* * * * * /home/u4477/fslurp/fslurp2emoncms.sh
#*/5 * * * * /home/u4477/fslurp/fslurp2dataciemat.sh
# el segundo envía un fichero con fecha completa.

#para evitar el overrun

PIDFILE=/home/u4477/fslurp2emoncms123solar.pid
if [ -f $PIDFILE ]
then
  PID=$(cat $PIDFILE)
  ps -p $PID > /dev/null 2>&1
  if [ $? -eq 0 ]
  then
    echo "Process already running"
    exit 1
  else
    ## Process not found assume not running
    echo $$ > $PIDFILE
    if [ $? -ne 0 ]
    then
      echo "Could not create PID file"
      exit 1
    fi
  fi
else
  echo $$ > $PIDFILE
  if [ $? -ne 0 ]
  then
    echo "Could not create PID file"
    exit 1
  fi
fi

#find de overrun
#borrar pid file al final



FECHA=$(date +"%Y%m%d")
#echo $FECHA
HORA=$(date +"%H:%M:%S")
#echo $HORA

ServiceURL=http://163.117.157.189/emoncms_spectrum/input/post.json
apikey=66999c52a47cec0a579ec0a81e98ba23

# Location of fslurp
FSlurp=/home/u4477/fslurp/fslurp

# Location of curl
CURL=/usr/bin/curl
# curl_setopt($CURL, CURLOPT_RETURNTRANSFER, 1)
#######################################

# Options for the report from fslurp
Delimiter=,                                     # fslurp report field delimiter
FSlurpOptions="-p /dev/ttyS0"                   # baudios
FSlurpOptions="${FSlurpOptions} -b 19200"       # format date as YYYYMMDD
FSlurpOptions="${FSlurpOptions} -t %Y%m%d" # format date as YYYYMMDD
FSlurpOptions="${FSlurpOptions} -r all"         # Use day report
FSlurpOptions="${FSlurpOptions} -d $Delimiter"  # Use delimited report

# Get the data from fslurp
# reportOutput=`$FSlurp $FSlurpOptions | cut -d$Delimiter -f1,5`

#echo "Espere, reclamando datos..."

reportOutput=`$FSlurp $FSlurpOptions`
#eliminamos los overflow por ceros
lectura=`echo "$reportOutput" | sed 's/overflow/0/g'`
reportOutput=$lectura

echo $reportOutput >> /home/u4477/fslurp/datos/$FECHA"_datos_fronius_ciemat.log"

echo $reportOutput

echo "Este ha sido el report de los inversores"

LONGITUDLECTURA=`echo "${#reportOutput}"`

echo $LONGITUDLECTURA

if [ $LONGITUDLECTURA -lt 6 ]
then
rm $PIDFILE
exit 2
fi


# Lectura de los canales del datataker  ================================

fichero="/home/u4477/fslurp/datos/123solar/ultimalectura_datataker.csv"

Delimiter=","
delimiter2=";"
while read line; do
 #   echo $line
CADENADT80a=`echo $line | cut -d$delimiter2 -f2`
CADENADT80=${CADENADT80a:4} #cadena sólo con los 40 canales de datos
for i in $(seq 1 40) #para los 40 canales del dt80
do
eval "campo$i=`echo $CADENADT80 | cut -d$Delimiter -f$i`";
eval y='$'campo$i
#echo $yecho $y
done
done < $fichero

echo "todo bien"

G_este=`echo "scale=2;$campo1 * 1000 / 54.87" | bc -l`
G_sur=`echo "scale=2;$campo5 * 1000 / 54.20" | bc -l`
G_oeste=`echo "scale=2;$campo3 * 1000 / 53.91" | bc -l`
Gh=`echo "scale=2;$campo2 * 1000 / 53.91" | bc -l`
G30=`echo "scale=2;$campo4 * 1000 / 54.72" | bc -l`
#echo  $campo36
T_este=`echo "scale=2;$campo36 * 1 / 1" | bc -l`
T_sur=`echo "scale=2;$campo36 * 1 / 1" | bc -l`
T_oeste=`echo "scale=2;$campo36 * 1 / 1" | bc -l`
Ta=`echo "scale=2;$campo36 * 1 / 1" | bc -l`
#echo $Ta
#echo "hasta aqui he llegado"
# Estos canales se graban en los ficheros de los inversores
# FIN DATATAKER======================

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

echo $INV
echo $POWER
echo $DC_CURENT
echo "el de arriba es el nº de inversor"

if [ $INV -lt 6 ] && [ $POWER -gt -87 ] && [ $INV -gt 0 ] && [ $POWER -lt 25000 ]
then
echo "paso por aqui"

echo $INV
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
$var1$MAXIMUM_POWER_DAY\
$var2$MAXIMUM_AC_VOLTAGE_DAY\
$var3$MINIMUM_AC_VOLTAGE_DAY\
$var4$AXIMUM_DC_VOLTAGE_DAY\
$var5$OPERATING_HOURS_DAY\
"}&apikey="$apikey > /dev/null 2>&1

var0="ENERGY_TOTAL_"$INV":"
var1=",MAXIMUM_POWER_TOTAL_"$INV":"
var2=",MAXIMUM_AC_VOLTAGE_TOTAL_"$INV":"
var3=",MINIMUM_AC_VOLTAGE_TOTAL_"$INV":"
var4=",MAXIMUM_DC_VOLTAGE_TOTAL_"$INV":"
var5=",OPERATING_HOURS_TOTAL_"$INV":"


$CURL $ServiceURL$a\
$var0$ENERGY_TOTAL\
$var1$MAXIMUM_POWER_TOTAL\
$var2$MAXIMUM_AC_VOLTAGE_TOTAL\
$var3$MINIMUM_AC_VOLTAGE_TOTAL\
$var4$MAXIMUM_DC_VOLTAGE_TOTAL\
$var5$OPERATING_HOURS_TOTAL\
"}&apikey="$apikey > /dev/null 2>&1

# creación de ficheros para 123solar
#verifica si existe el fichero, al inicio escribe cabecera
# en /home/u4477/fslurp/datos/123solar/invt1
cadena="Time,I1V,I1A,I1P,I2V,I2A,I2P,I3V,I3A,I3P,I4V,I4A,I4P,"\
"G1V,G1A,G1P,G2V,G2A,G2P,G3V,G3A,G3P,FRQ,EFF,INVT,BOOT,SR,KWHT,"\
"MAXIMUM_POWER_DAY,MAXIMUM_AC_VOLTAGE_DAY,MINIMUM_AC_VOLTAGE_DAY,MAXIMUM_DC_VOLTAGE_DAY,"\
"OPERATING_HOURS_DAY,ENERGY_TOTAL,MAXIMUM_POWER_TOTAL,MAXIMUM_AC_VOLTAGE_TOTAL,"\
"MINIMUM_AC_VOLTAGE_TOTAL,MAXIMUM_DC_VOLTAGE_TOTAL,OPERATING_HOURS_TOTAL,INV,G_este,G_sur,G_Oeste,"\
"T_este,T_sur,T_Oeste,Ta,Gh,G30"

#escribe las cabeceras
INPUT="/home/u4477/fslurp/datos/123solar/invt1/"$FECHA".csv"
[ ! -f $INPUT ] && { echo $cadena >> /home/u4477/fslurp/datos/123solar/invt1/$FECHA".csv"; }
INPUT="/home/u4477/fslurp/datos/123solar/invt2/"$FECHA".csv"
[ ! -f $INPUT ] && { echo $cadena >> /home/u4477/fslurp/datos/123solar/invt2/$FECHA".csv"; }
INPUT="/home/u4477/fslurp/datos/123solar/invt3/"$FECHA".csv"
[ ! -f $INPUT ] && { echo $cadena >> /home/u4477/fslurp/datos/123solar/invt3/$FECHA".csv"; }
INPUT="/home/u4477/fslurp/datos/123solar/invt4/"$FECHA".csv"
[ ! -f $INPUT ] && { echo $cadena >> /home/u4477/fslurp/datos/123solar/invt4/$FECHA".csv"; }
INPUT="/home/u4477/fslurp/datos/123solar/invt5/"$FECHA".csv"
[ ! -f $INPUT ] && { echo $cadena >> /home/u4477/fslurp/datos/123solar/invt5/$FECHA".csv"; }


#Time,I1V,I1A,I1P,,,,,,,,,,
#G1V,G1A,G1P,,,,,,,FRQ,,,,,KWHT

# Generated=`echo ${Generated} \* 1000 | bc`

DC_POWER=`echo "scale=2; $DC_CURRENT * $DC_VOLTAGE" | bc -l`
min=0
EFF=0
valor=`echo "$DC_POWER > $min" | bc`
if [ 1 -eq $valor ]
then
EFF=`echo "scale=3;$POWER / $DC_POWER" | bc -l`
fi

cadena=$HORA","$DC_VOLTAGE","$DC_CURRENT","$DC_POWER",,,,,,,,,,"\
$AC_VOLTAGE","$AC_CURRENT","$POWER",,,,,,,"$AC_FREQUENCY","$EFF","$T_este","$Ta","$G_este","$ENERGY_DAY","\
$MAXIMUM_POWER_DAY","$MAXIMUM_AC_VOLTAGE_DAY","$MINIMUM_AC_VOLTAGE_DAY","$MAXIMUM_DC_VOLTAGE_DAY","\
$OPERATING_HOURS_DAY","$ENERGY_TOTAL","$MAXIMUM_POWER_TOTAL","$MAXIMUM_AC_VOLTAGE_TOTAL","\
$MINIMUM_AC_VOLTAGE_TOTAL","$MAXIMUM_DC_VOLTAGE_TOTAL","$OPERATING_HOURS_TOTAL","$INV","\
$G_este","$G_sur","$G_oeste","$T_este","$T_sur","$T_oeste","$Ta","$Gh","$G30
echo $cadena >> /home/u4477/fslurp/datos/123solar/invt$INV"/"$FECHA".csv"


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

if [ $INV -lt 6 ] && [ $POWER -gt -87 ] && [ $INV -gt 0 ] && [ $POWER -lt 25000 ]
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
$var1$MAXIMUM_POWER_DAY\
$var2$MAXIMUM_AC_VOLTAGE_DAY\
$var3$MINIMUM_AC_VOLTAGE_DAY\
$var4$AXIMUM_DC_VOLTAGE_DAY\
$var5$OPERATING_HOURS_DAY\
"}&apikey="$apikey > /dev/null 2>&1

var0="ENERGY_TOTAL_"$INV":"
var1=",MAXIMUM_POWER_TOTAL_"$INV":"
var2=",MAXIMUM_AC_VOLTAGE_TOTAL_"$INV":"
var3=",MINIMUM_AC_VOLTAGE_TOTAL_"$INV":"
var4=",MAXIMUM_DC_VOLTAGE_TOTAL_"$INV":"
var5=",OPERATING_HOURS_TOTAL_"$INV":"


$CURL $ServiceURL$a\
$var0$ENERGY_TOTAL\
$var1$MAXIMUM_POWER_TOTAL\
$var2$MAXIMUM_AC_VOLTAGE_TOTAL\
$var3$MINIMUM_AC_VOLTAGE_TOTAL\
$var4$MAXIMUM_DC_VOLTAGE_TOTAL\
$var5$OPERATING_HOURS_TOTAL\
"}&apikey="$apikey > /dev/null 2>&1

DC_POWER=`echo "scale=2; $DC_CURRENT * $DC_VOLTAGE" | bc -l`
min=0
EFF=0
valor=`echo "$DC_POWER > $min" | bc`
if [ 1 -eq $valor ]
then
EFF=`echo "scale=3;$POWER / $DC_POWER" | bc -l`
fi


cadena=$HORA","$DC_VOLTAGE","$DC_CURRENT","$DC_POWER",,,,,,,,,,"\
$AC_VOLTAGE","$AC_CURRENT","$POWER",,,,,,,"$AC_FREQUENCY","$EFF","$T_este","$Ta","$G_este","$ENERGY_DAY","\
$MAXIMUM_POWER_DAY","$MAXIMUM_AC_VOLTAGE_DAY","$MINIMUM_AC_VOLTAGE_DAY","$MAXIMUM_DC_VOLTAGE_DAY","\
$OPERATING_HOURS_DAY","$ENERGY_TOTAL","$MAXIMUM_POWER_TOTAL","$MAXIMUM_AC_VOLTAGE_TOTAL","\
$MINIMUM_AC_VOLTAGE_TOTAL","$MAXIMUM_DC_VOLTAGE_TOTAL","$OPERATING_HOURS_TOTAL","$INV","\
$G_este","$G_sur","$G_oeste","$T_este","$T_sur","$T_oeste","$Ta","$Gh","$G30
echo $cadena >> /home/u4477/fslurp/datos/123solar/invt$INV"/"$FECHA".csv"

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

if [ $INV -lt 6 ] && [ $POWER -gt -87 ] && [ $INV -gt 0 ] && [ $POWER -lt 25000 ]
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
$var1$MAXIMUM_POWER_DAY\
$var2$MAXIMUM_AC_VOLTAGE_DAY\
$var3$MINIMUM_AC_VOLTAGE_DAY\
$var4$AXIMUM_DC_VOLTAGE_DAY\
$var5$OPERATING_HOURS_DAY\
"}&apikey="$apikey > /dev/null 2>&1

var0="ENERGY_TOTAL_"$INV":"
var1=",MAXIMUM_POWER_TOTAL_"$INV":"
var2=",MAXIMUM_AC_VOLTAGE_TOTAL_"$INV":"
var3=",MINIMUM_AC_VOLTAGE_TOTAL_"$INV":"
var4=",MAXIMUM_DC_VOLTAGE_TOTAL_"$INV":"
var5=",OPERATING_HOURS_TOTAL_"$INV":"


$CURL $ServiceURL$a\
$var0$ENERGY_TOTAL\
$var1$MAXIMUM_POWER_TOTAL\
$var2$MAXIMUM_AC_VOLTAGE_TOTAL\
$var3$MINIMUM_AC_VOLTAGE_TOTAL\
$var4$MAXIMUM_DC_VOLTAGE_TOTAL\
$var5$OPERATING_HOURS_TOTAL\
"}&apikey="$apikey > /dev/null 2>&1

DC_POWER=`echo "scale=2; $DC_CURRENT * $DC_VOLTAGE" | bc -l`
min=0
EFF=0
valor=`echo "$DC_POWER > $min" | bc`
if [ 1 -eq $valor ]
then
EFF=`echo "scale=3;$POWER / $DC_POWER" | bc -l`
fi

cadena=$HORA","$DC_VOLTAGE","$DC_CURRENT","$DC_POWER",,,,,,,,,,"\
$AC_VOLTAGE","$AC_CURRENT","$POWER",,,,,,,"$AC_FREQUENCY","$EFF","$T_este","$Ta","$G_este","$ENERGY_DAY","\
$MAXIMUM_POWER_DAY","$MAXIMUM_AC_VOLTAGE_DAY","$MINIMUM_AC_VOLTAGE_DAY","$MAXIMUM_DC_VOLTAGE_DAY","\
$OPERATING_HOURS_DAY","$ENERGY_TOTAL","$MAXIMUM_POWER_TOTAL","$MAXIMUM_AC_VOLTAGE_TOTAL","\
$MINIMUM_AC_VOLTAGE_TOTAL","$MAXIMUM_DC_VOLTAGE_TOTAL","$OPERATING_HOURS_TOTAL","$INV","\
$G_este","$G_sur","$G_oeste","$T_este","$T_sur","$T_oeste","$Ta","$Gh","$G30
echo $cadena >> /home/u4477/fslurp/datos/123solar/invt$INV"/"$FECHA".csv"

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

if [ $INV -lt 6 ] && [ $POWER -gt -87 ] && [ $INV -gt 0 ] && [ $POWER -lt 25000 ]
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
$var1$MAXIMUM_POWER_DAY\
$var2$MAXIMUM_AC_VOLTAGE_DAY\
$var3$MINIMUM_AC_VOLTAGE_DAY\
$var4$AXIMUM_DC_VOLTAGE_DAY\
$var5$OPERATING_HOURS_DAY\
"}&apikey="$apikey > /dev/null 2>&1

var0="ENERGY_TOTAL_"$INV":"
var1=",MAXIMUM_POWER_TOTAL_"$INV":"
var2=",MAXIMUM_AC_VOLTAGE_TOTAL_"$INV":"
var3=",MINIMUM_AC_VOLTAGE_TOTAL_"$INV":"
var4=",MAXIMUM_DC_VOLTAGE_TOTAL_"$INV":"
var5=",OPERATING_HOURS_TOTAL_"$INV":"


$CURL $ServiceURL$a\
$var0$ENERGY_TOTAL\
$var1$MAXIMUM_POWER_TOTAL\
$var2$MAXIMUM_AC_VOLTAGE_TOTAL\
$var3$MINIMUM_AC_VOLTAGE_TOTAL\
$var4$MAXIMUM_DC_VOLTAGE_TOTAL\
$var5$OPERATING_HOURS_TOTAL\
"}&apikey="$apikey > /dev/null 2>&1

DC_POWER=`echo "scale=2; $DC_CURRENT * $DC_VOLTAGE" | bc -l`
min=0
EFF=0
valor=`echo "$DC_POWER > $min" | bc`
if [ 1 -eq $valor ]
then
EFF=`echo "scale=3;$POWER / $DC_POWER" | bc -l`
fi

cadena=$HORA","$DC_VOLTAGE","$DC_CURRENT","$DC_POWER",,,,,,,,,,"\
$AC_VOLTAGE","$AC_CURRENT","$POWER",,,,,,,"$AC_FREQUENCY","$EFF","$T_oeste","$Ta","$G_oeste","$ENERGY_DAY","\
$MAXIMUM_POWER_DAY","$MAXIMUM_AC_VOLTAGE_DAY","$MINIMUM_AC_VOLTAGE_DAY","$MAXIMUM_DC_VOLTAGE_DAY","\
$OPERATING_HOURS_DAY","$ENERGY_TOTAL","$MAXIMUM_POWER_TOTAL","$MAXIMUM_AC_VOLTAGE_TOTAL","\
$MINIMUM_AC_VOLTAGE_TOTAL","$MAXIMUM_DC_VOLTAGE_TOTAL","$OPERATING_HOURS_TOTAL","$INV","\
$G_este","$G_sur","$G_oeste","$T_este","$T_sur","$T_oeste","$Ta","$Gh","$G30
echo $cadena >> /home/u4477/fslurp/datos/123solar/invt$INV"/"$FECHA".csv"

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

if [ $INV -lt 6 ] && [ $POWER -gt -87 ] && [ $INV -gt 0 ] && [ $POWER -lt 25000 ]
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
$var1$MAXIMUM_POWER_DAY\
$var2$MAXIMUM_AC_VOLTAGE_DAY\
$var3$MINIMUM_AC_VOLTAGE_DAY\
$var4$AXIMUM_DC_VOLTAGE_DAY\
$var5$OPERATING_HOURS_DAY\
"}&apikey="$apikey > /dev/null 2>&1

var0="ENERGY_TOTAL_"$INV":"
var1=",MAXIMUM_POWER_TOTAL_"$INV":"
var2=",MAXIMUM_AC_VOLTAGE_TOTAL_"$INV":"
var3=",MINIMUM_AC_VOLTAGE_TOTAL_"$INV":"
var4=",MAXIMUM_DC_VOLTAGE_TOTAL_"$INV":"
var5=",OPERATING_HOURS_TOTAL_"$INV":"


$CURL $ServiceURL$a\
$var0$ENERGY_TOTAL\
$var1$MAXIMUM_POWER_TOTAL\
$var2$MAXIMUM_AC_VOLTAGE_TOTAL\
$var3$MINIMUM_AC_VOLTAGE_TOTAL\
$var4$MAXIMUM_DC_VOLTAGE_TOTAL\
$var5$OPERATING_HOURS_TOTAL\
"}&apikey="$apikey > /dev/null 2>&1

DC_POWER=`echo "scale=2; $DC_CURRENT * $DC_VOLTAGE" | bc -l`
min=0
EFF=0
valor=`echo "$DC_POWER > $min" | bc`
if [ 1 -eq $valor ]
then
EFF=`echo "scale=3;$POWER / $DC_POWER" | bc -l`
fi

cadena=$HORA","$DC_VOLTAGE","$DC_CURRENT","$DC_POWER",,,,,,,,,,"\
$AC_VOLTAGE","$AC_CURRENT","$POWER",,,,,,,"$AC_FREQUENCY","$EFF","$T_sur","$Ta","$G_sur","$ENERGY_DAY","\
$MAXIMUM_POWER_DAY","$MAXIMUM_AC_VOLTAGE_DAY","$MINIMUM_AC_VOLTAGE_DAY","$MAXIMUM_DC_VOLTAGE_DAY","\
$OPERATING_HOURS_DAY","$ENERGY_TOTAL","$MAXIMUM_POWER_TOTAL","$MAXIMUM_AC_VOLTAGE_TOTAL","\
$MINIMUM_AC_VOLTAGE_TOTAL","$MAXIMUM_DC_VOLTAGE_TOTAL","$OPERATING_HOURS_TOTAL","$INV","\
$G_este","$G_sur","$G_oeste","$T_este","$T_sur","$T_oeste","$Ta","$Gh","$G30
echo $cadena >> /home/u4477/fslurp/datos/123solar/invt$INV"/"$FECHA".csv"

fi

 echo "Datos enviados a emoncms, FIN, OK"

rm $PIDFILE
