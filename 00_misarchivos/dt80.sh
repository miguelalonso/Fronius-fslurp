#!/bin/bash
FECHA=$(date +"%Y%m%d")
#echo $FECHA
HORA=$(date +"%H:%M:%S")
#echo $HORA

# Location of curl
CURL=/usr/bin/curl

ServiceURL=http://163.117.157.189/emoncms_spectrum/input/post.json
apikey=66999c52a47cec0a579ec0a81e98ba23

C=1
j=1
while [ $C -le 2 ]
do
j=$(( j+1 ))
#echo $j
lectura=`python /home/u4477/fslurp/dt80serialread.py`
#eliminamos los overflow 99999.9 por espacios en blanco
lectura=`echo "$lectura" | sed 's/99999.9//g'`

Delimiter=,
INICIO=`echo $lectura | cut -d$Delimiter -f1`
#echo $lectura
if echo $INICIO | grep -iq "D"
then
        C=3 #salimos del bucle, lectura buena
fi
done
#echo $lectura
delimiter2=";"
CADENADT80a=`echo $lectura | cut -d$delimiter2 -f2`
CADENADT80b=${CADENADT80a:4} #cadena sólo con los 40 canales de datos

for i in $(seq 1 40) #para los 40 canales del dt80
do
eval "campo$i=`echo $CADENADT80 | cut -d$Delimiter -f$i`";
eval y='$'campo$i
#nombre_canal$i=",DT80_canal"$i":"
#echo $yecho $y
done

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

cabecera="D,Nserie,Job,date,hour,msecond,OB,Cero,canal_101V,canal_102V,canal_103V,canal_104V,canal_105V,canal_106V,canal_107V,canal_108V,canal_109V,canal_110V,canal_111V,canal_112V,canal_113V,canal_114V,canal_115V,canal_116V,canal_117V,canal_118V,canal_119V,canal_120V,Canal_201C,canal_202C,canal_203C,canal_204C,canal_205C,canal_206C,canal_207C,canal_208C,canal_209C,canal_210C,canal_211C,canal_212C,canal_213C,canal_214C,canal_215C,canal_216C,canal_217C,canal_218C,canal_219C,canal_220C"
#se añaden los datos de las estaciones
lectura=$(echo  $lectura | sed "s/\r//g")

cabecera=$cabecera$cabvaisala
lectura=$lectura$vaisala

INPUT="/home/u4477/fslurp/datos/123solar/"$FECHA"_datataker.csv"
[ ! -f $INPUT ] && { echo $cabecera >> /home/u4477/fslurp/datos/123solar/$FECHA"_datataker.csv"; }

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
