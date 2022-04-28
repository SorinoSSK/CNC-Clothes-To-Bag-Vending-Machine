#include "ArduPID.h"
#include <AccelStepper.h>
#include <MultiStepper.h>

// Stepper Motor Declaration from front perspective 
// Pin Declaration: (AccelStepper::Driver,StepPin,DirPin);
AccelStepper TopLeftXY      (AccelStepper::DRIVER,47,46);
AccelStepper TopRightXY     (AccelStepper::DRIVER,49,48);
AccelStepper TopZ           (AccelStepper::DRIVER,51,50);

AccelStepper Cutter         (AccelStepper::DRIVER,53,52);

AccelStepper TemplateLeft   (AccelStepper::DRIVER,27,26);
AccelStepper TemplateRight  (AccelStepper::DRIVER,29,28);

AccelStepper BottomLeftXY   (AccelStepper::DRIVER,23,22);
AccelStepper BottomRightXY  (AccelStepper::DRIVER,25,24);

// Mechanism Change, Not In Use
//AccelStepper BottomZ        (AccelStepper::DRIVER,27,26);
//AccelStepper Bobbin         (AccelStepper::DRIVER,25,24);
//AccelStepper Needle         (AccelStepper::DRIVER,51,50);

// Endstops Pin Declaration
#define ES_Top_X           36
#define ES_Top_Y           34
#define ES_Top_Y_Max       42
#define ES_Top_Z           32
#define ES_Bottom_X        38
#define ES_Bottom_Y        40
#define ES_Cutter          37
#define ES_Template_Left   33
#define ES_Template_Right  35
#define ES_Door            39

// THERMISTOR
#define THERMISTORPIN         A0
#define HEATCARTRIDGE         A1
#define THERMISTORNOMINAL     100000      //87988.16    //Resistance at Room Temperature
#define TEMPERATURENOMINAL    25          //28.1        //Room Temperature
#define NUMSAMPLES            5           //
#define BCOEFFICIENT          3950        //Creality CR6-SE Thermistor
#define SERIESRESISTOR        10000       //Self-built Voltage Divider Circuit.


// PID instance
ArduPID myController;
// PID Values
#define OUTPUT_MIN            0
#define OUTPUT_MAX            255
#define KP                    20.16
#define KI                    1.26
#define KD                    80.95

// MultiStepper Grouping
MultiStepper stepperGrp;
MultiStepper sewingZ;
MultiStepper templates;

// MaxSpeed Tuning
int Speed_TopGantry           = 4000;
int Speed_TopGantry_Z         = 4000;
int Speed_BottomGantry        = 2000;
int Speed_Template            = 4000;
int Speed_Cutter_Raise_Lower  = 4000;
// Mechanism Change, Not In Use
//int Speed_Sewing              = 1000;
//int Speed_Bobbin              = 4000;

// Acceleration Tuning
int Accel_TopGantry           = 3000;
int Accel_TopGantry_Z         = 3000;
int Accel_BottomGantry        = 3000;
int Accel_Template            = 3000;
int Accel_Cutter_Raise_Lower  = 3000;
// Mechanism Change, Not In Use
//int Accel_Sewing              = 1000;

// Revolution per mm Tuning
int Step_TopLeftXY            = 40;
int Step_TopRightXY           = 40;
int Step_TopZ                 = 796;
int Step_Cutter               = 1;
int Step_TemplateLeft         = 1;
int Step_TemplateRight        = 1;
int Step_BottomLeftXY         = 40;
int Step_BottomRightXY        = 40;
// Mechanism Change, Not In Use
//int Step_Needle         = 1;
//int Step_Bobbin         = 1;

// Calibration Setting/ Offset
int ESCount[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
int ESNoOfCount       = 4;
int StitchLength      = 10;   // Even number only
int NoOfBobbinLoop    = 2;

// Settings Variable
bool Debug              = false;
bool CheckTemperature   = false;
bool PrintES            = false;
bool CoordPos           = false;
bool NeedleIsDropped    = false;
bool TemplateUnlock     = false;
bool TemplateLock       = false;
bool TempAllow          = false;        // Temperature Check for stamp
bool SafetySwitch       = false;        // Safety Switch
bool SafePause          = false;        // Enable/ Disable safety check
bool SSsend             = false;        // Safety Check Communication
bool SRsend             = true;
bool SG                 = false;
bool TEM                = false;
bool CUT                = false;
// Removed Settings Variable
//bool TestMode           = false;
//bool Sewing             = false;

// Machine Settings
long steps_To_Lock      = 800;
long TempSet            = 200;
long LengthDownToStamp  = 38;
long RaisedPosAfterStamp= 30;
long PressureTime       = 3;  // In Seconds

// Temporary Variable
int StitchVal = 0;
int samples[NUMSAMPLES];                                      // To Average Thermistor's value
float Coordinates[5]  = {-9999, -9999, -9999, -9999, -9999};  // {TX, TY, TZ, BX, BY}
float Task[6]         = {0, 0, 0, 0, 0, 0};                   // Task for motor movement
bool BGTask[20]       = {false, false, false, false, false,
                          false, false, false, false, false,
                          false, false, false, false, false,
                          false, false, false, false, false};
float TXHomeVal, TYHomeVal, TZHomeVal, BXHomeVal, BYHomeVal;
float TL_Val, TR_Val, BL_Val, BR_Val, Z_Val;            // Motor steps for XY movement
double Temperature, setPoint, outputVal, average;
long positions[8]     = {0, 0, 0, 0, 0, 0, 0, 0};             // {TLeft, TRight, TZ, BLeft, BRight, Cutter, TemplateLeft, TemplateRight}
long StepGrpPos[5]    = {0, 0, 0, 0, 0};
unsigned long STime;                                          // Time Counting
unsigned long CTime;

void setup() 
{
  Serial.begin(115200);
  Serial.println("This program written by team Stitch.");
  Serial.println("Member: Teo Wei Qing, Seow Sin Kiat, Liu Tong Han, Brian Chin, Ernest Ng.");
  Serial.println("Program Version 7.0. Initialising functions...");

  // Stepper Max Speed Initialisation
  TopLeftXY.setMaxSpeed(Speed_TopGantry);
  TopRightXY.setMaxSpeed(Speed_TopGantry);
  TopZ.setMaxSpeed(Speed_TopGantry_Z);
  Cutter.setMaxSpeed(Speed_Cutter_Raise_Lower);
  TemplateLeft.setMaxSpeed(Speed_Template);
  TemplateRight.setMaxSpeed(Speed_Template);
  BottomLeftXY.setMaxSpeed(Speed_BottomGantry);
  BottomRightXY.setMaxSpeed(Speed_BottomGantry);

  // Stepper Acceleration Initialisation
  TopLeftXY.setAcceleration(Accel_TopGantry);
  TopRightXY.setAcceleration(Accel_TopGantry);
  TopZ.setAcceleration(Accel_TopGantry_Z);
  Cutter.setAcceleration(Accel_Cutter_Raise_Lower);
  TemplateLeft.setAcceleration(Accel_Template);
  TemplateRight.setAcceleration(Accel_Template);
  BottomLeftXY.setAcceleration(Accel_BottomGantry);
  BottomRightXY.setAcceleration(Accel_BottomGantry);

  // Add Stepper To Group
  stepperGrp.addStepper(TopLeftXY);
  stepperGrp.addStepper(TopRightXY);
  stepperGrp.addStepper(TopZ);
  stepperGrp.addStepper(BottomLeftXY);
  stepperGrp.addStepper(BottomRightXY);

  // Add Stepper To Group
//  sewingZ.addStepper(Bobbin);
//  sewingZ.addStepper(Needle);

  // Add Stepper To Group
  templates.addStepper(TemplateLeft);
  templates.addStepper(TemplateRight);

  // Add EndStop
  pinMode(ES_Top_X,           INPUT_PULLUP);
  pinMode(ES_Top_Y,           INPUT_PULLUP);
  pinMode(ES_Top_Y_Max,       INPUT_PULLUP);
  pinMode(ES_Top_Z,           INPUT_PULLUP);
  pinMode(ES_Bottom_X,        INPUT_PULLUP);
  pinMode(ES_Bottom_Y,        INPUT_PULLUP);
  pinMode(ES_Cutter,          INPUT_PULLUP);
  pinMode(ES_Template_Left,   INPUT_PULLUP);
  pinMode(ES_Template_Right,  INPUT_PULLUP);
  pinMode(ES_Door,            INPUT_PULLUP);

  // Setting Up PID
  myController.begin(&Temperature, &outputVal, &setPoint, KP, KI, KD);
  myController.setOutputLimits(0, 255);
  myController.setBias(255.0 / 2.0);
  myController.setWindUpLimits(-2, 2); // Groth bounds for the integral term to prevent integral wind-up
  
  myController.start();

  Serial.println("Functions initialised. Waiting for Command.");
  Serial.println("---------------------");
}

void loop()
{
  safetySwitchChecker();
  displayLimitSwitch();             // Call to Display Limit Switches State
  if (!SafePause || Debug)
  {      
    backgroundTask();
    SG = stepperGrp.run();          // Constantly Running Stepper Motor
    TEM = templates.run();
    CUT = Cutter.run();
  }
  PIDTemperature();                 // Recompute PID
  if (Serial.available() > 0)       // Read Serial Input
  {
    CodeReader(Serial.readStringUntil('\r\n'));
  }
}

void CodeReader(String Val)
{
  Val.toUpperCase();
  if (Val == "ESSTATUS" || Val == "ESCheck" || Val == "STATUSES" || Val == "CHECKES")
  {
    PrintES = !PrintES;
  }
  else if (Val == "CHECKTEMPERATURE" || Val == "TEMPERATURECHECK")
  {
    CheckTemperature = !CheckTemperature;
  }
  else if (Val == "GETTEMPERATURE")
  {
    String TTemp = "T";
    TTemp += String(Temperature,2);
    Serial.println(TTemp);
  }
  else if (Val == "TOGGLESAFETY")
  {
    SafetySwitch = !SafetySwitch;
    Serial.println(95484);
  }
  else if (Val == "VIEWPOS")
  {
    Serial.println("Changing to position display");
    CoordPos = true;
  }
  else if (Val == "VIEWCOORD")
  {
    Serial.println("Changing to coordinate display");
    CoordPos = false;
  }
  else if (Val == "HOMETOPX")
  {
    if (Debug)
    {
      Serial.println("Homing Top X...");
    }
    TXHomeVal = 0;
    BGTask[0] = true;
  }
  else if (Val == "HOMETOPY")
  {
    if (Debug)
    {
      Serial.println("Homing Top Y...");
    }
    TYHomeVal = 0;
    BGTask[1] = true;
  }
  else if (Val == "HOMEBOTTOMX")
  {
    if (Debug)
    {
      Serial.println("Homing Bottom X...");
    }
    BGTask[2] = true;
  }
  else if (Val == "HOMEBOTTOMY")
  {
    if (Debug)
    {
      Serial.println("Homing Bottom Y...");
    }
    BGTask[3] = true;
  }
  else if (Val == "HOMETOPZ")
  {
    if (Debug)
    {
      Serial.println("Homing Top Z...");
    }
    BGTask[4] = true;
  }
  else if (Val == "HOMETOP")
  {
    if (Debug)
    {
      Serial.println("Homing All Top...");
    }
    BGTask[5] = true;
  }
  else if (Val == "LOWERCUTTER")
  {
    positions[5] = 1000;
    Cutter.moveTo(positions[5]);
    BGTask[6] = true;
  }
  else if (Val == "RAISECUTTER")
  {
    BGTask[7] = true; 
  }
  else if (Val == "OPENTEMPLATE" || Val == "UNLOCKTEMPLATE")
  {
    BGTask[8] = true;
  }
  else if (Val == "CLOSETEMPLATE" || Val == "LOCKTEMPLATE")
  {
    positions[6] = steps_To_Lock;
    positions[7] = steps_To_Lock;
    TemplateLeft.moveTo(positions[6]);
    TemplateRight.moveTo(positions[7]);
    BGTask[9] = true;
  }
  else if (Val == "RAISEHEATBLOCK")
  {
    if (Debug)
    {
      Serial.println("Raising Heater...");
    }
    BGTask[10] = true;
  }
  else if (Val == "LOWERHEATBLOCK")
  {
    if (Debug)
    {
      Serial.println("Lowering Heater...");
    }
    if (Coordinates[2] >= -5000)
    {
      positions[2] = 25*Step_TopZ;
      XYZStepsBuilder(positions[0],positions[1],positions[2],positions[3],positions[4]);
      StepGrpPos[0] = positions[0];
      StepGrpPos[1] = positions[1];
      StepGrpPos[2] = positions[2];
      StepGrpPos[3] = positions[3];
      StepGrpPos[4] = positions[4];
      stepperGrp.moveTo(StepGrpPos);
      Coordinates[2] += 25;
      BGTask[11] = true;
    }
    else
    {
      if (Debug)
      {
        Serial.println("Please Home Top Z.");  
      }
      Serial.println(95491);
    }
  }
  else if (Val == "STAMP" || Val == "STAMPING")
  {
    if (Debug)
    {
      Serial.println("Beign Stamping...");
    }
    positions[2] = LengthDownToStamp*Step_TopZ;
    BGTask[12] = true;
  }
  else
  {
    String BufferCoord = "";
    char BufferVal = NULL;
    for(int i = 0; i < Val.length(); i ++)
    {
      if (isSpace(Val[i]) || i == Val.length()-1)
      {
        if (i == Val.length()-1)
          BufferCoord += Val[i];
        TaskDissection(BufferVal, BufferCoord);
        BufferCoord = "";
        BufferVal = NULL;
      }
      else if (BufferVal == NULL)
        BufferVal = Val[i];
      else
        BufferCoord += Val[i];
    }
    GantryMovement();
  }
}

void backgroundTask()
{
  if (BGTask[0])
    {
      homeTopX();
    }
    if (BGTask[1])
    {
      homeTopY();
    }
    if (BGTask[2])
    {
      homeBottomX();
    }
    if (BGTask[3])
    {
      homeBottomY();
    }
    if (BGTask[4])
    {
      homeTopZ();
    }
    if (BGTask[5])
    {
      homeTop();
    }
    if (BGTask[6])
    {
      lowerCutter();
    }
    if (BGTask[7])
    {
      homeCutter();
    }
    if (BGTask[8])
    {
      TemplateOpenLock();
    }
    if (BGTask[9])
    {
      TemplateCloseLock();
    }
    if (BGTask[10])
    {
      raiseHeatBlock();
    }
    if (BGTask[11])
    {
      lowerHeatBlock();
    }
    if (BGTask[12])
    {
      lowerStamp();
    }
    if (BGTask[13])
    {
      raiseStamp();
    }
    if (BGTask[14])
    {
      CTime = millis();
      unsigned long Val = CTime - STime;
      if(Val >= (PressureTime*1000))
      {
        BGTask[14] = false;
        positions[2]= RaisedPosAfterStamp*Step_TopZ;
        BGTask[13] = true;
      }
    }
    if (BGTask[15])
    {
      moveVert();
    }
    if (BGTask[16])
    {
      if (!SG && ESCount[11] == 1)
      {
        Serial.println(95479);
        BGTask[16] = false;
        ESCount[11] = 0;
      }
      else
      {
        if(!SG)
        {
          ESCount[11] += 1;
        }
      }
    }
}

void moveVert()
{
  if (!SG)
  {
    StitchVal = 0;
    positions[2] = LengthDownToStamp*Step_TopZ;
    BGTask[12] = true;
    BGTask[15] = false;  
  }
}

void lowerHeatBlock()
{  
  if (!SG)
  {
    if (TopZ.currentPosition() == positions[2])
    {
      if (Debug)
      {
        Serial.println("Heatblock Position Reached.");
      }
      NeedleIsDropped = true;
      BGTask[11] = false;
      Serial.println(95483);
      displayCoordPos();
    }
  }
}

void raiseHeatBlock()
{
  if (IsTopZClose())
  {
    if (Debug)
    {
      Serial.println("Heatblock Position Reached.");
    }
    Serial.println(95492);
    BGTask[10] = false;
    Coordinates[2] = 0;
    NeedleIsDropped = false;
    displayCoordPos();
  }
  else if (!SG)
  {
    positions[2] -= 0.1*Step_TopZ;
    XYZStepsBuilder(positions[0],positions[1],positions[2],positions[3],positions[4]);
    StepGrpPos[0] = positions[0];
    StepGrpPos[1] = positions[1];
    StepGrpPos[2] = positions[2];
    StepGrpPos[3] = positions[3];
    StepGrpPos[4] = positions[4];
    stepperGrp.moveTo(StepGrpPos);
    Coordinates[2] -= 0.1;
  }
}

void lowerStamp()
{
  if (Temperature > TempSet || Debug)
  {
    TempAllow = true;
  }
  if (TempAllow && NeedleIsDropped)
  {
    if (!SG)
    {
      if (TopZ.currentPosition() == positions[2])
      {
        BGTask[12] = false;
        BGTask[14] = true;
        STime = millis();
        displayCoordPos();
      }
      else
      {
        positions[2] = LengthDownToStamp*Step_TopZ;
        XYZStepsBuilder(positions[0],positions[1],positions[2],positions[3],positions[4]);
        StepGrpPos[0] = positions[0];
        StepGrpPos[1] = positions[1];
        StepGrpPos[2] = positions[2];
        StepGrpPos[3] = positions[3];
        StepGrpPos[4] = positions[4];
        stepperGrp.moveTo(StepGrpPos);
        Coordinates[2] = LengthDownToStamp;
      }
      TempAllow = false;
    }
  }
}

void raiseStamp()
{
  if (!SG)
  {
    if (TopZ.currentPosition() == positions[2])
    {
      if (Coordinates[0] > 90 && Coordinates[0] < 560)
      {
        positions[0] += Step_TopLeftXY*StitchLength;
        positions[1] += Step_TopRightXY*StitchLength;
//        positions[3] += Step_BottomLeftXY*StitchLength;
//        positions[4] += Step_BottomRightXY*StitchLength; 
        XYZStepsBuilder(positions[0],positions[1],positions[2],positions[3],positions[4]);
        StepGrpPos[0] = positions[0];
        StepGrpPos[1] = positions[1];
        StepGrpPos[2] = positions[2];
        StepGrpPos[3] = positions[3];
        StepGrpPos[4] = positions[4];
        Coordinates[0] += StitchLength;
//        Coordinates[3] += StitchLength;
        stepperGrp.moveTo(StepGrpPos);
        BGTask[15] = true; 
      }
      else
      {
        Serial.println(95485);
      }
      BGTask[13] = false;
      displayCoordPos();
    }
    else
    {
      positions[2] = RaisedPosAfterStamp*Step_TopZ;
      XYZStepsBuilder(positions[0],positions[1],positions[2],positions[3],positions[4]);
      StepGrpPos[0] = positions[0];
      StepGrpPos[1] = positions[1];
      StepGrpPos[2] = positions[2];
      StepGrpPos[3] = positions[3];
      StepGrpPos[4] = positions[4];
      stepperGrp.moveTo(StepGrpPos);
      Coordinates[2] = RaisedPosAfterStamp;
    }
  }
}

void homeCutter()
{
  if (IsCutterClose())
  {
    if (Debug)
    {
      Serial.println("Cutter is raised.");
    }
    BGTask[7] = false;
    Cutter.setCurrentPosition(0);
    positions[5] = 0;
    Serial.println(95477);
  }
  else if (!CUT)
  {
    positions[5] -= 1;
    Cutter.moveTo(positions[5]);
  }
}

void lowerCutter()
{
  if (Cutter.currentPosition() == positions[5])
  {
    BGTask[6] = false;
    Serial.println(95478);
    if (Debug)
    {
      Serial.println("Cutter lowered"); 
    }
  }
}

void homeTop()
{
  if (IsTopXClose() && IsTopYClose() && IsTopZClose())
  {
    if (Debug)
    {
      Serial.println("Top X, Y, Z Homed. mm moved before home: ");
      Serial.print("x:");
      Serial.print(TXHomeVal);
      Serial.println("mm");
      Serial.print("y:");
      Serial.print(TYHomeVal);
      Serial.println("mm");
      Serial.print("z:");
      Serial.print(TZHomeVal);
      Serial.println("mm");
    }
    Serial.println(95480);
    BGTask[5] = false;
    TopZ.setCurrentPosition(0);
    positions[2] = 0;
    Coordinates[0] = 0;
    Coordinates[1] = 0;
    Coordinates[2] = 0;
    displayCoordPos();
  }
  else if (!SG)
  {
    if (Coordinates[0] >= -5000 || Coordinates[0] > 0)
    {
      positions[0] -= Coordinates[0]*Step_TopLeftXY;
      positions[1] -= Coordinates[0]*Step_TopRightXY;
      TXHomeVal -= Coordinates[0];
      Coordinates[0] -= Coordinates[0];
    }
    else if (IsTopXClose()){}
    else
    {
      positions[0] -= 0.1*Step_TopLeftXY;
      positions[1] -= 0.1*Step_TopRightXY;
      TXHomeVal -= 0.1;
    }
    if (Coordinates[1] >= -5000 || Coordinates[1] > 0)
    {
      positions[0] += Coordinates[1]*Step_TopLeftXY;
      positions[1] -= Coordinates[1]*Step_TopRightXY;
      TYHomeVal -= Coordinates[1];
      Coordinates[1] -= Coordinates[1];
    }
    else if (IsTopYClose()){}
    else
    {
      positions[0] += 0.1*Step_TopLeftXY;
      positions[1] -= 0.1*Step_TopRightXY;
      TZHomeVal -= 0.1;
    }
    if (Coordinates[2] >= -5000 || Coordinates[2] > 0)
    {
      positions[2] -= Coordinates[2]*Step_TopZ;
      TZHomeVal -= Coordinates[2];
      Coordinates[2] -= Coordinates[2];
    }
    else if (IsTopZClose()){}
    else
    {
      positions[2] -= 0.1*Step_TopRightXY;
      TZHomeVal -= 0.1;
    }
    XYZStepsBuilder(positions[0],positions[1],positions[2],positions[3],positions[4]);
    StepGrpPos[0] = positions[0];
    StepGrpPos[1] = positions[1];
    StepGrpPos[2] = positions[2];
    StepGrpPos[3] = positions[3];
    StepGrpPos[4] = positions[4];
    stepperGrp.moveTo(StepGrpPos);
  }
}

void homeTopX()
{
  if (IsTopXClose())
  {
    if (Debug)
    {
      Serial.print("Top X Homed. mm moved before home: ");
      Serial.print(TXHomeVal);
      Serial.println("mm");
    }
    Serial.println(95486);
    BGTask[0] = false;
    Coordinates[0] = 0;
    displayCoordPos();
  }
  else if (!SG)
  {
    if (Coordinates[0] >= -5000 || Coordinates[0] > 0)
    {
      positions[0] -= Coordinates[0]*Step_TopLeftXY;
      positions[1] -= Coordinates[0]*Step_TopRightXY;
      XYZStepsBuilder(positions[0],positions[1],positions[2],positions[3],positions[4]);
      StepGrpPos[0] = positions[0];
      StepGrpPos[1] = positions[1];
      StepGrpPos[2] = positions[2];
      StepGrpPos[3] = positions[3];
      StepGrpPos[4] = positions[4];
      stepperGrp.moveTo(StepGrpPos);
      TXHomeVal -= Coordinates[0];
      Coordinates[0] -= Coordinates[0];
    }
    else
    {
      positions[0] -= 0.1*Step_TopLeftXY;
      positions[1] -= 0.1*Step_TopRightXY;
      XYZStepsBuilder(positions[0],positions[1],positions[2],positions[3],positions[4]);
      StepGrpPos[0] = positions[0];
      StepGrpPos[1] = positions[1];
      StepGrpPos[2] = positions[2];
      StepGrpPos[3] = positions[3];
      StepGrpPos[4] = positions[4];
      stepperGrp.moveTo(StepGrpPos);
      TXHomeVal -= 0.1;
    }
  }
}

void homeTopY()
{
  if(IsTopYClose())
  {
    if (Debug)
    {
      Serial.print("Top Y Homed. mm moved before home: ");
      Serial.print(TYHomeVal);
      Serial.println("mm");
    }
    Serial.println(95487);
    BGTask[1] = false;
    Coordinates[1] = 0;
    displayCoordPos();
  }
  else if (!SG)
  {
    if (Coordinates[1] >= -5000 || Coordinates[1] > 0)
    {
      positions[0] += Coordinates[1]*Step_TopLeftXY;
      positions[1] -= Coordinates[1]*Step_TopRightXY;
      XYZStepsBuilder(positions[0],positions[1],positions[2],positions[3],positions[4]);
      StepGrpPos[0] = positions[0];
      StepGrpPos[1] = positions[1];
      StepGrpPos[2] = positions[2];
      StepGrpPos[3] = positions[3];
      StepGrpPos[4] = positions[4];
      stepperGrp.moveTo(StepGrpPos);
      TYHomeVal -= Coordinates[1];
      Coordinates[1] -= Coordinates[1];
    }
    else
    {
      positions[0] += 0.1*Step_TopLeftXY;
      positions[1] -= 0.1*Step_TopRightXY;
      XYZStepsBuilder(positions[0],positions[1],positions[2],positions[3],positions[4]);
      StepGrpPos[0] = positions[0];
      StepGrpPos[1] = positions[1];
      StepGrpPos[2] = positions[2];
      StepGrpPos[3] = positions[3];
      StepGrpPos[4] = positions[4];
      stepperGrp.moveTo(StepGrpPos);
      TYHomeVal -= 0.1;
    }
  }
}

void homeTopZ()
{
  if (IsTopZClose())
  {
    if (Debug)
    {
      Serial.println("Top Z Homed.");
    }
    Serial.println(95490);
    TopZ.setCurrentPosition(0);
    positions[2] = 0;
    BGTask[4] = false;
    Coordinates[2] = 0;
    displayCoordPos();
  }
  else if (!SG)
  {
    if (Coordinates[2] >= -5000 || Coordinates[2] > 0)
    {
      positions[2] -= Coordinates[2]*Step_TopZ;
      XYZStepsBuilder(positions[0],positions[1],positions[2],positions[3],positions[4]);
      StepGrpPos[0] = positions[0];
      StepGrpPos[1] = positions[1];
      StepGrpPos[2] = positions[2];
      StepGrpPos[3] = positions[3];
      StepGrpPos[4] = positions[4];
      stepperGrp.moveTo(StepGrpPos);
      TZHomeVal -= Coordinates[2];
      Coordinates[2] -= Coordinates[2];
    }
    else
    {
      positions[2] -= 0.1*Step_TopZ;
      XYZStepsBuilder(positions[0],positions[1],positions[2],positions[3],positions[4]);
      StepGrpPos[0] = positions[0];
      StepGrpPos[1] = positions[1];
      StepGrpPos[2] = positions[2];
      StepGrpPos[3] = positions[3];
      StepGrpPos[4] = positions[4];
      stepperGrp.moveTo(StepGrpPos);
    }
  }
}

void homeBottomX()
{
  if(IsBottomXClose())
  {
    if (Debug)
    {
      Serial.print("Bottom X Homed. mm moved before home: ");
      Serial.print(BXHomeVal);
      Serial.println("mm");
    }
    Serial.println(95488);
    BGTask[2] = false;
    Coordinates[3] = 0;
    displayCoordPos();
  }
  else if (!SG)
  {
    if (Coordinates[3] >= -5000 || Coordinates[3] > 0)
    {
      positions[3] -= Coordinates[3]*Step_BottomLeftXY;
      positions[4] -= Coordinates[3]*Step_BottomRightXY;
      XYZStepsBuilder(positions[0],positions[1],positions[2],positions[3],positions[4]);
      StepGrpPos[0] = positions[0];
      StepGrpPos[1] = positions[1];
      StepGrpPos[2] = positions[2];
      StepGrpPos[3] = positions[3];
      StepGrpPos[4] = positions[4];
      stepperGrp.moveTo(StepGrpPos);
      BXHomeVal -= Coordinates[3];
      Coordinates[3] -= Coordinates[3];
    }
    else
    {
      positions[3] -= 0.1*Step_BottomLeftXY;
      positions[4] -= 0.1*Step_BottomRightXY;
      XYZStepsBuilder(positions[0],positions[1],positions[2],positions[3],positions[4]);
      StepGrpPos[0] = positions[0];
      StepGrpPos[1] = positions[1];
      StepGrpPos[2] = positions[2];
      StepGrpPos[3] = positions[3];
      StepGrpPos[4] = positions[4];
      stepperGrp.moveTo(StepGrpPos);
      BXHomeVal -= 0.1;
    }
  }
}

void homeBottomY()
{
  if(IsBottomYClose())
  {
    if (Debug)
    {
      Serial.print("Bottom Y Homed. mm moved before home: ");
      Serial.print(BYHomeVal);
      Serial.println("mm");
    }
    Serial.println(95489);
    BGTask[3] = false;
    Coordinates[4] = 0;
    displayCoordPos();
  }
  else if (!SG)
  {
    if (Coordinates[4] >= -5000 || Coordinates[4] > 0)
    {
      positions[3] += Coordinates[4]*Step_BottomLeftXY;
      positions[4] -= Coordinates[4]*Step_BottomRightXY;
      XYZStepsBuilder(positions[0],positions[1],positions[2],positions[3],positions[4]);
      StepGrpPos[0] = positions[0];
      StepGrpPos[1] = positions[1];
      StepGrpPos[2] = positions[2];
      StepGrpPos[3] = positions[3];
      StepGrpPos[4] = positions[4];
      stepperGrp.moveTo(StepGrpPos);
      BYHomeVal -= Coordinates[4];
      Coordinates[4] -= Coordinates[4];
    }
    else
    {
      positions[3] += 0.1*Step_BottomLeftXY;
      positions[4] -= 0.1*Step_BottomRightXY;
      XYZStepsBuilder(positions[0],positions[1],positions[2],positions[3],positions[4]);
      StepGrpPos[0] = positions[0];
      StepGrpPos[1] = positions[1];
      StepGrpPos[2] = positions[2];
      StepGrpPos[3] = positions[3];
      StepGrpPos[4] = positions[4];
      stepperGrp.moveTo(StepGrpPos);
      BYHomeVal -= 0.1;
    }
  }
}

// -- Movement dissection -- //
// -- "Gantry Only", accuracy to 0.1mm --//
void CoreXYMovement()
{
  BGTask[16] = false;
  TL_Val = 0;
  TR_Val = 0;
  BL_Val = 0;
  BR_Val = 0;
  Z_Val = 0;
  if (Task[0] != 0)
  {
    Coordinates[0] += Task[0];
    TL_Val += Task[0];
    TR_Val += Task[0];
    BGTask[16] = true;
    Task[0] = 0;
    
  }
  if (Task[1] != 0)
  {
    Coordinates[1] += Task[1];
    TL_Val -= Task[1];
    TR_Val += Task[1];
    BGTask[16] = true;
    Task[1] = 0;
  }
  if (Task[2] != 0)
  {
    Coordinates[2] += Task[2];
    Z_Val += Task[2];
    BGTask[16] = true;
    Task[2] = 0;
  }
  if (Task[3] != 0)
  {
    Coordinates[3] += Task[3];
    BL_Val += Task[3];
    BR_Val += Task[3];
    BGTask[16] = true;
    Task[3] = 0;
  }
  if (Task[4] != 0)
  {
    Coordinates[4] += Task[4];
    BL_Val -= Task[4];
    BR_Val += Task[4];
    BGTask[16] = true;
    Task[4] = 0;
  }
}

void TaskDissection(char BufferVal, String BufferCoord)
{
  if (BufferVal == 'X')
  {
    Task[0] = BufferCoord.toFloat();
  }
  else if (BufferVal == 'Y')
  {
    Task[1] = BufferCoord.toFloat();
  }
  else if (BufferVal == 'Z')
  {
    Task[2] = BufferCoord.toFloat();
  }
  else if (BufferVal == 'Q')
  {
    Task[3] = BufferCoord.toFloat();
  }
  else if (BufferVal == 'W')
  {
    Task[4] = BufferCoord.toFloat();
  }
  else if (BufferVal == 'T')
  {
    Task[5] = BufferCoord.toFloat();
    setPoint = Task[5];
  }
}

void GantryMovement()
{
  CoreXYMovement();   
  XYZStepsBuilder(positions[0]+Step_TopLeftXY*TL_Val,
                positions[1]+Step_TopRightXY*TR_Val,
                positions[2]+Step_TopZ*Z_Val,
                positions[3]+Step_BottomLeftXY*BL_Val,
                positions[4]+Step_BottomRightXY*BR_Val);
  StepGrpPos[0] = positions[0];
  StepGrpPos[1] = positions[1];
  StepGrpPos[2] = positions[2];
  StepGrpPos[3] = positions[3];
  StepGrpPos[4] = positions[4]; 
  displayCoordPos();
  stepperGrp.moveTo(StepGrpPos);
}

// -- Positions for Storer -- //
// -- "Gantry Only" -- //
void XYZStepsBuilder(long Pos_TopLeftXY, long Pos_TopRightXY, long Pos_TopZ, 
                  long Pos_BottomLeftXY, long Pos_BottomRightXY)
{
  positions[0] = Pos_TopLeftXY;
  positions[1] = Pos_TopRightXY;
  positions[2] = Pos_TopZ;
  positions[3] = Pos_BottomLeftXY;
  positions[4] = Pos_BottomRightXY;
}

// -- Limit Switch display generator -- //
void displayLimitSwitch()
{
  if (PrintES)
  {
    Serial.print(IsTopXClose());
    Serial.print(", ");
    Serial.print(IsTopYClose());
    Serial.print(", ");
    Serial.print(IsTopYMaxClose());
    Serial.print(", ");
    Serial.print(IsTopZClose());
    Serial.print(", ");
    Serial.print(IsBottomXClose());
    Serial.print(", ");
    Serial.print(IsBottomYClose());
    Serial.print(", ");
    Serial.print(IsCutterClose());
    Serial.print(", ");
    Serial.print(IsDoorClose());
    Serial.print(", ");
    Serial.print(IsTemplateLeftClose());
    Serial.print(", ");
    Serial.println(IsTemplateRightClose());
  }
}
// -- Coordinate display generator -- //
void displayCoordPos()
{
  if (Debug)
  {
    Serial.println("---------------------");
    if(CoordPos)
    {
      Serial.println("Positions: ");
      for (int i = 0; i < 4; i ++)
      {
        Serial.print(positions[i]);
        Serial.print(", ");
      }
      Serial.println(positions[4]);
    }
    else
    {
      Serial.println("Coordinates: ");
      Serial.print("Top X:");
      Serial.print(Coordinates[0]);
      Serial.print(", Top Y:");
      Serial.print(Coordinates[1]);
      Serial.print(", Top Z:");
      Serial.println(Coordinates[2]);
      Serial.print("Bottom X:");
      Serial.print(Coordinates[3]);
      Serial.print(", Bottom Y:");
      Serial.println(Coordinates[4]);
    }
    Serial.println("---------------------");
  }
}

void safetySwitchChecker()
{
  if (SafetySwitch)
  {
    if (!IsDoorClose())
    {
      if (!SSsend)
      {
        Serial.println(95481);
        SafePause = true;
        SSsend = true;
        SRsend = false;
      }
    }
    else
    {
      if (!SRsend)
      {
        Serial.println(95482);
        SafePause = false;
        SSsend = false;
        SRsend = true;
      }
    }
  }
  else
  {
    SafePause = false;
    SSsend = false;
    SRsend = true;
  }
}

void setStepperSpeed()
{
  TopLeftXY.setMaxSpeed(Speed_TopGantry);
  TopRightXY.setMaxSpeed(Speed_TopGantry);
  TopZ.setMaxSpeed(Speed_TopGantry_Z);
  Cutter.setMaxSpeed(Speed_Cutter_Raise_Lower);
  TemplateLeft.setMaxSpeed(Speed_Template);
  TemplateRight.setMaxSpeed(Speed_Template);
  BottomLeftXY.setMaxSpeed(Speed_BottomGantry);
  BottomRightXY.setMaxSpeed(Speed_BottomGantry);
}

// -- Temperature Conversion -- //
// -- Modified from example code -- //
void readTTemperature()
{
  average = 0;
  for (int i = 0; i < NUMSAMPLES; i++)
  {
    samples[i] = analogRead(THERMISTORPIN);    
  }  
  for (int i=0; i< NUMSAMPLES; i++) 
  {
     average += samples[i];
  }
  average /= NUMSAMPLES;
  // convert the value to resistance
  average = 1023 / average - 1;
  average = SERIESRESISTOR / average;
  
  Temperature = average / THERMISTORNOMINAL;          // (R/Ro)
  Temperature = log(Temperature);                     // ln(R/Ro)
  Temperature /= BCOEFFICIENT;                        // 1/B * ln(R/Ro)
  Temperature += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
  Temperature = 1.0 / Temperature;                    // Invert
  Temperature -= 273.15;                              // convert absolute temp to C
  Temperature = round(Temperature);
  if (CheckTemperature)
  {
    Serial.print("Temperature: "); 
    Serial.print(Temperature);
    Serial.println(" *C");
  }  
}

// -- PID Control -- //
void PIDTemperature()
{
  readTTemperature();
  myController.compute();
  analogWrite(A1, outputVal);
}

// -- Decimal Sign checker -- //
int signs(double val)
{
  if (val > 0)        {return 1;}
  else if (val <0)    {return -1;}
  else if (val == 0)  {return 0;}
}

void TemplateOpenLock()
{
  if(IsTemplateLeftClose() && IsTemplateRightClose())
  {
    BGTask[8] = false;
    TemplateLeft.setCurrentPosition(0);
    TemplateRight.setCurrentPosition(0);
    positions[6] = 0;
    positions[7] = 0;
    Serial.println(95476);
    if (Debug)
    {
      Serial.println("Template unlocked.");
    }
  }
  else
  {
    long ValPos[2];
    if (!IsTemplateLeftClose() && !TEM)
    {
      positions[6] -= 1;
    }
    if (!IsTemplateRightClose()  && !TEM)
    {
      positions[7] -= 1;
    }
    ValPos[0] = positions[6];
    ValPos[1] = positions[7];
    templates.moveTo(ValPos);    
  }
}

void TemplateCloseLock()
{
  if (TemplateLeft.currentPosition() == steps_To_Lock && TemplateRight.currentPosition() == steps_To_Lock)
  {
    BGTask[9] = false;
    Serial.println(95475);
    if (Debug)
    {
      Serial.println("Template locked.");
    }
  }
}

// -- Limit Switches -- //
// -- Check top X is close --//
bool IsTopXClose()
{
  for (int i = 0; i < ESNoOfCount; i++)
  {
    if (digitalRead(ES_Top_X) == LOW)
      ESCount[0] +=1;  
  }
  if (ESCount[0] >= ESNoOfCount/2)
  {
    ESCount[0] = 0;
    return true;
  }
  else
  {
    ESCount[0] = 0;
    return false;
  }    
}

// -- Check top Y is close --//
bool IsTopYClose()
{
  for (int i = 0; i < ESNoOfCount; i++)
  {
    if (digitalRead(ES_Top_Y) == LOW)
      ESCount[1] +=1;      
  }
  if (ESCount[1] >= ESNoOfCount/2)
  {
    ESCount[1] = 0;
    return true;
  }
  else
  {
    ESCount[1] = 0;
    return false;
  }
}

// -- Check top Y max is close --//
bool IsTopYMaxClose()
{
  for (int i = 0; i < ESNoOfCount; i++)
  {
    if (digitalRead(ES_Top_Y_Max) == LOW)
      ESCount[2] +=1;      
  }
  if (ESCount[2] >= ESNoOfCount/2)
  {
    ESCount[2] = 0;
    return true;
  }
  else
  {
    ESCount[2] = 0;
    return false;
  }
}

// -- Check top Z is close --//
bool IsTopZClose()
{
  for (int i = 0; i < ESNoOfCount; i++)
  {
    if (digitalRead(ES_Top_Z) == LOW)
      ESCount[3] +=1;   
  }
  if (ESCount[3] >= ESNoOfCount/2)
  {
    ESCount[3] = 0;
    return true;
  }
  else
  {
    ESCount[3] = 0;
    return false;
  }   
}

// -- Check bottom X is close --//
bool IsBottomXClose()
{
  for (int i = 0; i < ESNoOfCount; i++)
  {
    if (digitalRead(ES_Bottom_X) == LOW)
      ESCount[4] +=1;  
  }
  if (ESCount[4] >= ESNoOfCount/2)
  {
    ESCount[4] = 0;
    return true;
  }
  else
  {
    ESCount[4] = 0;
    return false;
  }    
}

// -- Check top Y is close --//
bool IsBottomYClose()
{
  for (int i = 0; i < ESNoOfCount; i++)
  {
    if (digitalRead(ES_Bottom_Y) == LOW)
      ESCount[5] +=1;      
  }
  if (ESCount[5] >= ESNoOfCount/2)
  {
    ESCount[5] = 0;
    return true;
  }
  else
  {
    ESCount[5] = 0;
    return false;
  }
}

// -- Check cutter is close --//
bool IsCutterClose()
{
  for (int i = 0; i < ESNoOfCount; i++)
  {
    if (digitalRead(ES_Cutter) == LOW)
      ESCount[6] +=1;    
  }
  if (ESCount[6] >= ESNoOfCount/2)
  {
    ESCount[6] = 0;
    return true;
  }
  else
  {
    ESCount[6] = 0;
    return false;
  }  
}

// -- Check template left is close --//
bool IsTemplateLeftClose()
{
  for (int i = 0; i < ESNoOfCount; i++)
  {
    if (digitalRead(ES_Template_Left) == LOW)
      ESCount[7] +=1;    
  }
  if (ESCount[7] >= ESNoOfCount/2)
  {
    ESCount[7] = 0;
    return true;
  }
  else
  {
    ESCount[7] = 0;
    return false;
  }  
}

// -- Check template right is close --//
bool IsTemplateRightClose()
{
  for (int i = 0; i < ESNoOfCount; i++)
  {
    if (digitalRead(ES_Template_Right) == LOW)
      ESCount[8] +=1;    
  }
  if (ESCount[8] >= ESNoOfCount/2)
  {
    ESCount[8] = 0;
    return true;
  }
  else
  {
    ESCount[8] = 0;
    return false;
  }  
}

// -- Check door is close --//
bool IsDoorClose()
{
  for (int i = 0; i < ESNoOfCount; i++)
  {
    if (digitalRead(ES_Door) == LOW)
      ESCount[9] +=1;    
  }
  if (ESCount[9] >= ESNoOfCount/2)
  {
    ESCount[9] = 0;
    return true;
  }
  else
  {
    ESCount[9] = 0;
    return false;
  }  
}
