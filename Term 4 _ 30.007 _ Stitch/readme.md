# Stitch Machine Program
Beta versions of the machine is stored as reference as components used in each version differs.  
The latest code version:
- Arduino: 7.0
## Libraries used in Arduino

#include "ArduPID.h"<br />
#include <AccelStepper.h><br />
#include <MultiStepper.h><br />

## Functions in Arduino
| S/N | Function Name |Description|
| --- | --- |---|
| 1 | IsDoorClose() | Check if limitswitch of door is closed. |
| 2 | IsTemplateRightClose() | Check if limitswitch of template's right is closed. |
| 3 | IsTemplateLeftClose() | Check if limitswitch of template's left is closed. |
| 4 | IsCutterClose() | Check if limitswitch of cutter is closed. |
| 5 | IsBottomYClose() | Check if limitswitch of bottom gantry's Y axis is closed. |
| 6 | IsBottomXClose() | Check if limitswitch of bottom gantry's X axis is closed.  |
| 7 | IsTopZClose() | Check if limitswitch of top gantry's Z axis is closed. |
| 8 | IsTopYMaxClose() | Check if limitswitch of top gantry's max Y axis is closed. |
| 9 | IsTopYClose() | Check if limitswitch of top gantry's Y axis is closed. |
| 10 | IsTopXClose() | Check if limitswitch of top gantry's X axis is closed.  |
| 11 | signs (double val) | - if > 0, return 1<br />- if < 0, return -1<br />- else, return 0 |
| 12 | PIDTemperature() | Compute PID and analogWrite() to heatcartridge. |
| 13 | readTTemperature() | Reads thermistor and convert to degree celcius. |
| 14 | setStepperSpeed() | Set speed of all stepper to declared values. |
| 15 | displayCoordPos() | Display only when debug is set to "true".<br />CoordPos = True: <br />Display all gantry's stepper step position.<br />CoordPos = False:<br />Display gantry's coordinates.|
| 16 | displayLimitSwitch() | Constantly display all limitswitches state<br />( TopX, TopY, TopYMax, TopZ, BottomX, BottomY, Cutter, Door, LeftTemplate, RightTemplate) |
| 17 | XYZStepsBuilder<br />(long Pos_TopLeftXY, long Pos_TopRightXY, long Pos_TopZ, long Pos_BottomLeftXY, long Pos_BottomRightXY) | Stores positions of all motors. |
| 18 | GantryMovement() | Consolidate all gantry's function and moves gantry. Position is recorded in this function. |
| 19 | TaskDissection (char BufferVal, String BufferCoord) | - For manual movement X__ Y__ Z__ Q__ W__ T__ <br />- Dissect serial input into coordinates.|
| 20 | CoreXYMovement() | Convert coordinates of manual movement to positions read by stepper motors. |
| 21 | CodeReader (String Val) | Function to dissection serial input. |
| 22 | safetySwitchChecker() | - Activate only when safety mechanism is activated.<br /> - Constantly check for status of limitswitch of door.<br />Function:<br />1) Pause all function when door's limitswitch is opened and inform Rasberry Pi. <br />2) Unpause all function when door's limitswitch is closed and inform Rasberry Pi.|
| 23 | PIDTemperature() | Read temperature from thermistor and update PID |
| 24 | readTTemperature() | Read thermistor and convert to degree C, (Modified from example code) |
| 25 | setStepperSpeed() | Initialise all stepper's speed |
| 26 | safetySwitchChecker() | If activated, check if door, IsDoorClose() is close. If door is not close, stop all program. |
| 27 | homeBottonY() | - |
| 28 | homeBottomX() | - |
| 29 | homeTopZ() | - |
| 30 | homeTopY() | - |
| 31 | homeTopX() | - |
| 32 | homeTop() | Home all X, Y, Z axis |
| 33 | lowerCutter2() | Similar to lowerCutter() but for self-designed circuit board (Designed for Circuits and Electronics module) |
| 34 | homeCutter2() | Similar to homeCutter() but for self-designed circuit board (Designed for Circuits and Electronics module) |
| 35 | lowerCutter() | Lower cutter to cutting position |
| 36 | homeCutter() | Raise cutter to limit switch |
| 37 | raiseStamp() | Raise hotend after stamping |
| 38 | lowerStamp() | Pressed hotend onto the shirt. Function will only activate when it is lowered to position and the hotend is hot enough. |
| 39 | raiseHeatBlock() | Raise hotend all the way up to home position |
| 40 | lowerHeatBlock() | Lower hotend to position for stamping |
| 41 | moveVert() | Move sealing mechanism vertically by indicated steps |
| 42 | backgroundTask() | Function to run every loop |
| 43 | CodeReader(String Val) | Function to read serial communication and dissect |
