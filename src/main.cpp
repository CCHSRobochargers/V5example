/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       VEX                                                       */
/*    Created:      Thu Sep 26 2019                                           */
/*    Description:  Competition Template                                      */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// LeftDriveFront       motor         1               
// LeftDriveRear        motor         2               
// RightDriveRear       motor         9               
// RightDriveFront      motor         10              
// Controller1          controller                    
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
#include "vex_controller.h"

using namespace vex;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here
motor_group LeftDrive(LeftDriveFront, LeftDriveRear);
motor_group RightDrive(RightDriveFront, RightDriveRear);

// 4" wheels on a 12" (width/track) by 5" (length/wheelbase)
// platform
// 1.0 => external gear ratio (direct drive)
drivetrain Drivetrain = drivetrain(LeftDrive, RightDrive,
  4.0 * M_PI, 12.0, 5.0, inches, 1.0);

typedef enum {Red, Blue} allianceSelType;
const char *allianceText[] = {"Red", "Blue"};
allianceSelType allianceSelect = Red;

#define NUM_AUTO 5
const char *autoText[NUM_AUTO] = {"None", "Big", "Small", "Big1", "Small1"};
int autoSelect = 0;

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {
  int sel = 0;
  bool bLeft = false;
  bool bRight = false;
  bool bUp = false;
  bool bDown = false;

  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  // Select the autonomous routine to run
  Controller1.rumble(rumbleShort);
  wait(50, msec);

  // Select the autonomous routine to run
  while(!Controller1.ButtonA.pressing()) {
    if ((!bUp && Controller1.ButtonUp.pressing()) ||
        (!bDown && Controller1.ButtonDown.pressing())) {
      if (sel == 0) {
        sel = 1;
      } else {
        sel = 0;
      }
    }

     if (sel == 0) {
      if ((!bRight && Controller1.ButtonRight.pressing()) ||
          (!bLeft && Controller1.ButtonLeft.pressing())) {
        if (allianceSelect == Blue) {
          allianceSelect = Red;
        } else {
          allianceSelect = Blue;
        }
      }
    }
    if (sel == 1) {
      if (!bRight && Controller1.ButtonRight.pressing()) {
        if (autoSelect == NUM_AUTO - 1) {
          autoSelect = 0;
        } else {
          autoSelect++;
        }
      }
      if (!bLeft && Controller1.ButtonLeft.pressing()) {
        if (autoSelect == 0) {
          autoSelect = NUM_AUTO - 1;
        } else {
          autoSelect--;
        }
      }
    }

    // Update the selection
    Controller1.Screen.clearLine(3);
    wait(50, msec);
    if (sel == 0) {
      Controller1.Screen.print("Alliance: %s", allianceText[allianceSelect]);
    } else {
      Controller1.Screen.print("Auto: %s", autoText[autoSelect]);
    }

    // Save the buttons for press detection
    bUp = Controller1.ButtonUp.pressing();
    bDown = Controller1.ButtonDown.pressing();
    bRight = Controller1.ButtonRight.pressing();
    bLeft = Controller1.ButtonLeft.pressing();

    wait(50, msec);
  }

  Controller1.Screen.clearLine(3);
  wait(50, msec);
  Controller1.Screen.print("%s | %s", allianceText[allianceSelect], autoText[autoSelect]);

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
  Drivetrain.setDriveVelocity(60.0, rpm);
  Drivetrain.setTurnVelocity(60.0, rpm);
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void autonomous(void) {
  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................
  Drivetrain.driveFor(24.0 * 3.0, inches); // Three tiles forward
  Drivetrain.turnFor(180, degrees);
  Drivetrain.driveFor(24.0 * 2.0, inches); // Two tiles back
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void usercontrol(void) {
  // User control code here, inside the loop
  while (1) {
    // This is the main execution loop for the user control program.
    // Each time through the loop your program should update motor + servo
    // values based on feedback from the joysticks.

    // ........................................................................
    // Insert user code here. This is where you use the joystick values to
    // update your motors, etc.
    // ........................................................................

    // calculate the drivetrain motor velocities from the controller joystick axies
    // left = Axis3
    // right = Axis2
    int drivetrainLeftSideSpeed = Controller1.Axis3.position();
    int drivetrainRightSideSpeed = Controller1.Axis2.position();

    // only tell the left drive motor to spin if the values are not in the deadband range
    if ((drivetrainLeftSideSpeed > 5.0) || (drivetrainLeftSideSpeed < -5.0)) {
      LeftDrive.setVelocity(drivetrainLeftSideSpeed, percent);
      LeftDrive.spin(forward);
    } else {
      LeftDrive.stop();
    }
    // only tell the right drive motor to spin if the values are not in the deadband range
    if ((drivetrainRightSideSpeed > 5.0) || (drivetrainRightSideSpeed < -5.0)) {
      RightDrive.setVelocity(drivetrainRightSideSpeed, percent);
      RightDrive.spin(forward);
    } else {
      RightDrive.stop();
    }

    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
