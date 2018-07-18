#include <Adafruit_Sensor.h>
#include <Adafruit_LSM9DS1.h>
#include <SPI.h>
#include <String.h>

// i2c
Adafruit_LSM9DS1 lsm = Adafruit_LSM9DS1();
#define LSM9DS1_SCK A5
#define LSM9DS1_M6ISO 12
#define LSM9DS1_MOSI A4
#define LSM9DS1_XGCS 
#define LSM9DS1_MCS 5

void setupSensor()
{
  // 1.) Set the accelerometer range
  lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_16G);
  // 2.) Set the magnetometer sensitivity
  lsm.setupMag(lsm.LSM9DS1_MAGGAIN_4GAUSS);
  // 3.) Setup the gyroscope
  lsm.setupGyro(lsm.LSM9DS1_GYROSCALE_245DPS);
}

//data smoothing
const int numReadings = 8;

//get the data from gyroscope 
double readings_array_z[numReadings];
double readings_array_y[numReadings];
double readings_array_updown[numReadings];
double readings_array_leftright[numReadings];

int readIndex = 0;
double total_z = 0;
double total_y = 0;
double total_updown = 0;
double total_leftright = 0;
double average_z = 0;
double average_y = 0;
double average_updown = 0;
double average_leftright = 0;

//setup the botton pin
const int buttonPin = 7;
int buttonState;
int lastButtonState = LOW;

//setup the press button
const int buttonPin_press = 6;
int pressState;
int lastPressState = LOW;

//setup the mode button
const int buttonPin_mode = 8;
int modeState;
int lastModeState = LOW;
int button_num[2] = {0,0};
int button_count = 0;
int button_mode = 0;

//setup the button for car racing game
const int button_f_game = 9;
int racing_f_button;

bool alt_press = false;

//global variable for the game mode
boolean right_state = false;
boolean left_state = false;

//GUI pin mode indicater
int game_pin = 15;
int win_pin = 14;


/*****************************************************************************/
/*
 * data array
 */
//for the transfer data structure will be
//mouse_x, mouse_y, press, mode, up, down, left, right


void setup(){
  Wire.begin();
  Serial.begin(57600);
  pinMode(buttonPin, INPUT);
  pinMode(buttonPin_press, INPUT);
  pinMode(game_pin, OUTPUT);
  pinMode(win_pin, OUTPUT);
  
  //initalize all the reading to 0
  for ( int thisReading = 0; thisReading < numReadings; thisReading++)
  {
    readings_array_z[thisReading] = 0;
    readings_array_y[thisReading] = 0;
    readings_array_updown[thisReading] = 0;
    readings_array_leftright[thisReading] = 0;
  }
    
  while (!Serial) {
    delay(100); // will pause Zero, Leonardo, etc until serial console opens
  }
  
  //Serial.println("1");
  
  // Try to initialise and warn if we couldn't detect the chip
  if (!lsm.begin())
  {
    //Serial.println("Oops ... unable to initialize the LSM9DS1. Check your wiring!");
    while (1);
  }
  //Serial.println("Found LSM9DS1 9DOF");

  // helper to just set the default scaling we want, see above!
  delay(100);
 
  setupSensor();
}

void loop() 
{
  lsm.read();  /* ask it to read in the data */ 

  /* Get a new sensor event */ 
  sensors_event_t a, m, g, temp;
  lsm.getEvent(&a, &m, &g, &temp);  

  //subtract the last reading:
  total_z = total_z - readings_array_z[readIndex];
  total_y = total_y - readings_array_y[readIndex];
  total_updown = total_updown - readings_array_updown[readIndex];
  total_leftright = total_leftright - readings_array_leftright[readIndex];
  
  //read from the sensor:
  readings_array_z[readIndex] = g.gyro.z; //get the data from gyroscope from 
  readings_array_y[readIndex] = g.gyro.x;
  readings_array_updown[readIndex] = a.acceleration.y;
  readings_array_leftright[readIndex] = a.acceleration.x;
  
  //add the reading to the total:
  total_z = total_z + readings_array_z[readIndex];
  total_y = total_y + readings_array_y[readIndex];
  total_updown = total_updown + readings_array_updown[readIndex];
  total_leftright = total_leftright + readings_array_leftright[readIndex];
  readIndex = readIndex + 1;

  //if we are at the end of the array
  if(readIndex >= numReadings){
    readIndex = 0; //setup to the beginning
  }

  //calculate the average
  average_z = total_z / numReadings;
  average_y = total_y / numReadings;
  average_updown = total_updown / numReadings;
  average_leftright = total_leftright / numReadings;
  
  double z_arg = (average_z + 12)/10 -0.8; //deducte the offset value
  double y_arg = (average_y + 12)/10 -1.5;
  
  /*********************************************************/
  /*
   * car driving game mode
   */
  
  double updown_avg = (average_updown)*10;
  double leftright_avg = average_leftright*10;

  if(digitalRead(buttonPin_mode)== LOW && digitalRead(buttonPin) == LOW && digitalRead(buttonPin_press)== LOW)
  {
    Serial.println("1000");
  }
  if(digitalRead(buttonPin_mode)== HIGH)
  { 
    /*
     * for speical operation function for car driving; transfer data x, y, press, mode, up, down, left, right
     * 1000 release key
     * 998 right
     * 997 left
     * 996 down
     * 995 up
     */
    //click up and down key
    if(leftright_avg < -40)
    {
      Serial.println("998"); //998 means right
      }
    if(leftright_avg > 40)
    {
      Serial.println("997"); //997 means left
    }
    //click left and right key
    if(updown_avg < 0 ) // right turn
    { 
      Serial.println("996"); //996 keypress down
    }
/*
    if((leftright_avg > -40 && leftright_avg < 40) && (updown_avg > -20 && updown_avg < 20))
    {
      Serial.println("1000");//release all
    }
*/   
    
    if(updown_avg > 65) // left turn
    {
       Serial.println("995"); // keypress up
    }
  }

 if(digitalRead(buttonPin)== HIGH && digitalRead(buttonPin_press) == HIGH)
 {
  Serial.println("994");
  delay(100);
 }
  
 /*
  * cursor move function
  */
 double z_mouse = z_arg*8;
 double y_mouse = -y_arg*6;
 if(digitalRead(buttonPin)== HIGH && digitalRead(buttonPin_press) == LOW)
 {
   // in total 3 data, x, y
    Serial.println(String(int(z_mouse))+":"+String(int(y_mouse)));
 }

 /*
  * click and drag function
  */
  if(digitalRead(buttonPin)== LOW && digitalRead(buttonPin_press) == HIGH)
  {
    Serial.println(999);
    delay(200);
  }
  
  /*
   * set the button mode for differnt gui 
   * 0 cursor move function
   * 1 game mode
   */
/*
  modeState = digitalRead(buttonPin_mode);
  if(modeState == HIGH)
  {
    //button_num status 
    button_num[0] = button_num[1];
    button_num[1] = 1;
  }
  if(modeState == LOW)
  {
    button_num[0] = button_num[1];
    button_num[1] = 0;
  }

  //calculate the number of press
  if(button_num[0] + button_num[1] == 1) button_count++;

  if(int(button_count/2) == 2)
  {
    button_mode = 0;
    button_count = 0;
  }
  else
  {
    button_mode = int(button_count/2);
  }
*/
  delay(5);
}

