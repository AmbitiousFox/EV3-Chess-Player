int x_motor = motorA;
int y_motor1 = motorB;
int y_motor2 = motorC;

const double WHEEL_RADI = 2.75;

void moveArm(int x, int y) 
{
	// assume that the side length of a square
	const int SQUARE_SIDE = 2;
	const double DIST_PER_ROTATION = WHEEL_RADI*PI*2;
	
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
