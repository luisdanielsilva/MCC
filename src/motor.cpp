#include "motor.h"

// ISR/main shared variables must be defined as volatile
// extern int input_data();

// NANO Configuration
// int PUL=4; //define Pulse pin
// int DIR=3; //define Direction pin
// int ENA=2; //define Enable Pin

// MEGA 2560 Configuration
// int PUL=9;                   //define Pulse pin
// int DIR=8;                   //define Direction pin
// int ENA=10;                   //define Enable Pin


// Shared variables

// MOTOR INTERFACE
int ENA = 0;
int PUL = 0;
int DIR = 0;

// MOTOR
long int motor_speed_rpm = 0;
long int motor_steps_per_revolution = 0;
int motor_microstepping = 0;
bool motor_direction = 0;
long int motor_speed_temp = 0;

// MOVEMENT
unsigned long int movement_steps = 0;
long int movement_laps = 0;
unsigned long int movement_mm = 0;
int movement_type = 0;
unsigned long int movement_steps_temp = 0;

// AUTOMATION
int auto_cycles = 0;
int auto_positions = 0;
int auto_type = 0;


// Internal to .cpp
int minutes = 60;
int leadscrew_pitch = 8;

// Function declarations

void setupMotor(int pin_ena)
{
  digitalWrite(pin_ena, LOW);

  Serial.println("Turning off the output stage of motor driver");
}

bool change_direction(int pin_ena, int pin_dir, bool direction){

  delayMicroseconds(500);
    digitalWrite(pin_ena,HIGH);
  delayMicroseconds(100);
    digitalWrite(pin_dir,direction);
  delayMicroseconds(500);
    digitalWrite(pin_ena,LOW);
  delayMicroseconds(100);
    Serial.print("Read Dir: ");
    Serial.println(digitalRead(pin_dir));
  
  return digitalRead(pin_dir);
}

unsigned long int calculate_travel_laps(long int movement_laps)
{
  unsigned long int steps = 0;

  // convert laps to steps here

  return steps;
}

long int calculate_speed(long int speed){
  
  float steps_per_second = 0;         // truncating a float to int -> error chance here
  float temp_speed = 0;
  
  Serial.print(F("Calculate_speed: "));
  Serial.print(speed);
  Serial.println(" RPM");

  steps_per_second = (speed * motor_steps_per_revolution) / minutes;
  
  Serial.print(F("steps_per_second: "));
  Serial.println(steps_per_second);

  temp_speed = (1 / steps_per_second);

  Serial.print(F("temp_speed: "));
  Serial.println(temp_speed,4);
  
  temp_speed = temp_speed / 2;        // to find Ton and Toff
  
  Serial.print(F("temp_speed/2: "));
  Serial.println(temp_speed,4);

  temp_speed = temp_speed / 0.000001; // to convert to microseconds (input to delayMicroseconds() function)

  Serial.print(F("temp_speed/0,000001: "));
  Serial.println(temp_speed,4);

  temp_speed = (int) temp_speed;
  Serial.print(F("(int)temp_speed: "));
  Serial.println(temp_speed,4);

  Serial.print(F("_speed in microseconds:  "));
  Serial.print(temp_speed);
  Serial.println(" uS");
    
  return temp_speed;
}

unsigned long int calculate_travel_mm(long int ftravel_mm){
  
  // all variables here should be local
  Serial.print("Steps per Revolution: ");
  Serial.println(motor_steps_per_revolution);
  Serial.print("Microstepping: ");
  Serial.println(motor_microstepping);
  Serial.print("Leadscrew Pitch: ");
  Serial.println(leadscrew_pitch);

  movement_steps = ftravel_mm * ((motor_steps_per_revolution * motor_microstepping) / leadscrew_pitch);

  Serial.print("Travel Steps: ");
  Serial.println(movement_steps);
  
  return movement_steps;
}

void rotate_motor(int pin_ena, int pin_pul, unsigned long int motor_steps, long int speed){
  Serial.println("START");
  Serial.print("Rotating: ");
  Serial.print(motor_steps);
  Serial.println(" steps");
  
  digitalWrite(pin_ena, LOW);
  
  delay(500);
  
  cli();                                          // disable interrupt  before rotating the motor
  for (unsigned long int i=0; i<motor_steps; i++)
  {
    digitalWrite(pin_pul,HIGH);
    delayMicroseconds(speed);
    digitalWrite(pin_pul,LOW);
    delayMicroseconds(speed);
  }
  sei();                                          // enable interrupts after the motor has rotated
  
  //  DEBUG 
  // to avoid the motor outputs to be disconnected immediately after the last step 
  // is taken and the motor moving. Might need adjustment ...
  delay(1000);
  //  DEBUG
  digitalWrite(pin_ena, HIGH);
  
  Serial.print("Rotated: ");
  Serial.print(motor_steps);
  Serial.println(" steps");
  Serial.println("STOP");
}








bool read_direction(int pin_dir){
  
  if(digitalRead(pin_dir) == HIGH)
  {
    Serial.println("LEFT");
  }
  else
  {
    if(digitalRead(pin_dir) == LOW)
    {
      Serial.println("RIGHT");
    }
  }
  return digitalRead(pin_dir);
}


void go_to_position(int pin_ena, int pin_dir, int target_position){
  
  change_direction(pin_ena, pin_dir, LEFT); 
  
  Serial.print("Moving to position: ");
  Serial.println(target_position);

  for(int i=0; i<target_position; i++)
    {
      Serial.print("POSITION: ");
      Serial.println(i+1);
      //rotate_motor(total_laps);
      delay(1000);
    }
  Serial.println("Target Position reached!");
  delay(3000);
  Serial.println("Going Back.");
  change_direction(pin_ena, pin_dir, RIGHT); 

  for(int i=0; i<target_position; i++)
    {
      Serial.print("POSITION: ");
      Serial.println(i+1);
      //rotate_motor(total_laps);
      delay(1000);
    }
}


void run_sequence(int pin_ena, int pin_dir, int pin_pul, int cycles, int positions){
  int j = 0;
    
  for(j=0; j<cycles; j++)
  {
    Serial.print("Cycle:      ");
    Serial.println(j);
    // Start always moving to LEFT
    change_direction(pin_ena, pin_dir, LEFT);
    delay(1000);
    Serial.println("Direction changed to: LEFT");
    
    for(int i=0; i<positions; i++)
    {
      Serial.print("FWD Pos.: ");
      Serial.println(i);
      /* if(option_mm_steps == 0)
      {
        //rotate_motor(total_laps);
      }
      if(option_mm_steps == 1)
      {
        calculate_travel_mm(travel_mm);
        
        Serial.print("TRAVEL_STEPS: ");
        Serial.println(travel_steps);
        
        rotate_motor(pin_ena, pin_pul, travel_steps, _speed);
      }  */   
      delay(1000);
    }
    delay(1000);
    change_direction(pin_ena, pin_dir, RIGHT);
    Serial.println("Direction changed to: RIGHT");
    delay(1000);
    
    for(int i=0; i<positions; i++)
    {
      Serial.print("BWD Pos.: ");
      Serial.println(i);
     /*  if(option_mm_steps == 0)
      {
        //rotate_motor(total_laps);
      }
      if(option_mm_steps == 1)
      {
        calculate_travel_mm(travel_mm);
        rotate_motor(pin_ena, pin_pul, travel_steps, _speed);
      } */
      delay(1000);
    }
  }
}