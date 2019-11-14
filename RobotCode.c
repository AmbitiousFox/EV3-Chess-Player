#include "mindsensors-motormux.h"


// chess piece type
struct piece
{
	int piece_type;
	float extend_dist;
	bool colour; // true = white, false = black
};

// chess board
piece chess_board[8][8];

// extra / captured pieces
piece white_extra_pieces[3][8];
piece black_extra_pieces[3][8];

// physical system constants
const float WHEEL_RADI = 2.00;
const float TILE_SIDE = 3;
const float EXTEND_DIST_Z = 5.2;
const float DIST_PER_ROTATION_X = WHEEL_RADI*PI*2;
const float DIST_PER_ROTATION_Y = WHEEL_RADI*PI*2;
const float DIST_PER_ROTATION_Z = 3.6;

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

// motors
const int x_motor1 = mmotor_S3_1;
const int x_motor2 = mmotor_S3_2;
const int y_motor = motorC;
const int z_motor = motorA;
const int claw_motor = motorB;


/*
 * Moves the robot in the x direction to column x.
 * 
 * CINDY
 */
void move_x(int x)
{
	if(MSMMotorEncoder(x_motor1)/360.0*DIST_PER_ROTATION_X < x*TILE_SIDE)
	{
		MSMMotor(x_motor1, 20);
		MSMMotor(x_motor2, 20);
		while(MSMMotorEncoder(x_motor1)/360.0*DIST_PER_ROTATION_X < x*TILE_SIDE) {}
		MSMotorStop(x_motor1);
		MSMotorStop(x_motor2);
	}
	else
	{
		MSMMotor(x_motor1, -20);
		MSMMotor(x_motor2, -20);
		while(MSMMotorEncoder(x_motor1)/360.0*DIST_PER_ROTATION_X > x*TILE_SIDE) {}
		MSMotorStop(x_motor1);
		MSMotorStop(x_motor2);
	}
	wait1Msec(200);
}

/*
 * Moves the robot in the y direction to row y.
 * 
 * CINDY
 */
void move_y(int y)
{
	if(nMotorEncoder(y_motor)/360.0*DIST_PER_ROTATION_Y < y*TILE_SIDE)
	{
		motor[y_motor] = 20;
		while(nMotorEncoder(y_motor)/360.0*DIST_PER_ROTATION_Y < y*TILE_SIDE) {}
		motor[y_motor] = 0;
	}
	else
	{
		motor[y_motor] = -40;
		while(nMotorEncoder(y_motor)/360.0*DIST_PER_ROTATION_Y > y*TILE_SIDE) {}
		motor[y_motor] = 0;
	}
	wait1Msec(200);
}

/*
 * Moves the robot to the tile (x,y).
 * 
 * CINDY
 */
void moveArm(int x, int y)
{
	move_x(x);
	move_y(y);
}

/*
 * Closes or opens the claw.
 * 
 * CINDY
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
 * Lowers the claw.
 * 
 * CINDY
 */
void lowerClaw()
{
	motor[z_motor] = -40;
	while(nMotorEncoder(z_motor)/360.0*DIST_PER_ROTATION_Z > -EXTEND_DIST_Z) {}

	motor[z_motor] = 0;
	wait1Msec(500);
}

/*
 * Raises the claw.
 * 
 * CINDY
 */
void raiseClaw()
{
	motor[z_motor] = 40;
	while(nMotorEncoder(z_motor)/360.0*DIST_PER_ROTATION_Z < 0) {}

	motor[z_motor] = 0;
	wait1Msec(500);
}

/*
 * Moves the chess piece from tile (x_start, y_start) to (x_end, y_end).
 * 
 * ALEX
 */
void movePiece(int x_start, int y_start, int x_end, int y_end, int piece_type)
{
	// TO DO
}

/*
 * Removes the chess piece at tile (x, y) and places it at the correct location.
 * 
 * ALEX
 */
void removePiece(int x, int y, int piece_type)
{
	// TO DO
}

/*
 * Checks whether the chess move of the piece at (x_start, y_start) to (x_end, y_end)
 * is valid.
 * 
 * AARON
 */
bool moveIsValid(int x_start, int y_start, int x_end, int y_end, bool white_turn)
{
	// TO DO
}

/*
 * Checks whether the player is in check (1), checkmate (2), or stalemate (3).
 * Returns 0 otherwise.
 * 
 * AARON
 */
int check(bool white)
{
	// TO DO
}

/*
 * Returns the corresponding number to a letter.
 * A = 0
 * B = 1
 * C = 2
 * D = 3
 * E = 4
 * F = 5
 * G = 6
 * H = 7
 * 
 * CINDY
 */
int num(char letter)
{
	if (letter == 'A')
		return 0;
	else if (letter == 'B')
		return 1;
	else if (letter == 'C')
		return 2;
	else if (letter == 'D')
		return 3;
	else if (letter == 'E')
		return 4;
	else if (letter == 'F')
		return 5;
	else if (letter == 'G')
		return 6;
	else if (letter == 'H')
		return 7;
	else
		return -1;
}

/*
 * Records the chess move to a file.
 * 
 * HANK
 */
void recordMove(int x_start, int y_start, int x_end, int y_end)
{
	// TO DO
}

/*
 * Idk how this function works...
 * 
 * HANK
 */
void readMoves()
{
	// TO DO
}

/*
 * Idk how this function works...
 * 
 * HANK
 */
void gameList()
{
	// TO DO
}

/*
 * Idk how this function works...
 * 
 * HANK
 */
void addGame()
{
	// TO DO
}

/*
 * Executes player vs player mode.
 * 
 * AARON
 */
void playerVsPlayer()
{
	// TO DO
}

/*
 * Executes player vs AI mode.
 * 
 * HANK
 */
void playerVsAI()
{
	// TO DO
}

/*
 * Executes replay saved match mode.
 * 
 * AARON
 */
void replaySavedMatch()
{
	// TO DO
}


// currently for testing purposes
task main()
{
	// initialize motor multiplexer
	SensorType[S3] = sensorI2CCustom;
	MSMMUXinit();

	wait1Msec(50);

	// reset motor encoders
	nMotorEncoder(z_motor) = 0;

	// testing
	lowerClaw();
	wait1Msec(200);
	raiseClaw();
	wait1Msec(200);
}
