const int motor = 16;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(motor, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
   for(int i=0;i<=255;i++){
      analogWrite(motor, i);
      Serial.println(i);
      delay(30);
  }
  for(int i=255;i>=0;i--){
      analogWrite(motor, i);
      Serial.println(i);
      delay(30);
  }

}
