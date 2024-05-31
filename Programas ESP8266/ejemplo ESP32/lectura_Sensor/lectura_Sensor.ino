// Potenciometro conectado a GPIO 35
const int portPin = 35;
// Almacenamiento del valor de puerto (Rango de 0-4095)
int potValor = 0;
void setup() {
  Serial.begin(115200);
  delay(1000);
}
void loop() {
  // Lectura del valor en cada vuelta del bucle
  potValor = ((analogRead(portPin)*6*100)/4095)-1;
  Serial.println(potValor);  //Envío del valor al puerto serie
  delay(1000);
}
