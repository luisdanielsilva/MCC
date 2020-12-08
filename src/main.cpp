#include <Arduino.h>

#include "main.h"
#include "motor.h"
#include "serial.h"

// #define INFO 
// NANO Configuration
 int PUL=4; //define Pulse pin
 int DIR=3; //define Direction pin
 int ENA=2; //define Enable Pin

// MEGA 2560 Configuration
// int PUL=9;                   //define Pulse pin
// int DIR=8;                   //define Direction pin
// int ENA=10;                   //define Enable Pin

String mcc_version = "2.4";
String mcc_date = "24/11/2020";
String mcc_coder = "Luis Silva";

long int input_value = 0;
long int _speed = 0;
long int temp_speed_rpm = 0;
long int speed_rpm = 0;
long int steps = 0;
int cycles = 0;
int positions = 0;
int destiny_position = 0;
long int rpm = 0;
long int travel_mm = 0;
bool option_mm_steps = 0;


// control flag to show the menu
boolean refresh_commands = false;

void menu_print(){
  Serial.println(F(" ********************** MOTOR CONTROL CENTER ********************** "));
  Serial.println(F("p   --> Display Parameters"));
  Serial.println(F("ss  --> Set Speed (tON/tOFF)"));
  Serial.println(F("ssr --> Set Speed (RPM)"));
  Serial.println(F("sd  --> Configure direction"));
  Serial.println(F("sst --> Set Steps"));
  Serial.println(F("smm --> Set mm"));
  Serial.println(F("spr --> Set Steps per Revolution (default = 200)"));
  Serial.println(F("sm  --> Set MicroStepping (default = 1)"));
  Serial.println(F("sp  --> Set Positions"));
  Serial.println(F("sc  --> Set Cycles"));
  Serial.println(F("sl  --> Set Laps"));
  Serial.println(F("1   --> Move LEFT x steps"));
  Serial.println(F("2   --> Move RIGHT x steps"));
  Serial.println(F("3   --> Move LEFT 1 position"));
  Serial.println(F("4   --> Move RIGHT 1 position"));
  Serial.println(F("5   --> Move LEFT x mm"));
  Serial.println(F("6   --> Move RIGHT x mm"));
  Serial.println(F("7   --> Go to Target Position"));
  Serial.println(F("8   --> Steps (0) or mm (1)"));
  Serial.println(F("rs  --> Run Sequence"));
  Serial.println(F("rl  --> Run Laps"));
  Serial.println(F("i   --> Info"));
  Serial.println(" ");
}

void info_print(){
  Serial.println("INFORMATION ");
  Serial.print("VERSION:  ");
  Serial.println(mcc_version);
  Serial.print("DATE:     ");
  Serial.println(mcc_date);
  Serial.print("CODER:    ");
  Serial.println(mcc_coder);
  Serial.println(" ");
}

void variable_print(){
  Serial.print("Speed:         ");
  Serial.print(_speed); 
  Serial.println(" us");
  Serial.print("Speed:         ");
  Serial.print(temp_speed_rpm); 
  Serial.println(" RPM");
  Serial.print("Steps:         ");
  Serial.println(steps);
  Serial.print("Steps per Revolution: ");
  Serial.println(steps_per_revolution);
  Serial.print("Microstepping: ");
  Serial.println(microstepping);
  Serial.print("Positions:     ");
  Serial.println(positions);
  Serial.print("Cycles:        ");
  Serial.println(cycles);
  Serial.print("Laps:          ");
  Serial.println(laps);
  Serial.print("Travel :       ");
  Serial.print(travel_mm);
  Serial.println(" mm");
  if(option_mm_steps == 0)
  {
    Serial.print(option_mm_steps);
    Serial.println(" - STEPS Configured");
  }
  if(option_mm_steps == 1)
  {
    Serial.print(option_mm_steps);
    Serial.println(" - mm Configured");
  }
  Serial.print("Direction:     ");
  read_direction(DIR);                    
}

void setup() {
  pinMode (PUL, OUTPUT);
  pinMode (DIR, OUTPUT);
  pinMode (ENA, OUTPUT);

  Serial.begin(115200);
  
  digitalWrite(ENA, HIGH);

  menu_print();
}

void loop()
{ 
  if(Serial.available()){
        command = Serial.readStringUntil('\n');

        if(command.equals("p")){
            variable_print();
            refresh_commands == true;
        }
        if(command.equals("ss")){
            //Serial.println("0");
            _speed = input_data();
            Serial.print(_speed);
            Serial.println(" uS");
            refresh_commands == true;
        }
        if(command.equals("ssr")){
            //Serial.println("ssr");
            temp_speed_rpm = input_data();
            _speed = calculate_speed(temp_speed_rpm);
            refresh_commands == true;
        }
        if(command.equals("sd")){                   // checks if one direction is set, changes and then changes back again
            if(digitalRead(DIR) == HIGH)
            {
              change_direction(ENA, DIR, RIGHT);
              Serial.println("Direction changed to: RIGHT");
            }
            else
            {
              if(digitalRead(DIR) == LOW)
              {
                change_direction(ENA, DIR, LEFT);
                Serial.println("Direction changed to: LEFT");
              }
            }
            refresh_commands == true;
        }
        if(command.equals("sst")){
            //Serial.println("sst");
            steps = input_data();
            Serial.println(" ");
            Serial.print(steps);
            Serial.println(" - steps configured");
            refresh_commands == true;
        }
        if(command.equals("smm")){
            //Serial.println("smm");
            travel_mm = input_data();
            Serial.print(travel_mm);
            Serial.println(" mm configured");
            refresh_commands == true;
        }
        if(command.equals("sm")){
            //Serial.println("sm");
            Serial.println("sm --> how many microsteps?");
            microstepping = input_data();
            Serial.println(microstepping);
            refresh_commands == true;
        }
        if(command.equals("spr")){
            Serial.println("spr --> how many steps per revolution?");
            steps_per_revolution = input_data();
            Serial.println(steps_per_revolution);
            refresh_commands == true;
        }
        if(command.equals("sp")){
            //Serial.println("sp");
            positions = input_data();
            Serial.println("sp --> Configure Positions");
            Serial.println(positions);
            refresh_commands == true;
        }
        if(command.equals("sc")){
            //Serial.println("sc");
            cycles = input_data();
            Serial.println("sc --> Configure Cycles");
            Serial.println(cycles);
            refresh_commands == true;
        }
        if(command.equals("sl")){
            //Serial.println("sl");
            Serial.println("sl --> how many laps?");
            laps = input_data();
            Serial.print(laps);
            refresh_commands == true;
        }
        if(command.equals("1")){
            //Serial.println("1");
            change_direction(ENA, DIR, LEFT);
            steps = input_data();       // Asks before for how many steps to rotate and changes the value.
            rotate_motor(ENA, PUL, steps, _speed);
            refresh_commands == true;
        }
        if(command.equals("2")){
            //Serial.println("2");
            change_direction(ENA, DIR, RIGHT);
            steps = input_data();       // Asks before for how many steps to rotate and changes the value.
            rotate_motor(ENA, PUL, steps, _speed);
            refresh_commands == true;
        }
        if(command.equals("3")){
            //Serial.println("3");
            change_direction(ENA, DIR, LEFT);
            rotate_motor(ENA, PUL, steps, _speed);
            refresh_commands == true;
        }
        if(command.equals("4")){
            //Serial.println("4");
            change_direction(ENA, DIR, RIGHT);
            rotate_motor(ENA, PUL, steps, _speed);
            refresh_commands == true;
        }
        if(command.equals("5")){
            //Serial.println("5");
            travel_mm = input_data();
            calculate_travel_mm(travel_mm);
            change_direction(ENA, DIR, LEFT);
            rotate_motor(ENA, PUL, steps, _speed);
            refresh_commands == true;
        }
        if(command.equals("6")){
            //Serial.println("6");
            travel_mm = input_data();
            rotate_motor(ENA, PUL, steps, _speed);
            change_direction(ENA, DIR, RIGHT);
            rotate_motor(ENA, PUL, steps, _speed);
            refresh_commands == true;
        }
        if(command.equals("7")){
            //Serial.println("7");
            destiny_position = input_data();
            total_laps = laps * steps_per_revolution * microstepping;
            go_to_position(ENA, DIR, destiny_position);         
            refresh_commands == true;
        }
        if(command.equals("8")){
            //Serial.println("8");
            Serial.println("Please select:");
            Serial.println("0 = STEPS");
            Serial.println("1 = mm");
            option_mm_steps = input_data();
            if(option_mm_steps == 0)
            {
              Serial.print(option_mm_steps);
              Serial.println(" - STEPS Configured");
            }
            if(option_mm_steps == 1)
            {
              Serial.print(option_mm_steps);
              Serial.println(" - mm Configured");
            }
            refresh_commands == true;  
        }
        if(command.equals("rs")){
            Serial.println("rs --> Run Sequence");
            variable_print();
            delay(3000);
            total_laps = laps * steps_per_revolution * microstepping;
            // run the planned sequence
            run_sequence(ENA, DIR, PUL, cycles, positions);
            refresh_commands == true;
        }
        if(command.equals("rl")){
            //Serial.println("rl");
            Serial.println("rl --> how any laps? ");
            variable_print();
            delay(3000);
            total_laps = laps * steps_per_revolution * microstepping;
            // calculate laps and activate motor
            run_laps(ENA, DIR);
            refresh_commands == true;
        }
        if(command.equals("i")){
            info_print();
            refresh_commands == true;
        }
        else{
           Serial.println("Invalid command");
           refresh_commands = true;
          }        
      refresh_commands == true;
  }
    if(refresh_commands == true){
      menu_print();
      refresh_commands = false;
    }
}

