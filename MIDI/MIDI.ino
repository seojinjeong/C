byte noteON = 144;//note on command
byte note;//storage for currently playing note
//int buttonPin1 = 2;
//int buttonPin2 = 3;
//int buttonPin3 = 4;
//int buttonPin4 = 5;
int potPin = A0;
boolean currentState = LOW;//stroage for current button state
boolean lastState = LOW;//storage for last button state
void setup(){
//  pinMode(buttonPin1, INPUT);
//  pinMode(buttonPin2, INPUT);
//  pinMode(buttonPin3, INPUT);
//  pinMode(buttonPin4, INPUT);
  //this time we will set the pin as INPUT
  Serial.begin(9600);//initialize Serial connection
}
void loop(){
  int PotVal = analogRead(potPin);
    byte velocity = map(PotVal, 0, 1023, 0, 127);
//    
//  currentState = digitalRead(buttonPin1);
//  if (currentState == HIGH && lastState == LOW){
//    
//    
//    
//    MIDImessage(noteON, 60, velocity);//turn note 60 on with 127 velocity
//    delay(200);//crude form of button debouncing
//  } else if(currentState == LOW && lastState == HIGH){
//    MIDImessage(noteON, 60, 0);//turn note 60 off
//    delay(2);//crude form of button debouncing
//  }
//  lastState = currentState;
//  currentState = digitalRead(buttonPin2);
//  if (currentState == HIGH && lastState == LOW){//if button has just been pressed
//    MIDImessage(noteON, 61, velocity);//turn note 60 on with 127 velocity
//    delay(200);//crude form of button debouncing
//  } else if(currentState == LOW && lastState == HIGH){
//    MIDImessage(noteON, 60, 0);//turn note 60 off
//    delay(2);//crude form of button debouncing
//  }
//  lastState = currentState;
//  currentState = digitalRead(buttonPin3);
//  if (currentState == HIGH && lastState == LOW){//if button has just been pressed
//    MIDImessage(noteON, 62, velocity);//turn note 60 on with 127 velocity
//    delay(200);//crude form of button debouncing
//  } else if(currentState == LOW && lastState == HIGH){
//    MIDImessage(noteON, 60, 0);//turn note 60 off
//    delay(2);//crude form of button debouncing
//  }
//  lastState = currentState;
//  currentState = digitalRead(buttonPin4);
//  if (currentState == HIGH && lastState == LOW){//if button has just been pressed
//    MIDImessage(noteON, 63, velocity);//turn note 60 on with 127 velocity
//    delay(200);//crude form of button debouncing
//  } else if(currentState == LOW && lastState == HIGH){
//    MIDImessage(noteON, 60, 0);//turn note 60 off
//    delay(2);//crude form of button debouncing
//  }
//  lastState = currentState;
}
//send MIDI message
void MIDImessage(byte command, byte data1, byte data2) {
  Serial.write(command);
  Serial.write(data1);
  Serial.write(data2);
}
