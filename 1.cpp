int x_motor = motorA;
int y_motor1 = motorB;
int y_motor2 = motorC;
int claw_y = motorD;
int claw_x = motorE;
int claw_z = motorF;

const double WHEEL_RADI = 2.75;

void moveArm(int x, int y) 
{
	const int SQUARE_SIDE = 3;
	const double DIST_PER_ROTATION = WHEEL_RADI*PI*2;
	
	motor[x_motor] = 20;
	while(nMotorEncoder(x_motor)*DIST_PER_ROTATION < x*SQUARE_SIDE) {}
	motor[x_motor] = 0;
	
	motor[y_motor1] = motor[y_motor2] = 20;
	while(nMotorEncoder(y_motor1)*DIST_PER_ROTATION < y*SQUARE_SIDE) {}
	motor[y_motor1] = motor[y_motor2] = 0;
}

void closeOpenClaw(int piece)
{
	int sign = 0;
	
	const double CLAW_WHEEL = 1;
	const double DIST_PER_ROTATION = CLAW_WHEEL*PI*2;
	const double EXTEND_DIST = 1.5;
	const double CLAW_TOL = 0.05;
	
	if(nMotorEncoder(claw_y) > 0 + TOL)
		sign = -1;
		
	motor[claw_y] = 10*sign;
	while(nMotorEncoder*sign*DIST_PER_ROTATION < piece+EXTEND_DIST+CLAW_TOL) {}
	motor[claw_y] = 0;
}

void raiseLowerClaw()
{
	int sign = 0;
	
	const double CLAWZ_WHEEL = 1;
	const double DIST_PER_ROTATION = CLAWZ_WHEEL*PI*2;
	const double EXTEND_DIST = 1.5;
	const double CLAWZ_TOL = 0.05;
	
	if(nMotorEncoder(claw_z) > 0)
		sign = -1;
	
	while(nMotorEncoder(claw_z)*sign*DIST_PER_ROTATION < EXTEND_DIST) {}
	
}

task main()
{	
	
}
