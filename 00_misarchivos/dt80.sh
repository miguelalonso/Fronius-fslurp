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
#echo $j
lectura=`python /home/u4477/fslurp/dt80serialread.py`
#echo $reportOutput
Delimiter=,
INICIO=`echo $lectura | cut -d$Delimiter -f1`
#echo $lectura
if echo $INICIO | grep -iq "D"
then
        C=3 #salimos del bucle, lectura buena
fi
done
#echo $lectura
#delimiter2=";"
#CADENADT80a=`echo $lectura | cut -d$delimiter2 -f2`
#CADENADT80=${CADENADT80a:4} #cadena sólo con los 40 canales de datos
#for i in $(seq 1 40) #para los 40 canales del dt80
#do
#eval "campo$i=`echo $CADENADT80 | cut -d$Delimiter -f$i`";
#eval y='$'campo$i
#echo $yecho $y
#done
#ahora  leemos los datos de las estaciones meteo de emoncms_spectrum
url="http://163.117.157.189/emoncms_spectrum/feed/value.json?id="
apikey="&apikey=66999c52a47cec0a579ec0a81e98ba23"
id=1916

#datos del node 42
vaisala=""
cabvaisala=""
for i in `seq 1876 1886`
do
resultado=$(curl -s $url$i$apikey)
resultado=$(echo  $resultado | sed "s/\"//g")
vaisala=$vaisala","$resultado
cabvaisala=$cabvaisala","$i
done

#datos dle node 22
for i in `seq 1897 1903`
do
resultado=$(curl -s $url$i$apikey)
resultado=$(echo  $resultado | sed "s/\"//g")
vaisala=$vaisala","$resultado
cabvaisala=$cabvaisala","$i
done
# ahora del node 23
for i in `seq 1904 1918`
do
resultado=$(curl -s $url$i$apikey)
resultado=$(echo  $resultado | sed "s/\"//g")
vaisala=$vaisala","$resultado
cabvaisala=$cabvaisala","$i
done
#echo $vaisala
#echo  "cabecera"
#echo $cabvaisala

# FIN lectura de datos de estaciones

cabecera="canal_101V,canal_102V,canal_103V,canal_104V,canal_105V,canal_106V,canal_107V,canal_108V,canal_109V,cana$
#se añaden los datos de las estaciones
cabecera=$cabecera$cabvaisala
lectura=$lectura$vaisala

INPUT="/home/u4477/fslurp/datos/123solar/"$FECHA"_datataker.csv"
[ ! -f $INPUT ] && { echo $cabecera >> /home/u4477/fslurp/datos/123solar/$FECHA".csv"; }

echo $lectura >> /home/u4477/fslurp/datos/123solar/$FECHA"_datataker.csv"
echo $lectura > /home/u4477/fslurp/datos/123solar/"ultimalectura_datataker.csv"
echo $cabecera > /home/u4477/fslurp/datos/123solar/"cabecera_datataker.csv"

#echo $CADENADT80

# envia datos a emoncms

a="?json={"\
dt80_canal1:$campo1\
,dt80_canal2:$campo2\
,dt80_canal3:$campo3\
,dt80_canal4:$campo4\
,dt80_canal5:$campo5\
,dt80_canal6:$campo6\
,dt80_canal7:$campo7\
,dt80_canal8:$campo8\
,dt80_canal9:$campo9\
,dt80_canal10:$campo10\
,dt80_canal11:$campo11\
,dt80_canal12:$campo12\
,dt80_canal13:$campo13\
,dt80_canal14:$campo14\
,dt80_canal15:$campo15\
,dt80_canal16:$campo16\
,dt80_canal17:$campo17\
,dt80_canal18:$campo18\
,dt80_canal19:$campo19\
,dt80_canal20:$campo20\
"}&apikey="$apikey

$CURL $ServiceURL$a > /dev/null 2>&1

a="?json={"\
dt80_canal21:$campo21\
,dt80_canal22:$campo22\
,dt80_canal23:$campo23\
,dt80_canal24:$campo24\
,dt80_canal25:$campo25\
,dt80_canal26:$campo26\
,dt80_canal27:$campo27\
,dt80_canal28:$campo28\
,dt80_canal29:$campo29\
,dt80_canal30:$campo30\
,dt80_canal31:$campo31\
,dt80_canal32:$campo32\
,dt80_canal33:$campo33\
,dt80_canal34:$campo34\
,dt80_canal35:$campo35\
,dt80_canal36:$campo36\
,dt80_canal37:$campo37\
,dt80_canal38:$campo38\
,dt80_canal39:$campo39\
,dt80_canal40:$campo40\
"}&apikey="$apikey

$CURL $ServiceURL$a > /dev/null 2>&1

#echo "FIN"
