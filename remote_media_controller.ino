#include <HCSR04.h>             // ultrasonic sensors
#include <Wire.h>
#include <SparkFun_APDS9960.h>  // gesture sensor
#include <math.h>
#include <SPI.h>
#define APDS9960_INT    2       // needs to be an interrupt pin

// global variables
SparkFun_APDS9960 apds = SparkFun_APDS9960();
int isr_flag = 0;

//set up left ultrasonic sensor
//initialize with trigger pin 7 and echo pin 6
UltraSonicDistanceSensor distanceSensor_left(7, 6);  
//set up right ultrasonic sensor
//initialize with trigger pin 4 and echo pin 3
UltraSonicDistanceSensor distanceSensor_right(4, 3);

int distance_left;  // set up global variable for left ultrasonic sensor
int distance_right; // set up global variable for right ultrasonic sensor

/*
* read distance function
* step the maximum ultrasonic value to 40 and minimum to 0 to avoid data distortion
*/
double f_distance(double currentvalue)
{
  return (currentvalue > 40) ? 40 : currentvalue;
  return (currentvalue < 0) ? 0 : currentvalue;
}

void interruptRoutine() {
  isr_flag = 1;
}

int mode = 1; // indicator for Normal or Movie Mode

// Gesture Sensor
void handleGesture() {
    if ( apds.isGestureAvailable() ) {  // Normal Mode
    if (mode % 2 == 1) {
      switch ( apds.readGesture() ) {
      case DIR_UP:
        Serial.println("Normal UP");
        digitalWrite(9, HIGH);          // blink red LED as signifier for correct movement
        delay(50);
        digitalWrite(9, LOW);
        delay(150);                     // total delay = 200
        break;
      case DIR_DOWN:
        Serial.println("Normal DOWN");
        digitalWrite(9, HIGH);
        delay(50);
        digitalWrite(9, LOW);       
        delay(150);
        break;
      case DIR_LEFT:
        Serial.println("Normal LEFT");
        digitalWrite(9, HIGH);
        delay(50);
        digitalWrite(9, LOW);
        delay(150);
        break;
      case DIR_RIGHT:
        Serial.println("Normal RIGHT");
        digitalWrite(9, HIGH);
        delay(50);
        digitalWrite(9, LOW);
        delay(150);
        break;
      case DIR_NEAR:
        Serial.println("Normal NEAR");
        digitalWrite(9, HIGH);
        delay(50);
        digitalWrite(9, LOW);
        delay(250);                     // total delay = 300
        break;
      case DIR_FAR:
        Serial.println("Normal FAR");
        digitalWrite(9, HIGH);
        delay(50);
        digitalWrite(9, LOW);        
        delay(250);                     // total delay = 300
        mode = mode+1;                  // change to Movie Mode
        digitalWrite(11, LOW);          // turn off green LED for Normal Mode
        digitalWrite(10, HIGH);         // turn on green LED for Movie Mode
        Serial.println("You are now in Movie Mode");
        break;
      default:
        Serial.println("TRY AGAIN! Go slower");
        digitalWrite(8, HIGH);          // blink white LED as signifier for wrong movement
        delay(100);
        digitalWrite(8, LOW);        
    }
    }
    else if (mode % 2 == 0) {  // Movie Mode
      switch ( apds.readGesture() ) {
      case DIR_UP:
        Serial.println("Movie UP");
        digitalWrite(9, HIGH);         // blink red LED as signifier for correct movement
        delay(50);
        digitalWrite(9, LOW);
        delay(150);                    // total delay = 200
        break;
      case DIR_DOWN:
        Serial.println("Movie DOWN");
        digitalWrite(9, HIGH);
        delay(50);
        digitalWrite(9, LOW);
        delay(150);
        break;
      case DIR_LEFT:
        Serial.println("Movie LEFT");
        digitalWrite(9, HIGH);
        delay(50);
        digitalWrite(9, LOW);
        delay(150);
        break;
      case DIR_RIGHT:
        Serial.println("Movie RIGHT");
        digitalWrite(9, HIGH);
        delay(50);
        digitalWrite(9, LOW);
        delay(150);
        break;
      case DIR_NEAR:
        Serial.println("Movie NEAR");
        digitalWrite(9, HIGH);
        delay(50);
        digitalWrite(9, LOW);
        delay(250);                    // total delay = 300
        break;
      case DIR_FAR:
        Serial.println("Movie FAR");
        digitalWrite(9, HIGH);
        delay(50);
        digitalWrite(9, LOW);
        delay(250);                    // total delay = 300
        mode = mode+1;                 // change to Normal Mode
        digitalWrite(10, LOW);         // turn off green LED for Movie Mode
        digitalWrite(11, HIGH);        // turn on green LED for Normal Mode
        Serial.println("You are now in Normal Mode");
        break;
      default:
        Serial.println("TRY AGAIN! Go slower");
        digitalWrite(8, HIGH);         // blink white LED as signifier for wrong movement
        delay(100);
        digitalWrite(8, LOW); 
       }
    }
  }
}

void setup() {
  pinMode(8, OUTPUT);   // white LED: try again
  pinMode(9, OUTPUT);   // red LED: gesture detected
  pinMode(10, OUTPUT);  // green LED: Movie Mode
  pinMode(11, OUTPUT);  // green LED: Normal Mode
  pinMode(12, OUTPUT);  // blue LED: ultrasonic right Active
  pinMode(13, OUTPUT);  // blue LED: ultrasonic left Active

  // set interrupt pin as input
  pinMode(APDS9960_INT, INPUT);
  // initialize Serial port
  Serial.begin(57600);
  Serial.println();
  Serial.println(F("--------------------------------"));
  Serial.println(F("SparkFun APDS-9960 - GestureTest"));
  Serial.println(F("--------------------------------"));
  
  // initialize interrupt service routine
  attachInterrupt(0, interruptRoutine, FALLING);

  // initialize APDS-9960 (configure I2C and initial values)
  if ( apds.init() ) {
    Serial.println(F("APDS-9960 initialization complete"));
  } else {
    Serial.println(F("Something went wrong during APDS-9960 init!"));
  }
  
  // start running the APDS-9960 gesture sensor engine
  if ( apds.enableGestureSensor(true) ) {
    Serial.println(F("Gesture sensor is now running"));
  } else {
    Serial.println(F("Something went wrong during gesture sensor init!"));
  }
}

void loop() {
  if( isr_flag == 1 ) {
    detachInterrupt(0);
    handleGesture();
    isr_flag = 0;
    attachInterrupt(0, interruptRoutine, FALLING);
  }
    // temp variable to confirm the Active Mode, in which the ultrasonic functions would be activated
    int dist_temp_left;
    distance_left = int(f_distance(distanceSensor_left.measureDistanceCm()));
    // every 500 miliseconds, do a measurement using the sensor and print the distance in centimeters
    Serial.println(distance_left);

    // when user's hand distance is ~ 15cm in front of the left ultrasonic sensor
    if(distance_left >= 10 && distance_left <= 15)
    {
      delay(500); // hold hand there for 0,5 second
      dist_temp_left = int(f_distance(distanceSensor_left.measureDistanceCm()));
      if(fabs(dist_temp_left - distance_left) <= 2)
      {
        // Active Mode for left ultrasonic
        Serial.println("LEFT Ultrasonic Active -- Pull back and hold to 'VOLUME UP' / Push forward and hold to 'VOLUME DOWN'");
        digitalWrite(13, HIGH); // blue LED on
        delay(700);
        while(f_distance(distanceSensor_left.measureDistanceCm()) < 35)
        {
          distance_left = int(f_distance(distanceSensor_left.measureDistanceCm()));
          if (mode % 2 == 1) {    // Normal Mode
          if(distance_left < 10)  // gentle push
          {
            Serial.println("A");
            digitalWrite(13, LOW);
            delay(50);
            digitalWrite(13, HIGH);
            delay(500);               // total delay = 550
          }
          if(distance_left > 15)  // gentle pull
          {
            Serial.println("B");
            digitalWrite(13, LOW);
            delay(50);
            digitalWrite(13, HIGH);
            delay(500); 
          }
          }
          else if (mode % 2 == 0) { // Movie Mode
          if(distance_left < 10)  // gentle push
          {
            Serial.println("C");
            digitalWrite(13, LOW);
            delay(50);
            digitalWrite(13, HIGH);
            delay(500);
          }
          if(distance_left > 15) // gentle pull
          {
            Serial.println("D");
            digitalWrite(13, LOW);
            delay(50);
            digitalWrite(13, HIGH);
            delay(500);
          }
         }
        }
      }
    }
    digitalWrite(13, LOW);  // blue LED off
    delay(100);

    // temp variable to confirm the Active Mode, in which the ultrasonic functions would be activated
    int dist_temp_right;
    distance_right = int(f_distance(distanceSensor_right.measureDistanceCm()));
    // every 500 miliseconds, do a measurement using the sensor and print the distance in centimeters
    Serial.println(distance_right);

    // when user's hand distance is ~ 15cm in front of the right ultrasonic sensor
    if(distance_right >= 10 && distance_right <= 15)
    {
      delay(500); // hold hand there for 0,5 second
      dist_temp_right = int(f_distance(distanceSensor_right.measureDistanceCm()));
      if(fabs(dist_temp_right - distance_right) <= 2)
      {
        // Active Mode for right ultrasonic
        Serial.println("RIGHT Ultrasonic Active -- Pull back and hold to 'ZOOM OUT' / Push forward and hold to 'ZOOM IN'");
        digitalWrite(12, HIGH); // blue LED on
        delay(700);
        while(f_distance(distanceSensor_right.measureDistanceCm()) < 35)
        {
          distance_right = int(f_distance(distanceSensor_right.measureDistanceCm()));
          if (mode % 2 == 1) {  // Normal Mode
          if(distance_right < 10) // gentle push
          {
            Serial.println("E");
            digitalWrite(12, LOW);
            delay(50);
            digitalWrite(12, HIGH);
            delay(500);               // total delay = 550
          }
          if(distance_right > 15) // gentle pull
          {
            Serial.println("F");
            digitalWrite(12, LOW);
            delay(50);
            digitalWrite(12, HIGH);
            delay(500); 
          }
         }
          else if (mode % 2 == 0) { // Movie Mode
          if(distance_right < 10) // gentle push
          {
            Serial.println("G");
            digitalWrite(12, LOW);
            delay(50);
            digitalWrite(12, HIGH);
            delay(500);
          }
          if(distance_right > 15) // gentle pull
          {
            Serial.println("H");
            digitalWrite(12, LOW);
            delay(50);
            digitalWrite(12, HIGH);
            delay(500);
          }
         }
        }
      }
    }
    digitalWrite(12, LOW);  // blue LED off
    delay(100);
}

