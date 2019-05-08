//Show Pinouts
//D3 = IN1
//D4 = nFault
//D5 = IN2

//Readings from hall effect
//198 = electromagnet OFF
//325 = electromagnet ON
//191 = electromagnet REVERSE
//520 = ideal

int levVal = 488;         //Desired value for the hall effect reading (past: 488)
int magOffset = 251;      //This the offset value for our electromagnet output (past: 251)

float P = - .2;           //Proportional Gain
float D = - .001;         //Derivative Gain
float dlyVal = 1000;      //Delay for obtaining value for dT

//Set up pins
int hallSensor = A2;
int IN1 = 3;
int IN2 = 5;

// Set up times for delta calc
unsigned long currentTime;
unsigned long lastTime;
float dT = 1;

// Set up magnet control variables
int sensorValue;
int error;
int lastError;

void setup() {
  pinMode(hallSensor, INPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  
  Serial.begin(115200);

  error = 0;
  lastError = 0;
  lastTime = millis();
}


void loop() {
  currentTime = millis();
  dT = currentTime - lastTime;

//  Serial.println(dT);
  
  sensorValue = analogRead(hallSensor);
  
//  Serial.println(sensorValue);

  error = sensorValue - levVal;                       //positive if magnet is too high, negative if magnet is too low
  int derivativeValue = (error - lastError) / dT;     //positive if magnet is higher than before, negative if magnet is lower than before
  lastError = error;
    
//  Serial.print(" , ");
//  Serial.print(derivativeValue);

  int writeValue = P * error + D * derivativeValue + magOffset;
  if(writeValue > 255){
    writeValue = 255;
  }
//  Serial.print(" , ");
//  Serial.println(writeValue);

  if(sensorValue < levVal)
  {
    analogWrite(IN1, 0);
    analogWrite(IN2, writeValue);
  }
  else
  {
    analogWrite(IN2, 0);
  }

  delayMicroseconds(dlyVal);
  lastTime = currentTime;

//  Serial.print(currentTime);
//  Serial.print(" , ");
//  Serial.print(sensorValue);
//  Serial.print(" , ");
//  Serial.println(writeValue);
}
