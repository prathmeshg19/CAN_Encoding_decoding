#include <memory> // for allocator, shared_ptr, __shared_ptr_access
#include <string> // for char_traits, operator+, to_string

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>
#include <thread>

#include "can_wrap.hpp"
#include "candata.h"
using can::operator<<;

const std::string canChannel = "vcan0";
const int canSocket = can::connect(canChannel);

uint16_t batteryvoltage;  // battery voltage
uint16_t frontleftspeed;  // speed of the front left wheel
uint16_t frontrightspeed; // speed of the front right wheel
uint16_t rearleftspeed;   // speed of the rear left wheel
uint16_t rearrightspeed;  // speed of the rear right wheel
_Float32 fronttorque;     // requested torque for the front wheels
_Float32 reartorque;      // requested torque for the rear wheels
_Float32 frontcurrent;    // calculated current for the front wheels
_Float32 rearcurrent;     // calculated current for the rear wheels
uint16_t maxtorquefront;  // maximum allowed torque for the front wheels
uint16_t maxtorquerear;   // maximum allowed torque for the rear wheels

/*Autogenerated Code from the DBC file
Packs the given value into CAN frame ,
@param value:Variable that stores the value to be packed in the frame
@param shift: Number of bytes to be shifted by
@param mask: Bit mask thats ANDed with the input*/
static inline uint8_t pack_left_shift_u16(uint16_t value, uint8_t shift,
                                          uint8_t mask) {
  return (uint8_t)((uint8_t)(value << shift) & mask);
}

/*Autogenerated Code from the DBC file
Packs the given value into CAN frame ,
@param value:Variable that stores the value to be packed in the frame
@param shift: Number of bytes to be shifted by
@param mask: Bit mask thats ANDed with the input*/
static inline uint8_t pack_right_shift_u16(uint16_t value, uint8_t shift,
                                           uint8_t mask) {
  return (uint8_t)((uint8_t)(value >> shift) & mask);
}

/*Autogenerated Code from DBC file
Takes data frame as a input and deocode it from HEX to Decimal value */
static inline uint16_t unpack_left_shift_u16(uint8_t value, uint8_t shift,
                                             uint8_t mask) {
  return (uint16_t)((uint16_t)(value & mask) << shift);
}

/*Autogenerated Code from DBC file
Takes data frame as a input and deocode it from HEX to Decimal value */
static inline uint16_t unpack_right_shift_u16(uint8_t value, uint8_t shift,
                                              uint8_t mask) {
  return (uint16_t)((uint16_t)(value & mask) >> shift);
}

/* Takes two parameters
@param a,b - inputs
@return - Max of the two inputs*/
_Float32 maxof(_Float32 a, _Float32 b) {
  if (a > b)
    return a;
  else
    return b;
}

/* Takes two parameters
@param a,b - inputs
@return - Min of the two inputs*/
_Float128 minof(_Float32 a, _Float32 b) {
  if (a < b)
    return a;
  else
    return b;
}

/*Reconstructs the value of the battery voltage from the CAN frame
Also checks whether the value of the parameter is within the limits from the DBC
file
@param frame : input can frame read from the CAN channel
@param batteryvoltage : stores the decodes value of battery voltage
*/
uint16_t batteryframe(const can_frame frame) {
  batteryvoltage = static_cast<uint16_t>(frame.data[0] << 8) +
                   static_cast<uint16_t>(frame.data[1]);

  if (batteryvoltage > 13) {
    std::cout << "Battery Voltage value Out of Range" << std::endl;
    return 1;
  }

  return 0;
}

/*Reconstructs the value of the wheel speeds from the CAN frame
Also checks whether the value of the parameter is within the limits from the DBC
file
@param frame : input can frame read from the CAN channel
@param frontleftspeed : stores the decodes value of speed for the front left
wheel
@param frontrightspeed : stores the decodes value of speed for the front right
wheel
@param rearleftspeed : stores the decodes value of speed for the rear left wheel
@param rearrightspeed : stores the decodes value of speed for the rear right
wheel
*/
uint16_t speedframe(const can_frame frame) {
  frontleftspeed = static_cast<uint16_t>(frame.data[0] << 8) +
                   static_cast<uint16_t>(frame.data[1]);

  frontrightspeed = static_cast<uint16_t>(frame.data[2] << 8) +
                    static_cast<uint16_t>(frame.data[3]);

  rearleftspeed = static_cast<uint16_t>(frame.data[4] << 8) +
                  static_cast<uint16_t>(frame.data[5]);

  rearrightspeed = static_cast<uint16_t>(frame.data[6] << 8) +
                   static_cast<uint16_t>(frame.data[7]);

  if (frontleftspeed > 65535 || frontrightspeed > 65535 ||
      rearleftspeed > 65535 || rearrightspeed > 65535) {
    std::cout << "One of the Wheel speed value is Out of Range" << std::endl;
    return 1;
  }

  return 0;
}

/*Reconstructs the value of the requested torque from the CAN frame
Also checks whether the value of the parameter is within the limits from the DBC
file
@param frame : input can frame read from the CAN channel
@param fronttorque : stores the decoded value of requested torque for the front
wheels
@param reartorque : stores the decoded value of requested torque for the rear
wheels
*/
int16_t torqueframe(const can_frame frame) {

  uint16_t tempF;
  uint16_t tempR;

  tempF = unpack_left_shift_u16(frame.data[0], 8u, 0xffu) +
                unpack_right_shift_u16(frame.data[1], 0u, 0xffu);

  tempR = unpack_left_shift_u16(frame.data[4], 8u, 0xffu) +
               unpack_right_shift_u16(frame.data[5], 0u, 0xffu);

  if(tempF<32767)
  {
    fronttorque = tempF;
  }
  else{

    fronttorque = tempF - 65536;

  };

  if(tempR<32767)
  {
    reartorque = tempF;
  }
  else{

    reartorque = tempR - 65536;

  };

  std::cout<<"Front torque is "<<fronttorque<<std::endl;
  std::cout<<"Rear Torque "<<reartorque<<std::endl;

  if (fronttorque > 32767 || fronttorque < -32768 || reartorque > 32767 ||
      reartorque < -32768) {
    std::cout << "One of the Torque Request is Out of Range" << std::endl;
    return 1;
  }

  return 0;
}

/*calculates the current based on the input parameters
Also determines the max torque limit for the wheels based on the speeds of the
wheel*/
void currentcal() {
  uint16_t frontwheelspeed =
      maxof(frontleftspeed,
            frontrightspeed); // the speed for the front wheel which is max of
                              // the speeds for the two wheels
  uint16_t rearwheelspeed = maxof(
      rearleftspeed, rearrightspeed); // the speed for the rear wheel which is
                                      // max of the speeds for the two wheels
  _Float32 qfactor = 0.879;
  _Float32 BEFF = 0.6721;

  _Float32 fronttorqueactual;
  _Float32 reartorqueactual;

  if (frontwheelspeed > 700) {
    maxtorquefront = 50;
  } else if (frontwheelspeed > 600) {
    maxtorquefront = 85;
  } else if (frontwheelspeed > 500) {
    maxtorquefront = 100;
  } else if (frontwheelspeed > 400) {
    maxtorquefront = 120;
  } else if (frontwheelspeed > 300) {
    maxtorquefront = 150;
  } else {
    maxtorquefront = 32767;
  }

  if (rearwheelspeed > 700) {
    maxtorquerear = 50;
  } else if (rearwheelspeed > 600) {
    maxtorquerear = 85;
  } else if (rearwheelspeed > 500) {
    maxtorquerear = 100;
  } else if (rearwheelspeed > 400) {
    maxtorquerear = 120;
  } else if (rearwheelspeed > 300) {
    maxtorquerear = 150;
  } else {
    maxtorquerear = 32767;
  }

  if (batteryvoltage < 2.8) {
    maxtorquefront = 20;
    maxtorquerear = 20;
  }

  // std::cout<<"Max Torque front = "<<maxtorquefront<<std::endl;
  // std::cout<<"Max torque Rear = "<<maxtorquerear<<std::endl;
  if(fronttorque<0){
    
    fronttorqueactual = maxof(fronttorque, -1*maxtorquefront);
  }
  else{

    fronttorqueactual = minof(fronttorque, maxtorquefront);

  }

if(reartorque <0){
  
  reartorqueactual=maxof(reartorque,-1*maxtorquerear);
}
else{
  int16_t reartorqueactual = minof(reartorque, maxtorquerear);
  };

  std::cout<<"Actual Torque front = "<<fronttorqueactual<<std::endl;
  std::cout<<"Actual torque Rear = "<<reartorqueactual<<std::endl;

  frontcurrent = (fronttorqueactual / batteryvoltage) *
                 (1 + (qfactor * qfactor * (fronttorqueactual / 100)) +
                  (frontwheelspeed * (BEFF / 100) * fronttorqueactual));
  rearcurrent = (reartorqueactual / batteryvoltage) *
                (1 + (qfactor * qfactor * (reartorqueactual / 100)) +
                 (rearwheelspeed * (BEFF / 100) * reartorqueactual));
}

/*@param frame: Input can frame read from the can channel
Identifies the type of the frame based on the can frame id and the sends the
frame to neccesary function to decode the value */
void process_frame(const can_frame frame) {
  switch (frame.can_id) {
  case 0x526:
    batteryframe(frame);
    break;

  case 0x525:
    speedframe(frame);
    break;

  case 0x521:
    torqueframe(frame);
    break;

  default:
    break;
  }
}

int main(int argc, char *argv[]) {
  // const std::string canChannel = "vcan0";

  can_frame Current;
  std::memset(&Current, 0, sizeof(Current));
  Current.can_id = 0x320;
  Current.can_dlc = 4;
  Current.data[0] = pack_right_shift_u16(frontcurrent, 8u, 0xffu);
  Current.data[1] = pack_left_shift_u16(frontcurrent, 0u, 0xffu);
  Current.data[2] = pack_right_shift_u16(rearcurrent, 8u, 0xffu);
  Current.data[3] = pack_left_shift_u16(rearcurrent, 0u, 0xffu);

  try {
    // const int canSocket = can::connect( canChannel );

    while (1) {
      const can_frame frame = can::read(canSocket);
      process_frame(frame);
      currentcal();

      Current.data[0] = pack_right_shift_u16(frontcurrent, 8u, 0xffu);
      Current.data[1] = pack_left_shift_u16(frontcurrent, 0u, 0xffu);
      Current.data[2] = pack_right_shift_u16(rearcurrent, 8u, 0xffu);
      Current.data[3] = pack_left_shift_u16(rearcurrent, 0u, 0xffu);

      // std::cout<<"Fornt Wheel speed is
      // "<<maxof(frontleftspeed,frontrightspeed)<<std::endl; std::cout<<"Rear
      // Wheel Speed is "<<maxof(rearleftspeed,rearrightspeed)<<std::endl;
      // std::cout<<"Torque Request for the Front is "<<fronttorque<<std::endl;
      // std::cout<<"Torque Request for the Rear is "<<reartorque<<std::endl;
      // std::cout<<"Front Current = "<<frontcurrent<<std::endl;
      // std::cout<<"Rear Current = "<<rearcurrent<<std::endl;
      std::cout << Current << std::endl;
      can::write(canSocket, Current);

      // std::cout<<"  "<<std::endl;
    }

    can::close(canSocket);
  } catch (const std::runtime_error &e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}
