# MCC
## Motor Control Center

This software is meant to be used with stepper motor drivers that support I/O interface of type ENA, PUL, DIR such as TB6600 and TMC2xxx series and uses Arduino as a platform.

To use the software please follow the steps below.

**Step 1:**
Configure pins ENABLE, DIRECTION, PULSE. Numbering should follow Arduino scheme.

*Example:* setpins -ENA 2 -DIR 3 -PUL 4
```setpins -ENA 9 -DIR 7 -PUL 8```


**Step 2:**
Configure speed, steps per rotation, microstepping and direction.
This depends on your motor driver and motor. Be careful when settings these values!

*Example:* 
```setmotor -RPM 100 -SPR 800 -US 4 -DIR 0```


**Step 3:**
Configure how much you want you motor to move. Whether it is steps, laps or milimeters (in a leadscrew type of arrangement) and how much pitch the threadscrew has.

*Example:*
```setmov -STEPS 800 -LAPS 2 -MM 50 -P 8 -TM 1```


**Step 4:**
Configure how the automatic movement setting works. Set cycles, positions and type of movement (steps, laps or mm) as defined in **Step 3**.
The automatic movement will go through the number of positions in the original directions then backward. It will do so a number of times (cycles).

*Example:*
```setauto -CYC 2 -POS 5```


**Step 5:**
Configure how much you want you motor to move. Whether it is steps, laps or milimeters (in a leadscrew type of arrangement) and how much pitch the threadscrew has.

*Example:*
```start a``` or ```start A```  -> for automatic movement
```start m``` or ```start M``   -> for a one time movement



## Author:
Luís Silva
e-mail me @ luisdanielsilva@gmail.com
Personal website: [www.luisdanielsilva.com](https://www.luisdanielsilva.com/).
