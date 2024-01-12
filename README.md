# PID Controller - DC Motor

# Members
- L Abe (Code development and documentation)

## Introduction

This is a attempt to implement a PID Controller using:
- Toradex Colibri VF50;
- Motor Maxon RE25 118754;
- H-Bridge L298;
- Encoder HEDL 5540 110514.

> This code was not tested, use at your own risk.

## How to build and execute?

Check [configure-toolchain](https://developer-archives.toradex.com/getting-started/module-2-my-first-hello-world-in-c/configure-toolchain-colibri-vfxx) to learn to configure Toradex Software Development Kit (SDK) for cross-compilation of C/C++ code to the ARM architecture.

- Setting environment for cross compilation:
  ```sh
   . /usr/local/oecore-x86_64/environment-setup-armv7at2hf-neon-angstrom-linux-gnueabi
  ```

- Compiling the controller:
  ```sh
  make all
  ```

- Clean directory:
  ```sh
  make clean
  ```

- Copying binary to colibri VF50:
  ```sh
  scp ./bin/controller root@192.168.0.21:/home/root
  ```

- In the target terminal, run the application:
  ```sh
  ./controller
  ```

## What to expect in this code?

- The original PID Controller was written by tfoliva.
- The driver to control the motor is based on a H-Bridge configuration, that can control direction and speed.
- The sensor code compute the motor's speed using the time difference between N pulses from the encoder's Channel I. A new thread was created to keep getting the sensor data and not interfere in the main code.
- To make a timer to update the pid control was created a loop to wait until the next execution.

## Links

 * [colibri-vf50](https://developer.toradex.com/hardware/colibri-som-family/modules/colibri-vf50/)
 * [configure-toolchain](https://developer-archives.toradex.com/getting-started/module-2-my-first-hello-world-in-c/configure-toolchain-colibri-vfxx)
 * [basic-gpio-usage](https://developer-archives.toradex.com/getting-started/module-3-hardware-peripherals/basic-gpio-usage-iris-carrier-board-colibri-vfxx)
