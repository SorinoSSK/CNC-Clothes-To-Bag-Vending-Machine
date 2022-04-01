#include <AccelStepper.h>
#include <MultiStepper.h>

int inByte = '0';

const int LstepperTotal = 2;  //Template Lock Steppers
const int XYstepperTotal = 4; //Core-XY Steppers
const int ZstepperTotal = 2;  //Z Height Steppers
const int SstepperTotal = 2;  //Sewing Mechanism Steppers
const int CstepperTotal = 1;  //Cutting Mechanism Steppers
const int stepperTotal = LstepperTotal + XYstepperTotal + ZstepperTotal + SstepperTotal;

long Lpositions[LstepperTotal] = {0,0};
long XYpositions[XYstepperTotal] = {0,0,0,0};
long Zpositions[ZstepperTotal] = {0,0};
long Spositions[SstepperTotal] = {0,0};
long Cpositions[CstepperTotal] = {0};

//Instantiate AccelStepper objects (StepPin, DirPin)
AccelStepper stepper0 (AccelStepper::DRIVER,48,49);   //Template Lock Left Motor
AccelStepper stepper1 (AccelStepper::DRIVER,50,51);   //Tempalte Lock Right Motor
AccelStepper stepper2 (AccelStepper::DRIVER,22,23);   //Top Gantry Left Motor
AccelStepper stepper3 (AccelStepper::DRIVER,24,25);   //Top Gantry Right Motor
AccelStepper stepper4 (AccelStepper::DRIVER,36,37);   //Bottom Gantry Left Motor    
AccelStepper stepper5 (AccelStepper::DRIVER,38,39);   //Bottom Gantry Right Mb otor
AccelStepper stepper6 (AccelStepper::DRIVER,26,27);   //Top Gantry Z Motor
AccelStepper stepper7 (AccelStepper::DRIVER,40,41);   //Bottom Gantry Z Motor
AccelStepper stepper8 (AccelStepper::DRIVER,28,29);   //Top Gantry Needle Motor
AccelStepper stepper9 (AccelStepper::DRIVER,42,43);   //Bottom Gantry Bobbin Motor
AccelStepper Cstepper (AccelStepper::DRIVER,30,31);   //Top Gantry Cutter Motor

//Stick pointers to the AccelStepper objects into an Array
AccelStepper* stepper[stepperTotal] = { &stepper0,
                                        &stepper1,
                                        &stepper2,
                                        &stepper3,
                                        &stepper4,
                                        &stepper5,
                                        &stepper6,
                                        &stepper7,
                                        &stepper8,
                                        &stepper9, };

//Instantiate MultiStepper objects
MultiStepper Lsteppers;
MultiStepper XYsteppers;
MultiStepper Zsteppers;
MultiStepper Ssteppers;


void setup() {
  Serial.begin(9600);
  
  for(int stepperNum = 0; stepperNum < stepperTotal; stepperNum++){
    stepper[stepperNum]->setMaxSpeed(1000);
    stepper[stepperNum]->setAcceleration(200);
    stepper[stepperNum]->setSpeed(50);
    if(stepperNum < LstepperTotal){
      Lsteppers.addStepper(stepper[stepperNum]);}
    else if(stepperNum < LstepperTotal + XYstepperTotal){
      XYsteppers.addStepper(stepper[stepperNum]);}
    else if(stepperNum < LstepperTotal + XYstepperTotal + ZstepperTotal){
      Zsteppers.addStepper(stepper[stepperNum]);}
    else if(stepperNum < LstepperTotal + XYstepperTotal + ZstepperTotal + SstepperTotal){
      Ssteppers.addStepper(stepper[stepperNum]);}
  }
  Cstepper.setMaxSpeed(1000);
  Cstepper.setSpeed(200);
}


void loop(){
  if(Serial.available() > 0){
    inByte = (Serial.read());
    print(inByte);
  }
  Cstepper.runSpeed();
  //MoveXY(1600, 1600);
}


/*void MoveXY(int x, int y){
  XYpositions[0] += x;
  XYpositions[1] += x;
  XYpositions[2] += x;
  XYpositions[3] += x;
  XYsteppers.moveTo(XYpositions);
  XYsteppers.runSpeedToPosition(); // Blocks until all are in position
  delay(100);

  XYpositions[0] += y;
  XYpositions[1] -= y;
  XYpositions[2] += y;
  XYpositions[3] -= y;
  XYsteppers.moveTo(XYpositions);
  XYsteppers.runSpeedToPosition();
  delay(100); 
}

void MoveZ(int z){
  Zpositions[0] += z;
  Zpositions[1] -= z;
  Zsteppers.moveTo(Zpositions);
}*/
