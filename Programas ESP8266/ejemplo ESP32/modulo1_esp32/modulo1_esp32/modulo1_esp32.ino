#include <WiFiManager.h>
#include <WiFi.h>

//------------------Servidor Web en puerto 80---------------------

WiFiServer server(80);

int val=0,bre=0;
String s1="",s="";

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
"<p><a href='/on'><button style='height:50px;width:100px'>ON</button></a></p>"
"<p><a href='/off'><button style='height:50px;width:100px'>OFF</button></a></p>"
"</center>"
"</body>"
"</html>";



 /// IP STATICA
  IPAddress ip(192, 168, 2, 110); // where xx is the desired IP Address
  IPAddress gateway(192, 168, 2, 1); // set gateway to match your network
  IPAddress subnet(255, 255, 255, 0); // set subnet mask to match your network

  
void setup()
{
  Serial.begin(115200);

  // Creamos una instancia de la clase WiFiManager
  WiFi.config(ip, gateway, subnet);
  WiFiManager wifiManager;
  
  // Descomentar para resetear configuración
  //wifiManager.resetSettings();

  // Cremos AP y portal cautivo
  wifiManager.autoConnect("ESP8266Temp","hola1234");

  Serial.println("Ya estás conectado");
  
  
  
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
  digitalWrite(led, val);
}

void loop() {
    WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  // Wait until the client sends some data
  Serial.println("new client");
  bre=0;
  while(!client.available()){
    delay(1);
    Serial.println("error");
    ///// revizar esta parte-.........
    bre++;
    if(bre>=10){
    break;
    }
   
  }
  
  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();
  
  // Match the request
  if (req.indexOf("/usuario") != -1){
  val= 0;
  s= "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n<p>GPIO2 <a href='/PRINCIPAL'><button>ON</button></a></p>";
  s += "</html>\n";
  }else if (req.indexOf("/PRINCIPAL") != -1){
   val= 102;
   s= "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n<p>LISTO CAMBIO HECHO <a href='https://www.google.com.co/'><button>OK</button></a></p>";
  s += "</html>\n";
  // Send the response to the client
  }else if (req.indexOf("/Habit_1_ON") != -1){
   val= 1024;
   s= "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html><head><title>ESP8266</title></head> <body bgcolor='#FFCC99' align='center'><table width='575' border='2' cellspacing='2' align='center'><tr align='center' valign='middle'> <th colspan='4'><marquee bgcolor='#006699' behavior='alternate' direction='right'><b><font color='#FFFFCC' size='5'>CONTROL DE ILUMINARIAS</font></b> </marquee></th></tr><th colspan='4'><font color='#CC3300' size='5'>LUZ ENCENDIDA</font></th></tr><tr align='center' valign='middle'><th colspan='2'><font color='#993366' size='4' align='center' face='Comic Sans MS, Arial, MS Sans Serif'><a href=''><img src='https://lh3.googleusercontent.com/rH8rF9loIplBhunHKqoeEDu3jhosxaTfjvCwR7MsQ_ZS9SO1lT8Fw2pduqY8xjSaH6DNdwoYEFKVrKkr1TcClCAC9rbkAZi41OE0qsaH6fTsY4uytICTKLscYwpm95mT0gTZxwcb6UnsAKbLkBOdBCOSCkQF2YlfgHap68trwJX2_t29owoFXT_CTqtXcK6B0IpkJhmLgzXS-6GoNvT47NTcVP5UCqKLTFCo_2fw8b0dcsL3I7qSRG7AW67ijR76LZv3f_TyLhraMTJOFwlFS7IRn85KfuTTKUtevSlS-H9CPTDyXVTrb0hkPdFuBJJpdmOOrG2jGgAbWIp_CYkxXm2vountcj6NWdFfufT9njlUHzleUHTEJJ5PsrlnWuJHVAgyRa3y-Sk41ba_7y9oV5ANXMHOzGkXOKN3fvzjn71wmhRjbbVfCoC3fiPCxAm3XORbXGXLBudRJvL6gIPAEsUEHtgfhpR21G3V7kV89XiqGOMZAWOUiYKnQ-9PhrYTmfTsKZR7qetmnwlUmKo5kFW-yYUhrtuoO2KpJzVqqkSVfKdFvSgufGKx1BABP5yWdgwlvmPQBt7vs64PHvZ4rF6TQiHtE4c=w455-h98-no' border='4' width='200' height='80' ><br>PRINCIPAL</a></font></th><th colspan='2'><font color='#993366' size='4' align='center' face='Comic Sans MS, Arial, MS Sans Serif'><a href=''><img src='https://lh3.googleusercontent.com/icNVOxOjEImPwCDlvNTuL2Vgh__u0aUvLHyPchwonYLh_giovND3EKEyht8azp2yGvBcyG2EaVkCuAKHzd7L1yM9Gar_Nci0MpaSaO71qqSK_e7J0Lp7zknqUkVRW4oro7BWdRrm0Jk8jeV64tVcTCixRBSqu4k6_Esb0WbBxqMzvK93nrDg0-PnVc0nXIccJJGS23GUoOoYs5voEhvVQqe0SbaBU_wX-DOZka02LwIO1wMoF4odm8UpO4sWxcd2kZaB2RN6eADjO6QoRgqD0nJjUZuAdbiVmrFbrhsNjHQ8uttUaF1sCh2ik6nrJKH3MdH5LuNp9MrNtXWPP4xl95X7mk53F684mXl0qfNlIv2bMcv7rmyjaJkyPiz7p8tjCAndDI6pqgifDHaPCdso4H9ibY5pj3lfd1wQVpiW_VE1445tbp1u1-MDriGMIHCVoA_-Kk9jpDLv8fpxT2Sapuan8oFQCOr23--h_XMvecxj7IeFv_TM5quJ0riEVUChhHJnPz91eB-6aVR1gmUoeZrnX7sk3HA8YmHBS9fsYDIm_Y3erjLap9CB4Rzw2gskj5qPm2rGqKpIxUxGCJqI-FDoF3Awn8k=w601-h103-no' border='4' width='200' height='80' ><br>HABITACION 1</a></font></th></tr></table><font color='#CC3300' size='5'>Hola, estoy haciendo pruebas.</font></body>";
  s += "</html>\n";
  }else if (req.indexOf("/Habit_1_OFF") != -1){
   val= 0;
  s= "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html><head><title>ESP8266</title></head> <body bgcolor='#FFCC99' align='center'><table width='575' border='2' cellspacing='2' align='center'><tr align='center' valign='middle'> <th colspan='4'><marquee bgcolor='#006699' behavior='alternate' direction='right'><b><font color='#FFFFCC' size='5'>CONTROL DE ILUMINARIAS</font></b> </marquee></th></tr><th colspan='4'><font color='#CC3300' size='5'>LUZ APAGADA</font></th></tr><tr align='center' valign='middle'><th colspan='2'><font color='#993366' size='4' align='center' face='Comic Sans MS, Arial, MS Sans Serif'><a href=''><img src='https://lh3.googleusercontent.com/rH8rF9loIplBhunHKqoeEDu3jhosxaTfjvCwR7MsQ_ZS9SO1lT8Fw2pduqY8xjSaH6DNdwoYEFKVrKkr1TcClCAC9rbkAZi41OE0qsaH6fTsY4uytICTKLscYwpm95mT0gTZxwcb6UnsAKbLkBOdBCOSCkQF2YlfgHap68trwJX2_t29owoFXT_CTqtXcK6B0IpkJhmLgzXS-6GoNvT47NTcVP5UCqKLTFCo_2fw8b0dcsL3I7qSRG7AW67ijR76LZv3f_TyLhraMTJOFwlFS7IRn85KfuTTKUtevSlS-H9CPTDyXVTrb0hkPdFuBJJpdmOOrG2jGgAbWIp_CYkxXm2vountcj6NWdFfufT9njlUHzleUHTEJJ5PsrlnWuJHVAgyRa3y-Sk41ba_7y9oV5ANXMHOzGkXOKN3fvzjn71wmhRjbbVfCoC3fiPCxAm3XORbXGXLBudRJvL6gIPAEsUEHtgfhpR21G3V7kV89XiqGOMZAWOUiYKnQ-9PhrYTmfTsKZR7qetmnwlUmKo5kFW-yYUhrtuoO2KpJzVqqkSVfKdFvSgufGKx1BABP5yWdgwlvmPQBt7vs64PHvZ4rF6TQiHtE4c=w455-h98-no' border='4' width='200' height='80' ><br>PRINCIPAL</a></font></th><th colspan='2'><font color='#993366' size='4' align='center' face='Comic Sans MS, Arial, MS Sans Serif'><a href=''><img src='https://lh3.googleusercontent.com/icNVOxOjEImPwCDlvNTuL2Vgh__u0aUvLHyPchwonYLh_giovND3EKEyht8azp2yGvBcyG2EaVkCuAKHzd7L1yM9Gar_Nci0MpaSaO71qqSK_e7J0Lp7zknqUkVRW4oro7BWdRrm0Jk8jeV64tVcTCixRBSqu4k6_Esb0WbBxqMzvK93nrDg0-PnVc0nXIccJJGS23GUoOoYs5voEhvVQqe0SbaBU_wX-DOZka02LwIO1wMoF4odm8UpO4sWxcd2kZaB2RN6eADjO6QoRgqD0nJjUZuAdbiVmrFbrhsNjHQ8uttUaF1sCh2ik6nrJKH3MdH5LuNp9MrNtXWPP4xl95X7mk53F684mXl0qfNlIv2bMcv7rmyjaJkyPiz7p8tjCAndDI6pqgifDHaPCdso4H9ibY5pj3lfd1wQVpiW_VE1445tbp1u1-MDriGMIHCVoA_-Kk9jpDLv8fpxT2Sapuan8oFQCOr23--h_XMvecxj7IeFv_TM5quJ0riEVUChhHJnPz91eB-6aVR1gmUoeZrnX7sk3HA8YmHBS9fsYDIm_Y3erjLap9CB4Rzw2gskj5qPm2rGqKpIxUxGCJqI-FDoF3Awn8k=w601-h103-no' border='4' width='200' height='80' ><br>HABITACION 1</a></font></th></tr></table><font color='#CC3300' size='5'>Hola, estoy haciendo pruebas.</font></body>";
  s += "</html>\n";
  }else {
    Serial.println("invalid request");
    client.stop();
    
    return;
  }

  // Set GPIO2 according to the request
  digitalWrite(led, val);
  client.flush();

  client.print(s);
  delay(1);
  s="";
  Serial.println("Client disonnected");
  
  
  // Prepare the response
  

  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed
  }
