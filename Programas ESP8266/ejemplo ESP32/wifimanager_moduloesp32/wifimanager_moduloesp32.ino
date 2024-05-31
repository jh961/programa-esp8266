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

const int led = 2;

String pagina = "<!DOCTYPE html>"
"<html>"
"<head>"
"<meta charset='utf-8' />"
"<title>Servidor Web ESP32</title>"
"</head>"
"<body>"
"<center>"
"<h1>Servidor Web ESP32</h1>"
"<p><a href='/Habit_1_ON'><button style='height:50px;width:100px'>ON</button></a></p>"
"<p><a href='/Habit_1_OFF'><button style='height:50px;width:100px'>OFF</button></a></p>"
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
  digitalWrite(pinLed, HIGH);
  
  
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
            client.println(pagina);
            
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
