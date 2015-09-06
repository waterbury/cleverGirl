#define front_left 3
#define back_left 10
#define back_right 11
#define front_right 9

#define X_CTRL A0
#define Y_CTRL A2
#define Z_CTRL A1

#define reverse 2


int j = 0;
int y_input = 0;

void setup() {

  Serial.begin(9600);

  pinMode(front_left, OUTPUT);
  pinMode(back_left, OUTPUT);
  pinMode(front_right, OUTPUT);
  pinMode(back_right, OUTPUT);

  pinMode(X_CTRL,INPUT);
  pinMode(Y_CTRL,INPUT);
  pinMode(Z_CTRL,INPUT);    
  // put your setup code here, to run once:

  analogWrite(front_left, 0 );
  analogWrite(front_right, 0 );
  analogWrite(back_left, 0 );
  analogWrite(back_right, 0 );

  delay(1000);
 // Serial.println("Test");
}


void loop() {


  for(int i =-100;i<=100;i++){
    j = 0;
 //   Serial.print("#");
 //   Serial.print(j);
    y_input = analogRead(Y_CTRL);
 //   Serial.print(" Y:");
 //   Serial.print(y_input);
 //   Serial.print(" ");
    j = Y_translate( y_input);
    MoveMotors(j,j,j,j);
    
   // delay(800);
  }

  
  //110 - 183 for reverse. Higher is slower 
  //190 - 255 for forward. Higher is faster

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

