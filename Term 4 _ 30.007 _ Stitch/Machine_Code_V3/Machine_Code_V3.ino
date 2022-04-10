#include <AccelStepper.h>
#include <MultiStepper.h>

// Stepper Motor Declaration from front perspective 
// Pin Declaration: (AccelStepper::Driver,StepPin,DirPin);
AccelStepper TopLeftXY      (AccelStepper::DRIVER,45,44);
AccelStepper TopRightXY     (AccelStepper::DRIVER,47,46);
AccelStepper TopZ           (AccelStepper::DRIVER,49,48);
AccelStepper Needle         (AccelStepper::DRIVER,51,50);
AccelStepper Cutter         (AccelStepper::DRIVER,53,52);

AccelStepper TemplateLeft   (AccelStepper::DRIVER,27,26);
AccelStepper TemplateRight  (AccelStepper::DRIVER,23,22);

AccelStepper BottomLeftXY   (AccelStepper::DRIVER,31,30);
AccelStepper BottomRightXY  (AccelStepper::DRIVER,29,28);
//AccelStepper BottomZ        (AccelStepper::DRIVER,27,26);
AccelStepper Bobbin         (AccelStepper::DRIVER,25,24);

// Endstops Pin Declaration
int ES_Top_X           = 36;
int ES_Top_Y           = 34;
int ES_Top_Y_Max       = 39;
int ES_Top_Z           = 32;
int ES_Bottom_X        = 38;
int ES_Bottom_Y        = 40;
int ES_Cutter          = 37;
int ES_Template_Left   = 33;
int ES_Template_Right  = 35;

// MaxSpeed Tuning
int Speed_TopGantry           = 500;
int Speed_TopGantry_Z         = 1000;
int Speed_BottomGantry        = 1000;
int Speed_Sewing              = 1000;
int Speed_Bobbin              = 4000;
int Speed_Template            = 100;
int Speed_Cutter_Raise_Lower  = 1000;

// Acceleration Tuning
int Accel_TopGantry           = 1000;
int Accel_TopGantry_Z         = 1000;
int Accel_BottomGantry        = 1000;
int Accel_Sewing              = 1000;
int Accel_Template            = 1000;
int Accel_Cutter_Raise_Lower  = 300;

// MultiStepper Grouping
MultiStepper stepperGrp;
MultiStepper sewingZ;
MultiStepper templates;

// Revolution per mm Tuning
int Step_TopLeftXY      = 40;
int Step_TopRightXY     = 40;
int Step_TopZ           = 796;
int Step_Needle         = 1;
int Step_Cutter         = 1;

int Step_TemplateLeft   = 1;
int Step_TemplateRight  = 1;

int Step_BottomLeftXY   = 40;
int Step_BottomRightXY  = 40;
int Step_Bobbin         = 1;

// Calibration Setting/ Offset
int ESCount[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
int ESNoOfCount = 4;
int StitchLength = 10;
int NoOfBobbinLoop = 2;

// Settings Variable
bool Debug              = true;
bool TestMode           = false;
bool PrintES            = false;
bool CoordPos           = false;
bool NeedleIsDropped    = false;
bool Sewing             = false;

// Temporary Variable
float Coordinates[5]  = {-9999, -9999, -9999, -9999, -9999};  // {TX, TY, TZ, BX, BY}
long positions[5]     = {0, 0, 0, 0, 0};                      // {TLeft, TRight, TZ, BLeft, BRight}
float Task[5]         = {0, 0, 0, 0, 0};
float TL_Val, TR_Val, BL_Val, BR_Val = 0.0;
String TaskAvailable  = "";
void setup() 
{
  Serial.begin(115200);
  Serial.println("This program written by team Stitch.");
  Serial.println("Member: Teo Wei Qing, Seow Sin Kiat, Liu Tong Han, Brian Chin, Ernest Ng.");
  Serial.println("Initialising functions...");

  // Stepper Max Speed Initialisation
  TopLeftXY.setMaxSpeed(Speed_TopGantry);
  TopRightXY.setMaxSpeed(Speed_TopGantry);
  TopZ.setMaxSpeed(Speed_TopGantry_Z);
  Needle.setMaxSpeed(Speed_Sewing);
  Cutter.setMaxSpeed(Speed_Cutter_Raise_Lower);

  TemplateLeft.setMaxSpeed(Speed_Template);
  TemplateRight.setMaxSpeed(Speed_Template);

  BottomLeftXY.setMaxSpeed(Speed_BottomGantry);
  BottomRightXY.setMaxSpeed(Speed_BottomGantry);
  Bobbin.setMaxSpeed(Speed_Bobbin);

  Cutter.setSpeed(1000);

  // Stepper Acceleration Initialisation
  TopLeftXY.setAcceleration(Accel_TopGantry);
  TopRightXY.setAcceleration(Accel_TopGantry);
  TopZ.setAcceleration(Accel_TopGantry_Z);
  Needle.setAcceleration(Accel_Sewing);
  Cutter.setAcceleration(Accel_Cutter_Raise_Lower);

  TemplateLeft.setAcceleration(Accel_Template);
  TemplateRight.setAcceleration(Accel_Template);

  BottomLeftXY.setAcceleration(Accel_BottomGantry);
  BottomRightXY.setAcceleration(Accel_BottomGantry);
  Bobbin.setAcceleration(Accel_Sewing);

  // Add Stepper To Group
  stepperGrp.addStepper(TopLeftXY);
  stepperGrp.addStepper(TopRightXY);
  stepperGrp.addStepper(TopZ);
  stepperGrp.addStepper(BottomLeftXY);
  stepperGrp.addStepper(BottomRightXY);

  // Add Stepper To Group
  sewingZ.addStepper(Bobbin);
  sewingZ.addStepper(Needle);

  // Add Stepper To Group
  templates.addStepper(TemplateLeft);
  templates.addStepper(TemplateRight);

  // Add EndStop
  pinMode(ES_Top_X, INPUT_PULLUP);
  pinMode(ES_Top_Y, INPUT_PULLUP);
  pinMode(ES_Top_Y_Max, INPUT_PULLUP);
  pinMode(ES_Top_Z, INPUT_PULLUP);
  pinMode(ES_Bottom_X, INPUT_PULLUP);
  pinMode(ES_Bottom_Y, INPUT_PULLUP);
  pinMode(ES_Cutter, INPUT_PULLUP);
  pinMode(ES_Template_Left, INPUT_PULLUP);
  pinMode(ES_Template_Right, INPUT_PULLUP);

  Serial.println("Functions initialised. Waiting for Command.");
  Serial.println("---------------------");
}

void loop() 
{
  if (TestMode)
  {
    Needle.moveTo(-1000);
    Needle.runToPosition();
    TestMode = false;
  }
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
//    Serial.print(", ");
//    Serial.print(IsBottomZClose());
    Serial.print(", ");
    Serial.print(IsCutterClose());
    Serial.print(", ");
    Serial.print(IsTemplateLeftClose());
    Serial.print(", ");
    Serial.println(IsTemplateRightClose());
  }
  if (TaskAvailable == "Movement")
  {
    CoreXYMovement();    
    StepsBuilder(positions[0]+Step_TopLeftXY*TL_Val,positions[1]+Step_TopRightXY*TR_Val,positions[2]+Step_TopZ*Task[2],positions[3]+Step_BottomLeftXY*BL_Val,positions[4]+Step_BottomRightXY*BR_Val);
    stepperGrp.moveTo(positions);
    stepperGrp.runSpeedToPosition();
    TL_Val = 0;
    TR_Val = 0;
    BL_Val = 0;
    BR_Val = 0;
    if (Task[2] != 0)
    {
      Task[2] = 0;
    }
    displayCoordPos();
//    Serial.write(95478);
    TaskAvailable = "";
  }
  else if (TaskAvailable == "Stitch" && (NeedleIsDropped || Debug))
  {
    long SewPos[2] = {-1600, -1600};
    Needle.setCurrentPosition(0);
    Bobbin.setCurrentPosition(0);
    Needle.moveTo(-1000);
    Needle.runToPosition();
    Bobbin.moveTo(-400);
    Bobbin.runToPosition();
//    for (int i = 0; i < NoOfBobbinLoop; i++)
//    {
//      Bobbin.moveTo(-800);
//      Bobbin.runToPosition();
//      Bobbin.moveTo(0);
//      Bobbin.runToPosition();
//    }
    sewingZ.moveTo(SewPos);
    sewingZ.runSpeedToPosition();
//    Bobbin.moveTo(-1600);
//    Bobbin.runToPosition();
//    Needle.moveTo(-1600);
//    Needle.runToPosition();
    TaskAvailable = "";
    if (Sewing && Coordinates[0] >= 95 && Coordinates[0] < 635)
    {
      positions[0] += Step_TopLeftXY*StitchLength/2;
      positions[1] += Step_TopRightXY*StitchLength/2;
      positions[2] -= Step_TopZ;
      positions[3] += Step_BottomLeftXY*StitchLength/2;
      positions[4] += Step_BottomRightXY*StitchLength/2;
      positions[5] += 0;
      StepsBuilder(positions[0],positions[1],positions[2],positions[3],positions[4]);
      stepperGrp.moveTo(positions);
      stepperGrp.runSpeedToPosition();
      Coordinates[0] += StitchLength/2;
      Coordinates[2] += 1;
      Coordinates[3] += StitchLength/2;
      displayCoordPos();
      positions[0] += Step_TopLeftXY*StitchLength/2;
      positions[1] += Step_TopRightXY*StitchLength/2;
      positions[2] = 0;
      positions[3] += Step_BottomLeftXY*StitchLength/2;
      positions[4] += Step_BottomRightXY*StitchLength/2;
      positions[5] += 0;
      StepsBuilder(positions[0],positions[1],positions[2],positions[3],positions[4]);
      stepperGrp.moveTo(positions);
      stepperGrp.runSpeedToPosition();
      Coordinates[0] += StitchLength/2;
      Coordinates[2] -= 1;
      Coordinates[3] += StitchLength/2;
      displayCoordPos();
    }
  }
  if (Serial.available() > 0)
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
  else if (Val == "HOMETOPY")
  {
    homeTopY();
  }
  else if (Val == "HOMETOPX")
  {
    homeTopX();
  }
  else if (Val == "HOMETOPZ")
  {
    homeTopZ();
  }
  else if (Val == "HOMEBOTTOMX")
  {
    homeBottomX();
  }
  else if (Val == "HOMEBOTTOMY")
  {
    homeBottomY();
  }
  else if (Val == "HOMECUTTER" || Val == "RAISECUTTER")
  {
    homeCutter();
  }
  else if (Val == "LOWERCUTTER" || Val == "DROPCUTTER")
  {
    lowerCutter();
  }
  else if (Val == "NEEDLEDROP" || Val == "DROPNEEDLE" || Val == "LOWERNEEDLE")
  {
    dropNeedle();
  }
  else if (Val == "NEEDLERAISE" || Val == "RAISENEEDLE")
  {
    raiseNeedle();
  }
  else if (Val == "STITCHING" || Val == "STITCH" || Val == "THREAD" || Val == "THREADING")
  {
    if (Debug)
    {
      Serial.println("Beign Stitching...");
    }
    TaskAvailable = "Stitch";
  }
  else if (Val == "STOPSTITCHING" || Val == "STOPSTITCH" || Val == "STOPTHREAD" || Val == "STOPTHREADING")
  {
    if (Debug)
    {
      Serial.println("Stitching Stopped...");
    }
    TaskAvailable = "";
  }
  else if (Val == "SEWING" || Val == "SEW")
  {
    Sewing = true;
  }
  else if (Val == "STOPSEWING" || Val == "STOPSEW")
  {
    Sewing = false;
  }
  else if (Val == "OPENTEMPLATE" || Val == "UNLOCKTEMPLATE")
  {
    TemplateOpenLock();
  }
  else if (Val == "CLOSETEMPLATE" || Val == "LOCKTEMPLATE")
  {
    TemplateCloseLock();
  }
  else if (Val == "FASTHOMETOPY")
  {
    homeFastTopY();
  }
  else if (Val == "FASTHOMETOPX")
  {
    homeFastTopX();
  }
  else if (Val == "RUNTEST" || Val == "TESTRUN")
  {
    TestMode = true;
    if (Debug)
      Serial.println("Running Test.");
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
        GantryMovementStorer(BufferVal, BufferCoord);
        BufferCoord = "";
        BufferVal = NULL;
      }
      else if (BufferVal == NULL)
        BufferVal = Val[i];
      else
        BufferCoord += Val[i];
    }
    TaskAvailable = "Movement";
  }
}

void homeFastTopX()
{
  // positions[] = {TLeft, TRight, TZ, BLeft, BRight}
  if (Debug)
    Serial.println("Fast Homing Top X...");
  bool ValTop = IsTopXClose();
  float PosVal = 0;
  TopLeftXY.setMaxSpeed(4000);
  TopRightXY.setMaxSpeed(4000);
  while (!ValTop)
  {
    positions[0] -= 0.1*Step_TopLeftXY;
    positions[1] -= 0.1*Step_TopRightXY;
    StepsBuilder(positions[0],positions[1],positions[2],positions[3],positions[4]);
    stepperGrp.moveTo(positions);
    stepperGrp.runSpeedToPosition();
    PosVal -= 0.1;
    ValTop = IsTopXClose();
  }
  Coordinates[0] = 0;
  if (Debug)
  {
    Serial.print("Top X Fast Homed. mm moved before home: ");
    Serial.print(PosVal);
    Serial.println("mm");
  }
  TopLeftXY.setMaxSpeed(Speed_TopGantry);
  TopRightXY.setMaxSpeed(Speed_TopGantry);
  displayCoordPos();
}

void homeFastTopY()
{
  // positions[] = {TLeft, TRight, TZ, BLeft, BRight}
  if (Debug)
    Serial.println("Fast Homing Top Y...");
  bool ValTop = IsTopYClose();
  float PosVal = 0;
  TopLeftXY.setMaxSpeed(4000);
  TopRightXY.setMaxSpeed(4000);
  while (!ValTop)
  {
    positions[0] += 0.1*Step_TopLeftXY;
    positions[1] -= 0.1*Step_TopRightXY;
    StepsBuilder(positions[0],positions[1],positions[2],positions[3],positions[4]);
    stepperGrp.moveTo(positions);
    stepperGrp.runSpeedToPosition();
    PosVal -= 0.1;
    ValTop = IsTopYClose();
  }
  Coordinates[1] = 0;
  if (Debug)
  {
    Serial.print("Top Y Fast Homed. mm moved before home: ");
    Serial.print(PosVal);
    Serial.println("mm");
  }
  TopLeftXY.setMaxSpeed(Speed_TopGantry);
  TopRightXY.setMaxSpeed(Speed_TopGantry);
  displayCoordPos();
}

void homeTopX()
{
  // positions[] = {TLeft, TRight, TZ, BLeft, BRight}
  if (Debug)
    Serial.println("Homing Top X...");
  bool ValTop = IsTopXClose();
  float PosVal = 0;
  while (!ValTop)
  {
    positions[0] -= 0.1*Step_TopLeftXY;
    positions[1] -= 0.1*Step_TopRightXY;
    StepsBuilder(positions[0],positions[1],positions[2],positions[3],positions[4]);
    stepperGrp.moveTo(positions);
    stepperGrp.runSpeedToPosition();
    PosVal -= 0.1;
    ValTop = IsTopXClose();
  }
  Coordinates[0] = 0;
  if (Debug)
  {
    Serial.print("Top X Homed. mm moved before home: ");
    Serial.print(PosVal);
    Serial.println("mm");
  }
  displayCoordPos();
}

void homeTopY()
{
  // positions[] = {TLeft, TRight, TZ, BLeft, BRight}
  if (Debug)
    Serial.println("Homing Top Y...");
  bool ValTop = IsTopYClose();
  float PosVal = 0;
  while (!ValTop)
  {
    positions[0] += 0.1*Step_TopLeftXY;
    positions[1] -= 0.1*Step_TopRightXY;
    StepsBuilder(positions[0],positions[1],positions[2],positions[3],positions[4]);
    stepperGrp.moveTo(positions);
    stepperGrp.runSpeedToPosition();
    PosVal -= 0.1;
    ValTop = IsTopYClose();
  }
  Coordinates[1] = 0;
  if (Debug)
  {
    Serial.print("Top Y Homed. mm moved before home: ");
    Serial.print(PosVal);
    Serial.println("mm");
  }
  displayCoordPos();
}

void homeTopZ()
{
  // positions[] = {TLeft, TRight, TZ, BLeft, BRight}
  if (Debug)
    Serial.println("Homing Top Z...");
  positions[2] = 0;
  bool ValTop = IsTopZClose();
  long ZPos = 0;
  while (!ValTop)
  {
    ZPos -= 0.1*Step_TopZ;
    StepsBuilder(positions[0],positions[1],ZPos,positions[3],positions[4]);
    stepperGrp.moveTo(positions);
    stepperGrp.runSpeedToPosition();
    ValTop = IsTopZClose();    
  }
  TopZ.setCurrentPosition(0);
  positions[2] = 0;
  Coordinates[2] = 0;
  if (Debug)
    Serial.println("Top Z Homed.");
  displayCoordPos();
}

void homeBottomX()
{
  // positions[] = {TLeft, TRight, TZ, BLeft, BRight}
  if (Debug)
    Serial.println("Homing Bottom X...");
  bool ValBtm = IsBottomXClose();
  float PosVal = 0;
  while (!ValBtm)
  {
    positions[3] -= 0.1*Step_BottomLeftXY;
    positions[4] -= 0.1*Step_BottomRightXY;
    StepsBuilder(positions[0],positions[1],positions[2],positions[3],positions[4]);
    stepperGrp.moveTo(positions);
    stepperGrp.runSpeedToPosition();
    PosVal -= 0.1;
    ValBtm = IsBottomXClose();
  }
  Coordinates[3] = 0;
  if (Debug)
  {
    Serial.print("Bottom X Homed. mm moved before home: ");
    Serial.print(PosVal);
    Serial.println("mm");
  }
  displayCoordPos();
}

void homeBottomY()
{
  // positions[] = {TLeft, TRight, TZ, BLeft, BRight}
  if (Debug)
    Serial.println("Homing Bottom Y...");
  bool ValBtm = IsBottomYClose();
  float PosVal = 0;
  while (!ValBtm)
  {
    positions[3] += 0.1*Step_BottomLeftXY;
    positions[4] -= 0.1*Step_BottomRightXY;
    StepsBuilder(positions[0],positions[1],positions[2],positions[3],positions[4]);
    stepperGrp.moveTo(positions);
    stepperGrp.runSpeedToPosition();
    PosVal -= 0.1;
    ValBtm = IsBottomYClose();
  }
  Coordinates[4] = 0;
  if (Debug)
  {
    Serial.print("Bottom Y Homed. mm moved before home: ");
    Serial.print(PosVal);
    Serial.println("mm");
  }
  displayCoordPos();
}

void homeCutter()
{
  long CutterPos = 0;
  while(!IsCutterClose())
  {
    CutterPos -= 1;
    Cutter.moveTo(CutterPos);
    Cutter.runToPosition();    
  }
  Cutter.setCurrentPosition(0);
  if (Debug)
    Serial.println("Cutter is raised.");
}

void lowerCutter()
{
  Cutter.moveTo(900);
  Cutter.runToPosition();
  if (Debug)
    Serial.println("Cutter lowered");
}

void dropNeedle()
{
  if (Coordinates[2] != -9999 || Debug)
  {
    if (Debug)
      Serial.println("Lowering Needle...");
    for(int i = 0; i < 385; i++) //23
    {
      positions[2] += 0.1*Step_TopZ;
      StepsBuilder(positions[0],positions[1],positions[2],positions[3],positions[4]);
      stepperGrp.moveTo(positions);
      stepperGrp.runSpeedToPosition();
      Coordinates[2] += 0.1;
    }
    NeedleIsDropped = false;
    if (Debug)
      Serial.println("Needle Position Reached.");
    displayCoordPos();
  }
  else
  {
    if (Debug)
      Serial.println("Please Home Top Z.");  
  }
}

void raiseNeedle()
{
  if (Debug)
    Serial.println("Raising Needle...");
  while(!IsTopZClose())
  {
    positions[2] -= 0.1*Step_TopZ;
    StepsBuilder(positions[0],positions[1],positions[2],positions[3],positions[4]);
    stepperGrp.moveTo(positions);
    stepperGrp.runSpeedToPosition();
    Coordinates[2] -= 0.1;
  }
  NeedleIsDropped = false;
  if (Debug)
    Serial.println("Needle Position Reached.");
  displayCoordPos();
}

void GantryMovementStorer(char BufferVal, String BufferCoord)
{
  if (BufferVal == 'X')
  {
    Task[0] = BufferCoord.toFloat();
    Coordinates[0] += Task[0];
  }
  else if (BufferVal == 'Y')
  {
    Task[1] = BufferCoord.toFloat();
    Coordinates[1] += Task[1];
  }
  else if (BufferVal == 'Z')
  {
    Task[2] = BufferCoord.toFloat();
    Coordinates[2] += Task[2];
  }
  else if (BufferVal == 'Q')
  {
    Task[3] = BufferCoord.toFloat();
    Coordinates[3] += Task[3];
  }
  else if (BufferVal == 'W')
  {
    Task[4] = BufferCoord.toFloat();
    Coordinates[4] += Task[4];
  }
}

void CoreXYMovement()
{
  if (Task[0] != 0)
  {
    TL_Val += float(Task[0]);
    TR_Val += float(Task[0]);
    Task[0] = 0;
  }
  if (Task[1] != 0)
  {
    TL_Val -= float(Task[1]);
    TR_Val += float(Task[1]);
    Task[1] = 0;
  }
  if (Task[3] != 0)
  {
    BL_Val += float(Task[3]);
    BR_Val += float(Task[3]);
    Task[3] = 0;
  }
  if (Task[4] != 0)
  {
    BL_Val -= float(Task[4]);
    BR_Val += float(Task[4]);
    Task[4] = 0;  
  }
}

// Build Positions
void StepsBuilder(long Pos_TopLeftXY, long Pos_TopRightXY, long Pos_TopZ, 
                  long Pos_BottomLeftXY, long Pos_BottomRightXY)
{
  // positions[] = {TLeft, TRight, TZ, BLeft, BRight}
  positions[0] = Pos_TopLeftXY;
  positions[1] = Pos_TopRightXY;
  positions[2] = Pos_TopZ;
  positions[3] = Pos_BottomLeftXY;
  positions[4] = Pos_BottomRightXY;
}

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

void TemplateOpenLock()
{
  long ValPos[2] = {0, 0};
  while(!IsTemplateLeftClose() || !IsTemplateRightClose())
  {
    if(!IsTemplateLeftClose())
    {
      ValPos[0] -= 1;
    }
    if(!IsTemplateRightClose())
    {
      ValPos[1] -= 1;
    }
    templates.moveTo(ValPos);
    templates.runSpeedToPosition();
  }
  TemplateLeft.setCurrentPosition(0);
  TemplateRight.setCurrentPosition(0);
  if (Debug)
    Serial.println("Template unlocked.");
}

void TemplateCloseLock()
{
  long ValPos[2] = {830, 830};
  templates.moveTo(ValPos);
  templates.runSpeedToPosition();
  if (Debug)
    Serial.println("Template locked.");
}

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
