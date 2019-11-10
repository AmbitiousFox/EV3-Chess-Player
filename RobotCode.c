
const float WHEEL_RADI = 2.00;

void move_y(int y)
{
	const int y_motor1 = motorA;
	const int y_motor2 = motorB;

	const float SQUARE_SIDE = 3;
	const float DIST_PER_ROTATION = WHEEL_RADI*PI*2;

	if(nMotorEncoder(y_motor1)/360.0*DIST_PER_ROTATION < y*SQUARE_SIDE)
	{
		motor[y_motor1] = motor[y_motor2] = 20;
		while(nMotorEncoder(y_motor1)/360.0*DIST_PER_ROTATION < y*SQUARE_SIDE) {}
		motor[y_motor1] = motor[y_motor2] = 0;
	}
	else
	{
		motor[y_motor1] = motor[y_motor2] = -20;
		while(nMotorEncoder(y_motor1)/360.0*DIST_PER_ROTATION > y*SQUARE_SIDE) {}
		motor[y_motor1] = motor[y_motor2] = 0;
	}
	wait1Msec(200);
}

void move_x(int x)
{
	const int x_motor = motorA;

	const int SQUARE_SIDE = 3;
	const float DIST_PER_ROTATION = WHEEL_RADI*PI*2;

	if(nMotorEncoder(x_motor)/360.0*DIST_PER_ROTATION < x*SQUARE_SIDE)
	{
		motor[x_motor] = 20;
		while(nMotorEncoder(x_motor)/360.0*DIST_PER_ROTATION < x*SQUARE_SIDE) {}
		motor[x_motor] = 0;
	}
	else
	{
		motor[x_motor] = -40;
		while(nMotorEncoder(x_motor)/360.0*DIST_PER_ROTATION > x*SQUARE_SIDE) {}
		motor[x_motor] = 0;
	}
	wait1Msec(200);
}

void moveArm(int x, int y)
{
	move_y(y);
	move_x(x);
}


void closeOpenClaw(int piece)
{
	const int claw_y = motorB;

	const double CLAW_WHEEL = 1;
	const double DIST_PER_ROTATION = CLAW_WHEEL*PI*2;
	const double EXTEND_DIST = 30;
	//const double CLAW_TOL = 0.05;

	if(nMotorEncoder(claw_y)/360.0*DIST_PER_ROTATION < EXTEND_DIST)
	{
		motor[claw_y] = 100;
		while(nMotorEncoder(claw_y)/360.0*DIST_PER_ROTATION < EXTEND_DIST) {}
	}
	else
	{
		motor[claw_y] = -100;
		while(nMotorEncoder(claw_y)/360.0*DIST_PER_ROTATION > 0) {}
	}
	motor[claw_y] = 0;
	wait1Msec(1000);
}

void raiseLowerClaw()
{
	const int claw_z = motorA;

	const float CLAWZ_WHEEL = 1;
	const float DIST_PER_ROTATION = CLAWZ_WHEEL*PI*2;
	const float EXTEND_DIST = 6;
	//const float CLAWZ_TOL = 0.05;

	if(nMotorEncoder(claw_z)/360.0*DIST_PER_ROTATION < -EXTEND_DIST)
	{
		motor[claw_z] = 40;
		while(nMotorEncoder(claw_z)/360.0*DIST_PER_ROTATION < 0) {}
	}
	else
	{
		motor[claw_z] = -40;
		while(nMotorEncoder(claw_z)/360.0*DIST_PER_ROTATION > -EXTEND_DIST) {}
	}

	motor[claw_z] = 0;
	wait1Msec(500);
}

task main()
{
	//nMotorEncoder(motorA) = nMotorEncoder(motorB) = nMotorEncoder(motorC) = nMotorEncoder(motorD) = 0;
	closeOpenClaw(1);
	closeOpenClaw(1);
}
