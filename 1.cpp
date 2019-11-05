int x_motor = motorA;
int y_motor1 = motorB;
int y_motor2 = motorC;

// S2 is the touch sensor in the y-direction.
// S3 is the touch sensor is the x-direction.

// declare the wheel radius as a global variable
const double WHEEL_RADI = 2.75;

//beginning of calibrateClaw function
void calibrateClaw()
{
	motor[x_motor] = -20;
	while(!SensorValue[S3]) {}
	motor[x_motor] = 0;
	nMotorEncoder(x_motor) = 0;
		
	motor[y_motor1] = -20;
	motor[y_motor2] = -20;
	while(!SensorValue[S2]) {}
	motor[y_motor1] = motor[y_motor2] = 0;
	nMotorEncoder(y_motor1) = nMotorEncoder(y_motor2) = 0;
 } // end function
 

void moveArm(int x, int y) 
{
	// assume that the side length of a square is 2
	// set the tolerance for the squares as 0.1 cm
	const int SQUARE_SIDE = 2;
	const double DIST_PER_ROTATION = WHEEL_RADI*PI*2;
	const double DIST_TOL = 0.10;
	const double ROT_TOL = DIST_TOL / (PI * 2);
	
	motor[x_motor] = 20;
	while(nMotorEncoder(x_motor)*DIST_PER_ROTATION < x*SQUARE_SIDE) {}
	motor[x_motor] = 0;
	
	motor[y_motor1] = motor[y_motor2] = 20;
	while(nMotorEncoder(y_motor1)*DIST_PER_ROTATION < y*SQUARE_SIDE) {}
	motor[y_motor1] = motor[y_motor2] = 0;
	
}

task main()
{	
	
}
