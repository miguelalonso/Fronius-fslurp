#!/usr/bin/python
import csv
import sys
import glob
import numpy as np
from datetime import datetime
import os

def get_column(array, col):
        result = []
        for row in array:
                result.append(row[col])
        return result



for numinv in range (1,6):
    E_dia=[]
    path = '/home/u4477/fslurp/datos/123solar/invt'+str(numinv)+'/*.csv'
    ano=datetime.today().year
    fichero_energia='/home/u4477/fslurp/datos/123solar/invt'+str(numinv)+'/production/energy'+str(ano$
    files=glob.glob(path)
    files=sorted(files)
    files=files[-5:]    #para tratar unicamente los ultimos 5 dias
    print ("Tratando fichero :"), path
    for file in files:
        try:
                    horas=[]
                    potencias=[]
                    energias=[]
                    energia_inversor=0
                    irradiancia=[]
                    E_dia_fila=[]
                    fecha_dia=os.path.splitext(os.path.basename(file))[0]
                    E_dia_fila.append(fecha_dia)
                    f = open(file, 'r')
                    reader = csv.reader(f, delimiter=',')
                    #read header
                    header = reader.next()
                    for row in reader:
                            try:
                                valor = float(row[15])
                                horas.append(row[0])
                                potencias.append(float(row[15]))
                                energia_inversor=float(row[27])
                                if len(row[26])== 0 or row[26]=='':
                                    irradiancia.append(0)
                                else:
                                    irradiancia.append(float(row[26]))
                            except Exception,e:
                                pass
                    f.close()
                    i=0
                    energia_dia=0
                    irrad_dia=0
                    #for value in horas:
                    for i in range(len(horas)-1):
                            t0=datetime.strptime(horas[i], '%H:%M:%S')
                            t1=datetime.strptime(horas[i+1], '%H:%M:%S')
                            d=t1-t0
                            #var2=d.seconds
                            energias=potencias[i]*d.seconds/3600.0
                            irrad=irradiancia[i]*d.seconds/3600.0
                            energia_dia=energia_dia+energias
                            irrad_dia=irrad_dia+irrad
                            #print i,  horas[i],potencias[i], energias, energia_dia
                            #i +=1
                    energia_dia=round(energia_dia/1000,2)
                    irrad_dia=irrad_dia/1000
                    print("Energia diaria: "), fecha_dia,(" : "), energia_dia
                    E_dia_fila.append(round(energia_dia,2))
                    E_dia_fila.append(energia_inversor)
                    E_dia_fila.append(round(irrad_dia,2))
                    E_dia.append(E_dia_fila)
        except Exception,e:
                print("Unexpected error")
                pass

    #print E_dia
    E_dia=sorted(E_dia,key=lambda x:x[0])
    #print E_dia
    #print ("Resultado: ")
    #print E_dia[0][0], E_dia[0][1],  E_dia[0][2] , E_dia[0][3]
    #print E_dia[1][0], E_dia[1][1],  E_dia[1][2] , E_dia[1][3]
    #imprime fecha,energia_calculada,energia_inversor,irradiacion diaria del canal 26

    #ahora lee todo el fichero de energia y lo anade a los valores calculados
    try:
        f = open(fichero_energia, 'r')
        reader = csv.reader(f, delimiter=',')
        #read header, los ficheros de energia no llevan cabecera (de momento)
        #header = reader.next()
        dias=get_column(E_dia,0)
        for row in reader:
                if row[0] not in dias:
                        E_dia.append(row)
        f.close
    except Exception,e:
        pass

    print dias
    print E_dia


    #los ordena para eliminar duplicados
    #E_dia=sorted(E_dia,key=lambda x:x[0])
    #E_dia=[list(t) for t in set(tuple(element) for element in E_dia)]
    E_dia=sorted(E_dia,key=lambda x:x[0])

    with open(fichero_energia, "wb") as f:
        writer = csv.writer(f)
        writer.writerows(E_dia)
