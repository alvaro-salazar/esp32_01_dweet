#include <Arduino.h>
#include "libwifi.h"
#include <WiFi.h>
 
const char * ssid = "virus";
const char * password = "a1b2c3d4";
const char * host = "dweet.io";
const int puerto = 80;
String url = "/dweet/for/uceva00?temperatura=30.4";

//@author Alvaro
/**
 * @brief Esta es la funcion de configuracion del dispositivo
 * 
 */
void setup() {
  pinMode(2, OUTPUT);  //Coloco el pin 2 como salida
  Serial.begin(115200);
  Serial.println("Inicializando dispositivo");
  conectarWifi(ssid, password);
}


/**
 * @brief Esta funcion es un bucle infinito que corresponde al main()
 * 
 */
void loop() {
  WiFiClient cliente; //Creamos un cliente TCP por wifi

  //Bloque que realiza la conexion al servidor
  if(!cliente.connect(host, puerto)){
    Serial.println("Error conexion al host fallida");
    delay(2000);
    return;
  }

  //Peticion (request) GET al servidor http
  cliente.print("GET "+url+" HTTP/1.1\r\nHost: "+String(host)+"\r\n"+"Connection: close\r\n\r\n");

  //Debemos darle un tiempo al servidor a que responda (response) la peticion
  //delay(5000); //No funciona aqui
  unsigned long milisegundos = millis(); //Hora de inicio
  while(cliente.available()==0){  //Preguntamos si no hay datos recibidos disponibles
      if(millis()-milisegundos > 5000){
         Serial.println("Se expiro el tiempo de la conexion");
         cliente.stop();
      }
  }

  while(cliente.available()){
    String linea = cliente.readStringUntil('\r');
    Serial.println(linea);
  }

  Serial.println("Fin de la conexion");
  cliente.stop();

  delay(2000);  //Espero 2s
}
