#!/usr/bin/python
import json
import requests
import subprocess


def validacion():
    condicion = True
    ingreso = 0
    while (condicion):
        try:
            ingreso = int(input("Ingresa un numero entre 1 y 5: "))
            condicion = False
        except ValueError:
            print('Ingresa un numero')
    return ingreso

proceso = subprocess.Popen(["./bin/servidor"])

salir = True

while salir:

    print ("1. Listar Dispositivos")
    print ("2. Renombrar dispositivo")
    print ("3. Leer archivo")
    print ("4. Escribir archivo")
    print ("5. Salir")

    print ("Ingrese un numero")
    opcion = validacion()
    if opcion == 1:
        print ("Listar Dispositivos")
	archivo = open("log_usb")

	for i in range(9):
		line = archivo.readline()
		if ("no hay dispositivos conectado" not in line):
			print(line)
	print("No se encontraron mas dispositivos")
        archivo.close()

    elif opcion == 2:
        print("Renombrar Archivos")
        

    elif opcion == 3:
        print("Leer Archivo")
        

    elif opcion == 4:
        print("Escribir Archivo")
        

    elif opcion == 5:
        proceso.kill()
        salir = False
    else:
        print ("Ingrese un numero entre 1 y 5")

print ("Gracias por usar nuestro servicio xD")
