#include <memory>  // for allocator, shared_ptr, __shared_ptr_access
#include <string>  // for char_traits, operator+, to_string
 
#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"       // for Slider, Renderer, Vertical
#include "ftxui/component/component_base.hpp"  // for ComponentBase
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for separator, operator|, Element, size, text, vbox, xflex, bgcolor, hbox, GREATER_THAN, WIDTH, border, HEIGHT, LESS_THAN
#include "ftxui/screen/color.hpp"  // for Color

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
using namespace ftxui;

const std::string canChannel = "vcan0";
const int canSocket = can::connect( canChannel );

    uint8_t buffer1;
    uint8_t buffer2;

/*Element ColorTile(int red, int green, int blue) {
  return text("") | size(WIDTH, GREATER_THAN, 14) |
         size(HEIGHT, GREATER_THAN, 7) | bgcolor(Color::RGB(red, green, blue));
}*/
 
Element Display (int batV, int frontL, int frontR, int rearL, int rearR , int torqueF, int torqueR ) {
  return text("Battery Voltage = " +               
              std::to_string(batV) + "\n" +    //
              "Wheel Speeds \n"+
              "Front Left"+
              std::to_string(frontL) + "\n" +  //
              "Front Right"+
              std::to_string(frontR) + "\n" + 
              "Rear Left"+
              std::to_string(rearL) + "\n" + 
              "Rear Right"+
              std::to_string(rearR) + "\n" + 
              "Requested Torque \n"+
              "Front = " +
              std::to_string(torqueF) + "\n" +
              "Rear = " + 
              std::to_string(torqueR)+"\n"    //
  );
}

static inline uint8_t pack_left_shift_u16(
    uint16_t value,
    uint8_t shift,
    uint8_t mask)
{
    return (uint8_t)((uint8_t)(value << shift) & mask);
}

static inline uint8_t pack_right_shift_u16(
    uint16_t value,
    uint8_t shift,
    uint8_t mask)
{
    return (uint8_t)((uint8_t)(value >> shift) & mask);
}

int candata_vcu_battery_pack1(
    uint8_t *dst_p,
    const struct candata_vcu_battery_t *src_p,
    size_t size)
{

   //const std::string canChannel = "vcan0";
    //const int canSocket = can::connect( canChannel );

    if (size < 2u) {
        return (-EINVAL);
    }
    
    memset(&dst_p[0], 0, 2);

    dst_p[0] |= pack_left_shift_u16(src_p->voltage, 0u, 0xffu);
    dst_p[1] |= pack_right_shift_u16(src_p->voltage, 8u, 0xffu);

     can_frame Battery;
        std::memset( &Battery, 0, sizeof(Battery) );

        Battery.can_id = 0x510;
        Battery.can_dlc = 4;
        Battery.data[0] = dst_p[0]; //battVolt; //Battery
        Battery.data[1] = dst_p[1];

        try
        {
            //std::cout << RPM << torque << std::endl;
            std::cout << Battery << std::endl;
            can::write( canSocket, Battery );
        }
        catch( const std::runtime_error& e )
        {
            std::cerr << e.what() << std::endl;
        }

        std::this_thread::sleep_for( std::chrono::seconds( 1 ) );


    return (2);
}

int candata_vcu_wheel_speeds_pack1(
    uint8_t *dst_p,
    const struct candata_vcu_wheel_speeds_t *src_p,
    size_t size)
{
    //const std::string canChannel = "vcan0";
    //const int canSocket = can::connect( canChannel );
    
    if (size < 8u) {
        return (-EINVAL);
    }

    memset(&dst_p[0], 0, 8);

    dst_p[0] |= pack_left_shift_u16(src_p->fl_wheel_speed, 0u, 0xffu);
    dst_p[1] |= pack_right_shift_u16(src_p->fl_wheel_speed, 8u, 0xffu);
    dst_p[2] |= pack_left_shift_u16(src_p->fr_wheel_speed, 0u, 0xffu);
    dst_p[3] |= pack_right_shift_u16(src_p->fr_wheel_speed, 8u, 0xffu);
    dst_p[4] |= pack_left_shift_u16(src_p->rl_wheel_speed, 0u, 0xffu);
    dst_p[5] |= pack_right_shift_u16(src_p->rl_wheel_speed, 8u, 0xffu);
    dst_p[6] |= pack_left_shift_u16(src_p->rr_wheel_speed, 0u, 0xffu);
    dst_p[7] |= pack_right_shift_u16(src_p->rr_wheel_speed, 8u, 0xffu);

    can_frame RPM;
        std::memset( &RPM, 0, sizeof(RPM) );

        RPM.can_id = 0x511;
        RPM.can_dlc = 8;
        RPM.data[0] = dst_p[0]; 
        RPM.data[1] = dst_p[1];
        RPM.data[2] = dst_p[2];
        RPM.data[3] = dst_p[3];
        RPM.data[4] = dst_p[4];
        RPM.data[5] = dst_p[5];
        RPM.data[6] = dst_p[6];
        RPM.data[7] = dst_p[7];

        try
        {
            //std::cout << RPM << torque << std::endl;
            std::cout << RPM << std::endl;
            can::write( canSocket, RPM);
        }
        catch( const std::runtime_error& e )
        {
            std::cerr << e.what() << std::endl;
        }

        std::this_thread::sleep_for( std::chrono::seconds( 1 ) );

    return (8);
}

int candata_ai_drive_request_pack1(
    uint8_t *dst_p,
    const struct candata_ai_drive_request_t *src_p,
    size_t size)
{
    uint16_t front_trq_request;
    uint16_t rear_trq_request;
    uint16_t steering_request;

    //const std::string canChannel = "vcan0";
    //const int canSocket = can::connect( canChannel );

    if (size < 6u) {
        return (-EINVAL);
    }

    memset(&dst_p[0], 0, 6);

    front_trq_request = (uint16_t)src_p->front_trq_request;
    dst_p[0] |= pack_left_shift_u16(front_trq_request, 0u, 0xffu);
    dst_p[1] |= pack_right_shift_u16(front_trq_request, 8u, 0xffu);
    steering_request = (uint16_t)src_p->steering_request;
    dst_p[2] |= pack_left_shift_u16(steering_request, 0u, 0xffu);
    dst_p[3] |= pack_right_shift_u16(steering_request, 8u, 0xffu);
    rear_trq_request = (uint16_t)src_p->rear_trq_request;
    dst_p[4] |= pack_left_shift_u16(rear_trq_request, 0u, 0xffu);
    dst_p[5] |= pack_right_shift_u16(rear_trq_request, 8u, 0xffu);

    can_frame TorqueReq;
        std::memset( &TorqueReq, 0, sizeof(TorqueReq) );

        TorqueReq.can_id = 0x511;
        TorqueReq.can_dlc = 6;
        TorqueReq.data[0] = dst_p[0]; 
        TorqueReq.data[1] = dst_p[1];
        TorqueReq.data[2] = dst_p[2];
        TorqueReq.data[3] = dst_p[3];
        TorqueReq.data[4] = dst_p[4];
        TorqueReq.data[5] = dst_p[5];

        try
        {
            //std::cout << RPM << torque << std::endl;
            std::cout << TorqueReq << std::endl;
            can::write( canSocket, TorqueReq);
        }
        catch( const std::runtime_error& e )
        {
            std::cerr << e.what() << std::endl;
        }

        std::this_thread::sleep_for( std::chrono::seconds( 1 ) );

    return (6);
}

int send_data(int battVolt,
              int16_t speedFL,int16_t speedFR,int16_t speedRL,int16_t speedRR,
              int16_t torqueF,int16_t torqueR){

    //int16_t battVolt; //battery Voltage
    //int16_t speedFR;  //front right wheeel speed
    //int16_t speedFL; //front left wheel speed
    //int16_t speedRR; //Rear right wheel speed
    //int16_t speedRL; //Rear left wheel speed
    //int16_t torqueF; //Requested torque front
    //int16_t torqueR; //Requested torque rear

     //const std::string canChannel = "vcan0";
    //const int canSocket = can::connect( canChannel );

    struct candata_vcu_battery_t battery;
    battery.voltage = battVolt;
    size_t batSize = 2;

    struct candata_vcu_wheel_speeds_t speed ;
    speed.fl_wheel_speed = speedFL;
    speed.fr_wheel_speed = speedFR;
    speed.rl_wheel_speed = speedRL;
    speed.rr_wheel_speed = speedRR;
    size_t speedSize = 8;

    struct candata_ai_drive_request_t torqueReq;
    torqueReq.front_trq_request = torqueF;
    torqueReq.rear_trq_request = torqueR;
    torqueReq.steering_request = 0;
    size_t torqueSize = 6 ;
    
    //uint8_t battframe;
    //const struct candata_vcu_battery_t *src_b;
   

    const struct candata_vcu_battery_t *batptr = &battery;

    

    while (true)
    {
       candata_vcu_battery_pack1(&buffer1,batptr,batSize);
       candata_vcu_wheel_speeds_pack1(&buffer2,&speed,speedSize);
    }

	//can::close( canSocket );

	return 0;

}

 
int main(int argc, const char* argv[]) {

  //const std::string canChannel = "vcan0";
  //const int canSocket = can::connect( canChannel );

  int bat ;//= 128;
  int SFL ;//= 25;
  int SFR;
  int SRL;
  int SRR;
  int TF ;//= 100;
  int TR;
  auto slider_1 = Slider("Battery Voltage  :", &bat, 0, 13, 1);
  auto slider_2 = Slider("Speed Front Left :", &SFL, -65535, 65535, 10);
  auto slider_3 = Slider("Speed Front Right:", &SFR, -65535, 65535, 10);
  auto slider_4 = Slider("Speed Rear Left  :", &SRL, -65535, 65535, 10);
  auto slider_5 = Slider("Speed Rear Right :", &SRR, -65535, 65535, 10);
  auto slider_6 = Slider("Front Torque     :", &TF, -32768, 32767, 10);
  auto slider_7 = Slider("Rear Torque      :", &TR, -32768, 32767, 10);
 
  auto container = Container::Vertical({
      slider_1,
      slider_2,
      slider_3,
      slider_4,
      slider_5,
      slider_6,
      slider_7,
  });
 
  auto renderer = Renderer(container, [&] {
    return hbox({
               //ColorTile(red, green, blue),
               separator(),
               vbox({
                   slider_1->Render(),
                   separator(),
                   slider_2->Render(),
                   separator(),
                   slider_3->Render(),
                   separator(),
                   slider_4->Render(),
                   separator(),
                   slider_5->Render(),
                   separator(),
                   slider_6->Render(),
                   separator(),
                   slider_7->Render(),
                   separator(),
                   Display (bat, SFL, SFR,SRL,SRR,TF,TR),
               }) | xflex,
           }) |
           border | size(WIDTH, LESS_THAN, 80);
  });
  auto screen = ScreenInteractive::TerminalOutput();
  screen.Loop(renderer);

  send_data(bat,(int16_t)SFL,(int16_t)SFR,(int16_t)SRL,(int16_t)SRR,(int16_t)TF,(int16_t)TR);
  //send_data(10,10000,10000,10000,20000,30000,20000);
  return 0;
}
 
 
// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.

