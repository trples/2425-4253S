#include "main.h"

extern bool enableDrivePID;
extern bool enableTurnPID;

void resetDriveEncoders();

double getAverageEncoderVal();

void updateMotors(double left, double right);

void setDrive(); // use controller input

int drivePID(int goal);

int rotatePID(int deg);

void translate(double distance, int direction); 

void rotate(double deg, int direction);

