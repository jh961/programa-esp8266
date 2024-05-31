#include <WiFiManager.h>
#include <WiFi.h>
#include <FS.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <Ticker.h>


Ticker ticker;
byte pinLed = 2;
const int lm = 35;

int potValor = 0;

/// IP STATICA
  IPAddress ip(192, 168, 2, 110); // where xx is the desired IP Address
  IPAddress gateway(192, 168, 2, 1); // set gateway to match your network
  IPAddress subnet(255, 255, 255, 0); // set subnet mask to match your network

void parpadeoLed(){
  // Cambiar de estado el LED
  byte estado = digitalRead(pinLed);
  digitalWrite(pinLed, !estado);
}

String Estado_Rele; //Variable String para mandar el estado del rele a nuestra página web

AsyncWebServer server(80); //Puerto TCP

float temperatura = 0;
float valor=0;
String readTempera() {
  // Read temperature as Celsius (the default)
    valor=analogRead(lm);
    temperatura=((valor*6*100)/4095);//lectura de temperatura
    Serial.println(temperatura);
    return String(temperatura);
  }


void setup()
{
  Serial.begin(115200);
  pinMode(pinLed, OUTPUT);
  //pinMode(lm,INPUT);
   
   
   //Iniciamos el sistema Flash
  if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

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

  
  
  // Route for root / web page
 server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html");
  });

  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readTempera().c_str());
  });
  // Start server
  server.begin();
}


void loop() {
  //No se usará debido a la librería del servidor web asíncrono
  }
