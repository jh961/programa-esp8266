#include "DHT.h"
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include  <Wire.h>
#include  <LiquidCrystal_I2C.h>

#define DHTTYPE DHT11
#define DHTPIN 14
// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE);

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

// Variables to hold sensor readings
float temp;
float hum;

byte pinLed = 13;
byte lm = A0;
String estado1;
const int espera=1000;

int cont1 =0 ,cont=0,reloj=0,tiempopren = 0.5;

//Datos de nuestro WiFi
const char* ssid = "F_PADILLA";
const char* password = "F1D2I3A4Z5";

/// IP STATICA
  IPAddress ip(192, 168, 2, 122); // where xx is the desired IP Address
  IPAddress gateway(192, 168, 2, 1); // set gateway to match your network
  IPAddress subnet(255, 255, 255, 0); // set subnet mask to match your network

String Estado_Rele; //Variable String para mandar el estado del rele a nuestra página web

AsyncWebServer server(80); //Puerto TCP

float temperatura = 0;
float valor=0;
String readTempera() {
  // Read temperature as Celsius (the default)
    temperatura = dht.readTemperature();
    hum = dht.readHumidity();
    
    Serial.println("\nTemperatura");
    Serial.println(temperatura);
    Serial.println("\nHumedad");
    Serial.println(hum);

    lcd.clear(); 
    lcd.setCursor ( 0, 0 );
    lcd.print("TEMPERATURA");
    lcd.setCursor ( 0, 1 );
    lcd.print(temperatura);
    lcd.setCursor ( 0, 2 );
    lcd.print("HUMEDAD");
    lcd.setCursor ( 0, 3 );
    lcd.print(hum);
      
    if (temperatura <=42){
        digitalWrite(pinLed, HIGH);
    }else {
        digitalWrite(pinLed, LOW);
    }
    return String(temperatura);
  }

 String readhumedad() {
    hum = dht.readHumidity();
       
    return String(hum);
  }


String proceso(const String& var){
  
     if(digitalRead(pinLed)==1 and var == "estado1")
     {
      estado1= "Encendido";
      reloj=0;
      return estado1;
     }
     else if (digitalRead(pinLed)==0 and var == "estado1") 
     {
      estado1 = "Apagado";
      return estado1;
     }
            
}

void setup()
{
  Serial.begin(115200);
  pinMode(pinLed, OUTPUT);
  //pinMode(lm,INPUT);
  lcd.begin(20, 4);

  dht.begin();
   
 if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Conectamos a WiFi Modo estación con dirección IP estática
  WiFi.mode(WIFI_STA);
  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, password);

  //Mientras se conecta a la red que se imprima conectando a WiFi
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a WiFi..");
  }
  
  
  Serial.println("");
  Serial.println("********************************************");
  Serial.print("Conectado a la red WiFi: ");
  Serial.println(WiFi.SSID());
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("macAdress: ");
  Serial.println(WiFi.macAddress());
  Serial.println("*********************************************");

  lcd.setCursor ( 0, 0 );
  lcd.print("AUTOR");
  lcd.setCursor ( 0, 1 );
  lcd.print("JAIME DIAZ");
  delay(espera);
  
  lcd.clear();
  lcd.print("INCUBADORA DHT11");
  lcd.setCursor(0, 1);
  lcd.print("V 1.2");
  delay(espera);
  
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false, proceso);
  });

  server.on("/estilos.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/estilos.css", "text/css");
  });
  
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readTempera().c_str());
  });

  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readhumedad().c_str());
  });
  
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false, proceso);
  });
  // Start server
  server.begin();
}


void loop() {
  //No se usará debido a la librería del servidor web asíncrono
  
    if(cont1 >= 1000){ // tiempo en milisegundos
  reloj++;
  cont=millis();
  Serial.println(reloj);

  if(reloj>=tiempopren*60){ // 60 segundos para mas tiempo se pone en segundo 
  reloj=0;
  //cont=millis();
  readTempera();
  
  }
}
cont1=millis()-cont;

}
