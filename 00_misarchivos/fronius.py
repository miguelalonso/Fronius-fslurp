#!/usr/bin/python
import csv
import sys
import glob
import numpy as np
from datetime import datetime
import os
import subprocess
import shlex

path = '/home/u4477/prueba/'
file='/home/u4477/prueba/reportOutput.csv'
filedt80="/home/u4477/fslurp/datos/123solar/ultimalectura_datataker.csv"
cmd="/home/u4477/fslurp/fslurp -b 19200 -r all -d , -p /dev/ttyS0"

cabecera=["Time","I1V","I1A","I1P","I2V","I2A","I2P","I3V","I3A","I3P","I4V","I4A","I4P",
"G1V","G1A","G1P","G2V","G2A","G2P","G3V","G3A","G3P","FRQ","EFF","INVT","BOOT","SR,KWHT",
"MAXIMUM_POWER_DAY","MAXIMUM_AC_VOLTAGE_DAY","MINIMUM_AC_VOLTAGE_DAY","MAXIMUM_DC_VOLTAGE_DAY",
"OPERATING_HOURS_DAY","ENERGY_TOTAL","MAXIMUM_POWER_TOTAL","MAXIMUM_AC_VOLTAGE_TOTAL",
"MINIMUM_AC_VOLTAGE_TOTAL","MAXIMUM_DC_VOLTAGE_TOTAL","OPERATING_HOURS_TOTAL","INV","G_este","G_sur","G_Oeste",
"T_este,T_sur","T_Oeste","Ta","Gh","G30"]

d=datetime.now()
fecha=d.strftime("%Y%m%d")
hora=d.strftime("%H:%m:%S")

#escribe las cabeceras de los ficheros
path_i="/home/u4477/fslurp/datos/123solar/"
for numinv in range (1,6):
        j=str(numinv)
        path=path_i+"invt"+j+"/csv/fv42_invt"+j+"_"+str(fecha)+".csv"
        if not os.path.isfile(path):
                with open(path, "wb") as f:
                        writer = csv.writer(f)
                        writer.writerow(cabecera)


def run_command(command):
    salida=''
    process = subprocess.Popen(shlex.split(command), stdout=subprocess.PIPE)
    while True:
        output = process.stdout.readline()
        if output == '' and process.poll() is not None:
            break
        if output:
            #print output.strip()
            salida+=output.strip()
    rc = process.poll()
    return salida

try:
#borrar leer del fichero (solo para pruebas)
        f = open (file , 'r' )
        reader = csv.reader(f,delimiter=',')
        rc=next(reader)
        #print report;
        #print len(report)
        #for j in range(0,len(report)):
        #       valor=report[j]
        #       print valor
        #f.close()

#fin lectura del fichero (solo pruebas). activar leer de cmd

#       rc=run_command(cmd)
        #print rc
        report=rc
        #report=rc.split(',')

        f= open (filedt80 , 'r' )
        reader = csv.reader(f,delimiter=',')
        dt80=next(reader)
        #print dt80
        for j in range(0,len(dt80)):
                valor=dt80[j]
                #print j,': ',valor
        f.close()

        G_este= str(float(dt80[8])*1000 / 54.87)
        G_sur=str(float(dt80[12])*1000 / 54.20)
        G_oeste=str(float(dt80[10])*1000 / 53.91)
        Gh=str(float(dt80[9])*1000 / 53.91)
        G30= str(float(dt80[11])*1000 / 54.72)
        T_este= str(float(dt80[44]))
        T_sur= str(float(dt80[44]))
        T_oeste= str(float(dt80[44]))
        Ta= str(float(dt80[44]))

        Date=report[0]
        INV=report[1]
        POWER=report[4]
        AC_CURRENT=report[5]
        AC_VOLTAGE=report[6]
        AC_FREQUENCY=report[7]
        DC_CURRENT=report[8]
        DC_VOLTAGE=report[9]

        ENERGY_DAY=report[10]
        MAXIMUM_POWER_DAY=report[11]
        MAXIMUM_AC_VOLTAGE_DAY=report[12]
        MINIMUM_AC_VOLTAGE_DAY=report[13]
        MAXIMUM_DC_VOLTAGE_DAY=report[15]
        OPERATING_HOURS_DAY=report[15]

        ENERGY_TOTAL=report[16]
        MAXIMUM_POWER_TOTAL=report[17]
        MAXIMUM_AC_VOLTAGE_TOTAL=report[18]
        MINIMUM_AC_VOLTAGE_TOTAL=report[19]
        MAXIMUM_DC_VOLTAGE_TOTAL=report[20]
        OPERATING_HOURS_TOTAL=report[21]

        if float(INV) < 6 and float(INV) > 0 and float(POWER) > -87:
                DC_POWER=float(DC_CURRENT)*float(DC_VOLTAGE)
                EFF=float(POWER)/DC_POWER
                DC_POWER=str(DC_POWER)
                EFF=str(EFF)

                T_inversor=str(T_este)
                G_inversor=str(G_este)
                v=''

                cadena=[hora,DC_VOLTAGE,DC_CURRENT,DC_POWER,v,v,v,v,v,v,v,v,v,
                AC_VOLTAGE,AC_CURRENT,POWER,v,v,v,v,v,v,AC_FREQUENCY,EFF,T_inversor,Ta,G_inversor,ENERGY_DAY,
                MAXIMUM_POWER_DAY,MAXIMUM_AC_VOLTAGE_DAY,MINIMUM_AC_VOLTAGE_DAY,MAXIMUM_DC_VOLTAGE_DAY,
                OPERATING_HOURS_DAY,ENERGY_TOTAL,MAXIMUM_POWER_TOTAL,MAXIMUM_AC_VOLTAGE_TOTAL,
                MINIMUM_AC_VOLTAGE_TOTAL,MAXIMUM_DC_VOLTAGE_TOTAL,OPERATING_HOURS_TOTAL,INV,
                G_este,G_sur,G_oeste,T_este,T_sur,T_oeste,Ta,Gh,G30]
                j=INV
                path=path_i+"invt"+j+"/csv/fv42_invt"+j+"_"+str(fecha)+".csv"
                with open(path, "ab") as f:
                        writer = csv.writer(f)
                        writer.writerow(cadena)


        print Date, INV, POWER, AC_CURRENT, 'year', fecha
        #print report


except  Exception,e:
        print("Unexpected error")
        pass
