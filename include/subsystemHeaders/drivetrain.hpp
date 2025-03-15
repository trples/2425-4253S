//#include "main.h"

extern bool enableDrivePID;
extern bool enableTurnPID;
//extern double accumulatedError;

void resetDriveEncoders();

double getAverageEncoderVal();

void updateMotors(double left, double right);

void setDrive(); // use controller input

// AUTON
int drivePID(int goal);

int rotatePID(int deg);

//void translate(double distance, int direction); 

void translate(double distance, int direction, int maxPower = 60, int breakoutTime = 2000); 

void rotate(double deg, int direction);

void slowTranslate(double distance, int direction, int maxPower = 50, int breakoutTime = 2000);

void resetPosition();

void resetRotation();

void test();

void skills();

//
void redLeftAWP();

void redLeftRingRush();

void redLeftDoubleStake();

void redLeftAB();

//
void redRightAWP();

void redRightLine();

void redRightAlliance();

void redRightGoalSlow();

void redRightAB();

//
void blueLeftAWP();

void blueLeftLine();

void blueLeftAlliance();

void blueLeftGoalSlow();

void blueLeftAB();

//
void blueRightAWP();

void blueRightRingRush(); 

void blueRightDoubleStake();

void blueRightAB();