#include "main.h"

extern bool enableDrivePID;
extern bool enableTurnPID;
//extern double accumulatedError;

void resetDriveEncoders();

double getAverageEncoderVal();

void updateMotors(double left, double right);

void setDrive(); // use controller input

int drivePID(int goal);

int rotatePID(int deg);

void translate(double distance, int direction); 

void rotate(double deg, int direction);

void slowTranslate(double distance, int direction);

void resetPosition();

void resetRotation();

void test();

void skills();

void redRight();

void blueLeft();

void blueRight();

