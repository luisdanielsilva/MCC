#ifndef MOTOR_H_
#define MOTOR_H_

#include <Arduino.h>
#include "main.h"

#define LEFT 1
#define RIGHT 0

// External/Shared Variables/Functions
extern int steps_per_revolution;
extern unsigned long int travel_steps;
extern int microstepping;
extern long int total_laps;
extern long int laps;

// Function prototypes
void rotate_motor(int pin_ena, int pin_pul, unsigned long int motor_steps, long int speed);
bool change_direction(int pin_ena, int pin_dir, bool direction);
bool read_direction(int pin_dir);
long int calculate_speed(long int speed);
unsigned long int calculate_travel_mm(long int ftravel_mm);
void go_to_position(int pin_ena, int pin_dir, int target_position);
void run_laps(int pin_ena, int pin_dir);
void run_sequence(int pin_ena, int pin_dir, int pin_pul, int cycles, int positions);

#endif