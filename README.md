# uirobot-fw

This repository is a fork of the source code for the UIROBOT UIM342 motor controller. The original source code can be downloaded from [this link](http://www.uirobot.com/?download_23/uirsdk301en.html).

## Setting up the puppet
Each joint should be aligned over its home position as staked on the ground with tape to start. The winches must be properly wound.

If the joints are hanging in the air and not aligned, hit the e-stop and then pull the joint by the weights until it is ~10cm above the stripe on the ground.

If the joints are on the ground and not aligned, then you will need to raise them using live_jog.exe

## Using live_jog.exe
In a powershell terminal, naviate to uirobot-fw/cloudgate and run live_jog.exe

If the script fails to find actuators (i.e. print out info for each motor), you probably have the e-stop pressed. Unpress that and try again.

Once the script finds the actuators, it promts for CANid entry.
- You can input a CANid for a single device
- You can input a jointID for by adding 100 to the jointID on the paper
- You can input 0 to actuate all at once (please don't do this)

Once the target ID is entered, hit the up or down arrows to jog. A good test is to enter the jointID for each one (100, 103, 106) and make sure that they track vertically.

To stop the motion, hit "space"

To put in a new target ID, hit "esc"

## Using puppet_demo.exe
Once everything is setup and ready to go, open up a powershell terminal and navigate to uirobot-fw/cloudgate and run puppet_demo.exe

If the script fails to find actuators (i.e. print out info for each motor), you probably have the e-stop pressed. Unpress that and try again.

It will quickly read info from the config.ini file to configure the locations of the motors, home positions, etc. and then one-by-one configure the actuators (starting at CANid:23 and ending at CANid:10). After this it will start listeining for OSC messages

When running, you have the following options:
- Hit "s" key to pause listening to OSC messages
- Hit "p" key to resume listening to OSC messages
- Hit "e" key to exit the program, returning the actuators to their home positions (or close)