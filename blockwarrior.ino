int ch1;
int ch2;
int ch3;
int ch4;
int movec,moveg,moves;
int turn;

int pwm_ml = 10; //PWM pins for motors;
int pwm_mr = 11; // only 3,5,6,9,10,11 can be used for PWM

int pwm_mg = 5; // PWM pins for gripper and slider
int pwm_ms = 6;

int dir_ml_1 = A2;// input pins for motor direction
int dir_ml_2 = A3;
int dir_mr_1 = A0; // Analog pins can be used for digital output pins
int dir_mr_2 = A1;

int dir_mg_1 = 12; // input pins for gripper and slidder motor direction
int dir_mg_2 = 13;
int dir_ms_1 = 8;
int dir_ms_2 = 9;
void setup()
{
  pinMode(2, INPUT);//setting pins as input for rc
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(7, INPUT);
  
  Serial.begin(9600); //setting baud rate
  
  pinMode(pwm_ml, OUTPUT);
  pinMode(pwm_mr, OUTPUT);
  pinMode(dir_ml_1, OUTPUT);
  pinMode(dir_ml_2, OUTPUT);
  pinMode(dir_mr_1, OUTPUT);
  pinMode(dir_mr_2, OUTPUT);
  
  pinMode(pwm_mg, OUTPUT);
  pinMode(pwm_ms, OUTPUT);
  pinMode(dir_mg_1, OUTPUT);
  pinMode(dir_mg_2, OUTPUT);
  pinMode(dir_ms_1, OUTPUT);
  pinMode(dir_ms_2, OUTPUT);
  
  analogWrite(pwm_ml, 0);
  analogWrite(pwm_mr, 0);
  analogWrite(pwm_mg, 0);
  analogWrite(pwm_ms, 0);
}
 void loop()
 {
   ch1 = pulseIn(2, HIGH, 25000); //reading pulse width of each channel
   ch2 = pulseIn(3, HIGH, 25000); // pulseIn(input pin,type of pulse expected, time for which we can wait for pulse (time-out number))
   ch3 = pulseIn(4, HIGH, 25000); //pulseIn( , , ) returns the length of pulse in microseconds
   ch4 = pulseIn(7, HIGH, 25000);
   
  /* Serial.print("Aileron :");
   Serial.println(ch1);
   Serial.println(map(ch1, 1473,2473,-500,500));
   
   Serial.print("Elevator :");
   Serial.println(ch2);
   Serial.println(map(ch2, 1316,2316,-500,500));
   
   Serial.print("Throttle :");
   Serial.println(ch3);
   Serial.println(map(ch3, 1380,2380,-500,500));
   
   Serial.print("Rudder :");
   Serial.println(ch4);
   Serial.println(map(ch4, 1433,2433,-500,500));
   
   Serial.println();
   
   delay(100);*/
   movec = map(ch2, 1316,2316,-500,500);
   if(abs(movec)>700)
   movec = 0;
   Serial.print("movec:");
   Serial.println(movec);
   movec = constrain(movec, -255,255); //analog pins only allows value of 0 to 255
   if(abs(movec)>20) // for forward as well as turning motion
   {
   
    if(movec>0)
    {
      Serial.println("forward");
      digitalWrite(dir_ml_1, HIGH); // for forward motion left motor in ccw and right motor in cw
      digitalWrite(dir_ml_2, LOW); // check motor directions once
      digitalWrite(dir_mr_1, LOW);
      digitalWrite(dir_mr_2, HIGH);
    }
    if(movec<0)
    {
      Serial.println("backward");
      digitalWrite(dir_ml_1, LOW);
      digitalWrite(dir_ml_2, HIGH);
      digitalWrite(dir_mr_1, HIGH);
      digitalWrite(dir_mr_2, LOW);
      movec = abs(movec);
    }
    turn = map(ch1, 1473,2473,-500,500);
    if(abs(turn)>700)
    turn = 0;
    Serial.println("turn");
    Serial.println(turn);
    turn = constrain(turn, -255,255);
    //Serial.println("turn");
    //Serial.println(turn);
    if(turn>10)
    {
      Serial.println("turning right");
      Serial.println("movec-turn");
      Serial.println(movec-turn);
      analogWrite(pwm_mr, abs(movec-turn)); //turn right decrease right motor's speed 
      analogWrite(pwm_ml, movec); 
    }
    if(turn<-50)
    {
      Serial.println("turning left");
      turn = abs(turn);
      Serial.println("movec-turn");
      Serial.println(movec-turn);
      turn = abs(turn);
      analogWrite(pwm_ml, abs(movec-turn)); //turn left decrease left motor's speed;
      analogWrite(pwm_mr, movec);
    }
    if(abs(turn)<10)
    {
      Serial.println("moving straight");
    analogWrite(pwm_ml, movec);
    analogWrite(pwm_mr, movec);
    }
   }
   else //for spinning motion
   {
     movec = 0;
     turn = map(ch1, 1433,2433,-500,500);
     if(abs(turn)>550)
     turn = 0;
     Serial.println("turn");
     Serial.println(turn);
     turn = constrain(turn, -255,255);
     if(turn>10)
     {
       Serial.println("spinning right");
       digitalWrite(dir_ml_1, HIGH);
       digitalWrite(dir_ml_2, LOW);
       digitalWrite(dir_mr_1, HIGH);
       digitalWrite(dir_mr_2, LOW);
       analogWrite(pwm_ml, turn);
       analogWrite(pwm_mr, turn);
     }
     if(turn<-10)
     {
       Serial.println("spinning left");
       turn = abs(turn);
       digitalWrite(dir_ml_1, LOW);
       digitalWrite(dir_ml_2, HIGH);
       digitalWrite(dir_mr_1, LOW);
       digitalWrite(dir_mr_2, HIGH);
       analogWrite(pwm_ml, turn);
       analogWrite(pwm_mr, turn);
     }
     if(abs(turn)<10)
     {
       turn = 0;
       Serial.println("stopped");
       Serial.println("turn");
       Serial.println(turn);
       digitalWrite(dir_ml_1, LOW);
       digitalWrite(dir_ml_2, LOW);
       digitalWrite(dir_mr_1, LOW);
       digitalWrite(dir_mr_2, LOW);
     }
   }
    moves = map(ch3, 1380,2380,-500,500);
    if(abs(moves)>1000)
    moves = 0;
   //Serial.println("moves:");
   //Serial.println(moves);
   moves = constrain(moves, -255,255);
   //Serial.println(moves);
   if(moves>50)
   {
     //Serial.println("up");
     digitalWrite(dir_ms_1, HIGH); // if throttle is above central psition move slider up
     digitalWrite(dir_ms_2, LOW);
     analogWrite(pwm_ms, moves);
   }
   if(moves<-50)
   {
     //Serial.println("down");
     moves = abs(moves); // if throttle is below central position move slider down
     digitalWrite(dir_ms_1, LOW);
     digitalWrite(dir_ms_2, HIGH);
     analogWrite(pwm_ms, moves);
   }
   if(abs(moves)<=50)
   {
     //Serial.println("stop");
     moves = 0; // if throttle is at central position stop the slider
     digitalWrite(dir_ms_1, LOW); // allowed range is +-10
     digitalWrite(dir_ms_2, LOW);
     analogWrite(pwm_ms, 0);
   }
   moveg = map(ch4, 1433,2433,-500,500);
   if(abs(moveg)>1000)
   moveg = 0;
   //Serial.println("moveg:");
   //Serial.println(moveg);
   moveg = constrain(moveg, -255,255);
   //Serial.println(moveg);
   if(moveg>50)
   {
     //Serial.println("grip");
     digitalWrite(dir_mg_1, HIGH);
     digitalWrite(dir_mg_2, LOW);
     analogWrite(pwm_mg, moveg);
   }
   if(moveg<-50)
   {
     //Serial.println("ungrip");
     digitalWrite(dir_mg_1, LOW);
     digitalWrite(dir_mg_2, HIGH);
     analogWrite(pwm_mg, moveg);
   }
   if(abs(moveg)<=50)
   {
    // Serial.println("stay");
     moveg = 0;
     digitalWrite(dir_mg_1, LOW);
     digitalWrite(dir_mg_2, LOW);
     analogWrite(pwm_mg, 0);
   } 

 /*Serial.print("movec:");
 Serial.println(movec);
 
 Serial.print("turn:");
 Serial.println(turn);
 
 Serial.println("movec-turn:");
 Serial.println(movec-turn);
 
 Serial.println("moves:");
 Serial.println(moves);
 Serial.println("moveg:");
 Serial.println(moveg);
 
 Serial.println();
 Serial.println();
 Serial.println();*/
   //delay(1000);
 }
