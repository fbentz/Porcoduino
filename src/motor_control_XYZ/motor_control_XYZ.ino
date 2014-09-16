
#define MOTOR_MAX_PWM 255.0

const int motor_right_forward = 7;
const int motor_right_backward = 8;
const int motor_right_enable = 3;

const int motor_left_forward = 13;
const int motor_left_backward = 12;
const int motor_left_enable = 11;

byte motor_right_pwm = 0;
byte motor_left_pwm = 0;

float motor_right_speed = 0;
float motor_left_speed = 0;

char incomingByte_X = 0;
char incomingByte_Y = 0;
char incomingByte_Z = 0;

const float X_factor = 2.0;
const float Y_factor = 1.0;
const float Z_factor = 1.0;

float min_right;
float max_right;
float min_left;
float max_left;

const long interval = 200;     
long previousMillis = 0;

float sign(char number){
  if(number >= 0) return 1.0;
  else return -1.0;
}

float compute_Right_Wheel_Raw_Speed(float value_x, float value_y){
  return (X_factor*value_x - Y_factor*value_y*sign(value_x));
}

float compute_Left_Wheel_Raw_Speed(float value_x, float value_y){
  return (X_factor*value_x + Y_factor*value_y*sign(value_x));
}

float compute_Right_Wheel_Speed(float value_x, float value_y){
    return map(compute_Right_Wheel_Raw_Speed(value_x, value_y), min_right, max_right,-MOTOR_MAX_PWM,MOTOR_MAX_PWM);
}

float compute_Left_Wheel_Speed(float value_x, float value_y){
  return map(compute_Left_Wheel_Raw_Speed(value_x, value_y), min_left, max_left,-MOTOR_MAX_PWM,MOTOR_MAX_PWM);
}

//
void compute_Resize_Parameters(){
   min_right = compute_Right_Wheel_Raw_Speed(-100.0, -100.0);
   max_right = compute_Right_Wheel_Raw_Speed(100.0, -100.0);
   min_left = compute_Left_Wheel_Raw_Speed(-100.0, 100.0);
   max_left = compute_Left_Wheel_Raw_Speed(100.0, 100.0);   
}


void setup() {
 Serial.begin(9600);
 Serial.flush();
 pinMode(motor_right_forward, OUTPUT);
 pinMode(motor_right_backward, OUTPUT);
 pinMode(motor_left_forward, OUTPUT);
 pinMode(motor_left_backward, OUTPUT); 

 digitalWrite(motor_right_enable, LOW);
 digitalWrite(motor_left_enable, LOW);
 compute_Resize_Parameters();
}

void loop() {
  
    analogWrite(motor_right_enable, motor_right_pwm);
    analogWrite(motor_left_enable, motor_left_pwm);
    delay(100);
}

void serialEvent(){
  if (Serial.available() > 0) {
    // read the speed commandqs for each wheel
    incomingByte_X = Serial.read();
    incomingByte_X= constrain(incomingByte_X, -100,100);
    incomingByte_Y = Serial.read();
    incomingByte_Y= constrain(incomingByte_Y, -100,100);
    //computing new speed for right and left wheels  
//    motor_right_speed = X_factor*float(incomingByte_X) - Y_factor *float(incomingByte_Y)*float(sign(incomingByte_X)) ;
//    motor_right_speed = map(motor_right_speed, -300.0, 300.0,-255.0,255.0);
//    motor_left_speed = X_factor*float(incomingByte_X) + Y_factor *float(incomingByte_Y)*float(sign(incomingByte_X));
//    motor_left_speed = map(motor_left_speed, -300.0, 300.0,-255.0,255.0);
    motor_right_speed = compute_Right_Wheel_Speed(float(incomingByte_X), float(incomingByte_Y));
    motor_left_speed = compute_Left_Wheel_Speed(float(incomingByte_X), float(incomingByte_Y));
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
    Serial.write(motor_right_pwm);
    Serial.write(motor_left_pwm);
    Serial.flush();   
  }
}
