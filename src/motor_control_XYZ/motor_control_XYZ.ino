
#define MOTOR_MAX_PWM 255.0

const int motor_right_forward = 7;
const int motor_right_backward = 8;
const int motor_right_enable = 3;

const int motor_left_forward = 13;
const int motor_left_backward = 12;
const int motor_left_enable = 11;

const int motor_up_forward = 5;//TODO set the adequate value
const int motor_up_backward = 6;//TODO set the adequate value
const int motor_up_enable = 10;//TODO set the adequate value


byte motor_right_pwm = 0;
byte motor_left_pwm = 0;
byte motor_up_pwm = 0;

float motor_right_speed = 0;
float motor_left_speed = 0;
float motor_up_speed = 0;

char incomingByte_X = 0;
char incomingByte_Y = 0;
char incomingByte_Z = 0;

const float X_factor = 1.0;
const float Y_factor = 1.0;
const float Z_factor = 1.0;

float min_right;
float max_right;
float min_left;
float max_left;
float min_up;
float max_up;

const long interval = 200;     
long previousMillis = 0;

float sign(char number){
  if(number >= 0) return 1.0;
  else return -1.0;
}


//computing Motor velocity VS an X,Y control vector
float compute_Right_Motor_Raw_Speed(float value_x, float value_y){
  return (X_factor*value_x - Y_factor*value_y*sign(value_x));
}

float compute_Left_Motor_Raw_Speed(float value_x, float value_y){
  return (X_factor*value_x + Y_factor*value_y*sign(value_x));
}

float compute_Up_Motor_Raw_Speed(float value_z){
  return (Z_factor*value_z);
}

//computing Motor velocity VS an X,Y control vector
float compute_Right_Motor_Speed(float value_x, float value_y){
    return map(compute_Right_Motor_Raw_Speed(value_x, value_y), min_right, max_right,-MOTOR_MAX_PWM,MOTOR_MAX_PWM);
}

float compute_Left_Motor_Speed(float value_x, float value_y){
  return map(compute_Left_Motor_Raw_Speed(value_x, value_y), min_left, max_left,-MOTOR_MAX_PWM,MOTOR_MAX_PWM);
}

float compute_Up_Motor_Speed(float value_z){
  return map(compute_Up_Motor_Raw_Speed(value_z), min_up, max_up,-MOTOR_MAX_PWM,MOTOR_MAX_PWM);
}


//this function is used to enable the use of compute_XXX_Motor_Speed 
void compute_Resize_Parameters(){
   min_right = compute_Right_Motor_Raw_Speed(-100.0, -100.0);
   max_right = compute_Right_Motor_Raw_Speed(100.0, -100.0);
   min_left = compute_Left_Motor_Raw_Speed(-100.0, 100.0);
   max_left = compute_Left_Motor_Raw_Speed(100.0, 100.0);   
   min_up = compute_Up_Motor_Raw_Speed(-100.0);
   max_up = compute_Up_Motor_Raw_Speed(100.0);   
}


void setup() {
 //initialize Serial communication
 Serial.begin(9600);
 Serial.flush();
 //initializing pins used to control motors 
 pinMode(motor_right_forward, OUTPUT);
 pinMode(motor_right_backward, OUTPUT);
 pinMode(motor_left_forward, OUTPUT);
 pinMode(motor_left_backward, OUTPUT); 
 pinMode(motor_up_forward, OUTPUT);
 pinMode(motor_up_backward, OUTPUT);
 
 
 digitalWrite(motor_right_enable, LOW);
 digitalWrite(motor_left_enable, LOW);
 digitalWrite(motor_up_enable, LOW);
 compute_Resize_Parameters();
}

void loop() {
  
    //continuously controlling motors
    analogWrite(motor_right_enable, motor_right_pwm);
    analogWrite(motor_left_enable, motor_left_pwm);
    analogWrite(motor_up_enable, motor_up_pwm);
    delay(100);//control loop = 10Hz
}

void serialEvent(){
  //function used to change the pwm motors command when receiving events from serial
  if (Serial.available() > 0) {
    // read the velocity commands for each wheel
    incomingByte_X = Serial.read();
    incomingByte_X= constrain(incomingByte_X, -100,100);
    incomingByte_Y = Serial.read();
    incomingByte_Y= constrain(incomingByte_Y, -100,100);
    incomingByte_Z = Serial.read();
    incomingByte_Z= constrain(incomingByte_Z, -100,100);
    
    //computing new speed for wheels  
    //configuring right motor PWM
    motor_right_pwm = byte(abs(motor_right_speed));
    if(motor_right_speed >= 0){
        digitalWrite(motor_right_forward, HIGH);
        digitalWrite(motor_right_backward, LOW);
    }
    else{
        digitalWrite(motor_right_forward, LOW);
        digitalWrite(motor_right_backward, HIGH);
    }
    //configuring left motor PWM
    motor_left_pwm = byte(abs(motor_left_speed));  
    if(motor_left_speed >= 0){
        digitalWrite(motor_left_forward, HIGH);
        digitalWrite(motor_left_backward, LOW);
    }
    else{
        digitalWrite(motor_left_forward, LOW);
        digitalWrite(motor_left_backward, HIGH);
    }
    //configuring up motor PWM
    motor_up_pwm = byte(abs(motor_up_speed));  
    if(motor_up_speed >= 0){
        digitalWrite(motor_up_forward, HIGH);
        digitalWrite(motor_up_backward, LOW);
    }
    else{
        digitalWrite(motor_up_forward, LOW);
        digitalWrite(motor_up_backward, HIGH);
    }
    //some debugging info
    //Serial.write(motor_right_pwm);
    //Serial.write(motor_left_pwm);
    //Serial.write(motor_up_pwm);
    Serial.flush();   
  }
}
