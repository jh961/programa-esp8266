#include <Servo.h>


Servo myservo;
const int servo = 14;
const int tiempo = 70;
int posicion =0;
void setup() {
  Serial.begin(115200);
  // put your setup code here, to run once:
  myservo.attach(servo);
  //myservo.write(150);
  //posicion=myservo.read();
  
  //delay(1000);

  //Serial.println(posicion);
  //myservo.write(30);
}

void loop() {
  // put your main code here, to run repeatedly:
//  for(int i=0;i<=180;i++){
//      myservo.write(i);
//      Serial.println(i);
//      delay(30);
//  }
//  for(int i=180;i>=0;i--){
//      myservo.write(i);
//      Serial.println(i);
//      delay(30);
//  }

      // en 60 grados esta centrado en al mitad
      //en 30 esta directo todo abajo
      // en 150 grados quedo bajando a las izquierda
      for(int i=150;i>=100;i-=10){
      myservo.write(i);
      delay(tiempo);
     }
     delay(10000);

     for(int i=100;i>=30;i-=10){
      myservo.write(i);
      delay(tiempo);
     }
     delay(10000);
     
     for(int i=30;i<=100;i+=10){
      myservo.write(i);
      delay(tiempo);
     }
     delay(10000);

     for(int i=100;i<=150;i+=10){
      myservo.write(i);
      delay(tiempo);
     }
     delay(10000);
//      myservo.write(150);
//      delay(tiempo);
//      myservo.write(90);
//      delay(tiempo);
//      myservo.write(30);
//      delay(tiempo);

}
