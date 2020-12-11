# MCC
## Motor Control Center

This software is an application of a library freely available online: [SimpleCLI](https://github.com/spacehuhn/SimpleCLI).  
**Motor Control Center** is meant to be used with stepper motor drivers that support I/O interface of type ENA, PUL, DIR such as TB6600 and TMC2xxx series and uses Arduino as a platform (can easily be adapted for other platforms).

## Supported boards
MCC runs on an Arduino Nano (ATMega328), its where it was developed.  
However due to the small RAM size, if you write much more code on top of this application it can lead to unexpected behaviou AKA crash. I recommend using any arduino with more than 2KB of SRAM.

### Arduino Nano (ATMega328)
* Flash Memory:	32 KB of which 2 KB used by bootloader
* SRAM:	2 KB

### Recommended
Any Arduino board with more than 2KB of SRAM.

## Installation
* Click [Download Zip](https://github.com/luisdanielsilva/MCC/archive/main.zip) to download the source code from GitHub.
* Unzip and rename the Folder name to "MCC".
* Drag it to your Visual Studio Code Workspace and if you can compile Arduino code on it, then everything should be ready.
* Compile and burn it to your arduino.
* Use any serial monitor to interface with it.

## Usage
Assuming you have Visual Studio Code installed with PlatformIO, to use the software please follow the steps below.

**Step 1:**
Configure pins ENABLE, DIRECTION, PULSE. Numbering should follow Arduino scheme.

*Example:*
```setpins -ENA 2 -DIR 3 -PUL 4```

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
I'm a dad, husband and an electrical engineer working in the automotive industry.  
On my time away from work I like to do my own electronics projects, with or without software. I only lack the time to do everything I want to do :)  
You can e-mail me: luisdanielsilva at gmail.com  
Personal website: [www.luisdanielsilva.com](https://www.luisdanielsilva.com/)  

## Credits:
This project could only be possible because of [SimpleCLI](https://github.com/spacehuhn/SimpleCLI) and its creator [Stefan Kremser](https://spacehuhn.de/).  
Please consider supporting him by becoming a [patron](patreon.com/spacehuhn) or buying his merchandising ([Stickers](https://www.tindie.com/products/Spacehuhn/spacehuhn-stickers/))

## License
**Motor Control Center** is licensed under the MIT License. See the license file for details
https://github.com/luisdanielsilva/MCC/blob/main/LICENSE  

**SimpleCLI** is licensed under the MIT License. See the license file for details
https://github.com/spacehuhn/SimpleCLI/blob/master/LICENSE  