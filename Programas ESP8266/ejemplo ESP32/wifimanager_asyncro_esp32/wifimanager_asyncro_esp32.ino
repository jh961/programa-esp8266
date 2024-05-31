#include <WiFiManager.h>
#include <WiFi.h>
#include <FS.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <Ticker.h>


Ticker ticker;
byte pinLed = 2;
byte led1 = 21;

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

String processor(const String& var) //Función Processor para regresar valores a página Web
{
  if(var == "ESTADO_RELE"){
     if(digitalRead(led1)==1)
     {
      Estado_Rele= "Encendido";
     }
     else
     {
      Estado_Rele = "Apagado";
     }
    return Estado_Rele;
    }
  
}


void setup()
{
  Serial.begin(115200);
  pinMode(pinLed, OUTPUT);
  pinMode(led1, OUTPUT);
   
   
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
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  server.on("/estilos.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/estilos.css", "text/css");
  });

  // Cuando pulsemos el botón de encendido
  server.on("/Habit_1_ON", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(led1, HIGH);    
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  // Cuando pulsemos el botón de apagado
  server.on("/Habit_1_OFF", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(led1, LOW);    
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  // Start server
  server.begin();
}


void loop() {
  //No se usará debido a la librería del servidor web asíncrono
  }
