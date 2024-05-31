#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>

byte led1 = 4;
byte led2 = 5;
String estado1; //Variable String para mandar el estado del rele a nuestra página web
String estado2; 

int cont1 =0 ,cont=0,reloj=0,tiempopren = 60;

//Datos de nuestro WiFi
const char* ssid = "F_PADILLA";
const char* password = "F1D2I3A4Z5";

/// IP STATICA
  IPAddress ip(192, 168, 2, 122); // where xx is the desired IP Address
  IPAddress gateway(192, 168, 2, 1); // set gateway to match your network
  IPAddress subnet(255, 255, 255, 0); // set subnet mask to match your network

AsyncWebServer server(80); //Puerto TCP

String processor1(const String& var) //Función processor1 para regresar valores a página Web
{
  
     if(digitalRead(led1)==1 and var == "estado1")
     {
      estado1= "Encendido";
      reloj=0;
      return estado1;
     }
     else if (digitalRead(led1)==0 and var == "estado1") 
     {
      estado1 = "Apagado";
      return estado1;
     }else if (digitalRead(led2)==1 and var == "estado2") 
     {
      estado2= "Encendido";
      reloj=0;
      return estado2;
     }else if (digitalRead(led2)==0 and var == "estado2") 
     {
      estado2 = "Apagado";
      return estado2;
     }
            
}

String tiempo(const String& tiemp) //Función processor1 para regresar valores a página Web
{
  Serial.println(tiemp);
}


void setup()
{
  Serial.begin(115200);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT); 
   
   //Iniciamos el sistema Flash
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

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false, processor1);
  });

  server.on("/estilos.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/estilos.css", "text/css");
  });

  // Cuando pulsemos el botón de encendido
  server.on("/Habit_1_ON", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(led1, HIGH);    
    request->send(SPIFFS, "/index.html", String(), false, processor1);
  });

  server.on("/Habit_2_ON", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(led2, HIGH);    
    request->send(SPIFFS, "/index.html", String(), false, processor1);
  });
  
  // Cuando pulsemos el botón de apagado
  server.on("/Habit_1_OFF", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(led1, LOW); 
    request->send(SPIFFS, "/index.html", String(), false, processor1);
  });

  server.on("/Habit_2_OFF", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(led2, LOW);
    request->send(SPIFFS, "/index.html", String(), false, processor1);
  });

  server.on("/Tiempo", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false, tiempo);
  });
  // Start server
  server.begin();
}


void loop() {
 if(cont1 >= 1000){ // tiempo en milisegundos
  reloj++;
  cont=millis();
  Serial.println(reloj);

  if(reloj>=tiempopren*60){ // 60 segundos para mas tiempo se pone en segundo 
  reloj=0;
  //cont=millis();
  
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  
  }
}
cont1=millis()-cont;
  }
