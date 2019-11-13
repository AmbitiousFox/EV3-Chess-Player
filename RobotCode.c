#include "mindsensors-motormux.h"


// chess board (DOES THIS NEED TO BE INITITALIZED????)
int chess_board[8][8];

// extra / captured pieces
int white_extra_pieces[3][8];
int black_extra_pieces[3][8];

// physical system constants
const float WHEEL_RADI = 2.00;
const float TILE_SIDE = 3;

// chess pieces
const int BLACK_PAWN = 0;
const int BLACK_ROOK = 1;
const int BLACK_KNIGHT = 2;
const int BLACK_BISHOP = 3;
const int BLACK_QUEEN = 4;
const int BLACK_KING = 5;
const int WHITE_PAWN = 6;
const int WHITE_ROOK = 7;
const int WHITE_KNIGHT = 8;
const int WHITE_BISHOP = 9;
const int WHITE_QUEEN = 10;
const int WHITE_KING = 11;

// motors (edit these)
const int x_motor1 = mmotor_S3_1;
const int x_motor2 = mmotor_S3_2;
const int y_motor = motorA;
const int z_motor = motorB;
const int claw_motor = motorC;


/*
 * Moves the robot in the x direction to column x.
 */
void move_x(int x)
{
	const float DIST_PER_ROTATION = WHEEL_RADI*PI*2;

	if(MSMMotorEncoder(x_motor1)/360.0*DIST_PER_ROTATION < x*TILE_SIDE)
	{
		MSMMotor(x_motor1, 20);
		MSMMotor(x_motor2, 20);
		while(MSMMotorEncoder(x_motor1)/360.0*DIST_PER_ROTATION < x*TILE_SIDE) {}
		MSMotorStop(x_motor1);
		MSMotorStop(x_motor2);
	}
	else
	{
		MSMMotor(x_motor1, -20);
		MSMMotor(x_motor2, -20);
		while(MSMMotorEncoder(x_motor1)/360.0*DIST_PER_ROTATION > x*TILE_SIDE) {}
		MSMotorStop(x_motor1);
		MSMotorStop(x_motor2);
	}
	wait1Msec(200);
}

/*
 * Moves the robot in the y direction to row y.
 */
void move_y(int y)
{
	const float DIST_PER_ROTATION = WHEEL_RADI*PI*2;

	if(nMotorEncoder(y_motor)/360.0*DIST_PER_ROTATION < y*TILE_SIDE)
	{
		motor[y_motor] = 20;
		while(nMotorEncoder(y_motor)/360.0*DIST_PER_ROTATION < y*TILE_SIDE) {}
		motor[y_motor] = 0;
	}
	else
	{
		motor[y_motor] = -40;
		while(nMotorEncoder(y_motor)/360.0*DIST_PER_ROTATION > y*TILE_SIDE) {}
		motor[y_motor] = 0;
	}
	wait1Msec(200);
}

/*
 * Moves the robot to the tile (x,y).
 */
void moveArm(int x, int y)
{
	move_x(x);
	move_y(y);
}

/*
 * Closes or opens the claw.
 */
void closeOpenClaw(int piece)
{
	const float CLAW_GEAR = 1;
	const float DIST_PER_ROTATION = CLAW_GEAR*PI*2;
	const float EXTEND_DIST = 30;
	//const double CLAW_TOL = 0.05;

	if(nMotorEncoder(claw_motor)/360.0*DIST_PER_ROTATION < EXTEND_DIST)
	{
		motor[claw_motor] = 100;
		while(nMotorEncoder(claw_motor)/360.0*DIST_PER_ROTATION < EXTEND_DIST) {}
	}
	else
	{
		motor[claw_motor] = -100;
		while(nMotorEncoder(claw_motor)/360.0*DIST_PER_ROTATION > 0) {}
	}
	motor[claw_motor] = 0;
	wait1Msec(1000);
}

/*
 * Raises and lowers the claw.
 */
void raiseLowerClaw()
{
	const float Z_GEAR = 1;
	const float DIST_PER_ROTATION = Z_GEAR*PI*2;
	const float EXTEND_DIST = 6;
	//const float CLAWZ_TOL = 0.05;

	if(nMotorEncoder(z_motor)/360.0*DIST_PER_ROTATION < -EXTEND_DIST)
	{
		motor[z_motor] = 40;
		while(nMotorEncoder(z_motor)/360.0*DIST_PER_ROTATION < 0) {}
	}
	else
	{
		motor[z_motor] = -40;
		while(nMotorEncoder(z_motor)/360.0*DIST_PER_ROTATION > -EXTEND_DIST) {}
	}

	motor[z_motor] = 0;
	wait1Msec(500);
}

/*
 * Moves the chess piece from tile (x_start, y_start) to (x_end, y_end).
 */
void movePiece(int x_start, int y_start, int x_end, int y_end)
{
	// TO DO
}

/*
 * Removes the chess piece at tile (x, y).
 */
void removePiece(int x, int y)
{
	// TO DO
}

/*
 * Checks whether the chess move of the piece at (x_start, y_start) to (x_end, y_end)
 * is valid.
 */
bool moveIsValid(int x_start, int y_start, int x_end, int y_end, bool white_turn)
{
	// TO DO
}

/*
 * Checks whether the player is in check (1) or checkmate (2).
 * Returns 0 otherwise. 
 */
int check(bool white)
{
	// TO DO
}

/*
 * Records the chess move to a file.
 */
void recordMove(int x_start, int y_start, int x_end, int y_end)
{
	// TO DO
}

/*
 * Idk how this function works...
 */
void readMoves()
{
	// TO DO
}

/*
 * Idk how this function works...
 */
void gameList()
{
	// TO DO
}

/*
 * Idk how this function works...
 */
void addGame()
{
	// TO DO
}

/*
 * Executes player vs player mode.
 */
void playerVsPlayer()
{
	// TO DO
}

/*
 * Executes player vs AI mode.
 */
void playerVsAI()
{
	// TO DO
}

/*
 * Executes replay saved match mode.
 */
void replaySavedMatch()
{
	// TO DO
}

// main
task main()
{
	// initialize motor multiplexer
	SensorType[S3] = sensorI2CCustom;
	MSMMUXinit();
}
