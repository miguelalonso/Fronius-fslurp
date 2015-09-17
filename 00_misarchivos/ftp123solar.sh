#!/bin/sh
#
#
# Sample script to send files from fslurp to 123solar server site (in leganes)
#
# Recordar instalar en ubuntu wput
# sudo apt-get install wput

# tipico comando de solicitud de datos:
# ./fslurp -b 19200 -r all -d , -p /dev/ttyS1
#
#Se hace un cron: sudo crontab -e
#* * * * * /home/u4477/fslurp/fslurp2emoncms.sh
#*/5 * * * * /home/u4477/fslurp/fslurp2dataciemat.sh
# el segundo envía un fichero con fecha completa.

FECHA=$(date +"%Y%m%d")
#echo $FECHA
HORA=$(date +"%H:%M:%S")
#echo $HORA

wput -u /home/u4477/fslurp/datos/123solar/invt5/$FECHA".csv" ftp://fronius:fronius@163.117.157.189/invt5/csv/$FECHA".csv" > /dev/null 2>&1

wput -u /home/u4477/fslurp/datos/123solar/invt4/$FECHA".csv" ftp://fronius:fronius@163.117.157.189/invt4/csv/$FECHA".csv" > /dev/null 2>&1

wput -u /home/u4477/fslurp/datos/123solar/invt3/$FECHA".csv" ftp://fronius:fronius@163.117.157.189/invt3/csv/$FECHA".csv" > /dev/null 2>&1

wput -u /home/u4477/fslurp/datos/123solar/invt2/$FECHA".csv" ftp://fronius:fronius@163.117.157.189/invt2/csv/$FECHA".csv" > /dev/null 2>&1

wput -u /home/u4477/fslurp/datos/123solar/invt1/$FECHA".csv" ftp://fronius:fronius@163.117.157.189/invt1/csv/$FECHA".csv" > /dev/null 2>&1
