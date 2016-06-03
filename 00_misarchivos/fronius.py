#!/usr/bin/python
# -*- coding: utf-8 -*-
demo=False
import csv
import sys
import glob
#import numpy as np
from datetime import datetime
import os
import subprocess
import shlex
from time import gmtime,strftime
import sys, string
import httplib
import time
import json
import serial, time
import urllib, urllib2
import socket
import ftplib
import pysftp #paramiko debe estar instalado
import math, ephem
import requests

o=ephem.Observer()
o.lat, o.long = '40:27:19.8', '-3:43:47.8'


#initialization and open the port
#possible timeout values:
#    1. None: wait forever, block call
#    2. 0: non-blocking mode, return immediately
#    3. x, x is bigger than 0, float allowed, timeout block call
ser = serial.Serial()
ser.port = "/dev/ttyUSB0"
#ser.port = "/dev/ttyS2"
ser.baudrate = 57600
ser.bytesize = serial.EIGHTBITS #number of bits per bytes
ser.parity = serial.PARITY_NONE #set parity check: no parity
ser.stopbits = serial.STOPBITS_ONE #number of stop bits
#ser.timeout = None          #block read
ser.timeout = 3             #non-block read
#ser.timeout = 2              #timeout block read
ser.xonxoff = False     #disable software flow control
ser.rtscts = False     #disable hardware (RTS/CTS) flow control
ser.dsrdtr = False       #disable hardware (DSR/DTR) flow control
ser.writeTimeout = 2     #timeout for write


domain = "163.117.157.189"
emoncmspath = "emoncms_spectrum"
apikey ="66999c52a47cec0a579ec0a81e98ba23"
nodeid = 0

emoncmspath_read = "163.117.157.189/emoncms_spectrum"
apikey_read ="66999c52a47cec0a579ec0a81e98ba23"


path = '/home/u4477/fronius/'
file='/home/u4477/fronius/reportOutput.csv'
filedt80="/home/u4477/fronius/data/datataker/ultimalectura_datataker.csv"
cmd="/home/u4477/fronius/fslurp_fv42 -b 19200 -r all -d , -p /dev/ttyS0"

cabecera=["Time","I1V","I1A","I1P","I2V","I2A","I2P","I3V","I3A","I3P","I4V","I4A","I4P",
"G1V","G1A","G1P","G2V","G2A","G2P","G3V","G3A","G3P","FRQ","EFF","INVT","BOOT","SR","KWHT",
"MAXIMUM_POWER_DAY","MAXIMUM_AC_VOLTAGE_DAY","MINIMUM_AC_VOLTAGE_DAY","MAXIMUM_DC_VOLTAGE_DAY",
"OPERATING_HOURS_DAY","ENERGY_TOTAL","MAXIMUM_POWER_TOTAL","MAXIMUM_AC_VOLTAGE_TOTAL",
"MINIMUM_AC_VOLTAGE_TOTAL","MAXIMUM_DC_VOLTAGE_TOTAL","OPERATING_HOURS_TOTAL","INV","G_este","G_sur","G_Oeste",
"T_este","T_sur","T_Oeste","Ta","Gh","G30"]

cabecera_dt80=['Fecha','Hora','D','Nserie','Job','date','hour','msecond','O','B','Cero',
'canal_101V','canal_102V','canal_103V','canal_104V','canal_105V',
'canal_106V','canal_107V','canal_108V','canal_109V','canal_110V',
'canal_111V','canal_112V','canal_113V','canal_114V','canal_115V',
'canal_116V','canal_117V','canal_118V','canal_119V','canal_120V',
'Canal_201C','canal_202C','canal_203C','canal_204C','canal_205C',
'canal_206C','canal_207C','canal_208C','canal_209C','canal_210C',
'canal_211C','canal_212C','canal_213C','canal_214C','canal_215C',
'canal_216C','canal_217C','canal_218C','canal_219C','canal_220C','chk','chk',
'1876','1877','1878','1879','1880','1881','1882','1883','1884','1885','1886',
'1897','1898','1899','1900','1901','1902','1903',
'1904','1905','1906','1907','1908','1909','1910','1911','1912','1913',
'1914','1915','1916','1917','1918']

cabecera_energia=['Fecha','E_dia_integrada','E_dia_inversor','G_dia','G_este','G_sur','G_oeste','Gh','G30']

print 'ok'

d=datetime.now()
fecha=d.strftime("%Y%m%d")
hora=d.strftime("%H:%M:%S")
httpServ = httplib.HTTPConnection(domain)
path_i="/home/u4477/fronius/data/"

def cabecera_energy():
      #escribe las cabeceras de los ficheros
      path=path_i+"datataker/"+str(fecha)+"_datataker.csv"
      if not os.path.isfile(path):
            with open(path, "wb") as f:
                  writer = csv.writer(f)
                  writer.writerow(cabecera_dt80)
      for numinv in range (1,6):
            ano=datetime.today().year
            fichero_energia='/home/u4477/fronius/data/invt'+str(numinv)+'/production/fv42_invt'+str(numinv)+'_energy'+str(ano)+'.csv'
            if not os.path.isfile(fichero_energia):
                  with open(fichero_energia, "wb") as f:
                        writer = csv.writer(f)
                        writer.writerow(cabecera_energia)
      #print 'cabecerasenergy OK'      


def cabecera_day():
      #escribe las cabeceras de los ficheros
      
      for numinv in range (1,6):
            j=str(numinv)
            path=path_i+"invt"+j+"/csv/fv42_invt"+j+"_"+str(fecha)+".csv"
            if not os.path.isfile(path):
                  with open(path, "wb") as f:
                        writer = csv.writer(f)
                        writer.writerow(cabecera)
      #print 'cabecerasday  OK'



def sendtoemoncmscsv(csv):
      try:
      	httpServ.connect()
      	time.sleep(10)
      	# Send to emoncms
      	httpServ.request("GET", "/"+emoncmspath+"/input/post.json?apikey="+apikey+"&node="+str(nodeid)+"&csv="+csv)
      	print("GET", "/"+emoncmspath+"/input/post.json?apikey="+apikey+"&node="+str(nodeid)+"&csv="+csv)
      	response = httpServ.getresponse()
      	print response.read()
      	##    time.sleep(10)
      	#http://localhost/emoncms/input/post.json?node=1&json={power:200}
      	httpServ.close()
      except Exception,e:
        print("Error sendtomencms csv")
        pass


def sendtoemoncms(nodeid,name,data):
      try:
      	nodeid=str(nodeid)
      	data=str(data)
      	name=str(name)
      	httpServ.connect()
      	time.sleep(0.001)
      	# Send to emoncms
      	httpServ.request("GET", "/"+emoncmspath+"/input/post.json?apikey="+apikey+"&node="+nodeid+"&json={"+name+":"+data+"}")
      	#print("GET", "GET", "/"+emoncmspath+"/input/post.json?apikey="+apikey+"&node="+nodeid+"&json={"+name+":"+data+"}")
      	response = httpServ.getresponse()
      	#print response.read()
	##    time.sleep(10)
      	httpServ.close()
      except Exception,e:
        print("Error send to emoncms")
        pass

def readfromemoncms(nodeid,feedid):
      try:
	data=[]
      	nodeid=str(nodeid)
      	feedid=str(feedid)
      	try:
		data_url = "http://"+emoncmspath_read+"/feed/value.json?apikey=" + apikey_read +"&node="+nodeid+"&id=" + feedid
		sock = urllib2.urlopen(data_url)
	except  URLError,HTTPError:
		print ("error url")
		pass
	else:
      		data_str = sock.read()
      		data = json.loads(data_str)
      		sock.close
      	
      except Exception,e:
	print("Error read from emoncms")
        pass
      return data

def is_website_online(host):
    """ This function checks to see if a host name has a DNS entry by checking
        for socket info. If the website gets something in return, 
        we know it's available to DNS.
    """
    try:
        socket.gethostbyname(host)
    except socket.gaierror:
        return False
    else:
        return True



def leer_vaisala_davis():
      try:
	print "inicio lectura vaisala_davis"
      	data=[]
      	for i in range (1876,1887):
            nodeid='42'
            data.append(readfromemoncms(nodeid,i))
            #datos del node 42
      	for i in range (1897,1904):
            nodeid='22'
            data.append(readfromemoncms(nodeid,i))
            #datos del node 22
      	for i in range (1904,1919):
            nodeid='23'
            data.append(readfromemoncms(nodeid,i))
            #datos del node 23
        print 'dat emoncms OK :'
	return data

      except Exception,e:
	print ("error 234")
	pass


def run_command(command):
      salida=''
      process = subprocess.Popen(shlex.split(command), stdout=subprocess.PIPE)
      while True:
            output = process.stdout.readline()
            if output == '' and process.poll() is not None:
                  break
            if output:
                  print output.strip()
                  salida+=output.strip()
            rc = process.poll()
      return salida

def save_dt80(data):
      path=path_i+"datataker/"+str(fecha)+"_datataker.csv"
      path1=path_i+"datataker/ultimalectura_datataker.csv"
      with open(path, "ab") as f:
            writer = csv.writer(f)
            writer.writerow(data)
      with open(path1, "wb") as f:
            writer = csv.writer(f)
            writer.writerow(data)

def save_fronius(data):
      path1=path_i+"ultimalectura_fronius.csv"
      with open(path1, "wb") as f:
            f.write(data)

def fronius1():
      try:
	data=[]
      	data_url = "http://"+emoncmspath_read
      	a = urllib.urlopen(data_url)	
      	if a.getcode()==200:
        	data=leer_vaisala_davis()
        	print "reading from emoncms OK:"
        	print data
        	return data
      	else:
        	print ("error reading from emoncms_spectrum")
      except Execption, e:
	 pass


def fronius():

      print 'leyendo dt80'
      dt80=lee_dt80()
      if len(dt80) < 10:
	return None

      print 'leer vaisala_davis:'
      data=[]
      try:
      	data=fronius1()
      except Exception,e:
	print "error emoncms 456"
      if len(data)>10:
        dt80=dt80+data
      print 'dt80 : ', dt80
      save_dt80(dt80)
      print "Datos dt80 guardados OK"


      print 'leyendo fronius:'	
      report=run_command(cmd)
      report=report.replace('overflow','0')
      print report
      save_fronius(report)
      print 'anterior:'
      if len(report) < 200:
        return None
      report=report.split(',')
      print 'Lectura Inversores:'
      print report
      G_este= str(round(float(dt80[11])*1000 / 54.87,2))
      G_sur=str(round(float(dt80[12])*1000 / 54.20,2))
      G_oeste=str(round(float(dt80[13])*1000 / 53.91,2))
      G30=str(round(float(dt80[14])*1000 / 53.91,2))
      Gh= str(round(float(dt80[15])*1000 / 54.72,2))
      #orden verificado 26nov15
      T_este= str(float(dt80[45]))
      T_sur= str(float(dt80[34]))
      T_oeste= str(float(dt80[35]))
      Ta= str(float(dt80[47]))
      if len(dt80) < 50:
	G_este= str(0)
      	G_sur=str(0)
      	G_oeste=str(0)
      	Gh=str(0)
      	G30= str(0)
      	T_este= str(0)
      	T_sur= str(0)
      	T_oeste= str(0)
      	Ta= str(0)
      print 'Inicio tratamiento:'
      for k in range (0,5):
            Date=report[0+k*28]
            INV=report[1+k*28]
	    STATUS=report[2+k*28]
	    TYPE=report[3+k*28]

            POWER=report[4+k*28]
            AC_CURRENT=report[5+k*28]
            AC_VOLTAGE=report[6+k*28]
            AC_FREQUENCY=report[7+k*28]
            DC_CURRENT=report[8+k*28]
            DC_VOLTAGE=report[9+k*28]

            AC_CURRENT1=report[10+k*28]
            AC_CURRENT2=report[11+k*28]
            AC_CURRENT3=report[12+k*28]
            AC_VOLTAGE1=report[13+k*28]
            AC_VOLTAGE2=report[14+k*28]
            AC_VOLTAGE3=report[15+k*28]
	    
            ENERGY_DAY=report[16+k*28]
            MAXIMUM_POWER_DAY=report[17+k*28]
            MAXIMUM_AC_VOLTAGE_DAY=report[18+k*28]
            MINIMUM_AC_VOLTAGE_DAY=report[19+k*28]
            MAXIMUM_DC_VOLTAGE_DAY=report[20+k*28]
            OPERATING_HOURS_DAY=report[21+k*28]

            ENERGY_TOTAL=report[22+k*28]
            MAXIMUM_POWER_TOTAL=report[23+k*28]
            MAXIMUM_AC_VOLTAGE_TOTAL=report[24+k*28]
            MINIMUM_AC_VOLTAGE_TOTAL=report[25+k*28]
            MAXIMUM_DC_VOLTAGE_TOTAL=report[26+k*28]
            OPERATING_HOURS_TOTAL=report[27+k*28]
	    #if float(POWER) > 9000 or STATUS !='0xB0':
	    
	    if float(POWER) > 9000 :
		  print "Error en la lectura del inversor", STATUS
		  return None

            if float(INV) < 6 and float(INV) > 0 and float(POWER) > -87 and float(DC_CURRENT) < 50:
                  DC_POWER=round(float(DC_CURRENT)*float(DC_VOLTAGE),2)
		  if DC_POWER >0:
                  	EFF=round(float(POWER)/DC_POWER,3)
		  else:
			EFF=0
                  DC_POWER=str(DC_POWER)
		  EFF=str(EFF)
                  temp=[T_sur,T_oeste,T_este,T_este,T_este]
		  irrad=[G_sur, G_oeste,G_este,G_este,G_este]

		  T_inversor=str(temp[int(INV)-1])
                  G_inversor=str(irrad[int(INV)-1])
                  v=''
# modificado 3 de Junio 2016 para leer tensiones y corrientes trifásicas en I4V,I4A, G2V,G2I,G3V,G3I
# se mantiene tensión y corriente AC en G1V,G1A por coherencia con los monofásicos y con los ficheros anteriores a esta fecha
                  cadena=[hora,DC_VOLTAGE,DC_CURRENT,DC_POWER,v,v,v,v,v,v,AC_VOLTAGE1,AC_CURRENT1,v,
		  AC_VOLTAGE,AC_CURRENT,POWER,AC_VOLTAGE2,AC_CURRENT2,v,AC_VOLTAGE3,AC_CURRENT3,v,
		  AC_FREQUENCY,EFF,T_inversor,Ta,G_inversor,ENERGY_DAY,
                  MAXIMUM_POWER_DAY,MAXIMUM_AC_VOLTAGE_DAY,MINIMUM_AC_VOLTAGE_DAY,MAXIMUM_DC_VOLTAGE_DAY,
                  OPERATING_HOURS_DAY,ENERGY_TOTAL,MAXIMUM_POWER_TOTAL,MAXIMUM_AC_VOLTAGE_TOTAL,
                  MINIMUM_AC_VOLTAGE_TOTAL,MAXIMUM_DC_VOLTAGE_TOTAL,OPERATING_HOURS_TOTAL,INV,
                  G_este,G_sur,G_oeste,T_este,T_sur,T_oeste,Ta,Gh,G30]
                  j=INV
                  print j
                  path=path_i+"invt"+j+"/csv/fv42_invt"+j+"_"+str(fecha)+".csv"
		  cabecera_day()
                  with open(path, "ab") as f:
                        writer = csv.writer(f)
                        writer.writerow(cadena)
                  #print "inversor : ",INV
                  for j in range(0,len(cadena)):
                        print "enviando a emoncms: ",INV,cabecera[j],cadena[j]
                        sendtoemoncms('0',cabecera[j]+'_'+INV,cadena[j])

#      print 'leer vaisala_davis:'
#      data=[]
#      try:
#      	data=fronius1()
#      except Exception,e:
#	print "error emoncms 456"
#      if len(data)>10:
#        dt80=dt80+data
#      print 'dt80 : ', dt80
#      save_dt80(dt80)
#      print "Datos dt80 guardados OK"




# iniciar dt80 serial read
def lee_dt80():
      response='D,089015,"UNTITLED",2015/11/20,07:55:09,0.150878,0;B,0,-0.175125,-0.195648,-0.368004,-0.250328,-0.223152,16.967686,2222.8226,99999.9,99999.9,99999.9,99999.9,99999.9,99999.9,99999.9,99999.9,99999.9,99999.9,99999.9,99999.9,99999.9,99999.9,99999.9,20.94786,19.040102,-9.362218,15.879416,0.044709,-24.373144,-22.2641,1.130739,-2.359496,1.98168,2.066939,1.022367,0.705967,11.329816,5.764506,99999.9,99999.9,99999.9;0410;AFFB'
      response=response.replace('99999.9','')
      try:
            ser.open()
      except Exception, e:
            print "Error al abrir el puerto: " + str(e)
            pass
      if ser.isOpen():
            try:
                  ser.flushInput() #flush input buffer, discarding all its contents
                  ser.flushOutput()#flush output buffer, aborting current output
                  #and discard all that is in buffer
                  #write data
                  ser.write("XB\r\n")
                  #print("write data: XB")
                  time.sleep(2)  #give the serial port sometime to receive the data
                  numOfLines = 0
                  while True:
                        response = ser.readline()
                        #print("read data: " + response)
                        #print (response)
                        numOfLines = numOfLines + 1
                        if (numOfLines >= 1):
                              break
                  ser.close()
                  response=response.replace('99999.9','0')
                  response=response.replace('\r\n','')
		  response=response.replace(';',',')
            except Exception, e1:
              print "error communicating...: " + str(e1)
              pass
      else:
          print "Error  en puerto serie DT80 "
      response=fecha+','+hora+','+response
      r=response.split(',')
      print 'retorno:'
      print r
      return r


# fin de serial read dt80
#ENEGIA
def get_column(array, col):
        result = []
        for row in array:
                result.append(row[col])
        return result

def energy():
      try:
            #calcular energia
            for numinv in range (1,6):
		E_dia=[]
                path = '/home/u4477/fronius/data/invt'+str(numinv)+'/csv/*.csv'
                ano=datetime.today().year
                fichero_energia='/home/u4477/fronius/data/invt'+str(numinv)+'/production/fv42_invt'+str(numinv)+'_energy'+str(ano)+'.csv'
                files=glob.glob(path)
                files=sorted(files)
                files=files[-5:]    #para tratar unicamente los ultimos 5 dias
                print ("Tratando fichero :"), fichero_energia
                for file in files:
                    try:
                                horas=[]
                                potencias=[]
                                energias=[]
                                energia_inversor=0
                                irradiancia=[]
				Geste=[]
				Gsur=[]
				Goeste=[]
				Gh=[]
				G30=[]
                                E_dia_fila=[]
                                fecha_dia=os.path.splitext(os.path.basename(file))[0]
                                var=fecha_dia.split('_')
				fecha_dia=var[2]
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
						Geste.append(0)
						Gsur.append(0)
						Goeste.append(0)
						Gh.append(0)
						G30.append(0)
                                            else:
                                                irradiancia.append(float(row[26]))
						Geste.append(float(row[40]))
						Gsur.append(float(row[41]))
						Goeste.append(float(row[42]))
						Gh.append(float(row[47]))
						G30.append(float(row[48]))
                                        except Exception,e:
                                            pass
                                f.close()
                                i=0
                                energia_dia=0
                                irrad_dia=0
				Geste_dia=0
				Gsur_dia=0
				Goeste_dia=0
				Gh_dia=0
				G30_dia=0
                                #for value in horas:
                                for i in range(len(horas)-1):
                                        t0=datetime.strptime(horas[i], '%H:%M:%S')
                                        t1=datetime.strptime(horas[i+1], '%H:%M:%S')
					if t1>t0:
                                        	d=t1-t0
					else:
						d=t0-t1
                                        #var2=d.seconds
                                        energias=potencias[i]*d.seconds/3600.0
                                        irrad=irradiancia[i]*d.seconds/3600.0
					G1=Geste[i]*d.seconds/3600.0
					G2=Gsur[i]*d.seconds/3600.0
					G3=Goeste[i]*d.seconds/3600.0
					G4=Gh[i]*d.seconds/3600.0
					G5=G30[i]*d.seconds/3600.0
                                        energia_dia=energia_dia+energias
                                        irrad_dia=irrad_dia+irrad
					Geste_dia=Geste_dia+G1
					Gsur_dia=Gsur_dia+G2
					Goeste_dia=Goeste_dia+G3
					Gh_dia=Gh_dia+G4
					G30_dia=G30_dia+G5
                                        #print 'hhh67',i,  horas[i],potencias[i], energias, energia_dia
                                        #i +=1
                                energia_dia=round(energia_dia/1000,2)
                                irrad_dia=irrad_dia/1000
				Geste_dia=Geste_dia/1000
				Gsur_dia=Gsur_dia/1000
				Goeste_dia=Goeste_dia/1000
				Gh_dia=Gh_dia/1000
				G30_dia=G30_dia/1000
                                #print("Energia diaria: "), fecha_dia,(" : "), energia_dia
                                E_dia_fila.append(round(energia_dia,2))
                                E_dia_fila.append(energia_inversor)
                                E_dia_fila.append(round(irrad_dia,2))
				E_dia_fila.append(round(Geste_dia,2))
				E_dia_fila.append(round(Gsur_dia,2))
				E_dia_fila.append(round(Goeste_dia,2))
				E_dia_fila.append(round(Gh_dia,2))
				E_dia_fila.append(round(G30_dia,2))
                                E_dia.append(E_dia_fila)
                    except Exception,e:
                            print("Error ficheros energía 33")
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
                    #leer la cabecera
                    header = reader.next()
                    dias=get_column(E_dia,0)
                    for row in reader:
                            if row[0] not in dias:
                                    E_dia.append(row)
                    f.close
                except Exception,e:
                    pass

                #print dias
                #print E_dia


                #los ordena para eliminar duplicados
                #E_dia=sorted(E_dia,key=lambda x:x[0])
                #E_dia=[list(t) for t in set(tuple(element) for element in E_dia)]
                E_dia=sorted(E_dia,key=lambda x:x[0])

                with open(fichero_energia, "wb") as f:
                    writer = csv.writer(f)
		    writer.writerow(cabecera_energia)
                    writer.writerows(E_dia)

      except Exception:
            print("Error 1 ficheros energía")
            pass
      
# fin de ENERGIA
#ftp

def ftp_fronius():
      try:
      	server = '163.117.157.189'
      	username = 'fronius'
      	password = 'fronius'
      	path='/home/u4477/fronius/data/'
      	ftp_connection = ftplib.FTP(server, username, password)
      	d=datetime.today()
            
      	if d.hour > 6 and d.hour < 23:
            for k in range(1,6):
                  flocal=path+'invt'+str(k)+'/csv/fv42_invt'+str(k)+'_'+fecha+'.csv'
                  if os.path.isfile(flocal):
		  	fremoto='/invt'+str(k)+'/csv'
			print 'ftp inversor: ', k , ': ',flocal
                  	ftp_connection.cwd(fremoto)
                  	fh = open(flocal, 'rb')
                  	nombre='fv42_invt'+str(k)+'_'+fecha+'.csv'
                  	ftp_connection.storbinary('STOR '+nombre, fh)
                  	fh.close()


        #ahora con los ficheros del datataker
      	fdt80=path+'datataker/'+str(fecha)+'_datataker.csv'
      	remote_path = "/datataker"
      	ftp_connection.cwd(remote_path)
      	fh = open(fdt80, 'rb')
      	nombre=str(fecha)+'_datataker.csv'
      	ftp_connection.storbinary('STOR '+nombre, fh)
      	fh.close()

      	fdt80=path+'datataker/ultimalectura_datataker.csv'

      	ftp_connection.cwd(remote_path)
      	fh = open(fdt80, 'rb')
      	nombre='ultimalectura_datataker.csv'
      	ftp_connection.storbinary('STOR '+nombre, fh)
      	fh.close()

	fdt80=path+'ultimalectura_fronius.csv'
	remote_path = "/"

        ftp_connection.cwd(remote_path)
        fh = open(fdt80, 'rb')
        nombre='ultimalectura_fronius.csv'
        ftp_connection.storbinary('STOR '+nombre, fh)
        fh.close()


      	ftp_connection.quit()
      	#print "transferido"+fich1
      except Exception:
       	print("Error 3 en ftp ficheros")
       	pass


def ftp_fronius_cmt2656():
      try:
        server = '172.17.121.133'
	#ordenador cmt2656-ubuntu
        username = 'fronius'
        password = 'fronius'
        path='/home/u4477/fronius/data/'
        d=datetime.today()

        if d.hour > 6 and d.hour < 23:
	    with pysftp.Connection(server, username=username, password=password) as sftp:
	        for k in range(1,6):
 	        	flocal=path+'invt'+str(k)+'/csv/fv42_invt'+str(k)+'_'+fecha+'.csv'
	                if os.path.isfile(flocal):
                        	fremoto='/var/www/fv42/data/invt'+str(k)+'/csv/'
                        	print 'ftp inversor: ', k , ': ',flocal
                        	nombre='fv42_invt'+str(k)+'_'+fecha+'.csv'
				fremoto=fremoto+nombre
                        	sftp.put(flocal,fremoto)


      #ahora con los ficheros del datataker
	with pysftp.Connection(server, username=username, password=password) as sftp:
	        fdt80=path+'datataker/'+str(fecha)+'_datataker.csv'
        	remote_path = "/var/www/fv42/data/datataker/"
        	nombre=str(fecha)+'_datataker.csv'
		fremoto=remote_path+nombre
		sftp.put(fdt80,fremoto)

	        fdt80=path+'datataker/ultimalectura_datataker.csv'
	        nombre='ultimalectura_datataker.csv'
        	fremoto=remote_path+nombre
		sftp.put(fdt80,fremoto)

		fdt80=path+'ultimalectura_fronius.csv'
                remote_path = "/var/www/fv42/data/"
                nombre='ultimalectura_fronius.csv'
                fremoto=remote_path+nombre
                sftp.put(fdt80,fremoto)

        #print "transferido"+fich1
      except Exception:
        print("Error 3 en sftp ficheros")
        pass



def ftp_energy():
      try:
            server = '163.117.157.189'
            username = 'fronius'
            password = 'fronius'
            d=datetime.today()
            y=d.strftime("%Y")
            if d.hour > 6 and d.hour < 23:
                  for k in range (1,6):
                        fichero_energia='/home/u4477/fronius/data/invt'+str(k)+'/production/'
                        nombre='fv42_invt'+str(k)+'_energy'+str(y)+'.csv'
                        fichero_energia=fichero_energia+nombre
                        print fichero_energia
			remote_path="/invt"+str(k)+"/production"
                        ftp_connection = ftplib.FTP(server, username, password)
                        ftp_connection.cwd(remote_path)
                        fh = open(fichero_energia, 'rb')
                        ftp_connection.storbinary('STOR '+nombre, fh)
                        fh.close()

                  ftp_connection.quit()
      except Exception:
            print("Error 2 en ftp ficheros energía")
            pass

def ftp_energy_cmt2656():
      try:
            server = '172.17.121.133'
	    #ordenador cmt2656-ubuntu
            username = 'fronius'
            password = 'fronius'
            d=datetime.today()
            y=d.strftime("%Y")
	    print d.hour
            if d.hour > 6 and d.hour < 23:
		with pysftp.Connection(server, username=username, password=password) as sftp:
	                  for k in range (1,6):
        	                fichero_energia='/home/u4477/fronius/data/invt'+str(k)+'/production/'
                	        nombre='fv42_invt'+str(k)+'_energy'+str(y)+'.csv'
                        	fichero_energia=fichero_energia+nombre
                        	print 'cmt2656', fichero_energia
                        	remote_path="/var/www/fv42/data/invt"+str(k)+"/production/"
				remote_path=remote_path+nombre
	                        sftp.put(fichero_energia,remote_path)

      except Exception:
            print("Error sftp342 en sftp ficheros energía")
            pass



#fin ftp

try:
      k=0
      while True:
	    d=datetime.now()
       	    fecha=d.strftime("%Y%m%d")
	    hora=d.strftime("%H:%M:%S")
	    print 'fecha: ', fecha, 'Hora: ', hora
            k=k+1
	    cabecera_energy()
            try:
	    	fronius()
	    except:
		pass
	    print 'fronius finalizado ok'
	    o.date  = datetime.utcnow()
            print 'UTC time: ', o.date
            sun = ephem.Sun(o)
            alturasolar= math.degrees(float(sun.alt))
            print 'altura solar: ', alturasolar
	    if alturasolar > -5:	    
            	#ftp_fronius()
		print ("ftp en ftpfronius.py")
	    	#ftp_fronius_cmt2656()
            print "esperando 30s, realizado OK: ",k
	    time.sleep(15)
            if not k%2 and alturasolar > -5:
            	energy()
                #ftp_energy()
		print 'ahora al cmt2656'
		#ftp_energy_cmt2656()
	    print 'FIN, esperando 30s'
	    time.sleep(15)

except KeyboardInterrupt:
      ser.close()

except Exception,e:
      print("Unexpected error")
      pass
