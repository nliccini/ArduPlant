/* This is ArduPlant! ArduPlant is a gardening system that takes care of the
 * watering for you. It uses a moisture sensor to detect how dry your plant's 
 * soil is, then it will evenly pump out water around the pot. Your only job
 * is to ensure that your water tank stays filled, and that's it! You can connect
 * the ArduPlant to your computer and access its processing infromation 
 * using the Arduino IDE and the integrated Serial Monitor. This will allow you
 * to see your plant's soil moisture level in real time and it will give you
 * updates on what the ArduPlant is doing: calibrating, reading sensors, watering.
 * Every couple of days the microcomputer will blink rapidly as a reminder to check
 * the water tank's level. No need to press any buttons, it's just a reminder!
 * Enjoy the fun of gardening without the stress of making sure you water your 
 * plant at the right time, don't overwater, or don't underwater. Just sit back
 * and focus on the more important things!
 */
 
/* Parts Required: 
 *  Arduino UNO board
 *  Mini breadboard
 *  Transistor
 *  Diode
 *  Moisutre sensor (with included connection module)
 *  Brushless DC submersible water pump
 *  Vinyl tubing
 *  Plastic water tank
 *  Plastic Arduino board housing
 *  5V Arduino compatible wall power adapter
 *  12V wall power adapter (to power the pump)
 *  Plant pot
 *  
 *  Created 21 July 2016
 *  by Nick Liccini
 */

/* Variables to initialize include moisture value, sensor, and actuator 
 * connection pins. You can choose different pins to connect to. Pin 13
 * is normally used as the board's built-in LED pin.
 */
int moistureVal;
unsigned long previousTime;
const int sensorPin = A0;
const int pumpPin = 9;
const int onBoardLED = 13;

/* These are the user's configurable variables depending on your needs:
 * The constant pumpTime is where you determine how long you want the pump 
 * to run when it is activated. Less time (in milliseconds) means less water.
 * The variable dryVal is the default "too dry" value for calibrating in wet
 * soil; it can be changed depending on how dry is "too dry" (as a default value
 * for when you calibrate in wet soil only). The constant interval is your
 * interval for how often you want to be reminded to check the water tank (in
 * milliseconds).
 */
const int pumpTime = 3500;
int dryVal = 550; // default dry soil value in case of wet calibration
const long interval = 173000000; // 2 days in milliseconds

/* The setup() method simply declares the pins' modes and sets everything intially
 * to off. Then it begins calibration to the "too dry" value using the intiate()
 * method.
 */
void setup() {
  pinMode(pumpPin, OUTPUT);
  digitalWrite(pumpPin, LOW);
  initiate();
} // end setup()

/* The main body of ArduPlant, the loop() method, continuously reads the value of 
 * the soil's moisture and prints it out the the Serial Monitor (visible when
 * connected to a computer), then checks to see if that moisture value is too dry
 * using the checkMoisture() method (a true or false value). If the soil is too
 * dry, then the microprocessor activates the pump, if not it continues reading
 * the sensor's values. Every couple of days, the onboard LED lights will flash
 * to remind you to check the level in the water tank. And that's all there is 
 * to it! More features to come such as temperature and humidity monitoring, light 
 * monitoring with control over artifical lighting fixtures, and more!
 */
void loop() {
  moistureVal = analogRead(sensorPin);
  Serial.println(" ");
  Serial.print("Moisture Level: ");
  Serial.println(moistureVal);
  Serial.println(" ");
  
  if(checkMoisture()) {
    pump(pumpTime);
  }
  else {
    // If soil is not too dry, do nothing.
  }

  wateringAlert();
} // end loop()

/* The initiate() method establishes a connection to the Serial Monitor and displays
 * a few messages, then it gives an indication that the system is online by using
 * the onboard LED, then it calibrates for the "too dry" value. 
 */
void initiate() {
  Serial.begin(9600);
  Serial.print("ArduPlant Status: ");
  Serial.println("online");
  delay(500);
  digitalWrite(onBoardLED, HIGH);
  dryVal = calibrate();
  Serial.print("Dry Value: ");
  Serial.println(dryVal);
  blinkLED(3);
  delay(500);
} // end initiate()

/* The calibrate() method uses the first 4.5 seconds to determine how dry is
 * "too dry" for your plant. If your plant has recently been watered when you
 * first set up ArduPlant, don't worry! ArduPlant knows this can happen and has
 * a default dry value that is a common value for soil that is in need of watering.
 */
int calibrate() {
  int reading;
  while(millis() < 4500) {
    reading = analogRead(sensorPin);
  }
  if(reading > dryVal && reading < 850) {
    return reading;
  }
  else {
    return dryVal;
  }
} // end calibrate()

/* The checkMoisture() boolean method is a comparason method that will return a 
 * true value if the current soil moisture level is more dry than the predetermined
 * "too dry" value, or it will return false if the soil is still moist.
 */
boolean checkMoisture() {
  if(moistureVal >= dryVal) {
    return true;
  }
  else {
    return false;
  }
} // end checkMoisture()

/* The pump() method is where the Arduino board tells the pump to turn on and water
 * your plant. It takes one parameter: duration. This simply tells the Arduino how
 * long to let the pump water for, and this is mutable in the loop() method. Some
 * plants need more water than others, some don't. You can configure the watering
 * needs for your specific plant!
 */
void pump(int duration) {
  Serial.println("Soil is dry...");
  digitalWrite(pumpPin, HIGH);
  Serial.println("Watering now.");
  Serial.println(" ");
  delay(duration);
  digitalWrite(pumpPin, LOW);
  Serial.println("Done.");
  Serial.println(" ");
  delay(1000);
  Serial.println("Soil is good.");
  Serial.println("Plant happy :)");
} // end pump()

/* The blinkLED method is used to, you guessed it, blink the onboard LED! It takes
 * one parameter: blinks. This is how many blinks you want to Arduino to perform. 
 * Remember, they're rapid blinks, each one lasts for only a quarter of a second,
 * so make sure to assign enough that you can notice them!
 */
void blinkLED(int blinks) {
  for(int i = 0; i < blinks; i++) {
    digitalWrite(onBoardLED, HIGH);
    delay(125);
    digitalWrite(onBoardLED, LOW);
    delay(125);
  }
} // end blinkLED()

/* The wateringAlert() method uses a timer to notify you every couple of days
 * to check the level of your water tank. It does so by constantly checking if
 * two days have passed, and if so, it rapidly blinks the onboard LED to however
 * many times you choose to set it. Default is 10 rapid blinks, but you can change
 * that by changing the parameters of the blink() method! More features to come 
 * such as an alert noise!
 */
void wateringAlert() {
  if(millis() - previousTime > interval) {
    previousTime += interval;
    Serial.println("Check water tank!");
    blinkLED(10);
    Serial.println(" ");
  } 
} // end wateringAlert()
