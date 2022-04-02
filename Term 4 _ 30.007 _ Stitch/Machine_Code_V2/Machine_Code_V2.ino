#include <AccelStepper.h>
#include <MultiStepper.h>

// Stepper Motor Declaration from front perspective 
// Pin Declaration: (AccelStepper::Driver,StepPin,DirPin);
AccelStepper TopLeftXY      (AccelStepper::DRIVER,45,44);
AccelStepper TopRightXY     (AccelStepper::DRIVER,47,46);
AccelStepper TopZ           (AccelStepper::DRIVER,49,48);
AccelStepper Needle         (AccelStepper::DRIVER,51,50);
AccelStepper Cutter         (AccelStepper::DRIVER,53,52);

AccelStepper TemplateLeft   (AccelStepper::DRIVER,5,4);
AccelStepper TemplateRight  (AccelStepper::DRIVER,3,2);

AccelStepper BottomLeftXY   (AccelStepper::DRIVER,31,30);
AccelStepper BottomRightXY  (AccelStepper::DRIVER,29,28);
AccelStepper BottomZ        (AccelStepper::DRIVER,27,26);
AccelStepper Bobbin         (AccelStepper::DRIVER,25,24);

// Endstops Pin Declaration
int ES_Top_X           = 36;
int ES_Top_Y           = 34;
int ES_Top_Z           = 32;
int ES_Bottom_X        = 42;
int ES_Bottom_Y        = 40;
int ES_Bottom_Z        = 38;
int ES_Cutter          = 37;
int ES_Template_Left   = 33;
int ES_Template_Right  = 35;

// MaxSpeed Tuning
int Speed_TopGantry           = 2500;
int Speed_TopGantry_Z         = 3500;
int Speed_BottomGantry        = 2500;
int Speed_BottomGantry_Z      = 3500;
int Speed_Sewing              = 1000;
int Speed_Bobbin              = 4000;
int Speed_Template            = 100;
int Speed_Cutter_Raise_Lower  = 1000;

// Acceleration Tuning
int Accel_TopGantry           = 3500;
int Accel_TopGantry_Z         = 3500;
int Accel_BottomGantry        = 3500;
int Accel_BottomGantry_Z      = 3500;
int Accel_Sewing              = 3500;
int Accel_Template            = 3500;
int Accel_Cutter_Raise_Lower  = 300;

// MultiStepper Grouping
MultiStepper stepperGrp;
MultiStepper sewingZ;

// Revolution per mm Tuning
int Step_TopLeftXY     = 40;
int Step_TopRightXY    = 40;
int Step_TopZ          = 796;
int Step_Needle        = 1;
int Step_Cutter        = 1;

int Step_TemplateLeft  = 1;
int Step_TemplateRight = 1;

int Step_BottomLeftXY  = 40;
int Step_BottomRightXY = 40;
int Step_BottomZ       = 796;
int Step_Bobbin        = 1;

// Calibration Setting/ Offset
int ESCount[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
int ESNoOfCount = 4;

// Data Setting
long positions[6];                          // Stepper Position {TopX, TopY, TopZ, BottomX, BottomY, BottomZ}
bool PrtES, CoordPos, SewingVert = false;   // Checker
String TaskAvailable = ""; 
float L_Val, R_Val = 0.0;                   // X and Y movement dissection, Left and Right Motor
int Task[6] = {0,0,0,0,0,0};                // Store Movement to mvoe information, {TopX, TopY, TopZ, BottomX, BottomY, BottomZ}
int Coordinates[4] = {-9999,-9999,-9999,-9999};   // Record coordinate {X, Y, TopZ, BottomZ}

// Debugging Only;
bool Debug = false;
bool PosDebug = true;

void setup() 
{
  Serial.begin(115200);
  Serial.println("Program written by Teo Wei Qing and Seow Sin Kiat.");
  Serial.println("Initialising Function...");

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
  BottomZ.setMaxSpeed(Speed_BottomGantry_Z);
  Bobbin.setMaxSpeed(Speed_Bobbin);

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
  BottomZ.setAcceleration(Accel_BottomGantry_Z);
  Bobbin.setAcceleration(Accel_Sewing);

  // Add Stepper To Group
  stepperGrp.addStepper(TopLeftXY);
  stepperGrp.addStepper(TopRightXY);
  stepperGrp.addStepper(TopZ);
//  stepperGrp.addStepper(Needle);
//  stepperGrp.addStepper(Cutter);

//  stepperGrp.addStepper(TemplateLeft);
//  stepperGrp.addStepper(TemplateRight);

  stepperGrp.addStepper(BottomLeftXY);
  stepperGrp.addStepper(BottomRightXY);
  stepperGrp.addStepper(BottomZ);
//  stepperGrp.addStepper(Bobbin);

  // Add Stepper To Group
  sewingZ.addStepper(Needle);
  sewingZ.addStepper(TopZ);

  // Add Stepper To Group
  
  // Add EndStop
  pinMode(ES_Top_X, INPUT_PULLUP);
  pinMode(ES_Top_Y, INPUT_PULLUP);
  pinMode(ES_Top_Z, INPUT_PULLUP);
  pinMode(ES_Bottom_X, INPUT_PULLUP);
  pinMode(ES_Bottom_Y, INPUT_PULLUP);
  pinMode(ES_Bottom_Z, INPUT_PULLUP);
  pinMode(ES_Cutter, INPUT_PULLUP);
  pinMode(ES_Template_Left, INPUT_PULLUP);
  pinMode(ES_Template_Right, INPUT_PULLUP);
  
  Serial.println("Initialisation Completed. Waiting for Command.");
}

void loop() 
{
  if (Debug)
  {
      Serial.println("In Sewing");
      Bobbin.setCurrentPosition(0);
      Bobbin.moveTo(200);
      Bobbin.runToPosition();
      Debug = false;
  }
  if (PrtES)
  {
    Serial.print(IsTopXClose());
    Serial.print(", ");
    Serial.print(IsTopYClose());
    Serial.print(", ");
    Serial.print(IsTopZClose());
    Serial.print(", ");
    Serial.print(IsBottomXClose());
    Serial.print(", ");
    Serial.print(IsBottomYClose());
    Serial.print(", ");
    Serial.print(IsBottomZClose());
    Serial.print(", ");
    Serial.print(IsCutterClose());
    Serial.print(", ");
    Serial.print(IsTemplateLeftClose());
    Serial.print(", ");
    Serial.println(IsTemplateRightClose());
  }
  if(TaskAvailable != "")
  {
    if (TaskAvailable == "Movement")
    {
      CoreXYMovement();    
      StepsBuilder(Step_TopLeftXY*L_Val,Step_TopRightXY*R_Val,Step_TopZ*Task[2],Step_BottomLeftXY*L_Val,Step_BottomRightXY*R_Val,0);
      stepperGrp.moveTo(positions);
      stepperGrp.runSpeedToPosition();
      displayCoordPos();
      for (int i = 0; i < 6; i++)
      {
        Task[i] = 0;
      }
  //    Serial.write(95478);
      TaskAvailable = "";
    }
    else if (TaskAvailable == "Sewing")
    {
//      if (Coordinates[2] != 0 || Coordinates[2] != -9999)
//      {
//        
//      }
      long TempPos[2] = {2, 0};
      for(int i = 0 ; i < 16; i++)
      {
        TopZ.setCurrentPosition(0);
        Needle.setCurrentPosition(0);
        long Pos1[2] = {50, long(Step_TopZ)};
        sewingZ.moveTo(Pos1);
        sewingZ.runSpeedToPosition(); 
      }
      Bobbin.setCurrentPosition(0);
      Bobbin.moveTo(-1600);
      Bobbin.runToPosition();
      for(int i = 0; i < 16; i++)
      {
        TopZ.setCurrentPosition(0);
        Needle.setCurrentPosition(0);
        long Temp = Step_TopZ*-1;
        long Pos2[2] = {50, long(Temp)};
        sewingZ.moveTo(Pos2);
        sewingZ.runSpeedToPosition();
      }
    }
  }
  if (Serial.available() > 0)
  {
    CodeReader(Serial.readStringUntil('\r\n'));
  }
}

//Read Serial Com
void CodeReader(String Val)
{
  String BufferCoord = "";
  char BufferVal = NULL;
  if (Val == "HOMEX")
  {
    homeX();
  }
  else if (Val == "HOMEY")
  {
    homeY();
  }
  else if (Val == "HOMEZ")
  {
    homeZ();
  }
  else if (Val == "HOMEALL")
  {
    homeY();
    homeX();
    homeZ();
  }
  else if (Val == "HOMECUTTER")
  {
    homeCutter();
  }
  else if (Val == "CHECKES")
  {
    PrtES = !PrtES;
  }
  else if (Val == "CUTTERON")
  {
    Cutter.moveTo(165);
    Cutter.runToPosition();
  }
  else if (Val == "CUTTEROFF")
  {
    Cutter.moveTo(0);
    Cutter.runToPosition();
  }
  else if (Val == "SEWINGON")
  {
    Serial.println("Sewing Mode On...");
    for(int i = 0; i < 23; i++)
    {
      StepsBuilder(positions[0],positions[1],long(Step_TopZ),positions[3],positions[4],long(Step_BottomZ*-1));
      stepperGrp.moveTo(positions);
      stepperGrp.runSpeedToPosition();
      TopZ.setCurrentPosition(0);
      BottomZ.setCurrentPosition(0);
      Coordinates[2] += 1;
      Coordinates[3] += 1;
    }
    for(int i = 0; i < 17; i++)
    {
      StepsBuilder(positions[0],positions[1],0,positions[3],positions[4],long(Step_BottomZ*-1));
      stepperGrp.moveTo(positions);
      stepperGrp.runSpeedToPosition();
      TopZ.setCurrentPosition(0);
      BottomZ.setCurrentPosition(0);
      Coordinates[3] += 1;
    }
    StepsBuilder(positions[0],positions[1],positions[2],positions[3]+10*Step_BottomLeftXY,positions[4]-10*Step_BottomLeftXY,positions[5]);
    stepperGrp.moveTo(positions);
    stepperGrp.runSpeedToPosition();
    displayCoordPos();
  }
  else if (Val == "SEWINGOFF")
  {
    Serial.println("Sewing Mode Off...");
    StepsBuilder(positions[0],positions[1],positions[2],positions[3]-10*Step_BottomLeftXY,positions[4]+10*Step_BottomLeftXY,positions[5]);
    stepperGrp.moveTo(positions);
    stepperGrp.runSpeedToPosition();
    homeZ();
    displayCoordPos();
  }
  else if (Val == "SEWINGACTIVATE")
  {
    Serial.println("Activating Sewing");
    TaskAvailable = "Sewing";
    displayCoordPos();
  }
  else if (Val == "SEWINGDEACTIVATE")
  {
    Serial.println("Deactivating Sewing");
    TaskAvailable = "";
    displayCoordPos();
  }
  else if (Val == "SEWINGVERTICLE")
  {
    SewingVert = true;
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
  else
  {
    if(PosDebug || Coordinates[0] != -9999 || Coordinates[1] != -9999 || Coordinates[2] != -9999 || Coordinates[3] != -9999)
    {
      for(int i = 0; i < Val.length(); i ++)
      {
        if (isSpace(Val[i]) || i == Val.length()-1)
        {
          if (i == Val.length()-1)
            BufferCoord += Val[i];
          TaskStorer(BufferVal, BufferCoord);
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
}

void TaskStorer(char BufferVal, String BufferCoord)
{
  if (BufferVal == 'X')
  {
    Task[0] = BufferCoord.toInt();
    Coordinates[0] += Task[0];
  }
  else if (BufferVal == 'Y')
  {
    Task[1] = BufferCoord.toInt();
    Coordinates[1] += Task[1];
  }
  else if (BufferVal == 'Z')
  {
    Task[2] = BufferCoord.toInt();
    Coordinates[2] += Task[2];
    TopZ.setCurrentPosition(0);
    BottomZ.setCurrentPosition(0);
  }
}

// Build Positions
void StepsBuilder(long Pos_TopLeftXY, long Pos_TopRightXY, long Pos_TopZ, 
                  long Pos_BottomLeftXY, long Pos_BottomRightXY, long Pos_BottomZ)
{
  positions[0] = Pos_TopLeftXY;
  positions[1] = Pos_TopRightXY;
  positions[2] = Pos_TopZ;
  positions[3] = Pos_BottomLeftXY;
  positions[4] = Pos_BottomRightXY;
  positions[5] = Pos_BottomZ;
}

void CoreXYMovement()
{
  if (Task[0] != 0)
  {
    L_Val += float(Task[0]);
    R_Val += float(Task[0]);
  }
  if (Task[1] != 0)
  {
    L_Val -= float(Task[1]);
    R_Val += float(Task[1]);
  }
}

void homeX()
{
  Serial.println("Homing X...");
  bool ValTop = IsTopXClose();
  bool ValBottom = IsBottomXClose();
  long TopPos[2] = {0,0};
  long BtmPos[2] = {0,0};
  resetXY();
  while (!ValTop || !ValBottom)
  {
    if(!ValTop)
    {
      TopPos[0] -= Step_TopLeftXY;
      TopPos[1] -= Step_TopRightXY;
    }
    if(!ValBottom)
    {
      BtmPos[0] -= Step_BottomLeftXY;
      BtmPos[1] -= Step_BottomRightXY;
    }
    StepsBuilder(TopPos[0],TopPos[1],positions[2],BtmPos[0],BtmPos[1],positions[5]);
    stepperGrp.moveTo(positions);
    stepperGrp.runSpeedToPosition();
    ValTop = IsTopXClose();
    ValBottom = IsBottomXClose();
  }
  resetXY();
  resetTask();
  resetPositions();
  Coordinates[0] = 0;
  Serial.println("X Homed.");
  displayCoordPos();
}

void homeY()
{
  Serial.println("Homing Y...");
  bool ValTop = IsTopYClose();
  bool ValBottom = IsBottomYClose();
  long TopPos[2] = {0,0};
  long BtmPos[2] = {0,0};
  resetXY();
  while (!ValTop || !ValBottom)
  {
    if(!ValTop)
    {
      TopPos[0] += Step_TopLeftXY;
      TopPos[1] -= Step_TopRightXY;
    }
    if(!ValBottom)
    {
      BtmPos[0] += Step_BottomLeftXY;
      BtmPos[1] -= Step_BottomRightXY;
    }
    StepsBuilder(TopPos[0],TopPos[1],positions[2],BtmPos[0],BtmPos[1],positions[5]);
    stepperGrp.moveTo(positions);
    stepperGrp.runSpeedToPosition();
    ValTop = IsTopYClose();
    ValBottom = IsBottomYClose();
  }
  resetXY();
  resetTask();
  resetPositions();
  Coordinates[1] = 0;
  Serial.println("Y Homed.");
  displayCoordPos();
}

void homeZ()
{
  Serial.println("Homing Z...");
  positions[2] = 0;
  positions[5] = 0;
  bool ValTop = IsTopZClose();
  bool ValBtm = IsBottomZClose();
  long ZPos[2] = {0,0};
  while (!ValTop || !ValBtm)
  {
    if (!ValTop)
    {
      ZPos[0] -= Step_TopZ;
    }
    if (!ValBtm)
    {
      ZPos[1] += Step_BottomZ;
    }
    StepsBuilder(positions[0],positions[1],ZPos[0],positions[3],positions[4],ZPos[1]);
    stepperGrp.moveTo(positions);
    stepperGrp.runSpeedToPosition();
    ValTop = IsTopZClose();
    ValBtm = IsBottomZClose();    
  }
  TopZ.setCurrentPosition(0);
  BottomZ.setCurrentPosition(0);
  resetTask();
  positions[2] = 0;
  positions[5] = 0;
  Coordinates[2] = 0;
  Coordinates[3] = 0;
  Serial.println("Z Homed.");
  displayCoordPos();
}

void homeCutter()
{
  Serial.println("Homing Cutter...");
  bool ValCut = IsCutterClose();
  long pos = 0;
  while(!ValCut)
  {
    pos -= Step_Cutter;
    Cutter.moveTo(pos);
    Cutter.runToPosition();
    ValCut = IsCutterClose();
  }
  Cutter.setCurrentPosition(0);
  Serial.println("Cutter Homed.");
}

void resetXY()
{
  L_Val = 0;
  R_Val = 0;
  TopLeftXY.setCurrentPosition(0);
  TopRightXY.setCurrentPosition(0);
  BottomLeftXY.setCurrentPosition(0);
  BottomRightXY.setCurrentPosition(0);
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

bool IsTopZClose()
{
  for (int i = 0; i < ESNoOfCount; i++)
  {
    if (digitalRead(ES_Top_Z) == LOW)
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

bool IsBottomZClose()
{
  for (int i = 0; i < ESNoOfCount; i++)
  {
    if (digitalRead(ES_Bottom_Z) == LOW)
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

bool IsCutterClose()
{
  for (int i = 0; i < ESNoOfCount; i++)
  {
    if (digitalRead(ES_Cutter) == LOW)
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

void resetTask()
{
  for (int i = 0; i < 6; i++)
  {
    Task[i] = 0;
  }
}

void resetPositions()
{
  for (int i = 0; i < 6; i++)
  {
    positions[i] = 0;
  }
}

void displayCoordPos()
{
  Serial.println("----------------------");
  if(CoordPos)
  {
    Serial.print("Positions - ");
    for (int i = 0; i < 5; i ++)
    {
      Serial.print(positions[i]);
      Serial.print(", ");
    }
    Serial.println(positions[5]);
  }
  else
  {
    Serial.print("Coordinates - ");
    Serial.print("X:");
    Serial.print(Coordinates[0]);
    Serial.print(", Y:");
    Serial.print(Coordinates[1]);
    Serial.print(", TopZ:");
    Serial.print(Coordinates[2]);
    Serial.print(", BottomZ:");
    Serial.println(Coordinates[3]);
  }
  Serial.println("----------------------");
}
