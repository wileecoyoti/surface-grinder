/*
2/3
9-13



a0-7


up/down -> up=10, down=11
go/stop -> 12


press "go" first time = traxel y

second stops that
third = null
fourth = run program

order:
  all axis static
  press 'go' button, Y starts to travel slowly out.
  Press 'go' again, Y stops, this sets y-max point and it re-sets to beginning point.
  press 'go' again, program runs
     run full X length back and forth until y-max achieved, then return to home, step Z and repeat

  alt: if all axis static, turn key thing to adjust Z

*/

#include <AccelStepper.h>

// temp, these should be translated to inches or something later //
  int travel = 5000; //X //8700 = full travel
  float traverseTotal = 25000; // Y
  float stepover = 500; // Y
  int zStepover = 1;
  int zCurrent=0; //assume z pos at init is starting point
  float yCurrent=0; 
  
  int goStatus = 0;
  int stopGrind=0;

int goButton = 12;
int zUpButton = 10;
int zDownButton = 11;



AccelStepper Xaxis(1, 4, 5); // pin 4 = step, pin 5 = direction
AccelStepper Yaxis(1, 6, 7); // mode, step, dir
AccelStepper Zaxis(1, 8, 9); // mode, step, dir


void setup() {
pinMode(8, OUTPUT);
pinMode(9, OUTPUT);


pinMode(goButton, INPUT);
pinMode(zUpButton, INPUT);
pinMode(zDownButton, INPUT);
  
  Xaxis.setMaxSpeed(2000); //was 3000
  Xaxis.setAcceleration(5000);
  Xaxis.setMinPulseWidth(1);

  Yaxis.setMaxSpeed(5000);
  Yaxis.setAcceleration(8000);
  Yaxis.setMinPulseWidth(1);

  Zaxis.setMaxSpeed(400);
  Zaxis.setSpeed(2);
  Zaxis.setAcceleration(4000);
  Zaxis.setMinPulseWidth(1);

  int loops = 1;
}

void loop() { 

//listen for go button
 if(digitalRead(goButton)==HIGH) {
  if(goStatus == 0) {
    goStatus = 1;
    setY();
  }
  if(goStatus == 1) {
    goStatus = 2;
    returnToHome();
  }
  if(goStatus == 2) {
    goStatus = 0;
    grindStuff();
  }
 }


//listen for z knob up
 if(digitalRead(zUpButton)==HIGH) {
  zUp();
 }
//listen for z knob down
 if(digitalRead(zDownButton)==HIGH) {
  zDown();
 }
}


void zUp() {
     Zaxis.moveTo(zCurrent+100);
        Zaxis.runToPosition();
        zCurrent=zCurrent+100;
}
void zDown() {
     Zaxis.moveTo(zCurrent-100);
        Zaxis.runToPosition();
        zCurrent=zCurrent-100;
}

void returnToHome() {
     Yaxis.moveTo(0);
        Yaxis.runToPosition();

}

void setY () {
  delay(500);
  while (digitalRead(goButton)==LOW) {
   Yaxis.moveTo(yCurrent+stepover);
        Yaxis.runToPosition();
        yCurrent=yCurrent+stepover;
  }
  traverseTotal = yCurrent;

}
void grindStuff () {
  delay(500);
stopGrind=digitalRead(goButton);
  while (stopGrind==LOW) {
  
       Xaxis.moveTo(travel);
            Xaxis.runToPosition();
      
       for(float i = 0; i < traverseTotal;) {
         
         Yaxis.moveTo(stepover + i);
              Yaxis.runToPosition();
      
         Xaxis.moveTo(0);
              Xaxis.runToPosition();
         
         i = stepover + i;
    
         Yaxis.moveTo((2 * stepover) + (i-stepover));
              Yaxis.runToPosition();
    
         Xaxis.moveTo(travel);
              Xaxis.runToPosition();
    
         i = stepover + i;
    
       }
      
       Yaxis.moveTo(0);
            Yaxis.runToPosition();
    
       /*Zaxis.moveTo(zCurrent-100);
          Zaxis.runToPosition();
          delay(200);
       Zaxis.moveTo(zCurrent);//-zStepover);
            Zaxis.runToPosition();
            //zCurrent=zCurrent-zStepover;
            */

           /* for (int i = 0; i < zStepover;) {
              delay(5);
              digitalWrite(8, HIGH);
              delay(5);
              digitalWrite(8, LOW);
              i++;
            }*/
            for (int j = 0; j < zStepover;) {
              delay(5);
              digitalWrite(8, HIGH);
              delay(5);
              digitalWrite(8, LOW);
              j++;
            }
            
      //delay(4000);
            
            
       grindStuff();
  }
  Xaxis.moveTo(0);
    Xaxis.runToPosition();

  Yaxis.moveTo(0);
    Yaxis.runToPosition();

}
