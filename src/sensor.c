/**
 * @file sensor.c
 *
 */

/*********************
 *      INCLUDES
 *********************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
#include <sys/time.h>
#include <string.h>

/*********************
 *      DEFINES
 *********************/

// GPIO SETUP - Channel I
#define GPIO_EXPORT_PATH      "/sys/class/gpio/export"
#define RX_GPIO               "49"
#define RX_DIR_PATH           "/sys/class/gpio/gpio49/direction"
#define RX_EDGE_PATH          "/sys/class/gpio/gpio49/edge"
#define RX_VAL_PATH           "/sys/class/gpio/gpio49/value"

// SENSOR DATA
#define COUNTS_PER_TURN       (500)
#define SAMPLES               (5)

/**********************
 *  STATIC PROTOTYPES
 **********************/

void * sensor_thread (void);

/**********************
 *  STATIC VARIABLES
 **********************/

static int fd;

static struct pollfd poll_gpio;

static pthread_t id1;

static double velocity;

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void Sensor_init (void)
{
  // export GPIO
  fd = open(GPIO_EXPORT_PATH, O_WRONLY);
  write(fd, RX_GPIO, 2);
  close(fd);

  // configure as input
  fd = open(RX_DIR_PATH, O_WRONLY);
  write(fd, "in", 2);
  close(fd);

  // configure interrupt
  fd = open(RX_EDGE_PATH, O_WRONLY);
  write(fd, "rising", 6); // configure as rising edge
  close(fd);

  // file descriptor from SW is being polled
  poll_gpio.fd = fd; 
  // poll events in GPIO 
  poll_gpio.events = POLLPRI;

  // Creates a thread to calculate velocity
  pthread_create (&id1, NULL , (void *) sensor_thread , NULL);
}

float Sensor_update (void)
{
  return velocity;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

void * sensor_thread (void)
{
  static struct timeval t0, t1;
  static double diff_time;
  int count;

	while (1) {
    // Get time 0
    gettimeofday(&t0, NULL);

    // wait for interrupt
    for(count = 0; count <  SAMPLES; count++) {
      lseek(fd, 0, SEEK_SET);
      poll(&poll_gpio, 1, -1);
    }

    // Get time 1
    gettimeofday(&t1, NULL);

    // Compute time diff
    diff_time = t1.tv_sec + t1.tv_usec / 1000000.0
                - t0.tv_sec - t0.tv_usec / 1000000.0;

    // Compute Velocity
    velocity = ( 60 / COUNTS_PER_TURN ) / diff_time; // rpm
	}

  // Code should never reach here
	pthread_exit(NULL);
}
