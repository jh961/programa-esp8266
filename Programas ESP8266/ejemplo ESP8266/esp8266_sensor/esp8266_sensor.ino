#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include  <Wire.h>
#include  <LiquidCrystal_I2C.h>


LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

byte pinLed = 13;
byte lm = A0;

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
    valor=analogRead(lm);
    temperatura=((valor*180)/1024);//lectura de temperatura
    Serial.println(temperatura);
    Serial.println(valor);

    lcd.clear(); 
    lcd.setCursor ( 0, 0 );
    lcd.print("TEMPERATURA");
    lcd.setCursor ( 0, 1 );
    lcd.print(temperatura);
      
    if (temperatura <=42){
        digitalWrite(pinLed, HIGH);
    }else {
        digitalWrite(pinLed, LOW);
    }
    return String(temperatura);
  }


void setup()
{
  Serial.begin(115200);
  pinMode(pinLed, OUTPUT);
  //pinMode(lm,INPUT);
  lcd.begin(16, 2);
   
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
  lcd.print("INCUBADORA");
  lcd.setCursor(0, 1);
  lcd.print("V 1.0");
  delay(espera);
  
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
