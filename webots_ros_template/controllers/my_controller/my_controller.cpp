// File:          my_controller.cpp
// Date:
// Description:
// Author:
// Modifications:

// You may need to add webots include files such as
// <webots/DistanceSensor.hpp>, <webots/Motor.hpp>, etc.
// and/or to add some other includes
#include <webots/Robot.hpp>
#include <webots/Motor.hpp>
#include <webots/PositionSensor.hpp>
#include <webots/Connector.hpp>
#include <iostream>
#include <math.h>
#define PI 3.1415926
// All the webots classes are defined in the "webots" namespace
using namespace webots;
long ticks=0;
// This is the main program of your controller.
// It creates an instance of your Robot instance, launches its
// function(s) and destroys it at the end of the execution.
// Note that only one instance of Robot should be created in
// a controller program.
// The arguments of the main function can be specified by the
// "controllerArgs" field of the Robot node
int main(int argc, char **argv) {
  // create the Robot instance.
  Robot *robot = new Robot();

  // get the time step of the current world.
  int timeStep = (int)robot->getBasicTimeStep();
  
  // You should insert a getDevice-like function in order to get the
  // instance of a device of the robot. Something like:
   Motor *motor = robot->getMotor("arm motor");
   motor->setVelocity(PI*0.1);
   PositionSensor * ps = robot->getPositionSensor("arm angle sensor");
   ps->enable(timeStep);
   
   Connector *cnt = robot->getConnector("connector");
  //  DistanceSensor *ds = robot->getDistanceSensor("dsname");
  //  ds->enable(timeStep);
   motor->setPosition(INFINITY);
  // Main loop:
  // - perform simulation steps until Webots is stopping the controller
  while (robot->step(timeStep) != -1) {
    ticks++;
    if(ticks<1000)
    {
      motor->setVelocity(PI*0.01*ticks);
    }
    // Read the sensors:
    // Enter here functions to read sensor data, like:
    //  double val = ds->getValue();
    double angle = ps->getValue();
    int round = angle/(2*PI);
    double _angle = (angle-round*2*PI)/PI*180.0f;
    std::cout<<"step "<<timeStep<<" round:"<<round<<" angle:"<<_angle<<std::endl;
    if( ticks>=1000 &&(_angle-45*7)>0)
    {
      std::cout<<"shooting!"<<std::endl;
      cnt->unlock();
    }
    // Process sensor data here.
    
    // Enter here functions to send actuator commands, like:
     
  };

  // Enter here exit cleanup code.

  delete robot;
  return 0;
}
