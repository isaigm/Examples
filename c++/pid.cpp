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
#include <SFML/Network.hpp>
#include <webots/Keyboard.hpp>
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
template<class T>
T clamp(T val, T min, T max)
{
    if (val < min) return min;
    if (val > max) return max;
    return val;
}
template<class T>
struct pid
{
    T update(T currVal, T target, T dt)
    {
        T error  = target - currVal;
        T derror = (error - lastError) / dt;

        integralError += dt * error;
        lastError = error;
        T output  = kp * clamp(error, -1.0, 1.0) + ki * integralError + kd * derror;

        return output;
    }
    T kp {0}, kd {0}, ki {0};
    T lastError {0};
    T integralError {0};
};
template<class T>
struct MotorVelocities
{
    T m1 {}, m2 {}, m3 {}, m4 {};
};
template<class T>
MotorVelocities<T> mixPids(T throttle, T roll, T pitch, T yaw)
{
    MotorVelocities<T> velocities {};
    velocities.m1 = clamp(throttle - roll + pitch + yaw, 55.3, 55.49);
    velocities.m2 = clamp(throttle - roll - pitch - yaw, 55.3, 55.49);
    velocities.m3 = clamp(throttle + roll - pitch + yaw, 55.3, 55.49);
    velocities.m4 = clamp(throttle + roll + pitch - yaw, 55.3, 55.49);
    return velocities;
}

int main(int argc, char **argv) {
    sf::UdpSocket socket;
    socket.setBlocking(false);
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
    GPS *gps           = robot->getGPS("gps");
    Gyro *gyro         = robot->getGyro("gyro");
    Keyboard *keyboard = robot->getKeyboard();
    keyboard->enable(timeStep);
    gps->enable(timeStep);
    gyro->enable(timeStep);
    double targetAltitude = 40.0;
    double vel = 55.4;
    motor1->setVelocity(-1);
    motor2->setVelocity(1);
    motor3->setVelocity(-1);
    motor4->setVelocity(1);

    while(robot->step(timeStep) != -1)
    {
        if (robot->getTime() > 2)
        {
            break;
        }
    }

    pid<double> altPid;
    altPid.kp = 1;
    altPid.ki = 0.1;
    altPid.kd = 0.5;

    pid<double> rollPid, pitchPid, yawPid;
    rollPid.kp =  1;
    rollPid.ki = 0.2;
    rollPid.kp = 0.5;

    pitchPid.kp =  1;
    pitchPid.ki = 0.1;
    pitchPid.kp = 0.5;

    yawPid.kp = 1;
    yawPid.ki = 0.08;
    yawPid.kd = 0.01;

    float t = 0;

    while (robot->step(timeStep) != -1) {
        const double *gpsValues  = gps->getValues();
        const double *gyroValues = gyro->getValues();
        double xvel = gyroValues[0];
        double yvel = gyroValues[1];
        double zvel = gyroValues[2];


        double dt = double (timeStep) / 1000.0;
        double altitude = gpsValues[2];

        double roll  = rollPid.update(xvel, 0, dt);
        double pitch = pitchPid.update(yvel, -0.04, dt);
        double yaw   = yawPid.update(zvel, 0, dt);

        auto velocities = mixPids(vel, roll, pitch, yaw);
        int key = keyboard->getKey();
        while (key > 0)
        {
            switch(key)
            {
            case Keyboard::UP:
                vel += 0.06;
                break;
            case Keyboard::DOWN:
                vel -= 0.06;
                break;
            default:
                break;
            }
            key = keyboard->getKey();
        }

        sf::IpAddress ip = sf::IpAddress::LocalHost;
        unsigned short port = 8080;

        //socket.send(&altitude, sizeof(double), ip, port);

        motor1->setVelocity(-velocities.m1);
        motor2->setVelocity(velocities.m2);
        motor3->setVelocity(-velocities.m3);
        motor4->setVelocity(velocities.m4);
        //std::printf("%.4f %.4f %.4f %.4f\n", state.m1, state.m2, state.m3, state.m4);
        std::printf("Curr: %.4f M1: %.4f M2: %.4f M3: %.4f M4: %.4f X: %.4f Y: %.4f Z: %.4f Vel: %.4f\n", altitude,
                    velocities.m1, velocities.m2, velocities.m3,
                    velocities.m4, xvel, yvel, zvel, vel);

    }


    delete robot;
    return 0;
}
