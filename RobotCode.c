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

void invalidInputMessage()
{
	displayString(3, "INVALID MOVE, PLEASE TRY AGAIN.")
	displayString(5, "Press any button..")
	while(!getButtonPress(buttonAny)) {}
	displayString(3, "");
	displayString(5, "");
}

/*
 * Checks whether the chess move of the piece at (x_start, y_start) to (x_end, y_end)
 * is valid.
 * 
 * AARON
 */
bool isMoveValid(int x_start, int y_start, int x_end, int y_end, bool white_turn)
{
	// TO DO
}

/* 
 * Checks whether the player is in check (1), checkmate (2), or stalemate (3).
 * Returns 0 otherwise.
 *
 * Params: teams 
 * 
 * AARON
 */
int check(int team)
{
	for()
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
 *
 *
 *  Alex
 *
 */
void setBoard()
{
	
}

void resetBoard()
{
}

void displaySelection() 
{
	displayString(1, "Please Enter A Selection Using the Right and Left Arrow Keys");
	displayString(4, "1. 2 Player");
	displayString(5, "2. Player Versus AI");
	displayString(6, "3. Replay Game");
	displayString(7, "4. Exit Game");
}

void squareSelect(int & x_value, int & y_value)
{
	displayString(5, "Select a Square by using the right and left arrows to"
	+ " change the value, and the up and down button to change the value being "
	"changed");
		
	displayString(7, "%c%d", (x+'A'), y+1);
	while(!getButtonPress(buttonEnter)){
		if(getButtonPress(buttonUp) || getButtonPress(buttonDown))
			y_selection = !y_selection;
		if(getButtonPress(buttonRight))
		{
			while(getButtonPress(buttonRight)) {}
			if(y_selection)
				++y_value %= 8;
			else
				++x_value %= 8;
		}
		if(getButtonPress(buttonLeft))
		{
			while(getButtonPress(buttonLeft)) {}
			if(y_selection)
				y_value = (y_value-1+8)%8;
			else
				x_value = (x_value-1+8)%8;
		}
	}
}

void makeMove(int team)
{
	if(team)
		displayString(1, "Player 1's Move");
	else
		displayString(1, "Player 2's Move");
		
	int init_x = 0, init_y = 0, final_x = 0, final_y = 0;
	
	do
	{
		
		while(getTeam(init_x, init_y) != team)
		{
			displayString(3, "Please enter your initial move.");
			squareSelect(init_x, init_y);
			displayString(3, "");
			
			if(getTeam(init_x, init_y) != team)
				invalidInputMessage();
		}
		
		while(!isMoveValid(init_x, init_y, final_x, final_y, team))
		{
			displayString(3, "Please enter where you want to move this piece");
			displayString(4, "If you want to go back, press the same square");
			squareSelect(final_x, final_y);
			displayString(3, "");
			displayString(4, "");
			
			if(!isMoveValid(init_x, init_y, final_x, final_y, team))
				invalidInputMessage();
		}
	}while(getTeam(init_x, init_y) != team && !isMoveValid(init_x, init_y, final_x, final_y, team) && (init_x != final_x || init_y != final_y));
	
	movePiece(init_x, init_y, final_x, final_y);
	
	displayString(1, "");
}

void clearDisplay()
{
	for(int counter  = 0; counter < 10; counter++)
		displayString(counter, "");
}

/*
 * Executes player vs player mode.
 * 
 * AARON
 */
void playerVsPlayer()
{
	bool run = true;
	while(run) 
	{
		makeMove(1);
		if(checkmate() == 1 || checkmate() == 2)
			run = false;
		if(run)
		makeMove(1);
		if(checkmate() == 1 || checkmate() == 2)
			run = false;
	}
	displayWinMessage();
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
