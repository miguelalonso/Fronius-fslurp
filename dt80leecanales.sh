#!/bin/bash
C=1
j=1
while [ $C -le 2 ]
do
j=$(( j+1 ))
#echo $C
reportOutput=`sudo python dt80serialread.py`
#echo $reportOutput
Delimiter=,
INICIO=`echo $reportOutput | cut -d$Delimiter -f1`

if echo $INICIO | grep -iq "D"
then
       # echo "vamos bien"
        C=3
#else
       #echo "lectura incorrecta"
fi
done

echo "final bueno: "
echo $reportOutput

delimiter2=";"
CADENADT80a=`echo $reportOutput | cut -d$delimiter2 -f2`
CADENADT80=${CADENADT80a:4}

for i in $(seq 1 5)
do
eval "campo$i=`echo $reportOutput | cut -d$Delimiter -f$i`";
eval y='$'campo$i
echo $yecho $y
done

echo $CADENADT80
