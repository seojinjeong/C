//Global Variables
    int sensorPin = 0;      //pin number to use the ADC
    int sensorValue = 0;    //initialization of sensor variable, equivalent to EMA Y
    float EMA_a = 0.6;      //initialization of EMA alpha
    int EMA_S = 0;          //initialization of EMA S
 
void setup(){
    Serial.begin(9600);           //setup of Serial module, 115200 bits/second
    EMA_S = analogRead(sensorPin);  //set EMA S for t=1
}
 
void loop(){
    sensorValue = analogRead(sensorPin);                //read the sensor value using ADC
    sensorValue = map(sensorValue, 0, 1023, 0, 600);
    EMA_S = (EMA_a*sensorValue) + ((1-EMA_a)*EMA_S);    //run the EMA
    Serial.println(EMA_S);                              //print digital value to serial
    delay(10);                                          //50ms delay
}