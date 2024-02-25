// File:          my_controller.cpp
// Date:
// Description:
// Author:
// Modifications:

// You may need to add webots include files such as
// <webots/DistanceSensor.hpp>, <webots+/Motor.hpp>, etc.
// and/or to add some other includes
#include <webots/Robot.hpp>
#include <webots/DistanceSensor.hpp>
#include <webots/Motor.hpp>
#include <iostream>
#include <webots/GPS.hpp>
#include <webots/Gyro.hpp>

using namespace webots;
void moveRight(Motor *motor1, Motor *motor2, Motor *motor3, Motor *motor4)
{
    float vel = 55.4;    
    motor1->setVelocity(-vel);
    motor2->setVelocity(vel);
    motor3->setVelocity(-vel - 0.4);
    motor4->setVelocity(vel + 0.4);    
}
void moveLeft(Motor *motor1, Motor *motor2, Motor *motor3, Motor *motor4)
{
    float vel = 55.4;
    motor1->setVelocity(-vel - 0.4);
    motor2->setVelocity(vel + 0.4);
    motor3->setVelocity(-vel);
    motor4->setVelocity(vel);   
}
void forward(Motor *motor1, Motor *motor2, Motor *motor3, Motor *motor4)
{
    float vel = 55.4;
    motor1->setVelocity(-vel);
    motor2->setVelocity(vel + 0.4);
    motor3->setVelocity(-vel - 0.4);
    motor4->setVelocity(vel); 
    
}
int main(int argc, char **argv) {
    
    
    Robot *robot = new Robot();
    Motor *motor1 = robot->getMotor("m1_motor"); //m1 arriba de m2, m3 opuesto
    Motor *motor2 = robot->getMotor("m2_motor");
    Motor *motor3 = robot->getMotor("m3_motor");
    Motor *motor4 = robot->getMotor("m4_motor");
    int timeStep = (int) robot->getBasicTimeStep();
    motor1->setPosition(INFINITY);
    motor2->setPosition(INFINITY);
    motor3->setPosition(INFINITY);
    motor4->setPosition(INFINITY);
    GPS *gps = robot->getGPS("gps");
    Gyro *gyro = robot->getGyro("gyro");
    std::printf("%p\n", gyro);
    gps->enable(timeStep);
    gyro->enable(timeStep);
    double targetAltitude = 5.0;
    double lastError = 0;
    double ierror = 0;
    double kp = 1;
    double ki = 0.1;
    double kd = 0.5;
    double vel = 55.4;
    
    float t = 0;
    bool changed = false;
    while (robot->step(timeStep) != -1) {
        const double *values = gps->getValues();
        double xvel = values[0];
        double yvel = values[1];
        double zvel = values[2];
        
        //std::printf("%.3f, %.3f, %.3f\n", xvel, yvel, zvel);
        
        /*
         double dt = double (timeStep) / 1000.0;
        t += dt;
        if (t > 10.0f && !changed)
        {
            forward(motor1, motor2, motor3, motor4);
            changed = true;
        }
        
        //std::printf("%.4f %.4f %.4f %.4f\n", state.m1, state.m2, state.m3, state.m4);
        std::printf("Target: %.4f Curr: %.4f Error: %.4f Error': %.4f Vel: %.4f\n", targetAltitude, altitude, error, derror, vel);
        */
        double dt = double (timeStep) / 1000.0;
        
        double altitude = values[2];
        double error = targetAltitude - altitude;
        double derror = (error - lastError) / dt;
        ierror += dt * error;
        lastError = error;
        double pid = kp * error + ki * ierror + kd * derror;
        vel = pid;
        motor1->setVelocity(-vel);
        motor2->setVelocity(vel);
        motor3->setVelocity(-vel);
        motor4->setVelocity(vel);
        //std::printf("%.4f %.4f %.4f %.4f\n", state.m1, state.m2, state.m3, state.m4);
        std::printf("Target: %.4f Curr: %.4f Error: %.4f DError': %.4f Vel: %.4f\n", targetAltitude, altitude, error, derror, vel);
    }
    
    
    delete robot;
    return 0;
}
