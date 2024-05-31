#define LEDC_CHANNEL_0     0    // use first channel of 16 channels (started from zero)
#define LEDC_TIMER_8_BIT   8    // use 8 bit precission for LEDC timer
#define LEDC_BASE_FREQ     5000 // use 5000 Hz as a LEDC base frequency
#define LED_PIN            21    // // Led Verde

int brillo = 0;    
int sensorT0 = 0;    
int sensorT3 = 0;  
const int pulsador = 18;
const int led1 = 22;


void setup() {
  Serial.begin(115200);
  
  ledcSetup(LEDC_CHANNEL_0, LEDC_BASE_FREQ, LEDC_TIMER_8_BIT); // Setup timer and attach timer to a led pin
  ledcAttachPin(LED_PIN, LEDC_CHANNEL_0);
  
  pinMode(pulsador, INPUT_PULLUP); // Pulsador
  pinMode(led1, OUTPUT);       // Led Rojo
}

void loop() {

  sensorT0 = touchRead(T0);
  sensorT3 = touchRead(T3);
  
  if (sensorT0 < 20 and sensorT0 != 0) {
    brillo = brillo + 5;
    if (brillo >= 255) {
      brillo = 255;
    }
  }
  if (sensorT3 < 20 and sensorT3 != 0) {
    brillo = brillo - 5;
    if (brillo <= 0) {
      brillo = 0;
    }
  }
  
  Serial.println("T3=" + String(sensorT3) + " T0=" + String(sensorT0) + " Brillo=" + String(brillo));

  ledcWrite(LEDC_CHANNEL_0, brillo);
  
  digitalWrite(led1, digitalRead(pulsador));
  
  delay(100);
}
