/* This is a basic program that isolates a single sensor on analog input pin
 * A0 and prints its value to the Serial monitor. Used to check the values from
 * various sensors without having an entire system connected.
 */

int sensorVal;
const int sensorPin = A0;

void setup() {
  Serial.begin(9600);
  Serial.println("Monitoring ready.");
  delay(500);
}

void loop() {
  sensorVal = analogRead(sensorPin);
  Serial.print("sensorVal: ");
  Serial.println(sensorVal);
}
