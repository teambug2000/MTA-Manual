// Declare L298N Dual H-Bridge Motor Controller directly since there is not a library to load.
#include <PS2X_lib.h>  //for v1.6

PS2X ps2x; // create PS2 Controller Class

//right now, the library does NOT support hot pluggable controllers, meaning 
//you must always either restart your Arduino after you conect the controller, 
//or call config_gamepad(pins) again after connecting the controller.
int error = 0; 
byte type = 0;
byte vibrate = 0;
 
// Motor 1
int dir1PinA = 26;
int dir2PinA = 27;
int speedPinA = 8; // Needs to be a PWM pin to be able to control motor speed
 
// Motor 2
int dir1PinB = 28;
int dir2PinB = 29;
int speedPinB = 9; // Needs to be a PWM pin to be able to control motor speed
int ballMotorR1 = 10;
int ballMotorL1 = 11;
int ballMotorR2 = 12;
int ballMotorL2 = 13;
void setup() {  // Setup runs once per reset
// initialize serial communication @ 9600 baud:
 
//Define L298N Dual H-Bridge Motor Controller Pins
 
pinMode(dir1PinA,OUTPUT);
pinMode(dir2PinA,OUTPUT);
pinMode(speedPinA,OUTPUT);
pinMode(dir1PinB,OUTPUT);
pinMode(dir2PinB,OUTPUT);
pinMode(speedPinB,OUTPUT);
pinMode(ballMotorR1, OUTPUT);
pinMode(ballMotorR2, OUTPUT);
pinMode(ballMotorL1, OUTPUT);
pinMode(ballMotorL2, OUTPUT);

Serial.begin(57600);

 //CHANGES for v1.6 HERE!!! **************PAY ATTENTION*************
  
 error = ps2x.config_gamepad(25,23,24,22, true, true);   //setup pins and settings:  GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error
 
 if(error == 0){
   Serial.println("Found Controller, configured successful");
   Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
  Serial.println("holding L1 or R1 will print out the analog stick values.");
  Serial.println("Go to www.billporter.info for updates and to report bugs.");
 }
   
  else if(error == 1)
   Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");
   
  else if(error == 2)
   Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");
   
  else if(error == 3)
   Serial.println("Controller refusing to enter Pressures mode, may not support it. ");
   
   //Serial.print(ps2x.Analog(1), HEX);
   
   type = ps2x.readType(); 
     switch(type) {
       case 0:
        Serial.println("Unknown Controller type");
       break;
       case 1:
        Serial.println("DualShock Controller Found");
       break;
       case 2:
         Serial.println("GuitarHero Controller Found");
       break;
     }
  
}
 
void loop() {
if(error == 1) //skip loop if no controller found
  return; 
  
 if(type == 2){ //Guitar Hero Controller
   
   ps2x.read_gamepad();          //read controller 
   
   if(ps2x.ButtonPressed(GREEN_FRET))
     Serial.println("Green Fret Pressed");
   if(ps2x.ButtonPressed(RED_FRET))
     Serial.println("Red Fret Pressed");
   if(ps2x.ButtonPressed(YELLOW_FRET))
     Serial.println("Yellow Fret Pressed");
   if(ps2x.ButtonPressed(BLUE_FRET))
     Serial.println("Blue Fret Pressed");
   if(ps2x.ButtonPressed(ORANGE_FRET))
     Serial.println("Orange Fret Pressed");
     

    if(ps2x.ButtonPressed(STAR_POWER))
     Serial.println("Star Power Command");
    
    if(ps2x.Button(UP_STRUM))          //will be TRUE as long as button is pressed
     Serial.println("Up Strum");
    if(ps2x.Button(DOWN_STRUM))
     Serial.println("DOWN Strum");
  
 
    if(ps2x.Button(PSB_START))                   //will be TRUE as long as button is pressed
         Serial.println("Start is being held");
    if(ps2x.Button(PSB_SELECT))
         Serial.println("Select is being held");

    
    if(ps2x.Button(ORANGE_FRET)) // print stick value IF TRUE
    {
        Serial.print("Wammy Bar Position:");
        Serial.println(ps2x.Analog(WHAMMY_BAR), DEC); 
    } 
 }

 else { //DualShock Controller
  
    ps2x.read_gamepad(false, vibrate);          //read controller and set large motor to spin at 'vibrate' speed
    
    if(ps2x.Button(PSB_START))                   //will be TRUE as long as button is pressed
         Serial.println("Start is being held");
    if(ps2x.Button(PSB_SELECT))
         Serial.println("Select is being held");
         
         
     if(ps2x.Button(PSB_PAD_UP)) {         //will be TRUE as long as button is pressed
       Serial.print("Up held this hard: ");
       Serial.println(ps2x.Analog(PSAB_PAD_UP), DEC);
       tien();
      }
      if(ps2x.Button(PSB_PAD_RIGHT)){
       Serial.print("Right held this hard: ");
        Serial.println(ps2x.Analog(PSAB_PAD_RIGHT), DEC);
        phai();
      }
      if(ps2x.Button(PSB_PAD_LEFT)){
       Serial.print("LEFT held this hard: ");
        Serial.println(ps2x.Analog(PSAB_PAD_LEFT), DEC);
        trai();
      }
      if(ps2x.Button(PSB_PAD_DOWN)){
       Serial.print("DOWN held this hard: ");
     Serial.println(ps2x.Analog(PSAB_PAD_DOWN), DEC);
     lui();
      }   
      if (!ps2x.Button(PSB_PAD_DOWN) && !ps2x.Button(PSB_PAD_UP) && !ps2x.Button(PSB_PAD_RIGHT) && !ps2x.Button(PSB_PAD_LEFT) && !ps2x.Button(PSB_RED) && !ps2x.Button(PSB_PINK)) {
        analogWrite(speedPinA, 0);
        analogWrite(speedPinB, 0);
        digitalWrite(dir1PinA, LOW); // Điều khiển động cơ A quay tiến về phía trước
        digitalWrite(dir2PinA, LOW);  //Nếu không quay theo đúng chiều ta đảo 2 dây motor ở 2 chân OUT1, OUT2 
        digitalWrite(dir1PinB, LOW); // Điều khiển động cơ B quay tiến về phía trước
        digitalWrite(dir2PinB, LOW);
        analogWrite(ballMotorR1, 0);
        analogWrite(ballMotorR2, 0);
        analogWrite(ballMotorL1, 0);
        analogWrite(ballMotorL2, 0);
      }
    
      vibrate = ps2x.Analog(PSAB_BLUE);        //this will set the large motor vibrate speed based on 
                                              //how hard you press the blue (X) button    
    
    if (ps2x.NewButtonState())               //will be TRUE if any button changes state (on to off, or off to on)
    {
     
       
         
        if(ps2x.Button(PSB_L3))
         Serial.println("L3 pressed");
        if(ps2x.Button(PSB_R3))
         Serial.println("R3 pressed");
        if(ps2x.Button(PSB_L2))
         Serial.println("L2 pressed");
        if(ps2x.Button(PSB_R2))
         Serial.println("R2 pressed");
        if(ps2x.Button(PSB_GREEN))
         Serial.println("Triangle pressed");
         
    }   
         
    
    if(ps2x.Button(PSB_RED)){           //will be TRUE if button was JUST pressed
         Serial.println("Circle just pressed");
         thaBong();
    }
         
    if(ps2x.Button(PSB_PINK)){             //will be TRUE if button was JUST released
         Serial.println("Square just released");     
         layBong();
    }
    
    if(ps2x.NewButtonState(PSB_BLUE))            //will be TRUE if button was JUST pressed OR released
         Serial.println("X just changed");    
    
    
    if(ps2x.Button(PSB_L1) || ps2x.Button(PSB_R1)) // print stick values if either is TRUE
    {
        Serial.print("Stick Values:");
        Serial.print(ps2x.Analog(PSS_LY), DEC); //Left stick, Y axis. Other options: LX, RY, RX  
        Serial.print(",");
        Serial.print(ps2x.Analog(PSS_LX), DEC); 
        Serial.print(",");
        Serial.print(ps2x.Analog(PSS_RY), DEC); 
        Serial.print(",");
        Serial.println(ps2x.Analog(PSS_RX), DEC); 
    } 
    
    
 }
 
 
 delay(50);

 
}
void tien(){
  analogWrite(speedPinA, 200);//Đặt tốc độ motor A thông qua PWM, các chỉ số đặt từ 0-1255 tương ứng với tốc độ từ 0 đến Max 
                            //Để tiết kiệm pin nên đặt chỉ số <=127 
digitalWrite(dir1PinA, HIGH); // Điều khiển động cơ A quay tiến về phía trước
digitalWrite(dir2PinA, LOW);  //Nếu không quay theo đúng chiều ta đảo 2 dây motor ở 2 chân OUT1, OUT2
 
analogWrite(speedPinB, 200);  //Đặt tốc độ motor B thông qua PWM, các chỉ số đặt từ 0-1255 tương ứng với tốc độ từ 0 đến Max  
digitalWrite(dir1PinB, LOW); // Điều khiển động cơ B quay tiến về phía trước
digitalWrite(dir2PinB, HIGH);
}
void lui(){
  analogWrite(speedPinA, 200);//Đặt tốc độ motor A thông qua PWM, các chỉ số đặt từ 0-1255 tương ứng với tốc độ từ 0 đến Max 
                            //Để tiết kiệm pin nên đặt chỉ số <=127 
digitalWrite(dir1PinA, LOW); // Điều khiển động cơ A quay tiến về phía trước
digitalWrite(dir2PinA, HIGH);  //Nếu không quay theo đúng chiều ta đảo 2 dây motor ở 2 chân OUT1, OUT2
 
analogWrite(speedPinB, 200);  //Đặt tốc độ motor B thông qua PWM, các chỉ số đặt từ 0-1255 tương ứng với tốc độ từ 0 đến Max  
digitalWrite(dir1PinB, HIGH); // Điều khiển động cơ B quay tiến về phía trước
digitalWrite(dir2PinB, LOW);
}
void trai(){
  analogWrite(speedPinA, 200);//Đặt tốc độ motor A thông qua PWM, các chỉ số đặt từ 0-1255 tương ứng với tốc độ từ 0 đến Max 
                            //Để tiết kiệm pin nên đặt chỉ số <=127 
digitalWrite(dir1PinA, HIGH); // Điều khiển động cơ A quay tiến về phía trước
digitalWrite(dir2PinA, LOW);  //Nếu không quay theo đúng chiều ta đảo 2 dây motor ở 2 chân OUT1, OUT2
 
analogWrite(speedPinB, 200);  //Đặt tốc độ motor B thông qua PWM, các chỉ số đặt từ 0-1255 tương ứng với tốc độ từ 0 đến Max  
digitalWrite(dir1PinB, HIGH); // Điều khiển động cơ B quay tiến về phía trước
digitalWrite(dir2PinB, LOW);
}
void phai(){
  analogWrite(speedPinA, 200);//Đặt tốc độ motor A thông qua PWM, các chỉ số đặt từ 0-1255 tương ứng với tốc độ từ 0 đến Max 
                            //Để tiết kiệm pin nên đặt chỉ số <=127 
digitalWrite(dir1PinA, LOW); // Điều khiển động cơ A quay tiến về phía trước
digitalWrite(dir2PinA, HIGH);  //Nếu không quay theo đúng chiều ta đảo 2 dây motor ở 2 chân OUT1, OUT2
 
analogWrite(speedPinB, 200);  //Đặt tốc độ motor B thông qua PWM, các chỉ số đặt từ 0-1255 tương ứng với tốc độ từ 0 đến Max  
digitalWrite(dir1PinB, LOW); // Điều khiển động cơ B quay tiến về phía trước
digitalWrite(dir2PinB, HIGH);
}
void sangTrai(){
  analogWrite(speedPinA, 200);//Đặt tốc độ motor A thông qua PWM, các chỉ số đặt từ 0-1255 tương ứng với tốc độ từ 0 đến Max 
                            //Để tiết kiệm pin nên đặt chỉ số <=127 
digitalWrite(dir1PinA, LOW); // Điều khiển động cơ A quay tiến về phía trước
digitalWrite(dir2PinA, HIGH);  //Nếu không quay theo đúng chiều ta đảo 2 dây motor ở 2 chân OUT1, OUT2
 
analogWrite(speedPinB, 200);  //Đặt tốc độ motor B thông qua PWM, các chỉ số đặt từ 0-1255 tương ứng với tốc độ từ 0 đến Max  
digitalWrite(dir1PinB, HIGH); // Điều khiển động cơ B quay tiến về phía trước
digitalWrite(dir2PinB, LOW);
}
void sangPhai(){
  analogWrite(speedPinA, 200);//Đặt tốc độ motor A thông qua PWM, các chỉ số đặt từ 0-1255 tương ứng với tốc độ từ 0 đến Max 
                            //Để tiết kiệm pin nên đặt chỉ số <=127 
digitalWrite(dir1PinA, HIGH); // Điều khiển động cơ A quay tiến về phía trước
digitalWrite(dir2PinA, LOW);  //Nếu không quay theo đúng chiều ta đảo 2 dây motor ở 2 chân OUT1, OUT2
 
analogWrite(speedPinB, 200);  //Đặt tốc độ motor B thông qua PWM, các chỉ số đặt từ 0-1255 tương ứng với tốc độ từ 0 đến Max  
digitalWrite(dir1PinB, LOW); // Điều khiển động cơ B quay tiến về phía trước
digitalWrite(dir2PinB, HIGH);
}
void layBong() {
  analogWrite(ballMotorR1, 0);
  analogWrite(ballMotorL1, 60);
}
void thaBong() {
  analogWrite(ballMotorR2, 0);
  analogWrite(ballMotorL2, 60);
}
