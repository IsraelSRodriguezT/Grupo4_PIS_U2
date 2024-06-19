#include <Servo.h> // Incluye la librería Servo.h para manejar los servomotores
Servo Servomotor1; // Variable para el primer servomotor
Servo Servomotor2; // Variable para el segundo servomotor
const int sensorLuzPin1 = A0; // Pin analógico A0 para el primer sensor de luz
const int sensorLuzPin2 = A1; // Pin analógico A1 para el segundo sensor de luz
const int Pinservo1 = 9; // Este pin va a ser conectado al primer servomotor
const int Pinservo2 = 10; // Este pin va a ser conectado al segundo servomotor
int valorSensor1 = 0; // Variable para almacenar el valor leído del primer sensor de luz
int valorSensor2 = 0; // Variable para almacenar el valor leído del segundo sensor de luz

void setup() {
  Serial.begin(9600); // Inicializa la comunicación serial a 9600 baudios
  Servomotor1.attach(Pinservo1); // Asocia el objeto Servomotor1 al pin Pinservo1
  Servomotor2.attach(Pinservo2); // Asocia el objeto Servomotor2 al pin Pinservo2
}

void loop() {
  valorSensor1 = analogRead(sensorLuzPin1); // Lee el valor del primer sensor de luz
  valorSensor2 = analogRead(sensorLuzPin2); // Lee el valor del segundo sensor de luz
  Serial.print("Sensor de luz 1: "); // Imprime un mensaje para el primer sensor de luz
  Serial.println(valorSensor1); // Imprime el valor leído del primer sensor de luz
  Serial.print("Sensor de luz 2: "); // Imprime un mensaje para el segundo sensor de luz
  Serial.println(valorSensor2); // Imprime el valor leído del segundo sensor de luz
  
  int diferencia = valorSensor1 - valorSensor2; // Calcula la diferencia entre los valores de los sensores
  int anguloServo1 = map(diferencia, -1023, 1023, 0, 180); // Mapea la diferencia a un rango de ángulo para el Servomotor1
  int anguloServo2 = map(diferencia, -1023, 1023, 0, 180); // Mapea la diferencia a un rango de ángulo para el Servomotor2
  
  Servomotor1.write(anguloServo1); // Imprime el ángulo calculado al Servomotor1
  Servomotor2.write(anguloServo2); // Imprime el ángulo calculado al Servomotor2
  
  delay(100); // Espera 100 milisegundos antes de la siguiente iteración del loop
}