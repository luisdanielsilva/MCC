#include <Arduino.h>
#include <SimpleCLI.h>

#include "main.h"
#include "motor.h"

#define DEBUG_ON 1
#define DEBUG_OFF 0
byte debugMode = DEBUG_ON;

#define DBG(...) debugMode == DEBUG_ON ? Serial.println(__VA_ARGS__) : NULL

String mcc_version = "Version: 0.1";
String mcc_date = "Date: 08/12/2020";
String mcc_coder = "Author: Luís Silva @ luisdanielsilva@gmail.com";

SimpleCLI cli;

// Commands available
Command ping;
Command help;
Command info;
Command print;
Command setpins;
Command setmotor;
Command setmovement;
Command setauto;
Command start;

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

    Serial.println(F("Available commands are: "));
    Serial.println(F("help        - this command"));
    Serial.println(F("info        - information"));
    Serial.println(F("print       - print command"));
    Serial.println(F("1. setpins  - set motor interface"));
    Serial.println(F("2. setmotor - set motor parameters"));
    Serial.println(F("3. setmov   - set movement parameters"));
    Serial.println(F("4. setauto  - set movement parameters"));
    Serial.println(F("5. start    - start motor"));

}

void infoCallback(cmd* c) {
    Command cmd(c); // Create wrapper object

    Serial.println(F("Information:"));
    Serial.println(mcc_version);
    Serial.println(mcc_date);
    Serial.println(mcc_coder);
}

void printCallback(cmd* c) {
    Command cmd(c); // Create wrapper object

    Serial.println(F(" - - - - - - - "));
    Serial.print(F("ENABLE PIN: "));
    Serial.println(ENA);
    Serial.print(F("PULSE PIN: "));
    Serial.println(PUL);
    Serial.print(F("DIR PIN: "));
    Serial.println(DIR);
    Serial.println(F(" - - - - - - - "));
    Serial.print(F("Speed: "));
    Serial.println(motor_speed_rpm);
    Serial.print(F("Steps per Revolution: "));
    Serial.println(motor_steps_per_revolution);
    Serial.print(F("Microstepping: "));
    Serial.println(motor_microstepping);
    Serial.print(F("Direction: "));
    Serial.println(motor_direction);
    Serial.println(F(" - - - - - - - "));
    Serial.print(F("Steps: "));
    Serial.println(movement_steps);
    Serial.print(F("Laps: "));
    Serial.println(movement_laps);
    Serial.print(F("Milimeters: "));
    Serial.println(movement_mm);
    Serial.print(F("Type: "));
    Serial.println(movement_type);
    Serial.println(F(" - - - - - - - "));
    Serial.print(F("Cycles: "));
    Serial.println(auto_cycles);
    Serial.print(F("Positions: "));
    Serial.println(auto_positions);
    Serial.print(F("Type: "));
    Serial.println(auto_type);
    Serial.println(F(" - - - - - - - "));  
}

void setpinsCallback(cmd* c) {
    Command cmd(c); // Create wrapper object

    Serial.println("PINS DEFINED AS:");
     // Get arguments
    Argument ena_pinArg = cmd.getArgument("ENA");
    Argument dir_pinArg = cmd.getArgument("DIR");
    Argument pul_pinArg = cmd.getArgument("PUL");

    // Get values
    ENA = ena_pinArg.getValue().toInt();
    DIR = dir_pinArg.getValue().toInt();
    PUL = pul_pinArg.getValue().toInt();
    
    // Validate pin range (between 1 and 50) or depends on platform

    // DEBUG
    Serial.println("ENA - DIR - PUL ");
    Serial.print(" ");
    Serial.print(ENA);
    Serial.print("  -  ");
    Serial.print(DIR);
    Serial.print("  -  ");
    Serial.println(PUL);

    // EXECUTE
    pinMode(ENA, OUTPUT);
    pinMode(DIR, OUTPUT);
    pinMode(PUL, OUTPUT);
}

void setmotorCallback(cmd* c) {
    Command cmd(c); // Create wrapper object

     // Get arguments
    Argument speedArg = cmd.getArgument(0);
    Argument sprArg = cmd.getArgument(1);
    Argument usArg = cmd.getArgument(2);
    Argument dirArg = cmd.getArgument(3);
    
    // Get values and pass them to the actual motor variables
    motor_speed_rpm = speedArg.getValue().toInt();
    motor_steps_per_revolution = sprArg.getValue().toInt();
    motor_microstepping = usArg.getValue().toInt();
    motor_direction = dirArg.getValue().toInt();              // this is a bool value and not int, some error can occur here

    // Validate pin range (between 1 and 50) or depends on platform

    // DEBUG
    Serial.print(F("Speed: "));
    Serial.print(motor_speed_rpm);
    Serial.println(F(" rpm"));

    Serial.print(F("SPR: "));
    Serial.print(motor_steps_per_revolution);
    Serial.println(F(" steps per revolution"));

    Serial.print(F("MicroStepping: "));
    Serial.println(motor_microstepping);
    
    Serial.print(F("DIR: "));
    Serial.println(motor_direction);

    // EXECUTE
}

void setmovementCallback(cmd* c) {
    Command cmd(c); // Create wrapper object

     // Get arguments
    Argument stepsArg = cmd.getArgument(0);
    Argument lapsArg = cmd.getArgument(1);
    Argument mmArg = cmd.getArgument(2);
    Argument typeArg = cmd.getArgument(3);
    
    
    // Get values and pass them to the actual motor variables
    movement_steps = stepsArg.getValue().toInt();
    movement_laps = lapsArg.getValue().toInt();
    movement_mm = mmArg.getValue().toInt();
    movement_type = typeArg.getValue().toInt();
    

    // Validate value range (between 1 and 50) or depends on platform

    // DEBUG
    Serial.print(F("Steps: "));
    Serial.print(movement_steps);
    Serial.println(" stps");

    Serial.print(F("LAPS: "));
    Serial.print(movement_laps);
    Serial.println(" laps");

    Serial.print(F("Milimeters: "));
    Serial.print(movement_mm);
    Serial.println(" mm");

    Serial.print(F("TYPE: "));
    Serial.println(movement_type);

}

void setautoCallback(cmd* c) {
    Command cmd(c); // Create wrapper object

     // Get arguments
    Argument cyclesArg = cmd.getArgument(0);
    Argument positionsArg = cmd.getArgument(1);
    Argument typemovArg = cmd.getArgument(2);
    
    
    // Get values and pass them to the actual motor variables
    auto_cycles = cyclesArg.getValue().toInt();
    auto_positions = positionsArg.getValue().toInt();
    auto_type = typemovArg.getValue().toInt();
    
    // Validate value range (between 1 and 50) or depends on platform

    // DEBUG
    Serial.print("CYCLES: ");
    Serial.println(auto_cycles);

    Serial.print("POSITIONS: ");
    Serial.println(auto_positions);
    
    Serial.print("TYPE OF MOVEMENT: ");
    Serial.println(auto_type);
}

void setstartCallback(cmd* c) {
    Command cmd(c); // Create wrapper object

    String argument;

     // Get arguments
    Argument typeArg = cmd.getArgument(0);
    
    //movement_type = typeArg.getValue().toInt();
    argument = typeArg.getValue();

  if ((strcmp(argument.c_str(), "A") == 0) || (strcmp(argument.c_str(), "a") == 0))  // Doing it with a C Library function
  {
    // Check here if every parameter necessary is configured

    //Serial.println(F("AUTOMATIC OPTION WORKS"));
    DBG(F("Automatic Sequence ..."));

    motor_speed_temp = calculate_speed(motor_speed_rpm);            // consider moving this and the line below to before these two if's

    change_direction(ENA, DIR, motor_direction);

    switch(auto_type)
    {
      case 1:
          // Steps direct to rotate function and rotate
          //motor_speed_temp = calculate_speed(motor_speed_rpm);
          //rotate_motor(ENA, PUL, movement_steps, motor_speed_temp);         // only rotates previously configured STEPS
          movement_steps_temp = movement_steps;
          DBG(F("DIRECT STEPS"));
          Serial.println(movement_steps_temp);
      break;
      case 2:
          // Convert  laps to steps and rotate
          DBG(F("convert laps to steps"));                                    // converts laps to steps and rotates that amount
          movement_steps_temp = calculate_travel_laps(movement_laps);
          DBG(movement_steps_temp);
          //motor_speed_temp = calculate_speed(motor_speed_rpm);
          //rotate_motor(ENA, PUL, movement_steps_temp, motor_speed_temp);
      break;
      case 3:
          // Convert  mm to steps and rotate                              
          DBG(F("convert mm to steps"));                                      // converts mm to steps and rotates that amount
          movement_steps_temp = calculate_travel_mm(movement_mm);
          DBG(movement_steps_temp);
          //motor_speed_temp = calculate_speed(motor_speed_rpm);
          //rotate_motor(ENA, PUL, movement_steps_temp, motor_speed_temp);
      break;
      default:
          Serial.println(F("Could not calculate steps!"));
          // default statement
    }
    
    run_sequence(ENA, DIR, PUL, auto_cycles, auto_positions, auto_type);
    
  }
  if ((argument.compareTo("M") == 0) || (argument.compareTo("m") == 0))  // Doing it with a String method
  {
    // Check here if every parameter necessary is configured

    DBG(F("Single Movement ..."));

    motor_speed_temp = calculate_speed(motor_speed_rpm);

    change_direction(ENA, DIR, motor_direction);

    switch(movement_type)
    {
      case 1:
          // Steps direct to rotate function and rotate
          //motor_speed_temp = calculate_speed(motor_speed_rpm);
          //rotate_motor(ENA, PUL, movement_steps, motor_speed_temp);         // only rotates previously configured STEPS
          movement_steps_temp = movement_steps;
          DBG(F("DIRECT STEPS"));
          DBG(movement_steps_temp);
      break;
      case 2:
          // Convert  laps to steps and rotate
          DBG(F("convert laps to steps"));                          // converts laps to steps and rotates that amount
          movement_steps_temp = calculate_travel_laps(movement_laps);
          //motor_speed_temp = calculate_speed(motor_speed_rpm);
          //rotate_motor(ENA, PUL, movement_steps_temp, motor_speed_temp);
          DBG(movement_steps_temp); 
      break;
      case 3:
          // Convert  mm to steps and rotate                              
          DBG(F("convert mm to steps"));                            // converts mm to steps and rotates that amount
          movement_steps_temp = calculate_travel_mm(movement_mm);
          //motor_speed_temp = calculate_speed(motor_speed_rpm);
          //rotate_motor(ENA, PUL, movement_steps_temp, motor_speed_temp);
          Serial.println(movement_steps_temp); 
      break;
      default:
          Serial.println(F("Could not calculate steps!"));
          // default statement
    }
    
    rotate_motor(ENA, PUL, movement_steps_temp, motor_speed_temp);          // one single instance instead of the top ones
    
  }
            
}

// ***********************************************    SETUP COMMANDS    ***********************************************
void setupCommandPing(){
  // Command PING
  ping = cli.addCommand("ping", pingCallback);
  ping.addArgument("number");
  ping.addPositionalArgument("str", "pong");
  ping.addFlagArgument("c");
}

void setupCommandHelp(){
  // Command PING
  help = cli.addCommand("help", helpCallback);
}

void setupCommandInfo(){
  // Command INFO
  info = cli.addCommand("info", infoCallback);
}

void setupCommandPrint(){
  // Command PRINT
  print = cli.addCommand("print", printCallback);
}

void setupCommandSetPins(){
  // Command Setpins
  setpins = cli.addCommand("setpins", setpinsCallback);
  
  setpins.addArgument("ENA");
  setpins.addArgument("DIR");
  setpins.addArgument("PUL");
}

void setupCommandSetMotor(){
  // Command Setpins
  setmotor = cli.addCommand("setmotor", setmotorCallback);
  
  //int i = 200;
  //char str[10];
  //sprintf(str, "%d", i);
  //Serial.println(str);
  //const char *cinput = "230";         // Works
  //const char *cinput = str;           // does not work
  //Serial.println(cinput);

  setmotor.addArgument("RPM","0");         // DEFAULT: 0        -> later change these default values to come from the motor configuration structure/variables
  setmotor.addArgument("SPR", "200");      // DEFAULT: 200
  setmotor.addArgument("US","1");          // DEFAULT: 1
  setmotor.addArgument("DIR","1");         // DEFAULT: ESQ

}

void setupCommandSetMovement(){
  // Command SetMovement
  setmovement = cli.addCommand("setmov", setmovementCallback);
  
  setmovement.addArgument("STEPS");    // -> later change these default values to come from the motor configuration structure/variables
  setmovement.addArgument("LAPS");    
  setmovement.addArgument("MM");       
  setmovement.addArgument("TM");       
}

void setupCommandSetAuto(){
  // Command SetAuto
  setauto = cli.addCommand("setauto", setautoCallback);
  
  setauto.addArgument("CYC");    // -> later change these default values to come from the motor configuration structure/variables
  setauto.addArgument("POS");    
  setauto.addArgument("TM");       
}

void setupCommandStart(){
  // Command Start
  start = cli.addSingleArgumentCommand("start", setstartCallback);
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

void setup() {
  
  Serial.begin(115200);
  
  pinMode(LED_BUILTIN, OUTPUT);             // we should blink the LED every 1 second to make sure it's alive. except when a critical function is being performed AKA rotating motor

  Serial.println(F("Welcome to MCC command line interface."));
  Serial.println(F("Type \"help\" to see a list of commands."));

  cli.setOnError(errorCallback);

  setupCommandPing();
  setupCommandHelp();
  setupCommandInfo();
  setupCommandSetPins();
  setupCommandSetMotor();
  setupCommandSetMovement();
  setupCommandSetAuto();
  setupCommandStart();
  setupCommandPrint();

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
