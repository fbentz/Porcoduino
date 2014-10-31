#define MODE_XYZ_CONTROL 0
#define MODE_CAP_POUSSEE_Z_CONTROL 1

#define TEST_WITHOUT_Z

//general definitions to interface with the hardware 
#ifndef TEST_WITHOUT_Z
#define UP_MOTOR_FORWARD 5
#define UP_MOTOR_BACKWARD 6
#define UP_MOTOR_ENABLE 10
#endif

#define RIGHT_MOTOR_FORWARD 7
#define RIGHT_MOTOR_BACKWARD 8
#define RIGHT_MOTOR_ENABLE 3

#define LEFT_MOTOR_FORWARD 13
#define LEFT_MOTOR_BACKWARD 12
#define LEFT_MOTOR_ENABLE 11

//general definitions as parameters of the  
#define MOTOR_MAX_PWM 255.0

byte mode = 0;
char incomingByte_Mode = 0;

byte right_motor_pwm = 0;
byte left_motor_pwm = 0;

float right_motor_velocity = 0;
float left_motor_velocity = 0;

//Cap poussee control
float incomingFloat_Alpha_rad=0;
char incomingByte_Power = 0;

//X, Y control
char incomingByte_X = 0;
char incomingByte_Y = 0;

const float X_factor = 1.0;
const float Y_factor = 1.0;

float min_right;
float max_right;
float min_left;
float max_left;


#ifndef TEST_WITHOUT_Z
byte up_motor_pwm = 0;
float up_motor_velocity = 0;
char incomingByte_Z = 0;
const float Z_factor = 1.0;
float min_up;
float max_up;
#endif


const long interval = 200;     
long previousMillis = 0;

float sign(char number){
  if(number >= 0) return 1.0;
  else return -1.0;
}


//computing Motor velocity VS an X,Y control vector
float compute_Right_Motor_Raw_Velocity(float value_x, float value_y){
  return (X_factor*value_x - Y_factor*value_y*sign(value_x));
}

float compute_Left_Motor_Raw_Velocity(float value_x, float value_y){
  return (X_factor*value_x + Y_factor*value_y*sign(value_x));
}

#ifndef TEST_WITHOUT_Z
float compute_Up_Motor_Raw_Velocity(float value_z){
  return (Z_factor*value_z);
}
#endif

//computing Motor velocity VS an X,Y control vector
float compute_Right_Motor_Velocity(float value_x, float value_y){
    return map(compute_Right_Motor_Raw_Velocity(value_x, value_y), min_right, max_right,-MOTOR_MAX_PWM,MOTOR_MAX_PWM);
}

float compute_Left_Motor_Velocity(float value_x, float value_y){
  return map(compute_Left_Motor_Raw_Velocity(value_x, value_y), min_left, max_left,-MOTOR_MAX_PWM,MOTOR_MAX_PWM);
}

#ifndef TEST_WITHOUT_Z
float compute_Up_Motor_Velocity(float value_z){
  return map(compute_Up_Motor_Raw_Velocity(value_z), min_up, max_up,-MOTOR_MAX_PWM,MOTOR_MAX_PWM);
}
#endif

//this function is used to enable the use of compute_XXX_Motor_Velocity 
void compute_Resize_Parameters(){
   min_right = compute_Right_Motor_Raw_Velocity(-100.0, -100.0);
   max_right = compute_Right_Motor_Raw_Velocity(100.0, -100.0);
   min_left = compute_Left_Motor_Raw_Velocity(-100.0, 100.0);
   max_left = compute_Left_Motor_Raw_Velocity(100.0, 100.0);   
#ifndef TEST_WITHOUT_Z
   min_up = compute_Up_Motor_Raw_Velocity(-100.0);
   max_up = compute_Up_Motor_Raw_Velocity(100.0);   
#endif
}


void setup() {
 //initialize Serial communication
 Serial.begin(9600);
 Serial.flush();
 //initializing pins used to control motors power
 pinMode(RIGHT_MOTOR_FORWARD, OUTPUT);
 pinMode(RIGHT_MOTOR_BACKWARD, OUTPUT);
 pinMode(LEFT_MOTOR_FORWARD, OUTPUT);
 pinMode(LEFT_MOTOR_BACKWARD, OUTPUT);
//initializing pins used to control motors rotation clockwise or trigonometricwise
 digitalWrite(RIGHT_MOTOR_ENABLE, LOW);
 digitalWrite(LEFT_MOTOR_ENABLE, LOW);

#ifndef TEST_WITHOUT_Z 
 pinMode(UP_MOTOR_FORWARD, OUTPUT);
 pinMode(UP_MOTOR_BACKWARD, OUTPUT);
 digitalWrite(UP_MOTOR_ENABLE, LOW);
#endif
 
 compute_Resize_Parameters();
 mode = 0;//initial mode is XYZ control
}

void loop() {
    //continuously controlling motors by applying the computed pwm
    analogWrite(RIGHT_MOTOR_ENABLE, right_motor_pwm);
    analogWrite(LEFT_MOTOR_ENABLE, left_motor_pwm);
#ifndef TEST_WITHOUT_Z
    analogWrite(UP_MOTOR_ENABLE, up_motor_pwm);
#endif
    delay(100);//control loop = 10Hz
}

void serialEvent(){
  //function used to change the pwm motors command when receiving events from serial
  if (Serial.available() > 0) {
    incomingByte_Mode = Serial.read();
    mode = constrain(incomingByte_Mode, 0,1);
    switch(mode){
     case: MODE_XYZ_CONTROL:
     // read the velocity commands for each wheel
    incomingByte_X = Serial.read();
    incomingByte_X= constrain(incomingByte_X, -100,100);
    incomingByte_Y = Serial.read();
    incomingByte_Y= constrain(incomingByte_Y, -100,100);
#ifndef TEST_WITHOUT_Z 
    incomingByte_Z = Serial.read();
    incomingByte_Z= constrain(incomingByte_Z, -100,100);
#endif      
    //computing new desired velocity for each motor
    right_motor_velocity = compute_Right_Motor_Velocity(float(incomingByte_X), float(incomingByte_Y));
    left_motor_velocity = compute_Left_Motor_Velocity(float(incomingByte_X), float(incomingByte_Y));
#ifndef TEST_WITHOUT_Z     
    up_motor_velocity = compute_Up_Motor_Velocity(float(incomingByte_Z));
#endif
    break;


    case MODE_CAP_POUSSEE_Z_CONTROL:    
    incomingFloat_Alpha_rad = Serial.parseFloat();
    incomingFloat_Alpha_rad= constrain(incomingFloat_Alpha_rad, -3.14,3.14);
    incomingByte_Power = Serial.read();
    incomingByte_Power= constrain(incomingByte_Power, 0,100);
    
#ifndef TEST_WITHOUT_Z 
    incomingByte_Z = Serial.read();
    incomingByte_Z= constrain(incomingByte_Z, -100,100);
#endif      
    //computing new desired velocity for each motor
    incomingByte_X = cos(incomingFloat_Alpha_rad) * incomingByte_Power;
    incomingByte_Y = -sin(incomingFloat_Alpha_rad) * incomingByte_Power;
   right_motor_velocity = compute_Right_Motor_Velocity(float(incomingByte_X), float(incomingByte_Y));
   left_motor_velocity = compute_Left_Motor_Velocity(float(incomingByte_X), float(incomingByte_Y));
#ifndef TEST_WITHOUT_Z     
    up_motor_velocity = compute_Up_Motor_Velocity(float(incomingByte_Z));
#endif
    
    break;

    }

    //configuring right motor PWM
    right_motor_pwm = byte(abs(right_motor_velocity));
    if(right_motor_velocity >= 0){
        digitalWrite(RIGHT_MOTOR_FORWARD, HIGH);
        digitalWrite(RIGHT_MOTOR_BACKWARD, LOW);
    }
    else{
        digitalWrite(RIGHT_MOTOR_FORWARD, LOW);
        digitalWrite(RIGHT_MOTOR_BACKWARD, HIGH);
    }
    //configuring left motor PWM
    left_motor_pwm = byte(abs(left_motor_velocity));  
    if(left_motor_velocity >= 0){
        digitalWrite(LEFT_MOTOR_FORWARD, HIGH);
        digitalWrite(LEFT_MOTOR_BACKWARD, LOW);
    }
    else{
        digitalWrite(LEFT_MOTOR_FORWARD, LOW);
        digitalWrite(LEFT_MOTOR_BACKWARD, HIGH);
    }
    
#ifndef TEST_WITHOUT_Z
   //configuring up motor PWM
    up_motor_pwm = byte(abs(up_motor_velocity));  
    if(up_motor_velocity >= 0){
        digitalWrite(UP_MOTOR_FORWARD, HIGH);
        digitalWrite(UP_MOTOR_BACKWARD, LOW);
    }
    else{
        digitalWrite(UP_MOTOR_FORWARD, LOW);
        digitalWrite(UP_MOTOR_BACKWARD, HIGH);
    }
#endif
    //some debugging info
    Serial.write(right_motor_pwm);
    Serial.write(left_motor_pwm);
#ifndef TEST_WITHOUT_Z
    Serial.write(up_motor_pwm);
#endif
    Serial.flush();   
  }
}
