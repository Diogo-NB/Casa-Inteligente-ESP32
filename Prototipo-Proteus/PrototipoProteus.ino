#define FAN_1_PIN 5  
#define FAN_2_PIN 4 

#define LAMP_1_PIN  3
#define LAMP_2_PIN  2   
#define LAMP_3_PIN  1  
#define LAMP_4_PIN  0

#define GP_1_PIN  13
#define GP_2_PIN  12

#define MOTION_SENSOR_PIN 19
#define TEMP_SENSOR_PIN   14

void setup() {
  pinMode(FAN_1_PIN, OUTPUT);
  pinMode(FAN_2_PIN, OUTPUT);

  pinMode(LAMP_1_PIN, OUTPUT);
  pinMode(LAMP_2_PIN, OUTPUT);
  pinMode(LAMP_3_PIN, OUTPUT);
  pinMode(LAMP_4_PIN, OUTPUT);

  pinMode(GP_1_PIN, OUTPUT);
  pinMode(GP_2_PIN, OUTPUT);

  pinMode(MOTION_SENSOR_PIN, INPUT);
  pinMode(TEMP_SENSOR_PIN, INPUT);
 
  digitalWrite(FAN_1_PIN, LOW);
  digitalWrite(FAN_2_PIN, LOW);  

  digitalWrite(LAMP_1_PIN, LOW);
  digitalWrite(LAMP_2_PIN, LOW);
  digitalWrite(LAMP_3_PIN, LOW);
  digitalWrite(LAMP_4_PIN, LOW);

  digitalWrite(GP_1_PIN, LOW);
  digitalWrite(GP_2_PIN, LOW);
}

void loop() {
  delay(5000);
}