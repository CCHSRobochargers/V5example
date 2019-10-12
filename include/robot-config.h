using namespace vex;

extern brain Brain;

// VEXcode devices
extern motor LeftDriveFront;
extern motor LeftDriveRear;
extern motor RightDriveRear;
extern motor RightDriveFront;
extern controller Controller1;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Text.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );