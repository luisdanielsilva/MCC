#include "serial.h"

// ISR/main shared variables must be defined as volatile
// extern int input_data();

// Shared variables
String command = "";

// Internal to .cpp
String inString = "";

// Function declarations
int input_data(){
  int inChar = 0;
  boolean flag = false;
  int input_value = 0;

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