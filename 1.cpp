int x_motor = motorA;
int y_motor1 = motorB;
int y_motor2 = motorC;
int claw_y = motorD;
int claw_x = motorE;
int claw_z = motorF;

// S2 is the touch sensor in the y-direction.
// S3 is the touch sensor is the x-direction.

// declare the wheel radius as a global variable
const double WHEEL_RADI = 2.75;

//start of the sign function which checks the sign of an integer value
int sign(int check_sign)
{
	if(check_sign > 0)
		return 1;
	else
		return -1;
} // end of sign function

//beginning of calibrateClaw function
void calibrateClaw()
{
	motor[x_motor] = -20;
	while(!SensorValue[S3]) {}
	motor[x_motor] = 0;
	nMotorEncoder(x_motor) = 0;
		
	motor[y_motor1] = motor[y_motor2] = -20;
	while(!SensorValue[S2]) {}
	motor[y_motor1] = motor[y_motor2] = 0;
	nMotorEncoder(y_motor1) = nMotorEncoder(y_motor2) = 0;
 } // end function
 
//beginning of the moveClaw function
void moveClaw(int x, int y) 
{
	const int SQUARE_SIDE = 3;
	const double DIST_PER_ROTATION = WHEEL_RADI*PI*2;
	const double DIST_TOL = 0.10;
	const double ROT_TOL = DIST_TOL / (PI * 2);
	
	int current_x = nMotorEncoder(x_motor)/DIST_PER_ROTATION/SQUARE_SIDE+ROT_TOL;
	int current_y = nMotorEncoder(y_motor1)/DIST_PER_ROTATION/SQUARE_SIDE+ROT_TOL;
	
	int sign_x = sign(current_x-x), sign_y = sign(current_y-y);
	
	motor[x_motor] = sign_x*20;
	while(sign_x*nMotorEncoder(x_motor)*DIST_PER_ROTATION < x*SQUARE_SIDE) {}
	motor[x_motor] = 0;
	
	motor[y_motor1] = motor[y_motor2] = sign_y*20;
	while(sign_y*nMotorEncoder(y_motor1)*DIST_PER_ROTATION < y*SQUARE_SIDE) {}
	motor[y_motor1] = motor[y_motor2] = 0;
}

void closeOpenClaw(int piece)
{
	
	const double CLAW_WHEEL = 1;
	const double DIST_PER_ROTATION = CLAW_WHEEL*PI*2;
	const double EXTEND_DIST = 1.5;
	const double CLAW_TOL = 0.05;
	
	if(nMotorEncoder(claw_y) > 0 + CLAW_TOL)
	{
		motor[claw_y] = -10;
		while(nMotorEncoder*DIST_PER_ROTATION > 0) {}
	}
	else
	{
		motor[claw_y] = 10;
		while(nMotorEncoder*DIST_PER_ROTATION < piece+EXTEND_DIST+CLAW_TOL) {}
	}
	motor[claw_y] = 0;
}

void raiseLowerClaw()
{
	int sign = 0;
	
	const double CLAWZ_WHEEL = 1;
	const double DIST_PER_ROTATION = CLAWZ_WHEEL*PI*2;
	const double EXTEND_DIST = 1.5;
	const double CLAWZ_TOL = 0.05;
	
	if(nMotorEncoder(claw_z) > 0 + CLAWZ_TOL)
	{
		motor[claw_z] = -10;
		while(nMotorEncoder(claw_z)*DIST_PER_ROTATION > EXTEND_DIST) {}
	}
	else
	{
		motor[claw_z] = 10;
		while(nMotorEncoder(claw_z)*DIST_PER_ROTATION < EXTEND_DIST) {}
	}
	
	motor[claw_z] = 0;
}

task main()
{	
	
}
