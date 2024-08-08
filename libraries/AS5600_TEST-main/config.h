#ifndef NIRYO_CONFIG_H
#define NIRYO_CONFIG_H

/*
 *    ----------- AS5600 Position sensor -------------
 */

#define AS5600_CPR 4096
#define AS5600_CPR_HALF 2048

#define AS5600_ADDRESS 0x36

#define AS5600_REG_CONF    0x07
#define AS5600_REG_ANGLE_H 0x0E
#define AS5600_REG_ANGLE_L 0x0F

/*
 *    ----------- Stepper controller -------------
 */

#define STEPPER_CPR 200

#define STEPPER_CONTROL_MODE_RELAX    0
#define STEPPER_CONTROL_MODE_STANDARD 1

#define STEPPER_DELAY_MIN 200

#define STEPPER_DEFAULT_MICRO_STEPS 8   // default 8
#define ONE_FULL_STEP 1800   // default 1800

#define STEPPER_CALIBRATION_OK        1
#define STEPPER_CALIBRATION_TIMEOUT   2
#define STEPPER_CALIBRATION_BAD_PARAM 3




#endif
