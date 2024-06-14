#include <Servo.h> // Include the Servo library for controlling the servo motor
#include <Wire.h> // Include the Wire library for I2C communication
#include <LiquidCrystal_I2C.h> // Include the LiquidCrystal_I2C library for LCD
LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

// Define pins for ultrasonic sensor, LED, and servo motor
#define TRIG_PIN 2  // Connect to HC-SR04 trigger pin
#define ECHO_PIN 3  // Connect to HC-SR04 echo pin
#define LED_PIN 5   // Connect to LED strip (anode or +)

// Constants for distance thresholds
const int MAX_DISTANCE = 30; // Distance threshold for LED behavior
const int MIN_DISTANCE = 15; // Minimum distance for dark red color

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

void setup() {
  myservo.attach(9);               // attaches the servo on pin 9 to the servo object
  //LED
  pinMode(TRIG_PIN, OUTPUT);      // Set TRIG_PIN as an output
  pinMode(ECHO_PIN, INPUT);       // Set ECHO_PIN as an input
  pinMode(LED_PIN, OUTPUT);       // Set LED_PIN as an output
  digitalWrite(LED_PIN, LOW);     // Turn off LED initially  
  lcd.begin();                    // initialize the lcd
  lcd.backlight();                // Turn on the LCD screen backlight
}

void loop() {
  for (pos = 0; pos <= 60; pos += 1) { // goes from 0 degrees to 60 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(10);                       // waits 10ms for the servo to reach the position
  }
  for (pos = 60; pos >= 0; pos -= 1) { // goes from 60 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(10);                       // waits 10ms for the servo to reach the position
  }
  // Measure distance
  long duration, distance;            // duration measures pulse round-trip time, and distance stores calculated distance.
  digitalWrite(TRIG_PIN, LOW);        // This line sets the trig pin low, preparing the HC-SR04 for an ultrasonic pulse.
  delayMicroseconds(2);               // The 2ms delay stabilizes the trigger signal before sending the ultrasonic pulse.
  digitalWrite(TRIG_PIN, HIGH);       // After a delay, setting the trig pin high triggers the ultrasonic sensor to emit a pulse.
  delayMicroseconds(10);              // The 10ms delay ensures pulse transmission before resetting the trigger pin low.
  digitalWrite(TRIG_PIN, LOW);        // This sets the trig pin back to low voltage 
  duration = pulseIn(ECHO_PIN, HIGH); // the pulseIn function counts the echo time
  distance = duration / 58;           // Calculate distance in cm 
  lcd.setCursor(1, 0);                // it sets cursor position on the LCD screen to column 1 and row 0
  lcd.print("Distance: ");            // it prints the distance
  lcd.setCursor(1, 1);                // it sets the cursor position to col 1 and row 1
  lcd.print(distance);
  lcd.println(" cm");
  delay(20);                          // This line introduces a delay of 20 milliseconds
  // Control LED based on distance
  if (distance != 0) 
  {
      if (distance > MAX_DISTANCE)
      {
        digitalWrite(LED_PIN, LOW); // Turn off LED
      }
      else if (distance < MIN_DISTANCE)
      {
        digitalWrite(LED_PIN, HIGH); // Set LED to dark red (adjust brightness as needed)
        delay(100);
      } 
    else 
{
      // Blinking effect
      int blinkSpeed = map(distance, MAX_DISTANCE, MIN_DISTANCE, 500, 100); // Adjust blinking speed
      digitalWrite(LED_PIN, HIGH);
      delay(blinkSpeed*2);
      digitalWrite(LED_PIN, LOW);
      delay(blinkSpeed/5);
    }
  }
  // Print distance for debugging
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
}
