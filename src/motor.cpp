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
int movement_pitch = 0;
int movement_type = 0;
unsigned long int movement_steps_temp = 0;

// AUTOMATION
int auto_cycles = 0;
int auto_positions = 0;
int auto_type = 0;

// Internal to .cpp
int minutes = 60;

// Function declarations

bool change_direction(int pin_ena, int pin_dir, bool direction){
    // this simply puts the direction on the pin, not the variable (COMMON MISTAKE). can be changed to check if direction has changed in the variable and if so to change in the pin otherwise let it go through
  delayMicroseconds(500);
    digitalWrite(pin_ena, HIGH);
  delayMicroseconds(100);

    digitalWrite(pin_dir, direction);

  delayMicroseconds(500);
    digitalWrite(pin_ena, LOW);
  delayMicroseconds(100);

  int temp = 99;
  temp = digitalRead(pin_dir);
  Serial.println(temp);
  
  return temp;
}

unsigned long int calculate_travel_laps(long int laps)
{
  unsigned long int steps = 0;

  Serial.print(F("Laps: "));
  Serial.println(laps);

  steps = laps * motor_steps_per_revolution;

  Serial.print(F("Steps: "));
  Serial.println(steps);
  
  return steps;
}

long int calculate_speed(long int speed){
  
  float steps_per_second = 0;         // truncating a float to int -> error chance here
  float temp_speed = 0;
  
  steps_per_second = (speed * motor_steps_per_revolution) / minutes;
  
  temp_speed = (1 / steps_per_second);

  temp_speed = temp_speed / 2;        // to find Ton and Toff

  temp_speed = temp_speed / 0.000001; // to convert to microseconds (input to delayMicroseconds() function)
  // Serial.print(F("temp_speed: "));
  // Serial.println(temp_speed,5);
  
  temp_speed = (int) temp_speed;

  Serial.print(F("Speed: "));
  Serial.print(speed);
  Serial.print(" RPM / ");
  Serial.print(F("SpS: "));
  Serial.print(steps_per_second);
  Serial.print(F(" / Speed:  "));
  Serial.print(temp_speed);
  Serial.println(" uS");
    
  return temp_speed;
}

unsigned long int calculate_travel_mm(long int ftravel_mm, int thread_pitch){
  
  // all variables here should be local
  Serial.print(F("Steps per Revolution: "));
  Serial.println(motor_steps_per_revolution);
  Serial.print(F("Microstepping: "));
  Serial.println(motor_microstepping);
  Serial.print(F("Leadscrew Pitch: "));
  Serial.println(thread_pitch);

  movement_steps = ftravel_mm * ((motor_steps_per_revolution * motor_microstepping) / thread_pitch);

  Serial.print(F("Travel Steps: "));
  Serial.println(movement_steps);
  
  return movement_steps;
}

void rotate_motor(int pin_ena, int pin_pul, unsigned long int motor_steps, long int speed){
  Serial.println("START");
  
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
  
  Serial.println("STOP");
}

void run_sequence(int pin_ena, int pin_dir, int pin_pul, int cycles, int positions, int type){
   
  Serial.println(F("Running sequence"));
  Serial.print(F("Cycles:    "));
  Serial.println(cycles);
  Serial.print(F("Positions: "));
  Serial.println(positions);

  for(int j=1; j<=cycles; j++)
  {
    Serial.print(F("Cycle:      "));
    Serial.println(j);
    
    change_direction(ENA, DIR, motor_direction);

    for(int i=1; i<=positions; i++)
    {
      Serial.print(F("FWD Pos.: "));
      Serial.print("DIR: ");
      Serial.println(motor_direction);
      Serial.println(i);
      
      rotate_motor(ENA, PUL, movement_steps_temp, motor_speed_temp);          

      delay(1000);
    }
    delay(1000);

    Serial.println(motor_direction);
    motor_direction = !motor_direction;
        change_direction(ENA, DIR, motor_direction);              // direction must be changed here. It is the contrary to whatever was selected before
    Serial.println(motor_direction);

    for(int i=1; i<=positions; i++)
    {
      Serial.print(F("BWD Pos.: "));
      Serial.print("DIR: ");
      Serial.println(motor_direction);
      Serial.println(i);
      rotate_motor(ENA, PUL, movement_steps_temp, motor_speed_temp);          
      delay(1000);
    }
    motor_direction = !motor_direction;
  }
  Serial.print(F("Sequence complete!"));
}