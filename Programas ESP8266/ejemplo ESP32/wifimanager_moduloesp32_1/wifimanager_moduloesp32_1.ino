#include <WiFiManager.h>
#include <WiFi.h>
#include <Ticker.h>


Ticker ticker;
 
// Pin LED azul
byte pinLed = 2;

//------------------Servidor Web en puerto 80---------------------

WiFiServer server(80);

int contconexion = 0;

String header; // Variable para guardar el HTTP request

String estadoled = "off";

const int led = 21;

String paginaInicio = "<!DOCTYPE html>"
"<html lang='en'>"
"<head>"
"<meta charset='utf-8' />"
"<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
"<META HTTP-EQUIV='Refresh' CONTENT='1'>"
"<title>Control de LED</title>"
"</head>"
"<body style='font-family: Century gothic; width: 800;'>"
"<center>"
"<div style='box-shadow: 0px 0px 20px 8px rgba(0,0,0,0.22); padding: 20px; width: 300px; display: inline-block; margin: 30px;'>"
"<h1>Datos del SENSOR</h1>";

String paginaFin =
"<h2>LED 1</h2>"
//"<h2>L</h2>"
"<p><a href='/Habit_1_ON'><button style='background-color:red; color:white; height:50px;width:100px; border-radius: 10px; border-color: rgb(25, 255, 4);'>ON</button></a></p>"
"<p><a href='/Habit_1_OFF'><button style='background-color:blue; color:white; height:50px;width:100px; border-radius: 10px; border-color: rgb(255, 0, 0);'>OFF</button></a></p>"
"</div>"
"</center>"
"</body>"
"</html>";



 /// IP STATICA
  IPAddress ip(192, 168, 2, 110); // where xx is the desired IP Address
  IPAddress gateway(192, 168, 2, 1); // set gateway to match your network
  IPAddress subnet(255, 255, 255, 0); // set subnet mask to match your network


void parpadeoLed(){
  // Cambiar de estado el LED
  byte estado = digitalRead(pinLed);
  digitalWrite(pinLed, !estado);
}
  
void setup()
{
  Serial.begin(115200);

   pinMode(pinLed, OUTPUT);
 
  // Empezamos el temporizador que hará parpadear el LED
  ticker.attach(0.2, parpadeoLed);
  
  // Creamos una instancia de la clase WiFiManager
  WiFi.config(ip, gateway, subnet);
  WiFiManager wifiManager;
  
  // Descomentar para resetear configuración
  //wifiManager.resetSettings();

  // Cremos AP y portal cautivo
  wifiManager.autoConnect("ESP8266Temp","hola1234");

  
  Serial.println("Ya estás conectado");

  ticker.detach();
  
  // Apagamos el LED
  digitalWrite(pinLed, LOW);
  
  
  Serial.println("");
  Serial.println("********************************************");
  Serial.print("Conectado a la red WiFi: ");
  Serial.println(WiFi.SSID());
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("macAdress: ");
  Serial.println(WiFi.macAddress());
  Serial.println("*********************************************");

  
  server.begin(); // iniciamos el servidor
  pinMode(led, OUTPUT); 
  digitalWrite(led, LOW);
}

void loop() {
   WiFiClient client = server.available();   // Escucha a los clientes entrantes

  if (client) {                             // Si se conecta un nuevo cliente
    Serial.println("New Client.");          // 
    String currentLine = "";                //
    while (client.connected()) {            // loop mientras el cliente está conectado
      if (client.available()) {             // si hay bytes para leer desde el cliente
        char c = client.read();             // lee un byte
        Serial.write(c);                    // imprime ese byte en el monitor serial
        header += c;
        if (c == '\n') {                    // si el byte es un caracter de salto de linea
          // si la nueva linea está en blanco significa que es el fin del 
          // HTTP request del cliente, entonces respondemos:
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // enciende y apaga el GPIO
            if (header.indexOf("GET /Habit_1_ON") >= 0) {
              Serial.println("GPIO ON");
              estadoled = "on";
              digitalWrite(led, HIGH);
            } else if (header.indexOf("GET /Habit_1_OFF") >= 0) {
              Serial.println("GPIO OFF");
              estadoled = "off";
              digitalWrite(led, LOW);
            }
            
            // Muestra la página web
            client.println(paginaInicio + String(hallRead()) + paginaFin);
            
            // la respuesta HTTP temina con una linea en blanco
            client.println();
            break;
          } else { // si tenemos una nueva linea limpiamos currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // si C es distinto al caracter de retorno de carro
          currentLine += c;      // lo agrega al final de currentLine
        }
      }
    }
    // Limpiamos la variable header
    header = "";
    // Cerramos la conexión
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
  }
