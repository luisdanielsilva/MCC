#include <Arduino.h>


// FUNCTION PROTOTYPES
bool change_direction(bool direction);
void rotate_motor(unsigned long int motor_steps);
unsigned long int calculate_travel_mm(long int ftravel_mm);
bool read_direction();


//#define INFO 
// NANO Configuration
 int PUL=4; //define Pulse pin
 int DIR=3; //define Direction pin
 int ENA=2; //define Enable Pin

// MEGA 2560 Configuration
// int PUL=9;                   //define Pulse pin
// int DIR=8;                   //define Direction pin
// int ENA=10;                   //define Enable Pin

#define LEFT 1
#define RIGHT 0



String mcc_version = "2.4";
String mcc_date = "24/11/2020";
String mcc_coder = "Luis Silva";

int steps_per_revolution = 200;
int minutes = 60;
long int input_value = 0;
long int _speed = 0;
long int temp_speed_rpm = 0;
long int speed_rpm = 0;
long int steps = 0;
int cycles = 0;
int positions = 0;
int destiny_position = 0;
long int laps = 0;
long int rpm = 0;
int microstepping = 0;
long int total_laps = 0;
long int travel_mm = 0;
unsigned long int travel_steps = 0;
int leadscrew_pitch = 8;
bool option_mm_steps = 0;

// Serial Commands
String command;
String inString = "";

// control flag to show the menu
boolean refresh_commands = false;

// DIRECTION LOW - MOVES RIGHT
// DIRECTION HIGH - MOVES LEFT


void go_to_position(int target_position){
  
  change_direction(LEFT); 
  
  Serial.print("Moving to position: ");
  Serial.println(target_position);

  for(int i=0; i<target_position; i++)
    {
      Serial.print("POSITION: ");
      Serial.println(i+1);
      rotate_motor(total_laps);
      delay(1000);
    }
  Serial.println("Target Position reached!");
  delay(3000);
  Serial.println("Going Back.");
  change_direction(RIGHT); 

  for(int i=0; i<target_position; i++)
    {
      Serial.print("POSITION: ");
      Serial.println(i+1);
      rotate_motor(total_laps);
      delay(1000);
    }
}

void run_sequence(int cycles, int positions){
  int j = 0;
    
  for(j=0; j<cycles; j++)
  {
    Serial.print("Cycle:      ");
    Serial.println(j);
    // Start always moving to LEFT
    change_direction(LEFT);
    delay(1000);
    Serial.println("Direction changed to: LEFT");
    
    for(int i=0; i<positions; i++)
    {
      Serial.print("FWD Pos.: ");
      Serial.println(i);
      if(option_mm_steps == 0)
      {
        rotate_motor(total_laps);
      }
      if(option_mm_steps == 1)
      {
        calculate_travel_mm(travel_mm);
        
        Serial.print("TRAVEL_STEPS: ");
        Serial.println(travel_steps);
        
        rotate_motor(travel_steps);
      }    
      delay(1000);
    }
    delay(1000);
    change_direction(RIGHT);
    Serial.println("Direction changed to: RIGHT");
    delay(1000);
    
    for(int i=0; i<positions; i++)
    {
      Serial.print("BWD Pos.: ");
      Serial.println(i);
      if(option_mm_steps == 0)
      {
        rotate_motor(total_laps);
      }
      if(option_mm_steps == 1)
      {
        calculate_travel_mm(travel_mm);
        rotate_motor(travel_steps);
      }
      delay(1000);
    }
  }
}

void rotate_motor(unsigned long int motor_steps){
  Serial.println("START");
  Serial.print("Rotating: ");
  Serial.print(motor_steps);
  Serial.println(" steps");
  
  digitalWrite(ENA, LOW);
  delay(500);
  for (unsigned long int i=0; i<motor_steps; i++)
  {
    digitalWrite(PUL,HIGH);
    delayMicroseconds(_speed);
    digitalWrite(PUL,LOW);
    delayMicroseconds(_speed);
    //Serial.println(i);
  }
  //  DEBUG 
  // to avoid the motor outputs to be disconnected immediately after the last step 
  // is taken and the motor moving. Might need adjustment ...
  delay(1000);
  //  DEBUG
  digitalWrite(ENA, HIGH);
  
  Serial.print("Rotated: ");
  Serial.print(motor_steps);
  Serial.println(" steps");
  Serial.println("STOP");
}

int input_data(){
  int inChar = 0;
  boolean flag = false;
  
  Serial.println("How many?");
  Serial.println(" ");

  do{
     while (Serial.available() > 0) {
        inChar = Serial.read();
        if (isDigit(inChar)) {
          // convert the incoming byte to a char and add it to the string:
          inString += (char)inChar;
        }
        // if you get a newline, print the string, then the string's value:
        if (inChar == '\n') {
          input_value = inString.toInt();
          
          // clear the string for new input:
          inString = "";
          flag = true;
        }
//        // if the received char is an 'Z', then it triggers a flag to leave the menu
//        if(inChar == 'Z')
//        {
//          flag = true;
//        }
      }
  }while(!flag);
  
  return input_value;
}

void run_laps(){
//  long int total_laps = 0;
//  total_laps = laps * steps_per_revolution * microstepping;
//  moved to the main cycle and changed run_sequence to include the laps
  Serial.print("Total Laps Steps: ");         // to be tested.
  Serial.println(total_laps);

  Serial.print("Laps: ");                   // to be tested.
  Serial.println(laps);

  Serial.print("Steps/rev: ");              // to be tested.
  Serial.println(steps_per_revolution);

  Serial.print("MicroStepping: ");          // to be tested.
  Serial.println(microstepping);

  // change direction
  change_direction(LEFT);

  // rotate motor
  rotate_motor(total_laps);              // variables must be long otherwise we cannot do the same number of steps as others

  delay(1000);

  // change direction
  change_direction(RIGHT);

  // rotate motor
  rotate_motor(total_laps);

  Serial.print(laps);
  Serial.println(" - Laps completed.");
}

bool change_direction(bool direction){
  delayMicroseconds(500);
    digitalWrite(ENA,HIGH);
  delayMicroseconds(100);
    digitalWrite(DIR,direction);
  delayMicroseconds(500);
    digitalWrite(ENA,LOW);
  delayMicroseconds(100);
    Serial.print("Read Dir: ");
    Serial.println(digitalRead(DIR));
    return digitalRead(DIR);
}

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
  read_direction();                    
}

bool read_direction(){
  
  if(digitalRead(DIR) == HIGH)
  {
    Serial.println("LEFT");
  }
  else
  {
    if(digitalRead(DIR) == LOW)
    {
      Serial.println("RIGHT");
    }
  }
  return digitalRead(DIR);
}

long int calculate_speed(long int _speed){
  
  float steps_per_second = 0;         // truncating a float to int -> error chance here
  float temp_speed = 0;
  
  Serial.print("FUNCTION: Calculate_speed: ");
  Serial.print(_speed);
  Serial.println(" RPM");

  steps_per_second = (_speed * steps_per_revolution) / minutes;
  
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
            refresh_commands = true;
        }
        if(command.equals("ss")){
            //Serial.println("0");
            _speed = input_data();
            Serial.print(_speed);
            Serial.println(" uS");
            refresh_commands = true;
        }
        if(command.equals("ssr")){
            //Serial.println("ssr");
            temp_speed_rpm = input_data();
            _speed = calculate_speed(temp_speed_rpm);
                        
            refresh_commands = true;
        }
        if(command.equals("sd")){                   // checks if one direction is set, changes and then changes back again
            if(digitalRead(DIR) == HIGH)
            {
              change_direction(RIGHT);
              Serial.println("Direction changed to: RIGHT");
            }
            else
            {
              if(digitalRead(DIR) == LOW)
              {
                change_direction(LEFT);
                Serial.println("Direction changed to: LEFT");
              }
            }
        
            refresh_commands = true;
        }
        if(command.equals("sst")){
            //Serial.println("sst");
            steps = input_data();
            Serial.println(" ");
            Serial.print(steps);
            Serial.println(" - steps configured");
            refresh_commands = true;
        }
        if(command.equals("smm")){
            //Serial.println("smm");
            travel_mm = input_data();
            Serial.print(travel_mm);
            Serial.println(" mm configured");
            refresh_commands = true;
        }
        if(command.equals("sm")){
            //Serial.println("sm");
            Serial.println("sm --> how many microsteps?");
            microstepping = input_data();
            Serial.println(microstepping);
            refresh_commands = true;
        }
        if(command.equals("spr")){
            Serial.println("spr --> how many steps per revolution?");
            steps_per_revolution = input_data();
            Serial.println(steps_per_revolution);
            refresh_commands = true;
        }
        if(command.equals("sp")){
            //Serial.println("sp");
            positions = input_data();
            Serial.println("sp --> Configure Positions");
            Serial.println(positions);
            refresh_commands = true;
        }
        if(command.equals("sc")){
            //Serial.println("sc");
            cycles = input_data();
            Serial.println("sc --> Configure Cycles");
            Serial.println(cycles);
            refresh_commands = true;
        }
        if(command.equals("sl")){
            //Serial.println("sl");
            Serial.println("sl --> how many laps?");
            laps = input_data();
            Serial.print(laps);
            refresh_commands = true;
        }
        if(command.equals("1")){
            //Serial.println("1");
            change_direction(LEFT);
            steps = input_data();       // Asks before for how many steps to rotate and changes the value.
            rotate_motor(steps);
            refresh_commands = true;
        }
        if(command.equals("2")){
            //Serial.println("2");
            change_direction(RIGHT);
            steps = input_data();       // Asks before for how many steps to rotate and changes the value.
            rotate_motor(steps);
            refresh_commands = true;
        }
        if(command.equals("3")){
            //Serial.println("3");
            change_direction(LEFT);
            rotate_motor(steps);
            refresh_commands = true;
        }
        if(command.equals("4")){
            //Serial.println("4");
            change_direction(RIGHT);
            rotate_motor(steps);
            refresh_commands = true;
        }
        if(command.equals("5")){
            //Serial.println("5");
            travel_mm = input_data();
            calculate_travel_mm(travel_mm);
            change_direction(LEFT);
            rotate_motor(travel_steps);
            refresh_commands = true;
        }
        if(command.equals("6")){
            //Serial.println("6");
            travel_mm = input_data();
            calculate_travel_mm(travel_mm);
            change_direction(RIGHT);
            rotate_motor(travel_steps);
            refresh_commands = true;
        }
        if(command.equals("7")){
            //Serial.println("7");
            destiny_position = input_data();
            total_laps = laps * steps_per_revolution * microstepping;
            go_to_position(destiny_position);
            refresh_commands = true;
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
            refresh_commands = true;
        }
        if(command.equals("rs")){
            Serial.println("rs --> Run Sequence");
            variable_print();
            delay(3000);
            total_laps = laps * steps_per_revolution * microstepping;
            // run the planned sequence
            run_sequence(cycles, positions);
            refresh_commands = true;
        }
        if(command.equals("rl")){
            //Serial.println("rl");
            Serial.println("rl --> how any laps? ");
            variable_print();
            delay(3000);
            total_laps = laps * steps_per_revolution * microstepping;
            // calculate laps and activate motor
            run_laps();
            refresh_commands = true;
        }
        if(command.equals("i")){
            info_print();
            refresh_commands = true;
        }
        else{
           Serial.println("Invalid command");
           refresh_commands = true;
        }        
        refresh_commands = true;            // added here to remove from all other commands
    }
    if (refresh_commands == true){
      menu_print();
      refresh_commands = false;
    }
}