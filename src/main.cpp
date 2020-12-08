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


// NEW CLI

// variables
boolean error_flag = false;
#define LINE_BUF_SIZE 128   // Maximum input string length
#define ARG_BUF_SIZE 64     // Maximum argument string length
#define MAX_NUM_ARGS 8      // Maximum number of arguments
  
char line[LINE_BUF_SIZE];
char args[MAX_NUM_ARGS][ARG_BUF_SIZE];




//List of functions pointers corresponding to each command
int (*commands_func[])(){
    &cmd_help,
    &cmd_led,
    &cmd_exit
};
 
//List of command names
const char *commands_str[] = {
    "help",
    "led",
    "exit"
};

int num_commands = sizeof(commands_str) / sizeof(char *);

int cmd_led(){
    
   return 0;
}
 
int cmd_exit(){
    Serial.println("Exiting CLI.");
    while(1);
    return 0;   // remove this. makes no sense
}

int cmd_help(){
    help_help();
}


void help_help(){
    Serial.println("The following commands are available:");
 
    for(int i=0; i<num_commands; i++){
        Serial.print("  ");
        Serial.println(commands_str[i]);
    }
    Serial.println("");
    Serial.println("You can for instance type \"help led\" for more info on the LED command.");
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
  read_direction(DIR);                    
}

void setup() {
  
  Serial.begin(115200);
  
  pinMode(LED_BUILTIN, OUTPUT);             // we should blink the LED every 1 second to make sure it's alive. except when a critical function is being performed AKA rotating motor

  Serial.println("Welcome to MCC command line interface.");
  Serial.println("Type \"help\" to see a list of commands.");
}

void loop()
{ 
  Serial.print("> ");

  read_line();

  if(!error_flag){
        parse_line();
    }
    if(!error_flag){
        execute();
    }
 
    memset(line, 0, LINE_BUF_SIZE);
    memset(args, 0, sizeof(args[0][0]) * MAX_NUM_ARGS * ARG_BUF_SIZE);
 
    error_flag = false;

}


void read_line(){
    String line_string;
 
    while(!Serial.available());
 
    if(Serial.available()){
        line_string = Serial.readStringUntil("\n");
        if(line_string.length() < LINE_BUF_SIZE){
          line_string.toCharArray(line, LINE_BUF_SIZE);
          Serial.println(line_string);
        }
        else{
          Serial.println(F("Input string too long."));
          error_flag = true;
        }
    }
}

void parse_line()
{
  Serial.println(F("1 - Line Parser"));

  char *argument;
    int counter = 0;
 
    argument = strtok(line, " ");
 
    while((argument != NULL)){
        if(counter < MAX_NUM_ARGS){
            if(strlen(argument) < ARG_BUF_SIZE){
                strcpy(args[counter],argument);
                argument = strtok(NULL, " ");
                counter++;
            }
            else{
                Serial.println(F("Input string too long."));
                error_flag = true;
                break;
            }
        }
        else{
            break;
        }
    }
}

int execute()
{
  Serial.println(F("2 - Command Execution"));

   for(int i=0; i<num_commands; i++){
        if(strcmp(args[0], commands_str[i]) == 0){
            return(*commands_func[i])();
        }
    }
 
    Serial.println(F("Invalid command. Type \"help\" for more."));
    return 0;
}