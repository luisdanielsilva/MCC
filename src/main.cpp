#include <Arduino.h>
#include <SimpleCLI.h>

#include "main.h"
#include "motor.h"
#include "serial.h"

String mcc_version = "Version: 0.1";
String mcc_date = "Date: 08/12/2020";
String mcc_coder = "Author: Luís Silva @ luisdanielsilva@gmail.com";

SimpleCLI cli;

Command ping;
Command help;
Command info;
Command setpins;
Command setspeed;

 // #define INFO 
// NANO Configuration
 int PUL=4; //define Pulse pin
 int DIR=3; //define Direction pin
 int ENA=2; //define Enable Pin

// MEGA 2560 Configuration
// int PUL=9;                   //define Pulse pin
// int DIR=8;                   //define Direction pin
// int ENA=10;                   //define Enable Pin

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


// ***********************************************    CALLBACK FUNCTIONS    ***********************************************
void pingCallback(cmd* c) {
    Command cmd(c); // Create wrapper object

    // Get arguments
    Argument numberArg = cmd.getArgument("number");
    Argument strArg    = cmd.getArgument("str");
    Argument cArg      = cmd.getArgument("c");

    // Get values
    int numberValue = numberArg.getValue().toInt();
    String strValue = strArg.getValue();
    bool   cValue   = cArg.isSet();

    if (cValue) strValue.toUpperCase();

    // Print response
    for (int i = 0; i<numberValue; i++) Serial.println(strValue);
}

void helpCallback(cmd* c) {
    Command cmd(c); // Create wrapper object

    Serial.println("Available commands are: ");
    Serial.println("help - this command");
    Serial.println("info - information");
    Serial.println("ping - ping command");
    Serial.println("setpins - set motor interface");
}

void infoCallback(cmd* c) {
    Command cmd(c); // Create wrapper object

    Serial.println("Information:");
    Serial.println(mcc_version);
    Serial.println(mcc_date);
    Serial.println(mcc_coder);
}

void setpinsCallback(cmd* c) {
    Command cmd(c); // Create wrapper object

    Serial.println("PINS DEFINED AS:");
     // Get arguments
    Argument ena_pinArg = cmd.getArgument("ENA");
    Argument dir_pinArg = cmd.getArgument("DIR");
    Argument pul_pinArg = cmd.getArgument("PUL");

    // Get values
    int ENA = ena_pinArg.getValue().toInt();
    int DIR = dir_pinArg.getValue().toInt();
    int PUL = pul_pinArg.getValue().toInt();
    
    // Validate pin range (between 1 and 50) or depends on platform

    Serial.println("ENA - DIR - PUL ");
    Serial.print(" ");
    Serial.print(ENA);
    Serial.print("  -  ");
    Serial.print(DIR);
    Serial.print("  -  ");
    Serial.println(PUL);
}

void setspeedCallback(cmd* c) {
    Command cmd(c); // Create wrapper object

     // Get arguments
    Argument speedArg = cmd.getArgument(0);
    
    // Get values
    long int speedrpm = speedArg.getValue().toInt();
    
    // Validate pin range (between 1 and 50) or depends on platform
    Serial.println("Speed: ");
    Serial.print(speedrpm);
    Serial.println(" rpm");
}


// ***********************************************    SETUP COMMANDS    ***********************************************
void setupCommandPing(){
  // Command PING
  ping = cli.addCommand("ping", pingCallback);
  ping.addArgument("number");
  ping.addPositionalArgument("str", "pong");
  ping.addFlagArgument("c");

  //Serial.println("Type: ping -str \"Hello World\" -number 1 -c");
}

void setupCommandHelp(){
  // Command PING
  help = cli.addCommand("help", helpCallback);
}


void setupCommandInfo(){
  // Command INFO
  info = cli.addCommand("info", infoCallback);
}

void setupCommandSetPins(){
  // Command Setpins
  setpins = cli.addCommand("setpins", setpinsCallback);
  
  setpins.addArgument("ENA");
  setpins.addArgument("DIR");
  setpins.addArgument("PUL");
}

void setupCommandSetSpeed(){
  // Command Setpins
  setspeed = cli.addSingleArgumentCommand("setspeed", setspeedCallback);
  
  //setspeed.addArgument("speed");
}

// Callback in case of an error
void errorCallback(cmd_error* e) {
    CommandError cmdError(e); // Create wrapper object

    Serial.print("ERROR: ");
    Serial.println(cmdError.toString());

    if (cmdError.hasCommand()) {
        Serial.print("Did you mean \"");
        Serial.print(cmdError.getCommand().toString());
        Serial.println("\"?");
    }
}


void menu_print(){
  Serial.println(F(" ********************** MOTOR CONTROL CENTER ********************** "));
  Serial.println(F("p   --> Display Parameters"));
  Serial.println(F("ss  --> Set Speed (tON/tOFF)"));
  Serial.println(F("ssr --> Set Speed (RPM)"));
  Serial.println(F("sd  --> Configure direction"));
  //Serial.println(F("sst --> Set Steps"));
  //Serial.println(F("smm --> Set mm"));
  Serial.println(F("spr --> Set Steps per Revolution (default = 200)"));
  Serial.println(F("sm  --> Set MicroStepping (default = 1)"));
  //Serial.println(F("sp  --> Set Positions"));
  //Serial.println(F("sc  --> Set Cycles"));
  //Serial.println(F("sl  --> Set Laps"));
  Serial.println(F("1   --> Move LEFT x steps"));
  Serial.println(F("2   --> Move RIGHT x steps"));
  Serial.println(F("3   --> Move LEFT 1 position"));
  Serial.println(F("4   --> Move RIGHT 1 position"));
  Serial.println(F("5   --> Move LEFT x mm"));
  Serial.println(F("6   --> Move RIGHT x mm"));
  Serial.println(F("7   --> Go to Target Position"));
  //Serial.println(F("8   --> Steps (0) or mm (1)"));
  //Serial.println(F("rs  --> Run Sequence"));
  //Serial.println(F("rl  --> Run Laps"));
  //Serial.println(F("i   --> Info"));
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

  cli.setOnError(errorCallback);

  setupCommandPing();
  setupCommandHelp();
  setupCommandInfo();
  setupCommandSetPins();
  setupCommandSetSpeed();
}

void loop()
{ 
  // Check if user typed something into the serial monitor
    if (Serial.available()) {
        // Read out string from the serial monitor
        String input = Serial.readStringUntil('\n');

        // Echo the user input
        Serial.print("> ");
        Serial.println(input);

        // Parse the user input into the CLI
        cli.parse(input);
    }

    if (cli.errored()) {
        CommandError cmdError = cli.getError();

        Serial.print("ERROR: ");
        Serial.println(cmdError.toString());

        if (cmdError.hasCommand()) {
            Serial.print("Did you mean \"");
            Serial.print(cmdError.getCommand().toString());
            Serial.println("\"?");
        }
    }

}
