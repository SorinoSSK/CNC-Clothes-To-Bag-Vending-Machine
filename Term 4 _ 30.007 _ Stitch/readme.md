# Stitch Machine Program
Beta versions of the machine is stored as reference as components used in each version differs.  
The latest code version:
- Arduino: 7.0
## Libraries used in Arduino

#include "ArduPID.h"<br />
#include <AccelStepper.h><br />
#include <MultiStepper.h><br />

## Functions in Arduino (Operation of bottom gantry is ceased.)
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
| 12 | displayCoordPos() | Display only when debug is set to "true".<br />CoordPos = True: <br />Display all gantry's stepper step position.<br />CoordPos = False:<br />Display gantry's coordinates.|
| 13 | displayLimitSwitch() | Constantly display all limitswitches state<br />( TopX, TopY, TopYMax, TopZ, BottomX, BottomY, Cutter, Door, LeftTemplate, RightTemplate) |
| 14 | XYZStepsBuilder<br />(long Pos_TopLeftXY, long Pos_TopRightXY, long Pos_TopZ, long Pos_BottomLeftXY, long Pos_BottomRightXY) | Stores positions of all motors. |
| 15 | GantryMovement() | Consolidate all gantry's function and moves gantry. Position is recorded in this function. |
| 16 | TaskDissection (char BufferVal, String BufferCoord) | - For manual movement X__ Y__ Z__ Q__ W__ T__ <br />- Dissect serial input into coordinates.|
| 17 | CoreXYMovement() | Convert coordinates of manual movement to positions read by stepper motors. |
| 18 | CodeReader (String Val) | Function to dissection serial input. |
| 19 | safetySwitchChecker() | - Activate only when safety mechanism is activated.<br /> - Constantly check for status of limitswitch of door.<br />Function:<br />1) Pause all function when door's limitswitch is opened and inform Rasberry Pi. <br />2) Unpause all function when door's limitswitch is closed and inform Rasberry Pi.|
| 20 | PIDTemperature() | Read temperature from thermistor and update PID. |
| 21 | readTTemperature() | Read thermistor and convert to degree C. (Modified from example code) |
| 22 | setStepperSpeed() | Set speed of all stepper to declared values. |
| 23 | homeBottonY() | home Y of buttom gantry. |
| 24 | homeBottomX() | home X of buttom gantry. |
| 25 | homeTopZ() | home Z of top gantry. |
| 26 | homeTopY() | home Y of top gantry. |
| 27 | homeTopX() | home X of top gantry. |
| 28 | homeTop() | Home X, Y, Z axis of top gantry. |
| 29 | lowerCutter() | Lower cutter to cutting position. |
| 30 | homeCutter() | Raise cutter to limit switch. |
| 31 | lowerCutter2() | Similar to lowerCutter() but for self-designed circuit board. (Designed for Circuits and Electronics module) |
| 32 | homeCutter2() | Similar to homeCutter() but for self-designed circuit board. (Designed for Circuits and Electronics module) |
| 33 | raiseStamp() | Raise hotend after stamping. |
| 34 | lowerStamp() | Pressed hotend onto the shirt. Function will only activate when it is lowered to position and the hotend is hot enough. |
| 35 | raiseHeatBlock() | Raise hotend all the way up to home position to avoid hitting screws on shirt template. |
| 36 | lowerHeatBlock() | Lower hotend to position for stamping. |
| 37 | moveVert() | Move sealing mechanism vertically by indicated steps. |
| 38 | backgroundTask() | Function to run every loop. |
| 39 | CodeReader(String Val) | Function to read serial communication and dissect task<br />-e.g. commands used in this machine. |
