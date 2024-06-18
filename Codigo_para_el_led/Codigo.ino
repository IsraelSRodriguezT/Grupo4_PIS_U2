#include <Servo.h>
Servo Servomotor1;
Servo Servomotor2;
const int sensorLuzPin = A0;
const int Pinservo1 = 9;
const int Pinservo2 = 10;
int valorSensor = 0;

void setup() {
  Serial.begin(9600);
  Servomotor1.attach(Pinservo1);
  Servomotor2.attach(Pinservo2);
}

void loop() {
  valorSensor = analogRead(sensorLuzPin);
  Serial.print("Sensor de luz: ");
  Serial.println(valorSensor);
  int anguloServo = map(valorSensor, 0, 1023, 0, 180);
  Servomotor1.write(anguloServo);
  Servomotor2.write(anguloServo);
  delay(100);
}
