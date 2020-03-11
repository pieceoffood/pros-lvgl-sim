#ifndef MOTOR_HPP
#define MOTOR_HPP
#include "main.h"

extern pros::Controller master;

extern pros::Motor left_back;
extern pros::Motor left_front;
extern pros::Motor right_back;
extern pros::Motor right_front;

extern pros::Motor tray;
extern pros::Motor arm;
extern pros::Motor left_roller;
extern pros::Motor right_roller;
extern int8_t lf, lb, rf, rb;

extern pros::ADIDigitalIn limitswitch ;
extern pros::ADIAnalogIn potentiameter;
extern pros::ADIGyro gyro;
extern pros::ADIDigitalIn limitswitchball;
extern pros::ADIEncoder lencoder; // left encoder 
extern pros::ADIEncoder rencoder; // right encoder
#endif
