#! /bin/bash
AYER=$(date -d "1 day ago" '+%Y%m%d')
ANO=$(date +"%Y")
FECHA=$(date +"%Y%m%d")
#echo $FECHA
HORA=$(date +"%H:%M:%S")
#echo $HORA
#echo $AYER

DIR="/home/u4477/fslurp/datos/123solar/invt1"

#for INPUT in `ls /home/u4477/fslurp/datos/123solar/invt1/*.csv`
for INPUT in `ls $DIR*/*.csv`
do

#INPUT=$DIR"/"$FECHA".csv"
#FECHA=`echo $INPUT| cut -d'.' -f 1`
#FECHA=`echo $INPUT| cut -d'/' -f 2`
FECHA=`echo ${INPUT:(-12):(-4)}`

echo $FECHA

OLDIFS=$IFS
IFS=,
[ ! -f $INPUT ] && { echo "$INPUT file not found"; exit 99; }
while read  Time I1V I1A I1P I2V I2A I2P I3V I3A I3P I4V I4A I4P G1V G1A G1P G2V G2A G2P G3V G3A G3P FRQ EFF INVT BOOT SR KWHT
do
        #echo "Time : $Time"
        #echo "I1V : $I1V"
        #echo "I1A : $I1A"
        #echo "I1P : $I1P"
        #echo "KWHT : $KWHT"
        energia_diaria=$KWHT
done < $INPUT
IFS=$OLDIFS



echo "KWHT : $energia_diaria"
echo $FECHA, $energia_diaria >>$DIR"/production/energy"$ANO".csv2"

sort -u $DIR"/production/energy"$ANO.csv2 >  $DIR"/production/energy"$ANO.csv
done
rm $DIR"/production/energy"$ANO".csv2"
