# Benewake LIDAR Tools
Arduino scripts to interface to a Benewake TF03 LIDAR via the CAN interface.

```
WORK IN PROGRESS
```

## Requirements
### Hardware
* Teensy 4.0, though the code is easily portable to other microcontrollers
* Waveshare SN65HVD230 CAN Board, though any compatible CAN transceiver will work
* `Add a wiring diagram in here`

### Software
* FlexCAN_t4 library 
  * Available at `https://github.com/tonton81/FlexCAN_T4`
  * `Maybe I should just do this as a git submodule(?)`


## Examples
* A script to output the range and signal quality
* A script to ping all the different IDs
* A script to change the IDs of the device
