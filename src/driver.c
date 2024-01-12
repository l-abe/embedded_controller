/**
 * @file driver.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

/*********************
 *      DEFINES
 *********************/
// GPIO SETUP
#define GPIO_EXPORT_PATH      "/sys/class/gpio/export"

#define IN1_GPIO              "46"
#define IN1_VAL_PATH          "/sys/class/gpio/gpio46/value"
#define IN1_DIR_PATH          "/sys/class/gpio/gpio46/direction"

#define IN2_GPIO              "47"
#define IN2_VAL_PATH          "/sys/class/gpio/gpio47/value"
#define IN2_DIR_PATH          "/sys/class/gpio/gpio47/direction"

// PWM SETUP
#define PWM_PERIOD            (1000000)

#define PWM_EXPORT_PATH       "/sys/class/pwm/pwmchip8/export"
#define PWM_PERIOD_PATH       "/sys/class/pwm/pwmchip8/pwm0/period"
#define PWM_DUTY_CYCLE_PATH   "/sys/class/pwm/pwmchip8/pwm0/duty_cycle"
#define PWM_POLARITY_PATH     "/sys/class/pwm/pwmchip8/pwm0/polarity"
#define PWM_ENABLE_PATH       "/sys/class/pwm/pwmchip8/pwm0/enable"

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void driver_init(void)
{
  int fd;

  // export INPUT1
  fd = open(GPIO_EXPORT_PATH, O_WRONLY);
  write(fd, IN1_GPIO, 2);
  close(fd);
  // export INPUT2
  fd = open(GPIO_EXPORT_PATH, O_WRONLY);
  write(fd, IN2_GPIO, 2);
  close(fd);

  // Configure INPUT1 as output
  fd = open(IN1_DIR_PATH, O_WRONLY);
  write(fd, "out", 3);
  close(fd);
  // Configure INPUT2 as output
  fd = open(IN2_DIR_PATH, O_WRONLY);
  write(fd, "out", 3);
  close(fd);

  // export PWM
  fd = open(PWM_EXPORT_PATH, O_WRONLY);
  write(fd, "0", 1);
  close(fd);
  // Set PWM Period in nanoseconds
  char cbuf[10];
  sprintf(cbuf, "%d", PWM_PERIOD);
  fd = open(PWM_PERIOD_PATH, O_WRONLY);
  write(fd, cbuf, strlen(cbuf));
  close(fd);
  // Set PWM Duty Cycle to 50%. Value must be less than the period.
  sprintf(cbuf, "%d", PWM_PERIOD / 2);
  fd = open(PWM_DUTY_CYCLE_PATH, O_WRONLY);
  write(fd, cbuf, strlen(cbuf));
  close(fd);
  // Set PWM Polarity (you can use "normal" or "inversed").
  fd = open(PWM_POLARITY_PATH, O_WRONLY);
  write(fd, "normal", 6);
  close(fd);
  // Enable PWM signal
  fd = open(PWM_ENABLE_PATH, O_WRONLY);
  write(fd, "1", 1);
  close(fd);

  return;
}

void motor_pwm_update(float p_duty_cycle)
{
  int fd;
  int duty_cycle = p_duty_cycle * PWM_PERIOD;
  // Clip duty cycle period
  if (duty_cycle >= PWM_PERIOD) duty_cycle = PWM_PERIOD - 1;
  if (duty_cycle < 0) duty_cycle = 0;
  // Set PWM Duty Cycle
  char cbuf[10];
  sprintf(cbuf, "%d", duty_cycle);
  fd = open(PWM_DUTY_CYCLE_PATH, O_WRONLY);
  write(fd, cbuf, strlen(cbuf));
  close(fd);

  return;
}

void motor_stop(void)
{
  int fd;

  // Set INPUT1 as LOW
  fd = open(IN1_VAL_PATH, O_WRONLY | O_SYNC);
  write(fd, "0", 1);
  close(fd);
  // Set INPUT2 as LOW
  fd = open(IN2_VAL_PATH, O_WRONLY | O_SYNC);
  write(fd, "0", 1);
  close(fd);

  return;
}

void motor_forward(void)
{
  int fd;

  // Set INPUT1 as HIGH
  fd = open(IN1_VAL_PATH, O_WRONLY | O_SYNC);
  write(fd, "1", 1);
  close(fd);
  // Set INPUT2 as LOW
  fd = open(IN2_VAL_PATH, O_WRONLY | O_SYNC);
  write(fd, "0", 1);
  close(fd);

  return;
}

void motor_backward(void)
{
  int fd;

  // Set INPUT1 as LOW
  fd = open(IN1_VAL_PATH, O_WRONLY | O_SYNC);
  write(fd, "0", 1);
  close(fd);
  // Set INPUT2 as HIGH
  fd = open(IN2_VAL_PATH, O_WRONLY | O_SYNC);
  write(fd, "1", 1);
  close(fd);

  return;
}
