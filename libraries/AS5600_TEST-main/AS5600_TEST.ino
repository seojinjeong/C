#include <Wire.h>
#include <SPI.h>

#include "config.h"
#include "AS5600.h"

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
    Wire.begin();

    // set register to read once
    init_position_sensor();
    
}

void loop() {
   // put your main code here, to run repeatedly:
   // read position from sensor
      update_current_position(STEPPER_DEFAULT_MICRO_STEPS);
   
}
