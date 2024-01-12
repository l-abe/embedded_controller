/**
 * @file driver.h
 *
 */

#ifndef DRIVER_H
#define DRIVER_H

/**********************
 * GLOBAL PROTOTYPES
 **********************/
 
/**
 * Initialize GPIOs and PWM to use with H-Bridge
 */
void driver_init(void);

/**
 * Set new PWM Duty cycle
 */
void motor_pwm_update(float p_duty_cycle);

/**
 * Stop motor
 */
void motor_stop(void);

/**
 * Set motor direction forward
 */
void motor_forward(void);

/**
 * Set motor direction backward
 */
void motor_backward(void);

#endif /* DRIVER_H */
