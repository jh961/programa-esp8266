
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

//const char* ssid = "F.DIAZ";
const char* ssid = "DomoHouse";
const char* password = "31021328";

const String numhabit = "2";
//const String ipservidor = "192,168,0,60";/// MARGARITA
const String ipservidor = "192.168.1.120"; /// DOMOHOUSE

// pines utilizados ESP8266
const int pinled = 2;
const int pinboton = 4;
const int pinmovi = 16;
const int pinalarm = 13;
//const char pinADC = A0;

const int pinfase = 14;
const float tfas=0.92;


// config static IP
   /// IP STATICA CON MARGARITA
 /* IPAddress ip(192, 168, 1, 121); // where xx is the desired IP Address
  IPAddress gateway(192, 168, 1, 1); // set gateway to match your network
  IPAddress subnet(255, 255, 255, 0); // set subnet mask to match your network
*/
  /// IP STATICA CON JAIME
 //IPAddress ip(192,168,0,122); // where xx is the desired IP Address MARGARITA
  IPAddress ip(192,168,1,122); // where xx is the desired IP Address DOMOHOUSE
  IPAddress gateway(192,168,1,1); // set gateway to match your network
  IPAddress subnet(255,255,255, 0); // set subnet mask to match your network
  
// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);
int val=0,bre=0,flag=0,reset=0,cont=0,cont1=0,reloj=0,movi;
int seguridad=0,normal=0,deshabilitado=1, tiempopren = 1; ///variable de tiempo, duracion de encendido.;
String s1="",s="";


void setup() {
  Serial.begin(115200);
  delay(10);
  
  analogWriteFreq(120); //// cambiar fracuencia de PWM 
  //TCCR7B = TCCR7B & 0b000 | 0x05;
  
  // prepare GPIO2
  pinMode(pinled, OUTPUT);
   pinMode(pinalarm, OUTPUT);
  pinMode(pinboton, INPUT);
  pinMode(pinmovi, INPUT);
  //pinMode(pinADC, INPUT);

  pinMode(pinfase, INPUT);
  
  // Connect to WiFi network
  Serial.println();
  Serial.print("IP STATIC TO: ");
  Serial.println(ip);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  //WiFi.config(ip);
  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
  ////Definicion de los pines analogos o digitales
  analogWrite(pinled, val);
   
}

void loop() {
  // Check if a client has connected

  WiFiClient client = server.available();
  //Serial.println(client);


  
  if((digitalRead(pinfase)== HIGH )){
     /*if(val==0){
      //analogWrite(pinled, 1023);
      delay(tfas*0);
    }*/ 
    
    if(val==102){
      delay(tfas*9);
    }else if(val==205){
      delay(tfas*8);
    }else if(val==307){
      delay(tfas*7);
    }else if(val==410){
      delay(tfas*6);
    }else if(val==512){
      delay(tfas*5);
    }else if(val==614){
      delay(tfas*4);
    }else if(val==717){
      delay(tfas*3);
    }else if(val==819){
      delay(tfas*2);
    }else if(val==922){
      delay(tfas);
    }else if(val==1023){
      //delay(1);
    }
    if (val!=0){
      analogWrite(pinled, 1023);
    }
    //delay(1);
    analogWrite(pinled, 0);
    //Serial.println(cont);
  }

if(reset==1){
  reloj=0;
  cont=millis();
  reset=0;
  }
  
if(cont1 >= 1000){ // tiempo en milisegundos
  reloj++;
  cont=millis();
  Serial.println(reloj);

  if(reloj>=tiempopren*60){ // 60 segundos para mas tiempo se pone en segundo 
  reloj=0;
  //cont=millis();
  val=0;
  //analogWrite(pinled, val);
  
  }
}
cont1=millis()-cont;
//Serial.println(cont1);



 
if((normal == 1)||(deshabilitado == 1)){
  if((digitalRead(pinboton)== HIGH )){
    while(digitalRead(pinboton)== HIGH){
    }
    if( val > 0){
      flag=1;
      val=0;
    }
    if ((flag==0)){
      val=1023;
      //analogWrite(pinled, val);
      Serial.println("Pin ON");
      flag=1;
      reset=1;
    }else{
      val=0;
      
      Serial.println("Pin OFF");
      flag=0;
    }
    
  }
  
  if(normal == 1){
    if ((digitalRead(pinmovi)== HIGH)){
      val=1023;
      //analogWrite(pinled, val);
      reset=1;
    }
   }

   //analogWrite(pinled, val);
}

 if(seguridad==1){
  movi=digitalRead(pinmovi);
  Serial.println(digitalRead(pinmovi));
    while (movi==HIGH){
    digitalWrite(pinalarm, HIGH);
    //delay(100);
    //digitalWrite(pinalarm, LOW);

    WiFiClient client = server.available();

    if (client==1) {
  
       // Wait until the client sends some data
       Serial.println("new client");
  
        // Read the first line of the request
       String req = client.readStringUntil('\r');
       Serial.println(req);
        client.flush();

       if (req.indexOf("/Normal") != -1){
          //val= 20;
          s1 = "Normal";
          normal=1;
          seguridad=0;
          deshabilitado=0;
          reset=1;
          movi=0;
          digitalWrite(pinalarm, LOW);
          break;
       }else if (req.indexOf("/deshabilitado") != -1){
          //val= 60;
          s1 = "deshabilitado";
          normal=0;
          seguridad=0;
          deshabilitado=1;
          reset=1;
          movi=0;
          digitalWrite(pinalarm, LOW);
          break;
       }

         client.flush();
         Serial.println("Client disonnected");
     }
    }
  
 }
  //Serial.println(digitalRead(pinmovi));
  //Serial.println(digitalRead(pinboton));
  //delay(100);
 

  
  
  if (client==1) {
  
  // Wait until the client sends some data
  Serial.println("new client");
  /*
  bre=0;
  while(!client.available()){
    delay(1);
    Serial.println("error");
    ///// revizar esta parte-.........
    bre++;
    if(bre>=2){
    break;
    }
   
  }*/
  
  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  
  if(req == 0){
    return;
  }
  client.flush();
  
  // Match the request

  if((normal == 1)||(deshabilitado==1)){
    if (req.indexOf("/Habit_"+numhabit+"_ON") != -1){
   val= 1023;
   s1 = "LUZ ENCENDIDA";
   reset=1;
  }else if (req.indexOf("/Habit_"+numhabit+"_OFF") != -1){
   val= 0;
   s1 = "LUZ APAGADA";
   //reset=1;
  }else if (req.indexOf("/Habit_"+numhabit+"_10%") != -1){
   val= 102;
   s1 = "LUZ 10%";
   reset=1;
  }else if (req.indexOf("/Habit_"+numhabit+"_20%") != -1){
   val= 205;
   s1 = "LUZ 20%";
   reset=1;
  }else if (req.indexOf("/Habit_"+numhabit+"_30%") != -1){
   val= 307;
   s1 = "LUZ 30%";
   reset=1;
  }else if (req.indexOf("/Habit_"+numhabit+"_40%") != -1){
   val= 410;
   s1 = "LUZ 40%";
   reset=1;
  }else if (req.indexOf("/Habit_"+numhabit+"_50%") != -1){
   val= 512;
   s1 = "LUZ 50%";
   reset=1;
  }else if (req.indexOf("/Habit_"+numhabit+"_60%") != -1){
   val= 614;
   s1 = "LUZ 60%";
   reset=1;
  }else if (req.indexOf("/Habit_"+numhabit+"_70%") != -1){
   val= 717;
   s1 = "LUZ 70%";
   reset=1;
  }else if (req.indexOf("/Habit_"+numhabit+"_80%") != -1){
   val= 819;
   s1 = "LUZ 80%";
   reset=1;
  }else if (req.indexOf("/Habit_"+numhabit+"_90%") != -1){
   val= 922;
   s1 = "LUZ 90%";
   reset=1;
  }else {
    Serial.println("invalid request");
    //client.stop();
    s1 = "NINGUNA ACCION";
    //return;
   }

   // establece el PWM
  //analogWrite(pinled, val);
  
  }
  
  
  if (req.indexOf("/Estado") != -1){
    
    if (val==0){
     s1 = "LUZ APAGADA";
    }else if(val==1023){
      s1 = "LUZ ENCENDIDA";
    }else if(val==102){
      s1 = "LUZ 10%";
    }else if(val==205){
      s1 = "LUZ 20%";
    }else if(val==307){
      s1 = "LUZ 30%";
    }else if(val==410){
      s1 = "LUZ 40%";
    }else if(val==512){
      s1 = "LUZ 50%";
    }else if(val==614){
      s1 = "LUZ 60%";
    }else if(val==717){
      s1 = "LUZ 70%";
    }else if(val==819){
      s1 = "LUZ 80%";
    }else if(val==922){
      s1 = "LUZ 90%";
    }
    
  }else if (req.indexOf("/Tiempo") != -1){

  int a=1;
  String dato1,req;
  int longi,p1;
  char revi;
  
  while(a==1){

    WiFiClient client = server.available();

  if (client==1) {
  req = client.readStringUntil('\r');
 
  longi = req.length();
  Serial.println(req);
  /*
  if(req == 0){
    return;
  }
  */
  client.flush();
  
  
  if (req.indexOf("/Tsalir") != -1){
   a=0;
  }else{
  for(int i=0;i < longi;i++){
    revi=req.charAt(i);
    if(revi=='$'){
      p1=i;
    }else if(revi=='&'){
      dato1=req.substring(p1+1,i);
      Serial.println("tiempo "+dato1);
      tiempopren=dato1.toInt();
      a=0;
      break;
    }
   }
   }
   client.flush();
   Serial.println("Client disonnected");
  }
  }
   /* 
  Serial.println(req.charAt(6));
  Serial.println(req.substring(5,10));
  Serial.println(req.length());
   */
  }else if (req.indexOf("/Normal") != -1){
   //val= 20;
   s1 = "Normal";
   normal=1;
   seguridad=0;
   deshabilitado=0;
   val=0;
  }else if (req.indexOf("/Seguridad") != -1){
   //val= 60;
   s1 = "Seguridad";
   normal=0;
   seguridad=1;
   deshabilitado=0;
   val=0;
   analogWrite(pinled, val);
   
  }else if (req.indexOf("/deshabilitado") != -1){
   val= 0;
   s1 = "Deshabilitado";
   normal=0;
   seguridad=0;
   deshabilitado=1;
  }else {
    if((normal == 0)&&(deshabilitado==0)){
    Serial.println("invalid request");
    //client.stop();
    s1 = "NINGUNA ACCION";
    //return;
    }
  }

  

  s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n";
  s +="<html><head><title>ESP8266</title></head> ";
  s +="<body bgcolor='#2EFE2E' align='center' link='#FF0000' vlink='#FF0000' alink='#FF9900'>";
  s +="<table width='575' border='5' cellspacing='5' align='center'>";
  s +="<tr align='center' valign='middle'>"; 
  s +="<th colspan='4'><marquee bgcolor='#2E2E2E' behavior='alternate' direction='right'><b><font color='#FFFFCC' size='5'>CONTROL DE CONFIRMACION</font></b> </marquee></th>";
  s +="</tr><th colspan='4'><font color='#FF9900' size='6'>";
  s +=s1;
  s +="</font></th></tr>";
  s +="<tr align='center' valign='middle'>";
  //s +="<th colspan='2'><font color='#2E2E2E' size='4' align='center' face='Jokerman'><a href='http://192.168.0.60/index.html'><img src='https://lh3.googleusercontent.com/rH8rF9loIplBhunHKqoeEDu3jhosxaTfjvCwR7MsQ_ZS9SO1lT8Fw2pduqY8xjSaH6DNdwoYEFKVrKkr1TcClCAC9rbkAZi41OE0qsaH6fTsY4uytICTKLscYwpm95mT0gTZxwcb6UnsAKbLkBOdBCOSCkQF2YlfgHap68trwJX2_t29owoFXT_CTqtXcK6B0IpkJhmLgzXS-6GoNvT47NTcVP5UCqKLTFCo_2fw8b0dcsL3I7qSRG7AW67ijR76LZv3f_TyLhraMTJOFwlFS7IRn85KfuTTKUtevSlS-H9CPTDyXVTrb0hkPdFuBJJpdmOOrG2jGgAbWIp_CYkxXm2vountcj6NWdFfufT9njlUHzleUHTEJJ5PsrlnWuJHVAgyRa3y-Sk41ba_7y9oV5ANXMHOzGkXOKN3fvzjn71wmhRjbbVfCoC3fiPCxAm3XORbXGXLBudRJvL6gIPAEsUEHtgfhpR21G3V7kV89XiqGOMZAWOUiYKnQ-9PhrYTmfTsKZR7qetmnwlUmKo5kFW-yYUhrtuoO2KpJzVqqkSVfKdFvSgufGKx1BABP5yWdgwlvmPQBt7vs64PHvZ4rF6TQiHtE4c=w455-h98-no' border='4' width='200' height='80' ><br>PRINCIPAL</a></font></th>";
  //s +="<th colspan='2'><font color='#2E2E2E' size='4' align='center' face='Jokerman'><a href='http://192.168.0.60/Modulabit2.html'><img src='https://lh3.googleusercontent.com/icNVOxOjEImPwCDlvNTuL2Vgh__u0aUvLHyPchwonYLh_giovND3EKEyht8azp2yGvBcyG2EaVkCuAKHzd7L1yM9Gar_Nci0MpaSaO71qqSK_e7J0Lp7zknqUkVRW4oro7BWdRrm0Jk8jeV64tVcTCixRBSqu4k6_Esb0WbBxqMzvK93nrDg0-PnVc0nXIccJJGS23GUoOoYs5voEhvVQqe0SbaBU_wX-DOZka02LwIO1wMoF4odm8UpO4sWxcd2kZaB2RN6eADjO6QoRgqD0nJjUZuAdbiVmrFbrhsNjHQ8uttUaF1sCh2ik6nrJKH3MdH5LuNp9MrNtXWPP4xl95X7mk53F684mXl0qfNlIv2bMcv7rmyjaJkyPiz7p8tjCAndDI6pqgifDHaPCdso4H9ibY5pj3lfd1wQVpiW_VE1445tbp1u1-MDriGMIHCVoA_-Kk9jpDLv8fpxT2Sapuan8oFQCOr23--h_XMvecxj7IeFv_TM5quJ0riEVUChhHJnPz91eB-6aVR1gmUoeZrnX7sk3HA8YmHBS9fsYDIm_Y3erjLap9CB4Rzw2gskj5qPm2rGqKpIxUxGCJqI-FDoF3Awn8k=w601-h103-no' border='4' width='200' height='80' ><br>HABITACION 2</a></font></th>";
  s +="<th colspan='2'><font color='#2E2E2E' size='4' align='center' face='Jokerman'><a href='http://"+ipservidor+"/index.html'><img src='https://lh3.googleusercontent.com/rH8rF9loIplBhunHKqoeEDu3jhosxaTfjvCwR7MsQ_ZS9SO1lT8Fw2pduqY8xjSaH6DNdwoYEFKVrKkr1TcClCAC9rbkAZi41OE0qsaH6fTsY4uytICTKLscYwpm95mT0gTZxwcb6UnsAKbLkBOdBCOSCkQF2YlfgHap68trwJX2_t29owoFXT_CTqtXcK6B0IpkJhmLgzXS-6GoNvT47NTcVP5UCqKLTFCo_2fw8b0dcsL3I7qSRG7AW67ijR76LZv3f_TyLhraMTJOFwlFS7IRn85KfuTTKUtevSlS-H9CPTDyXVTrb0hkPdFuBJJpdmOOrG2jGgAbWIp_CYkxXm2vountcj6NWdFfufT9njlUHzleUHTEJJ5PsrlnWuJHVAgyRa3y-Sk41ba_7y9oV5ANXMHOzGkXOKN3fvzjn71wmhRjbbVfCoC3fiPCxAm3XORbXGXLBudRJvL6gIPAEsUEHtgfhpR21G3V7kV89XiqGOMZAWOUiYKnQ-9PhrYTmfTsKZR7qetmnwlUmKo5kFW-yYUhrtuoO2KpJzVqqkSVfKdFvSgufGKx1BABP5yWdgwlvmPQBt7vs64PHvZ4rF6TQiHtE4c=w455-h98-no' border='4' width='200' height='80' ><br>PRINCIPAL</a></font></th>";
  s +="<th colspan='2'><font color='#2E2E2E' size='4' align='center' face='Jokerman'><a href='http://"+ipservidor+"/Modulabit2.html'><img src='https://lh3.googleusercontent.com/mKiK0S7yFw6k9Hod6f56u8SNqvVgY76zxyLJNi2gvyMmGrQM4YMP-dY5gCk8htyXM4XcHH3gUMv9HZQm5v1-r-XvqaytooTmjHpnIgOVhs1GIMZY7WSzclfo2taGD8J7_QGVGdo36daWTs6GpPi3wn3mptuKFVf3HAoY8PSOfzPXL8d1Bk-BM0wFWmQoUUPB2GAWfsg7P47e0j7M3Dz6uDJg0L10iYwZBmVdzpzz66qVZyFn3kgbRNvX17aAA8bE7nWQs7Cq2iJly6B8h2UhH0M8yWFfc4YuBtEb1KuJluYyIelTg8G90F5ZHBF4NOCJnoq7Y6p6VqMa1OLRovgS2w5Ry05nAucSJ7efWlG8lOp-uN7YV8WC4jF5AyeOeGZZX75HtWtXZV51YO1KZKCXCXIlcklwdUXoMzIP4n3t55k4iuaK2dR9SFW8SXNnvyMXzDksxlbyFK4GvhPh7SHxXFGYNjMCzfaW0JjndQ3AG8CFY8PVD0bLQMcSyyL9I3tFclMY5EJL80zQZTZ5AuqHsG94TfsEPmttr1NPGuiNMR3IPDZPDDVwc0A3_blKidrWGBqB3gI2eMrDUo_gsFGlZplJYchz8F8=w601-h103-no' border='4' width='200' height='80' ><br>HABITACION 2</a></font></th>";
  s +="</tr>";
  s +="</table>";
  //s +="<font color='#CC3300' size='5'>Hola, estoy haciendo pruebas.</font>";
  s +="</body>";
  s += "</html>\n";

  
  
  client.flush();
  
  client.print(s);
  delay(1);
  s="";
  Serial.println("Client disonnected");
  }
  
  
  // Prepare the response
  

  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed
  
}


