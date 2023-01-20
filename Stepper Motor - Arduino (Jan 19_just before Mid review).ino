
#include <Blynk.h>
// #include <BlynkSimpleEsp8266.h> //for ESP8266
#include <Stepper.h>

const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution
const int DistancePerStep = 1; //Do calibration and change the value

const int fwd_lim = 10;
const int left_lim = 10;
const int right_lim = 10;

uint8_t trigPin1 = A0, trigPin2 = A2, trigPin3 = A4;
uint8_t echoPin1 = A1, echoPin2 = A3, echoPin3 = A5;


// initialize the stepper library for both steppers:
Stepper stepper_R(stepsPerRevolution, 8,10,9,11);  
Stepper stepper_L(stepsPerRevolution, 3,5,4,6);



const int trigPins[3] = {trigPin1, trigPin2, trigPin3};
const int echoPins[3] = {echoPin1, echoPin2, echoPin3};

int x = 0;
int y = 0;
int movement = 0;

// char auth[] = "YourAuthToken";  \\ write specific Blynk authentication token

const int LEFT = 0;
const int RIGHT = 1;
const int FORWARD = 2;

//========================================================================

float firstsensor() {
  int distance1, duration1;
  digitalWrite (trigPin1, HIGH);
  delayMicroseconds (2);
  digitalWrite (trigPin1, LOW);
  duration1 = pulseIn (echoPin1, HIGH);
  distance1 = (duration1 / 2) / 29.1;
  Serial.print("FORWARD: ");
  Serial.print(distance1);

  return distance1;
}

float secondsensor() {
int duration2, distance2;
  digitalWrite (trigPin2, HIGH);
  delayMicroseconds (2);
  digitalWrite (trigPin2, LOW);
  duration2 = pulseIn (echoPin2, HIGH);
  distance2 = (duration2 / 2) / 29.1;
  Serial.print("LEFT: ");
  Serial.print(distance2);
  
  return distance2;
}
float thirdsensor() {
  int duration3, distance3;
  digitalWrite (trigPin3, HIGH);
  delayMicroseconds (2);
  digitalWrite (trigPin3, LOW);
  duration3 = pulseIn (echoPin3, HIGH);
  distance3 = (duration3 / 2) / 29.1;
  Serial.print("RIGHT: ");
  Serial.print(distance3);
 
  return distance3;
}
//========================================================================


int steps = 0; 

void setup() {
  // Blynk.begin(auth);

   // set the speed of the motors 
  stepper_R.setSpeed(300);    // set first stepper speed
  stepper_L.setSpeed(300);   // set second stepper speed


  char* Sensor_movement[3] = {"Fwd,", "Lft,", "Rt"};

  Serial.begin(9600);
  steps = 0;
  
  int i;
  for(i=0;i<3;i++){
      pinMode(echoPins[i], INPUT);
      pinMode(trigPins[i], OUTPUT);
      Serial.print (Sensor_movement[i]);
    }
}


void loop() {

  float f = firstsensor(), l = secondsensor(), r = thirdsensor();
// Check if the distance is less than a certain threshold
  if (f < 10) {
    // Determine the movement to move to avoid the obstacle
    if(l < r){
        movement = LEFT;
    }else{
        movement = RIGHT;
    }
    move(movement);
    updateCoordinates(movement);
  }else{
      forward();
      updateCoordinates(FORWARD);
  }

  // Blynk.virtualWrite(V1, x);
  // Blynk.virtualWrite(V2, y);
  // Blynk.run(); // Send the data to Blynk app in real-time
}



//====================================================================================================
void move(int movement) {
  if (movement == FORWARD) {
    forward();
  }
  else if (movement == LEFT) {
    left();
  }
  else if (movement == RIGHT) {
    right();
  }
  if (movement == LEFT || movement == RIGHT) {
    steps = 0;  // reset step count to zero after a left or right turn
  }
}


void forward() {

  stepper_R.step(stepsPerRevolution);
 
  stepper_L.step(stepsPerRevolution);
  steps += stepsPerRevolution;
}

void right() {
    stepper_L.step(stepsPerRevolution/4);
    stepper_R.step(-stepsPerRevolution/4);

    
}

void left() {
  // stepper_L.step(0);
  stepper_L.step(-stepsPerRevolution/4);
  stepper_R.step(stepsPerRevolution/4);
  // dir = (dir-1)%4;
}
void stopp() {
  stepper_L.step(0);
  stepper_R.step(0);
}

const int initial_direction = FORWARD; // set the initial direction of the car
int direction = initial_direction;


void updateCoordinates(int movement) {
  if (movement == FORWARD) {
    if (direction == FORWARD) {
      y+=steps*DistancePerStep;
    } else if (direction == LEFT) {
      x-=steps*DistancePerStep;
    } else if (direction == RIGHT) {
      x+=steps*DistancePerStep;
    }
  } else if (movement == LEFT) {
    if (direction == FORWARD) {
      x-=steps*DistancePerStep;
    } else if (direction == LEFT) {
      y-=steps*DistancePerStep;
    } else if (direction == RIGHT) {
      x-=steps*DistancePerStep;
    }
    direction = LEFT; // Update direction to LEFT
  } else if (movement == RIGHT) {
    if (direction == FORWARD) {
      x+=steps*DistancePerStep;
    } else if (direction == LEFT) {
      x+=steps*DistancePerStep;
    } else if (direction == RIGHT) {
      y-=steps*DistancePerStep;
    }
    direction = RIGHT; // Update direction to RIGHT
  }
  if (movement == LEFT || movement == RIGHT) {
    steps = 0;  // reset step count to zero after a left or right turn
  }
}


//=====================================================================================================
