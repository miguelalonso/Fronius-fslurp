#!/bin/bash
FECHA=$(date +"%Y%m%d")
#echo $FECHA
HORA=$(date +"%H:%M:%S")
#echo $HORA

C=1
j=1
while [ $C -le 2 ]
do
j=$(( j+1 ))
#echo $C
lectura=`sudo python dt80serialread.py`
#echo $reportOutput
Delimiter=,
INICIO=`echo $lectura | cut -d$Delimiter -f1`
if echo $INICIO | grep -iq "D"
then
        C=3 #salimos del bucle, lectura buena
fi
done
echo $lectura
delimiter2=";"
CADENADT80a=`echo $lectura | cut -d$delimiter2 -f2`
CADENADT80=${CADENADT80a:4} #cadena sólo con los 40 canales de datos
for i in $(seq 1 40) #para los 40 canales del dt80
do
eval "campo$i=`echo $CADENADT80 | cut -d$Delimiter -f$i`";
eval y='$'campo$i
#echo $yecho $y
done
echo $lectura >> /home/u4477/fslurp/datos/123solar/$FECHA"_datataker.csv"
echo $lectura > /home/u4477/fslurp/datos/123solar/"ultimalectura_datataker.csv"
#echo $CADENADT80
