#define front_left 3
#define back_left 10
#define back_right 11
#define front_right 9

#define X_CTRL A1
#define Y_CTRL A0
#define Z_CTRL A2

#define JOY_CONNECTED A5

#define reverse 2

//Fuck you, Shane
//Fuck you, Shane
//Fuck you, Shane
//Fuck you, Shane
//Fuck you, Shane//Fuck you, Shane//Fuck you, Shane//Fuck you, Shane//Fuck you, Shane//Fuck you, Shane//Fuck you, Shane//Fuck you, Shane//Fuck you, Shane//Fuck you, Shane//Fuck you, Shane//Fuck you, Shane//Fuck you, Shane//Fuck you, Shane//Fuck you, Shane//Fuck you, Shane//Fuck you, Shane//Fuck you, Shane//Fuck you, Shane//Fuck you, Shane//Fuck you, Shane//Fuck you, Shane//Fuck you, Shane//Fuck you, Shane//Fuck you, Shane//Fuck you, Shane//Fuck you, Shane//Fuck you, Shane//Fuck you, Shane//Fuck you, Shane//Fuck you, Shane//Fuck you, Shane//Fuck you, Shane//Fuck you, Shane//Fuck you, Shane//Fuck you, Shane//Fuck you, Shane//Fuck you, Shane//Fuck you, Shane//Fuck you, Shane//Fuck you, Shane//Fuck you, Shane//Fuck you, Shane//Fuck you, Shane//Fuck you, Shane//Fuck you, Shane//Fuck you, Shane//Fuck you, Shane//Fuck you, Shane//Fuck you, Shane//Fuck you, Shane//Fuck you, Shane//Fuck you, Shane


int   j = 0;
int   y_input     = 0;

void setup() {

  Serial.begin(9600);

  pinMode(front_left, OUTPUT);
  pinMode(back_left, OUTPUT);
  pinMode(front_right, OUTPUT);
  pinMode(back_right, OUTPUT);

  pinMode(X_CTRL,INPUT);
  pinMode(Y_CTRL,INPUT);
  pinMode(Z_CTRL,INPUT);    

  pinMode(JOY_CONNECTED, INPUT);
  digitalWrite(JOY_CONNECTED, HIGH); //enable pullup

  analogWrite(front_left, 0 );
  analogWrite(front_right, 0 );
  analogWrite(back_left, 0 );
  analogWrite(back_right, 0 );

  delay(3000);

  if ( digitalRead(JOY_CONNECTED) == HIGH ) //if joystick is connected, this pin will be pulled low.
  {
   Serial.println("No Joystick. Halting execution.");
   while(1); //if it is high, halt execution until reset.
  }
 
}


void loop() {

      int   carSpeed    = 0;
      int   carRotation = 0;
      float turnWeight  = 0.85;
      int  carWheels[4] = {0,0,0,0};
      

      int fl = 0;
      int bl = 0;
      int br = 0;
      int fr = 0;

      while(1){
       carSpeed    = Y_translate( analogRead(Y_CTRL) ); //Returns -100 to 100 for speed percentage. Reverse to Forward, respectfully.

       if (carSpeed < 5 && carSpeed > -5)
        carSpeed = 0;

       
       carRotation = Z_translate( analogRead(Z_CTRL) ); //Returns -100 to 100 for Car Rotation. Left, to Right, respectfully.
       carRotation *= turnWeight;// Adjusts roation by pre-definded weight.
       
       tankControl(carSpeed, carRotation,carWheels);
    //   tankControl(80, 50,carWheels);

       fl = carWheels[0];
       bl = carWheels[1];
       br = carWheels[2];
       fr = carWheels[3];


     Serial.print("X: ");
     Serial.print( analogRead(X_CTRL) );
     Serial.print(" Y: ");
     Serial.print( analogRead(Y_CTRL) );
     Serial.print(" Z: ");
     Serial.print( analogRead(Z_CTRL) );
     Serial.print(" - ");
     Serial.print(fl);
     Serial.print(" ");
     Serial.print(bl);
     Serial.print(" ");
     Serial.print(br);
     Serial.print(" ");
     Serial.println(fr);
     


       
/*
       Serial.print(fl);
       Serial.print(" ");
       Serial.print(bl);
       Serial.print(" ");
       Serial.print(br);
       Serial.print(" ");       
       Serial.println(fr);
*/
       MoveMotors(fl,bl,br,fr);
      }

  
}

void MoveMotors (int fl_percent, int bl_percent, 
                 int br_percent, int fr_percent){
 /*
  PWM Findings:
   184 - 119: Backwards. From slow to fast.
   190 - 255: Forward:   From slow to fast
   185 - 189: neutral  
  */

 int i = 0;
 int wheelValue[4] =   {0,0,0,0};
 int wheelPercent[4] = {0,0,0,0};

 wheelPercent[0] = fl_percent;
 wheelPercent[1] = bl_percent;
 wheelPercent[2] = br_percent;
 wheelPercent[3] = fr_percent; 

 //if any of the wheels have more than 100%
 //speed in either direction, fix it.
 for (i = 0;i<4;i++){
  if (wheelPercent[i]>100)
   wheelPercent[i] = 100;
  else if(wheelPercent[i] < -100)
   wheelPercent[i] = -100;
 
  //if a wheel have a percentage of 0, set to neutral
  if (wheelPercent[i] == 0)
   wheelValue[i] = 186;
  //if a wheel has a positive percentage
  else if (wheelPercent[i] > 0){
    wheelValue[i] = 190; //base PWM of 190
    wheelValue[i] += (int)(65.0 * (wheelPercent[i] / 100.0) ); //weight     
  }
  //if a wheel has a negative percentage
  else if (wheelPercent[i] < 0){
    wheelValue[i] = 184; // base PWM of 184
    wheelValue[i] -= (int)(65.0 * (wheelPercent[i] / -100.0) ); //weight
  }

 }
/*
  Serial.print(wheelValue[0]);
  Serial.print("-");
  Serial.print(wheelValue[1]);
  Serial.print("-");
  Serial.print(wheelValue[2]);
  Serial.print("-");  
  Serial.println(wheelValue[3]);
*/  
  analogWrite(front_left, wheelValue[0] );
  analogWrite(back_left, wheelValue[1] );
  analogWrite(back_right, wheelValue[2] );
  analogWrite(front_right, wheelValue[3] );


 

  
}



int X_translate(int x_value){
//147 MAX RIGHT

//492 - 560 CENTER

//901 MIN LEFT


if (x_value >= 601){
 x_value -= 601;
 x_value /= 3;  
}
else if (x_value <= 447){
 x_value -= 147; 
 x_value -= 300;
 x_value /= 3;
}
else
 x_value = 0;

 x_value *= -1;

return x_value;
  
}


int Y_translate(int y_value){
//842 MAX

//470 - 548 CENTER

//129 MIN


if (y_value >= 542){
 y_value -= 542;
 y_value /= 3;  
}
else if (y_value < 429){
 y_value -= 129; 
 y_value -= 300;
 y_value /= 3;
}
else
 y_value = 0;

return y_value;
  
}

int Z_translate(int z_value){
//820 MAX - LEFT

//490 - CENTER

//146 MIN - RIGHT


if (z_value >= 520){
 z_value -= 520;
 z_value /= 3;  
}
else if (z_value < 446){
 z_value -= 146; 
 z_value -= 300;
 z_value /= 3;
}
else
 z_value = 0;

 z_value *= -1;

return z_value;
  
}


int *tankControl(int carSpeed, int carRotation, int *carWheels){

//variables for left, and right wheels.
int leftWheels  = 0;
int rightWheels = 0;

//int carWheels[4] = {0,0,0,0};

  //sanity check. If more than 100%, or less than -100%, fix.
  if (carSpeed < -100)
   carSpeed = -100;
  else if (carSpeed > 100)
   carSpeed = 100; 

  if (carRotation < -100)
   carRotation = -100;
  else if (carRotation > 100)
   carRotation = 100;

  //car is not moving 
  if (carSpeed == 0){
   leftWheels = 0;
   rightWheels = 0;
   }

  //car is not rotating
  else if (carRotation == 0){
   //all wheels move forward at speed of vehicle  
   leftWheels = carSpeed;
   rightWheels = carSpeed;
   }

  //Moving Forward, or backwards
  else if (carSpeed > 0 || carSpeed < 0){

   //vehicle is turning right
   if(carRotation > 0){
    leftWheels = carSpeed; //left wheels go full speed
    rightWheels = carSpeed * ( (100 - carRotation) / 100.0); //rightWheel goes carRotation of a percent slower than left 
    }

   //vehicle is turning left
   else if (carRotation < 0){
    rightWheels = carSpeed; //right wheels go full speed
    leftWheels = carSpeed * ( (100 - (carRotation * -1)  ) / 100.0); //leftWheels goes carRotation of a percent slower than right 
    }
  }
  
/*
  //Moving Backwards 
  else if (carSpeed < 0){

   //carRotation is right; vehicle is moving left in reverse
   if(carRotation > 0){
    rightWheels = carSpeed; //right wheels go full speed
    leftWheels  = carSpeed * ( (100 - carRotation) / 100.0); //leftWheel goes carRotation of a percent slower than right
    }

    //vehicle is turning left; vehicle is moving right in reverse
    else if (carRotation < 0){
     leftWheels = carSpeed; //left wheels go full speed
     rightWheels = carSpeed * ( (100 - (carRotation * -1)  ) / 100.0); //rightWheels goes carRotation of a percent slower than left 
     }
   }*/

   carWheels[0] = leftWheels;
   carWheels[1] = leftWheels;
   carWheels[2] = rightWheels;
   carWheels[3] = rightWheels;   

   return carWheels;
  
}





