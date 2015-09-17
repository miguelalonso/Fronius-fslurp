#!/bin/sh
#
#
# Sample script to send files from fslurp to 123solar server site (in leganes)
#
#
#

# tipico comando de solicitud de datos:
# ./fslurp -b 19200 -r all -d , -p /dev/ttyS1
#
#Se hace un cron: sudo crontab -e
#* * * * * /home/u4477/fslurp/fslurp2emoncms.sh
#*/5 * * * * /home/u4477/fslurp/fslurp2dataciemat.sh
# el segundo envía un fichero con fecha completa.

FECHA=$(date +"%Y%m%d")
YEAR=$(date +"%Y")
#echo $FECHA
HORA=$(date +"%H:%M:%S")
#echo $HORA

/home/u4477/fslurp/datos/123solar/energy1.sh
/home/u4477/fslurp/datos/123solar/energy2.sh
/home/u4477/fslurp/datos/123solar/energy3.sh
/home/u4477/fslurp/datos/123solar/energy4.sh
/home/u4477/fslurp/datos/123solar/energy5.sh


wput -u /home/u4477/fslurp/datos/123solar/invt1/production/energy$YEAR".csv" ftp://fronius:fronius@163.117.157.189/invt1/production/energy$YEAR".csv" > /dev/null 2>&1

wput -u /home/u4477/fslurp/datos/123solar/invt2/production/energy$YEAR".csv" ftp://fronius:froniuwput -u /home/u4477/fslurp/datos/123solar/invt5/production/energy$YEAR".csv" ftp://fronius:fronius@163.117.157.189/invt2/production/energy$YEAR".csv" > /dev/null 2>&1

wput -u /home/u4477/fslurp/datos/123solar/invt3/production/energy$YEAR".csv" ftp://fronius:fronius@163.117.157.189/invt3/production/energy$YEAR".csv" > /dev/null 2>&1

wput -u /home/u4477/fslurp/datos/123solar/invt4/production/energy$YEAR".csv" ftp://fronius:fronius@163.117.157.189/invt4/production/energy$YEAR".csv" > /dev/null 2>&1

wput -u /home/u4477/fslurp/datos/123solar/invt5/production/energy$YEAR".csv" ftp://fronius:fronius@163.117.157.189/invt5/production/energy$YEAR".csv" > /dev/null 2>&1



