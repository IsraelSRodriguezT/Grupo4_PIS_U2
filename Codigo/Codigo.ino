#include <Servo.h>
Servo Servomotor1;
Servo Servomotor2;
const int sensorLuzPin1 = A0;
const int sensorLuzPin2 = A1;
const int Pinservo1 = 9;
const int Pinservo2 = 10;
int valorSensor1 = 0;
int valorSensor2 = 0;

void setup() {
  Serial.begin(9600);
  Servomotor1.attach(Pinservo1);
  Servomotor2.attach(Pinservo2);
}

void loop() {
  valorSensor1 = analogRead(sensorLuzPin1);
  valorSensor2 = analogRead(sensorLuzPin2);
  Serial.print("Sensor de luz 1: ");
  Serial.println(valorSensor1);
  Serial.print("Sensor de luz 2: ");
  Serial.println(valorSensor2);
  int diferencia = valorSensor1 - valorSensor2;
  int anguloServo1 = map(diferencia, -1023, 1023, 0, 180);
  int anguloServo2 = map(diferencia, -1023, 1023, 0, 180);
  Servomotor1.write(anguloServo1);
  Servomotor2.write(anguloServo2);
  delay(100);
}