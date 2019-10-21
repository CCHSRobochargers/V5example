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
#include "field-250.h"  // Field graphic (250x250 pixels PNG)

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

#define FIELD_GRAPHIC_WIDTH 250
#define FIELD_GRAPHIC_HEIGHT 250
#define BRAIN_SCREEN_WIDTH 480
#define BRAIN_SCREEN_HEIGHT 272
// Center on the X axis
#define FIELD_LOC_X ((BRAIN_SCREEN_WIDTH / 2) - (FIELD_GRAPHIC_WIDTH / 2))
// Put on the bottom on the Y axis
#define FIELD_LOC_Y ((BRAIN_SCREEN_HEIGHT - 25) - FIELD_GRAPHIC_HEIGHT)

typedef struct {
  const char *label;
  int x;
  int y;
  int width;
  int height;
} hotspotType;

// The (0,0) origin of the screen is top left
#define NUM_AUTO 5
hotspotType autoHotspots[NUM_AUTO] = {
  {"   None   ", (FIELD_GRAPHIC_HEIGHT / 2) - 25, (FIELD_GRAPHIC_WIDTH / 2) - 25, 50, 50},
  {" Red Big  ", 0, 0, 50, 50},
  {"Red Small ", 0, FIELD_GRAPHIC_WIDTH - 50, 50, 50},
  {" Blue Big ", FIELD_GRAPHIC_HEIGHT - 50, 0, 50, 50},
  {"Blue Small", FIELD_GRAPHIC_HEIGHT - 50, FIELD_GRAPHIC_WIDTH - 50, 50, 50},
};

// Which autonomous was selected (defaults to "none")
int autoSelect = 0;

// Button pressed (or not) math
bool isPressed(int idx, int X, int Y)
{
  if ((X >= autoHotspots[idx].x) && (X <= autoHotspots[idx].x + autoHotspots[idx].width) &&
      (Y >= autoHotspots[idx].y) && (Y <= autoHotspots[idx].y + autoHotspots[idx].width)) {
    return true;
  } else {
    return false;
  }
}

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
  int fingerX;
  int fingerY;
  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...

  // "Accept" button
  Brain.Screen.drawCircle(55, FIELD_LOC_Y + (FIELD_GRAPHIC_HEIGHT / 2), 50, color::green);

  // Set the screen origin to where we want the field grapic. Makes the math easier.
  Brain.Screen.setOrigin(FIELD_LOC_X, FIELD_LOC_Y);

  Brain.Screen.drawImageFromBuffer(field_250_png, 0, 0, field_250_png_len);
  // Draw squares for the buttons
  for (int j = 0; j < NUM_AUTO; j++) {
    Brain.Screen.drawRectangle(autoHotspots[j].x, autoHotspots[j].y,
                               autoHotspots[j].width, autoHotspots[j].height,
                               color::transparent);
  }

  while (true) {
    if (Brain.Screen.pressing()) {
      fingerX = Brain.Screen.xPosition();
      fingerY = Brain.Screen.yPosition();
      // Press to the left of the field graphic to accept
      if (fingerX < -10) {
        break;
      }
      for (int j = 0; j < NUM_AUTO; j++) {
        if (isPressed(j, fingerX, fingerY)) {
          autoSelect = j;
          // Need to reset the origin to print the label under the green button
          Brain.Screen.setOrigin(0, 0);
          Brain.Screen.setCursor(11, 1);
          Brain.Screen.print(autoHotspots[j].label);
          Brain.Screen.setOrigin(FIELD_LOC_X, FIELD_LOC_Y);
        }
      }
    }
    wait(20, msec);
  }

  // Redraw with only the selected routine
  Brain.Screen.drawImageFromBuffer(field_250_png, 0, 0, field_250_png_len);
  Brain.Screen.drawRectangle(autoHotspots[autoSelect].x, autoHotspots[autoSelect].y,
                              autoHotspots[autoSelect].width, autoHotspots[autoSelect].height,
                              color::transparent);

  Brain.Screen.setOrigin(0, 0);
  // Clear the temporary label
  Brain.Screen.setCursor(11, 1);
  Brain.Screen.print("          ");
  // Print the selected autonomous on the button
  Brain.Screen.setCursor(2, 1);
  Brain.Screen.print(autoHotspots[autoSelect].label);
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

  Drivetrain.setDriveVelocity(50, velocityUnits::pct);

  if (autoSelect == 1) {
    Drivetrain.arcade(1.0, 0.5); // Drive in an arc
    Drivetrain.driveFor(24.0 * 3.0, inches);
    Drivetrain.arcade(1.0, 0.0); // Drive straight
    Drivetrain.driveFor(24.0 * -1.0, inches); // One tile back
  }
  if (autoSelect == 2) {
    Drivetrain.driveFor(24.0 *  2.0, inches); // Two tiles forward
    Drivetrain.driveFor(24.0 * -1.0, inches); // One tile back
  }
  if (autoSelect == 3) {
    Drivetrain.arcade(1.0, -0.5); // Drive in an arc
    Drivetrain.driveFor(24.0 * 3.0, inches);
    Drivetrain.arcade(1.0, 0.0); // Drive straight
    Drivetrain.driveFor(24.0 * -1.0, inches); // One tile back
  }
  if (autoSelect == 4) {
    Drivetrain.driveFor(24.0 *  2.0, inches); // Two tiles forward
    Drivetrain.driveFor(24.0 * -1.0, inches); // One tile back
  }
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
