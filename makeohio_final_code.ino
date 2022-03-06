#define left1 3    // bottom left
#define left2 5    // middle left
#define left3 6    // top left
#define right1 9   // bottom right
#define right2 10  // middle right
#define right3 11  // top right

#define right_button 8  // back right button
#define left_button 7   // back left button

float voltage = 3.0*256/5;  // rated motor voltage converted to arduino units

long double prevMillis = 0;   // used in rh or lh modes, used to check last time voltage function was sampled
long double startMillis = 0;  // used to track start of function
long double stepMillis = 0;   // used to track time passed in function
int interval = 50;            // milliseconds between function sampling
char incomingByte = 0;        // allows python program to communicate
char previousByte = 0;
unsigned char hand = 'b';              // left, right, or both handed mode

long double aMillis = millis()-20000;        // makes sure we dont have any repeats
long double bMillis = millis()-20000;
long double cMillis = millis()-20000;
long double dMillis = millis()-20000;
long double eMillis = millis()-20000;
long double fMillis = millis()-20000;
long double currentMillis = 0;

void caller(char incomingByte, char hand);  // function definition

void setup() {
  // put your setup code here, to run once:
  pinMode(left1, OUTPUT);
  pinMode(left2, OUTPUT);  
  pinMode(left3, OUTPUT);
  pinMode(right1, OUTPUT);
  pinMode(right2, OUTPUT);
  pinMode(right3, OUTPUT);

  pinMode(right_button, INPUT);
  pinMode(left_button, INPUT);
  
  Serial.begin(9600);
  Serial.setTimeout(.1);
}



void loop() {
  currentMillis = millis();
  int t;
  if (Serial.available()) {        // checks for new information from serial
    incomingByte = Serial.read();  // reads new byte into variable

    if (incomingByte == 'A') {
      if ('A' == previousByte) {
        caller(incomingByte, hand);
        Serial.println('A');
      }
      caller(incomingByte, hand);
      delay(5000);
      for (int i = 0; i < 250; i++) {
        t = Serial.read();
      }
      aMillis = millis();
    }
    if (incomingByte == 'B') {
      if ('B' == previousByte) {
        caller(incomingByte, hand);
      }
      caller(incomingByte, hand);
      delay(5000);
      for (int i = 0; i < 250; i++) {
        t = Serial.read();
      }
      bMillis = millis();
    }
    if (incomingByte == 'C') {
      if ('C' == previousByte) {
        caller(incomingByte, hand);
      }
      caller(incomingByte, hand);
      delay(5000);
      for (int i = 0; i < 250; i++) {
        t = Serial.read();
      }
      cMillis = millis();
    }
    if (incomingByte == 'D') {
      if ('D' == previousByte) {
        caller(incomingByte, hand);
      }
      caller(incomingByte, hand);
      delay(5000);
      for (int i = 0; i < 250; i++) {
        t = Serial.read();
      }
      dMillis = millis();
    }
    if (incomingByte == 'E') {
      if ('E' == previousByte) {
        caller(incomingByte, hand);
      }
      caller(incomingByte, hand);
      delay(5000);
      for (int i = 0; i < 250; i++) {
        t = Serial.read();
      }
      eMillis = millis();
    }
    if (incomingByte == 'F') {
      if ('F' == previousByte) {
        caller(incomingByte, hand);
      }
      caller(incomingByte, hand);
      delay(5000);
      for (int i = 0; i < 250; i++) {
        t = Serial.read();
      }
      fMillis = millis();
    }

    
  }
  previousByte = incomingByte;
  if (digitalRead(right_button) == LOW && digitalRead(left_button) == LOW) {  // if both buttons are pressed at the same time, both handed mode
    hand = 'b';
  }
  else if (digitalRead(right_button) == LOW) {  // if right button is pressed, right handed mode
    hand = 'r';
  }
  else if (digitalRead(left_button) == LOW) {  // if left button is pressed,  left handed mode
    hand = 'l';
  }
}

void caller(char incB, char hand) {   // picker function, can choose right, left, between 1-3 times to iterate, and rh, lh, or both
  switch (incB) {           // variable taken from serial
    case 'A':               // right half mile out, buzzes once
      switch(hand) {        // checks hand mode for left right or both
        case 'l':
          lh_right(1);
          break;
        case 'r':
          rh_right(1);
          break;
        case 'b':
          both_right(1);
          break;
      }
    break;
    case 'B':               // right quarter mile out, buzzes twice
      switch(hand) {
        case 'l':
          lh_right(2);
          break;
        case 'r':
          rh_right(2);
          break;
        case 'b':
          both_right(2);
          break;
      }
    break;
    case 'C':               // right 200ft out, buzzes 3 times
      switch(hand) {
        case 'l':
          lh_right(3);
          break;
        case 'r':
          rh_right(3);
          break;
        case 'b':
          both_right(3);
          break;
      }
    break;
    case 'D':               // left half mile out, buzzes once
      switch(hand) {
        case 'l':
          lh_left(1);
          break;
        case 'r':
          rh_left(1);
          break;
        case 'b':
          both_left(1);
          break;
      }
      break;
    case 'E':                 // left quarter mile out, buzzes twice
      switch(hand) {
        case 'l':
          lh_left(2);
          break;
        case 'r':
          rh_left(2);
          break;
        case 'b':
          both_left(2);
          break;
      }
    break;
    case 'F':                // left 200ft out, buzzes three times
      switch(hand) {
        case 'l':
          lh_left(3);
          break;
        case 'r':
          rh_left(3);
          break;
        case 'b':
          both_left(3);
          break;
        }
    break;
  }
}

void lh_right(int n) {                // right turn using lh mode
  for (int i = 0; i < n; i++) {       // checks how many times to buzz
    
    startMillis = millis();           // clock to exist outside scope of function (delay is too inaccurate)
    float voltage1 = 0;               // initialize voltage values
    float voltage2 = 0;
    float voltage3 = 0;
  
    Serial.println("lh_right");       // debugging
    while (stepMillis < 1250) {       // total length of buzzes
      long nowMillis = millis();      // always keeps most current time
    
      if (nowMillis - prevMillis > interval) {  // checks if interval  has passed
        prevMillis = nowMillis;                 // updates prevMillis var
        voltage1 = (-1500)*(stepMillis/1000-0.5)*(stepMillis/1000-0.5)+0.6*256;      // inverse polynomial graph with peak at 3 volts and duration of ~= 4.5s
        voltage2 = (-1500)*(stepMillis/1000-1.0)*(stepMillis/1000-1.0)+0.6*256;       // same graph translated over .5 seconds
        voltage3 = (-1500)*(stepMillis/1000-1.5)*(stepMillis/1000-1.5)+0.6*256;       // same graph translated over 1 second
      }
      
      if (voltage1 > 0) {                 // makes sure voltage is positive
        digitalWrite(left1, voltage1);    // writes to motor
      }
      else {                              // else off
        digitalWrite(left1, LOW);
      }
      
      if (voltage2 > 0) {                 // same thing
        digitalWrite(left2, voltage2);
      }
      else {
        digitalWrite(left2, LOW);
      }
      
      if (voltage3 > 0) {
        digitalWrite(left3, voltage3);
      }
      else {
        digitalWrite(left3, LOW);
      }
      stepMillis = nowMillis - startMillis;
    
    }
    digitalWrite(left1, LOW);             // turns all motors off before proceeding
    digitalWrite(left2, LOW);
    digitalWrite(left3, LOW);
    delay(2000);                          // holds two seconds before iterating
    stepMillis = 0;
  }
}

/* All functions from here on out (besides both handed versions 
 * essentially follow the same format as lh_right(), so refer to 
 * that function for reference.  Only thing that changes is 
 * the pins and the pattern.
 */


void lh_left(int n) {
  for (int i = 0; i < n; i++) {
    
    startMillis = millis();
    float voltage1 = 0;
    float voltage2 = 0;
    float voltage3 = 0;
  
    Serial.println("lh_left");
    while (stepMillis < 1250) {
      long nowMillis = millis();
    
      if (nowMillis - prevMillis > interval) {
        prevMillis = nowMillis;
        voltage3 = (-1500)*(stepMillis/1000-0.5)*(stepMillis/1000-0.5)+0.6*256;
        voltage2 = (-1500)*(stepMillis/1000-1.0)*(stepMillis/1000-1.0)+0.6*256;
        voltage1 = (-1500)*(stepMillis/1000-1.5)*(stepMillis/1000-1.5)+0.6*256;
      }
      
      if (voltage1 > 0) {
        digitalWrite(left1, voltage1);
      }
      else {
        digitalWrite(left1, LOW);
      }
      
      if (voltage2 > 0) {
        digitalWrite(left2, voltage2);
      }
      else {
        digitalWrite(left2, LOW);
      }
      
      if (voltage3 > 0) {
        digitalWrite(left3, voltage3);
      }
      else {
        digitalWrite(left3, LOW);
      }
      stepMillis = nowMillis - startMillis;
    
    }
    digitalWrite(left1, LOW);
    digitalWrite(left2, LOW);
    digitalWrite(left3, LOW);
    delay(2000);
    stepMillis = 0;
  }
}

void rh_right(int n) {
  for (int i = 0; i < n; i++) {
    
    startMillis = millis();
    float voltage1 = 0;
    float voltage2 = 0;
    float voltage3 = 0;
  
    Serial.println("rh_right");
    while (stepMillis < 1250) {
      long nowMillis = millis();
    
      if (nowMillis - prevMillis > interval) {
        prevMillis = nowMillis;
        voltage3 = (-1500)*(stepMillis/1000-0.5)*(stepMillis/1000-0.5)+0.6*256;
        voltage2 = (-1500)*(stepMillis/1000-1.0)*(stepMillis/1000-1.0)+0.6*256;
        voltage1 = (-1500)*(stepMillis/1000-1.5)*(stepMillis/1000-1.5)+0.6*256;
      }
      
      if (voltage1 > 0) {
        digitalWrite(right1, voltage1);
      }
      else {
        digitalWrite(right1, LOW);
      }
      
      if (voltage2 > 0) {
        digitalWrite(right2, voltage2);
      }
      else {
        digitalWrite(right2, LOW);
      }
      
      if (voltage3 > 0) {
        digitalWrite(right3, voltage3);
      }
      else {
        digitalWrite(right3, LOW);
      }
      stepMillis = nowMillis - startMillis;
    
    }
    digitalWrite(right1, LOW);
    digitalWrite(right2, LOW);
    digitalWrite(right3, LOW);
    delay(2000);
    stepMillis = 0;
  }
}

void rh_left(int n) {
  for (int i = 0; i < n; i++) {
    
    startMillis = millis();
    float voltage1 = 0;
    float voltage2 = 0;
    float voltage3 = 0;
  
    Serial.println("rh_right");
    while (stepMillis < 1250) {
      long nowMillis = millis();
    
      if (nowMillis - prevMillis > interval) {
        prevMillis = nowMillis;
        voltage1 = (-1500)*(stepMillis/1000-0.5)*(stepMillis/1000-0.5)+0.6*256;
        voltage2 = (-1500)*(stepMillis/1000-1.0)*(stepMillis/1000-1.0)+0.6*256;
        voltage3 = (-1500)*(stepMillis/1000-1.5)*(stepMillis/1000-1.5)+0.6*256;
      }
      
      if (voltage1 > 0) {
        digitalWrite(right1, voltage1);
      }
      else {
        digitalWrite(right1, LOW);
      }
      
      if (voltage2 > 0) {
        digitalWrite(right2, voltage2);
      }
      else {
        digitalWrite(right2, LOW);
      }
      
      if (voltage3 > 0) {
        digitalWrite(right3, voltage3);
      }
      else {
        digitalWrite(right3, LOW);
      }
      stepMillis = nowMillis - startMillis;
    
    }
    digitalWrite(right1, LOW);
    digitalWrite(right2, LOW);
    digitalWrite(right3, LOW);
    delay(2000);
    stepMillis = 0;
  }
}

void both_left(int n) {               // used for both handed mode
  for (int i = 0; i < n; i++) {       // iterates through depending on distance
    digitalWrite(left1, voltage);     // constant voltage
    digitalWrite(left2, voltage);
    digitalWrite(left3, voltage);
    delay(400);                       // on .4 seconds
    digitalWrite(left1, LOW);         // voltage off
    digitalWrite(left2, LOW);
    digitalWrite(left3, LOW);
    delay(200);                       // off .2 seconds
    
  }
}

void both_right(int n) {              // same function different pins
  for (int i = 0; i < n; i++) {
    digitalWrite(right1, voltage);
    digitalWrite(right2, voltage);
    digitalWrite(right3, voltage);
    delay(400);
    digitalWrite(right1, LOW);
    digitalWrite(right2, LOW);
    digitalWrite(right3, LOW);
    delay(200);
  }
}
