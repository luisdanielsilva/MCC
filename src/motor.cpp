#include "motor.h"

// ISR/main shared variables must be defined as volatile
// extern int input_data();

// Shared variables
int steps_per_revolution = 200;
unsigned long int travel_steps = 0;
int microstepping = 0;
long int total_laps = 0;
long int laps = 0;

// Internal to .cpp
int minutes = 60;
int leadscrew_pitch = 8;

// Function declarations
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

long int calculate_speed(long int speed){
  
  float steps_per_second = 0;         // truncating a float to int -> error chance here
  float temp_speed = 0;
  
  Serial.print("FUNCTION: Calculate_speed: ");
  Serial.print(speed);
  Serial.println(" RPM");

  steps_per_second = (speed * steps_per_revolution) / minutes;
  
  Serial.print("FUNCTION: steps_per_second: ");
  Serial.println(steps_per_second);

  temp_speed = (1 / steps_per_second);

  Serial.print("FUNCTION: temp_speed: ");
  Serial.println(temp_speed);
  
  temp_speed = temp_speed / 2;        // to find Ton and Toff
  
  Serial.print("FUNCTION: temp_speed/2: ");
  Serial.println(temp_speed);

  temp_speed = temp_speed / 0.000001; // to convert to microseconds (input to delayMicroseconds() function)

  Serial.print("FUNCTION: temp_speed/0,000001: ");
  Serial.println(temp_speed);

  temp_speed = (int) temp_speed;
  Serial.print("FUNCTION: (int)temp_speed: ");
  Serial.println(temp_speed);

  Serial.print("FUNCTION: _speed in microseconds:  ");
  Serial.print(temp_speed);
  Serial.println(" uS");
    
  return temp_speed;
}


unsigned long int calculate_travel_mm(long int ftravel_mm){
  
  Serial.print("Steps per Revolution: ");
  Serial.println(steps_per_revolution);
  Serial.print("Microstepping: ");
  Serial.println(microstepping);
  Serial.print("Leadscrew Pitch: ");
  Serial.println(leadscrew_pitch);

  travel_steps = ftravel_mm * ((steps_per_revolution * microstepping) / leadscrew_pitch);

  Serial.print("Travel Steps: ");
  Serial.println(travel_steps);
  
  return travel_steps;
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

void run_laps(int pin_ena, int pin_dir){
//  long int total_laps = 0;
//  total_laps = laps * steps_per_revolution * microstepping;
//  moved to the main cycle and changed run_sequence to include the laps
  Serial.print("Total Laps Steps: ");         // to be tested.
  Serial.println(total_laps);

  Serial.print("Laps: ");                   // to be tested.
  Serial.println(laps);

  Serial.print("Steps/rev: ");              // to be tested.
  Serial.println(steps_per_revolution);

  //Serial.print("MicroStepping: ");          // to be tested.
  //Serial.println(microstepping);

  // change direction
  change_direction(pin_ena, pin_dir, LEFT);

  // rotate motor
  //rotate_motor(total_laps);              // variables must be long otherwise we cannot do the same number of steps as others


  delay(1000);

  // change direction
  change_direction(pin_ena, pin_dir, RIGHT);

  // rotate motor
  //rotate_motor(total_laps);

  Serial.print(laps);
  Serial.println(" - Laps completed.");
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