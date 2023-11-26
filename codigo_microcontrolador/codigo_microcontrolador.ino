/*
  Codigo para comunicarse con el socket de microservicio1 de la aplicacion lampara
  version: 1.0
  Author: Paul Santana
*/

#include <ArduinoWebsockets.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
StaticJsonDocument<200> doc;

float temperatura;
float humedad;
int pin = 5;

String obtenerJson(String jsonInfo,String clave){
  deserializeJson(doc, jsonInfo);
  return  doc.as<JsonObject>()[clave];
}

const char* ssid = "Casita-2"; //Enter SSID
const char* password = "guzman321"; //Enter Password
const char* websockets_server_host = "192.168.1.14"; //Enter server adress
const uint16_t websockets_server_port = 8080; // Enter server port

using namespace websockets;

//creamos los objetos para los consumir los web sockets
WebsocketsClient cliente_estado_lampara;
WebsocketsClient cliente_control_lampara;
WebsocketsClient cliente_peticion_estado_lampara;

void setup() {
    Serial.begin(9600);
    StaticJsonDocument<200> json;
    pinMode(pin,OUTPUT);
    
    // Conectando al wifi
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
      delay(60);
      Serial.print(".");
    }

    Serial.println("Conectado al  WiFi");

    // tratando de conectar al web socket
    bool connected1 = cliente_estado_lampara.connect(websockets_server_host, websockets_server_port, "/");
    bool connected2 = cliente_control_lampara.connect(websockets_server_host, websockets_server_port, "/");
    bool connected3 = cliente_peticion_estado_lampara.connect(websockets_server_host, websockets_server_port, "/");
    
    if(connected1) {//revisando que si nos conectamos al web socket para suscribirnos a la cola de mensajes de estado_lampara
      Serial.println("web socket SI Conectado a estado_lampara!");
      //nos conectamos a la cola de mensajes de lectura para mandar lecturas
      cliente_estado_lampara.send("{ \"type\": \"subscribe\", \"channel\": \"estado_lampara\" }");
    }
    
    if(connected2) {//revisando que si nos conectamos al web socket para suscribirnos a la cola de mensajes de control_lampara
      Serial.println("web socket SI Conectado control_lampara!");
      //nos conectamos a la cola de mensajes de lectura para mandar lecturas
      cliente_control_lampara.send("{ \"type\": \"subscribe\", \"channel\": \"control_lampara\" }");
    }
    
    if(connected3) {//revisando que si nos conectamos al web socket para suscribirnos a la cola de mensajes de peticion_estado_lampara
      Serial.println("web socket SI Conectado peticion_estado_lampara!");
      //nos conectamos a la cola de mensajes de lectura para mandar lecturas
      cliente_peticion_estado_lampara.send("{ \"type\": \"subscribe\", \"channel\": \"peticion_estado_lampara\" }");
    } 
    
    // para recibir los mensaje del websocker del control_lampara
    cliente_control_lampara.onMessage([&](WebsocketsMessage mensaje) {
        Serial.println("Recibiendo mensajes: control_lampara ");
        String resultado = obtenerJson(mensaje.data(),"content");
        digitalWrite(pin,resultado=="1"?1:0);
        cliente_estado_lampara.send("{ \"type\": \"message\", \"content\": \""+String(digitalRead(pin))+"\" }");
    });
    
    // para recibir los mensaje del websocker de peticion_estado_lampara
    cliente_peticion_estado_lampara.onMessage([&](WebsocketsMessage mensaje) {
        Serial.println("Recibiendo mensajes:  peticion_estado_lampara");
        cliente_estado_lampara.send("{ \"type\": \"message\", \"content\": \""+String(digitalRead(pin))+"\" }");
    });
}

void loop() {
    temperatura = random(15,20);
    humedad = random(2,14);
    // let the websockets client check for incoming messages
    if(cliente_estado_lampara.available()) {
        cliente_estado_lampara.poll();
        //el estado_lampara es el unico que tiene content
        //cliente_estado_lampara.send("{ \"type\": \"message\", \"content\": \""+String(digitalRead(pin))+"\" }");
        //cliente_estado_lampara.send("{ \"type\": \"message\", \"content\": \""+String(temperatura)+","+String(humedad)+" \" }");
    }
    
    if(cliente_control_lampara.available()) {
        cliente_control_lampara.poll();
    }
    
    if(cliente_peticion_estado_lampara.available()) {
        cliente_peticion_estado_lampara.poll();
    }
    
    delay(200);
}
