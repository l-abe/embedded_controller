#include <stdio.h>
#include <sys/time.h>

#include "driver.h"
#include "pid.h"
#include "sensor.h"

/*
int setitimer(int which, const struct itimerval* newValue,
              struct itimerval* oldValue);
int getitimer(int which, struct itimerval* value);

struct itimerval {
    struct timeval itInterval;  // next value
    struct timeval itValue;     // current value
};

struct timeval {
    long tv_sec;
    long tv_usec;
};
*/

// Maxon RE25 118754 Data
#define NOMINAL_VOLTAGE (42.0) // v
#define NOMINAL_SPEED   (9920.0) // rpm

float measurement;
float setpoint = NOMINAL_SPEED / 2;
float control_action;

int main(int argc, char* argv[])
{
  // Init sensor readings
  Sensor_init();

  // Init DC Motor Driver H-Bridge
  driver_init();
  // Set Motor direction to forward
  motor_forward();

  // PID Controller
  PIDController controller;
  PIDController_Init(&controller);

  // Timer Variables
  struct timeval cur_time, next_time;
  double diff_time;

  while(1) {
    // Get current time
    gettimeofday(&cur_time, NULL);

    // Get measurement from the encoder
    measurement = Sensor_update();

    // Compute new control signal
    control_action = PIDController_Update(&controller, setpoint, measurement);

    // Update output value
    motor_pwm_update(control_action / NOMINAL_VOLTAGE);

    // Loop until next execution
    do {
      // Update next time value
      gettimeofday(&next_time, NULL);
      // Calculate time diff
      diff_time = next_time.tv_sec + next_time.tv_usec / 1000000.0
                  - cur_time.tv_sec - cur_time.tv_usec / 1000000.0;
    } while (diff_time <= controller.T);
    //printf("diff time = %lf\t Sampling time = %lf\n", diff_time, controller.T);
  }

  return 0;
}
