#include <WiFiManager.h> //Incluimos librerías
#include <WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <Ticker.h>

//Datos de nuestro WiFi

byte pinLed = 2;

//Configurar una dirección IP Estática
  IPAddress ip(192, 168, 2, 121); // where xx is the desired IP Address
  IPAddress gateway(192, 168, 2, 1); // set gateway to match your network
  IPAddress subnet(255, 255, 255, 0); // set subnet mask to match your network


void parpadeoLed(){
  // Cambiar de estado el LED
  byte estado = digitalRead(pinLed);
  digitalWrite(pinLed, !estado);
}

const int Rele_Pin = 2; //Pin donde estará la señal al rele

String Estado_Rele; //Variable String para mandar el estado del rele a nuestra página web

AsyncWebServer server(80); //Puerto TCP

String processor(const String& var) //Función Processor para regresar valores a página Web
{
  if(var == "ESTADO_RELE")
    {
     if(digitalRead(Rele_Pin)==1)
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

void setup(){
  // Iniciamos comunicación Serial y ponemos el pin del rele como salida
  Serial.begin(115200);
  pinMode(Rele_Pin, OUTPUT);

  //Iniciamos el sistema Flash
  if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Conectamos a WiFi Modo estación con dirección IP estática
  
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

  //Imprimimos IP en el monitor serial
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

  
  //A partir de aqui vamos a resolver las solicitudes del cliente

  // Cuando ingresen a nuestra página
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  // Cuando nuestro archivo Html solicite el archivo Css
  server.on("/estilos.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/estilos.css", "text/css");
  });

  // Cuando pulsemos el botón de encendido
  server.on("/RELE=ON", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(Rele_Pin, HIGH);    
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  // Cuando pulsemos el botón de apagado
  server.on("/RELE=OFF", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(Rele_Pin, LOW);    
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  //Iniciamos servidor
  server.begin();
}
void loop() {
//No se usará debido a la librería del servidor web asíncrono
}
