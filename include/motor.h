#ifndef MOTOR_H_
#define MOTOR_H_

#include <Arduino.h>
#include "main.h"

#define LEFT 1
#define RIGHT 0

// External/Shared Variables/Functions

// MOTOR INTERFACE
extern int ENA;
extern int PUL;
extern int DIR;

// MOTOR
extern long int motor_speed_rpm;
extern long int motor_steps_per_revolution;
extern int motor_microstepping;
extern bool motor_direction;
extern long int motor_speed_temp;

// MOVEMENT
extern unsigned long int movement_steps;
extern long int movement_laps;
extern unsigned long int movement_mm;
extern int movement_pitch;
extern int movement_type;
extern unsigned long int movement_steps_temp;

// AUTOMATION
extern int auto_cycles;
extern int auto_positions;
extern int auto_type;

// Function prototypes
bool change_direction(int pin_ena, int pin_dir, bool direction);
unsigned long int calculate_travel_laps(long int movement_laps);
long int calculate_speed(long int speed);
unsigned long int calculate_travel_mm(long int ftravel_mm, int thread_pitch);
void rotate_motor(int pin_ena, int pin_pul, unsigned long int motor_steps, long int speed);
void run_sequence(int pin_ena, int pin_dir, int pin_pul, int cycles, int positions, int type);

#endif