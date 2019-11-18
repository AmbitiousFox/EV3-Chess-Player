#include "mindsensors-motormux.h"
#include "PC_FileIO.c"

//REMEMBER: we need to cite our open source code, but since it is used for
//giving files, we probably dont need to submit open source code, ask carol

/*
 * CHESS BOARD LAYOUT
 *
 * 	The chess board follows a predetermined coordinate system:
 *
 *  x = extra piece tile
 *  + = chess board tile
 *
 *
 * 	7	x	x	x		+	+	+	+	+	+	 +	 +	 		 x	 x	 x
 * 	6	x	x	x		+	+	+	+	+	+	 +	 +	 		 x	 x	 x
 * 	5	x	x	x		+	+	+	+	+	+	 +	 +	 		 x	 x	 x
 * 	4	x	x	x		+	+	+	+	+	+	 +	 +	 		 x	 x	 x
 * 	3	x	x	x		+	+	+	+	+	+	 +	 +	 		 x	 x	 x
 * 	2	x	x	x		+	+	+	+	+	+	 +	 +	 		 x	 x	 x
 * 	1	x	x	x		+	+	+	+	+	+	 +	 +	 		 x	 x	 x
 * 	0	x	x	x		+	+	+	+	+	+	 + 	 +	 		 x	 x	 x
 *
 * 		0	1	2	3	4	5	6	7	8	9	10	11	12	13	14	15
 * 					  a b c d e f  g   h
 */

// it is declared that an empty spot on the chess board is represented by null values

// files
string INPUT_FILE = "saved_match.txt";
string OUTPUT_FILE = "saved_match.txt";

// chess piece type
struct piece
{
	int piece_type;
	float extend_dist;
	float close_dist;
	bool colour; // true = white, false = black
};

//combined chess board and extra / capture pieces array that matches with coordinate system
piece board[16][8];

// physical system constants
const float WHEEL_RADIUS_X = 3.65;
const float WHEEL_RADIUS_Y = 1.65;
const float TILE_SIDE = 3;
const float EXTEND_DIST_Z = 5.2;
const float EXTEND_DIST_CLAW = 2.5;
const float DIST_PER_ROTATION_X = WHEEL_RADIUS_X*PI*2/5;
const float DIST_PER_ROTATION_Y = WHEEL_RADIUS_Y*PI*2;
const float DIST_PER_ROTATION_Z = 3.6;
const float DIST_PER_ROTATION_CLAW = 0.5;

const float MOTOR_X_SPEED = 60;
const float MOTOR_X_SPEED_NEG = -60;
const float MOTOR_Y_SPEED = 60;
const float MOTOR_Z_SPEED = 40;

const float PAWN_EXTEND_DIST = EXTEND_DIST_Z;
const float PAWN_CLOSE_DIST = 0;
const float ROOK_EXTEND_DIST = EXTEND_DIST_Z;
const float ROOK_CLOSE_DIST = 1.5;
const float KNIGHT_EXTEND_DIST = EXTEND_DIST_Z;
const float KNIGHT_CLOSE_DIST = 0.1;
const float BISHOP_EXTEND_DIST = EXTEND_DIST_Z;
const float BISHOP_CLOSE_DIST = 1.2;
const float QUEEN_EXTEND_DIST = EXTEND_DIST_Z - 1;
const float QUEEN_CLOSE_DIST = 1;
const float KING_EXTEND_DIST = EXTEND_DIST_Z - 1;
const float KING_CLOSE_DIST = 1.5;

// chess pieces
const int PAWN = 0;
const int ROOK = 1;
const int KNIGHT = 2;
const int BISHOP = 3;
const int QUEEN = 4;
const int KING = 5;
const int NULL_PIECE = 6;

// motors
const int x_motor1 = mmotor_S1_1;
const int x_motor2 = mmotor_S1_2;
const int y_motor = motorC;
const int z_motor = motorA;
const int claw_motor = motorD;
const int touch_x = S2;
const int touch_y = S3;


/*
 * Moves the robot in the x direction to column x.
 *
 * CINDY
 */
void move_x(int x, int offset)
{
	if(MSMMotorEncoder(x_motor1)/360.0*DIST_PER_ROTATION_X < x*TILE_SIDE + 1.5 + offset)
	{
		while(MSMMotorEncoder(x_motor1)/360.0*DIST_PER_ROTATION_X < x*TILE_SIDE + 1.5 + offset)
		{
			MSMMotor(x_motor1, MOTOR_X_SPEED);
			MSMMotor(x_motor2, MOTOR_X_SPEED);
			while(MSMMotorEncoder(x_motor1)/360.0*DIST_PER_ROTATION_X < x*TILE_SIDE + 1.5 + offset) {}
			MSMotorStop(x_motor1);
			MSMotorStop(x_motor2);
			wait1Msec(100);
		}
	}
	else
	{
		while(MSMMotorEncoder(x_motor1)/360.0*DIST_PER_ROTATION_X > x*TILE_SIDE + 2 + offset)
		{
			MSMMotor(x_motor1, MOTOR_X_SPEED_NEG);
			MSMMotor(x_motor2, MOTOR_X_SPEED_NEG);
			while(MSMMotorEncoder(x_motor1)/360.0*DIST_PER_ROTATION_X > x*TILE_SIDE + 2 + offset) {}
			MSMotorStop(x_motor1);
			MSMotorStop(x_motor2);
			wait1Msec(100);
		}
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
	if(nMotorEncoder(y_motor)/360.0*DIST_PER_ROTATION_Y > -y*TILE_SIDE - 1.2)
	{
		motor[y_motor] = -MOTOR_Y_SPEED;
		while(nMotorEncoder(y_motor)/360.0*DIST_PER_ROTATION_Y > -y*TILE_SIDE - 0.7) {}
	}
	else
	{
		motor[y_motor] = MOTOR_Y_SPEED;
		while(nMotorEncoder(y_motor)/360.0*DIST_PER_ROTATION_Y < -y*TILE_SIDE - 0.7) {}
	}
	motor[y_motor] = 0;
	wait1Msec(200);
}

/*
 * Calibrates the x direction movement.
 *
 * CINDY
 */
void calibrate_x()
{
	MSMMotor(x_motor1, MOTOR_X_SPEED_NEG);
	MSMMotor(x_motor2, MOTOR_X_SPEED_NEG);
	while (SensorValue[touch_x] == 0) {}
	MSMotorStop(x_motor1);
	MSMotorStop(x_motor2);
	MSMMotorEncoderReset(x_motor1);
	MSMMotorEncoderReset(x_motor2);
	wait1Msec(200);
}

/*
 * Calibrates the y direction movement.
 *
 * CINDY
 */
void calibrate_y()
{
	motor[y_motor] = MOTOR_Y_SPEED;
	while (SensorValue[touch_y] == 0) {}
	motor[y_motor] = 0;
	nMotorEncoder(y_motor) = 0;
	wait1Msec(200);
}

/*
 * Moves the robot to the tile (x,y).
 *
 * CINDY
 */
void moveArm(int x, int y, int offset_x)
{
	move_x(x, offset_x);
	calibrate_y();
	move_y(y);
}

/*
 * Closes the claw.
 *
 * CINDY
 */
void closeClaw(int dist)
{
	motor[claw_motor] = -100;
	while(nMotorEncoder(claw_motor)/360.0*DIST_PER_ROTATION_CLAW > dist) {}
	motor[claw_motor] = 0;
	wait1Msec(200);
}

/*
 * Opens the claw.
 *
 * CINDY
 */
void openClaw()
{
	motor[claw_motor] = 100;
	while(nMotorEncoder(claw_motor)/360.0*DIST_PER_ROTATION_CLAW < EXTEND_DIST_CLAW) {}
	motor[claw_motor] = 0;
	wait1Msec(200);
}

/*
 * Lowers the claw.
 *
 * CINDY
 */
void lowerClaw(int dist)
{
	motor[z_motor] = -MOTOR_Z_SPEED;
	while(nMotorEncoder(z_motor)/360.0*DIST_PER_ROTATION_Z > -dist) {}
	motor[z_motor] = 0;
	wait1Msec(200);
}

/*
 * Raises the claw.
 *
 * CINDY
 */
void raiseClaw()
{
	motor[z_motor] = MOTOR_Z_SPEED;
	while(nMotorEncoder(z_motor)/360.0*DIST_PER_ROTATION_Z < 0) {}

	motor[z_motor] = 0;
	wait1Msec(200);
}

/*
 * Removes a piece from the chess board array.
 *
 * ALEX
 */
void removeFromArray(int x, int y)
{
	piece null_piece;
	null_piece.piece_type = NULL_PIECE;
	board[x][y] = null_piece;
}

/*
 * Moves the chess piece from tile (x_start, y_start) to (x_end, y_end).
 *        Claw expects to be open at the start, and remain open at the end.
 * ALEX
 */ // it is decreed that white pieces are on the right side.
void movePiece(int x_start, int y_start, int x_end, int y_end, piece & currPiece)
{
	calibrate_y();
	move_y(7);
	calibrate_x();
	moveArm(x_start, y_start, 0);
	lowerClaw(currPiece.extend_dist);
	closeClaw(currPiece.close_dist);
	raiseClaw();
	moveArm(x_end, y_end, - 0.8);
	lowerClaw(currPiece.extend_dist);
	openClaw();
	raiseClaw();
	board[x_end][y_end] = currPiece;
	removeFromArray(x_start, y_start);
}

//subfunction of removePiece()
void holdingSpot(piece & currPiece, int & x, int & y,
 	int pawnColumn, int pieceColumn, int queenColumn)
{
	// look through the array of it's type, find the last available location for it
	if(currPiece.piece_type == PAWN)
	{
		for (int row = 0; row <= 7; row++)// search pawnColumn
		{
			if(board[pawnColumn][row].piece_type == NULL_PIECE)
			{
				x = pawnColumn;
				y = row;
			}
		}
	}
	else if(currPiece.piece_type == ROOK)
	{
		if(board[pieceColumn][0].piece_type == NULL_PIECE)
		{
			x = pieceColumn;
			y = 0;
		}
		else
		{
			x = pieceColumn;
			y = 7;
		}
	}
	else if(currPiece.piece_type == KNIGHT)
	{
		if(board[pieceColumn][1].piece_type == NULL_PIECE)
		{
			x = pieceColumn;
			y = 1;
		}
		else
		{
			x = pieceColumn;
			y = 6;
		}
	}
	else if(currPiece.piece_type == BISHOP)
	{
		if(board[pieceColumn][2].piece_type == NULL_PIECE)
		{
			x = pieceColumn;
			y = 2;
		}
		else
		{
			x = pieceColumn;
			y = 5;
		}
	}
	else if(currPiece.piece_type == QUEEN)
	{
		if(board[pieceColumn][3].piece_type == NULL_PIECE)
		{
			x = pieceColumn;
			y = 3;
		}
		else
		{
			for (int row = 0; row <= 7; row++)  // search queenColumn
			{
				if(board[queenColumn][row].piece_type == NULL_PIECE)
				{
					x = queenColumn;
					y = row;
				}
			}
		}
	}
}

/*
 * Removes the chess piece at tile (x, y) and places it at the correct location.
 *
 * ALEX
 */
void removePiece(int x, int y, piece & currPiece)
{
	int endx = 0, endy = 0; // endpoint of piece

	if(currPiece.colour)// find if piece is white
	{
		//search columns 13-15
		holdingSpot(currPiece, endx, endy, 13, 14, 15);
	}
	else
	{
		//search columns 0-2
		holdingSpot(currPiece, endx, endy, 2, 1, 0);
	}
	movePiece(x, y, endx, endy, currPiece);
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
 * Checks whether the player is in check.
 *
 * CINDY
 */
bool check(bool player)
{
	// find location of player's king
	int x_king = 0, y_king = 0;
	for (int x = 4; x <= 11; x++)
	{
		for (int y = 0; y < 8; y++)
		{
			if (board[x][y].piece_type == KING && board[x][y].colour == player)
			{
				x_king = x;
				y_king = y;
			}
		}
	}

	// for each of opponent's pieces, check if that piece can capture the king
	for (int x = 4; x <= 11; x++)
		for (int y = 0; y < 8; y++)
			if (board[x][y].colour != player && moveIsValid(x, y, x_king, y_king, player))
				return true;

	return false;
}

/*
 * Helper function for checkmate algorithm.
 *
 * CINDY
 */
bool movePieceAndCheck(int x_start, int y_start, int x_end, int y_end, bool player)
{
	piece temp;
	temp = board[x_end][y_end];
	piece null_piece;
	null_piece.piece_type = NULL;

	board[x_end][y_end] = board[x_start][y_start];
	board[x_start][y_start] = null_piece;

	if (!check(player))
		return false;

	board[x_start][y_start] = board[x_end][y_end];
	board[x_end][y_end] = temp;

	return true;
}

/*
 * Helper function for checkmate algorithm. (BRUTE FORCE ALGORITHM)
 *
 * CINDY
 */
bool canRelieveCheck(bool player)
{
	// for every possible move the player can make, check if it
	// results in check
	for (int x = 4; x <= 11; x++)
	{
		for (int y = 0; y < 8; y++)
		{
			piece curr_piece;
			curr_piece = board[x][y];

			if (curr_piece.colour == player)
			{
				// PAWN (NEED TO CHECK FOR EN-PASSANT)
				if (curr_piece.piece_type == PAWN)
				{
					// white
					if (player)
					{
						// move
						for (int y_end = y+1; y_end <= y+2; y_end++)
						{
							if (moveIsValid(x, y, x, y_end, player))
								if (!movePieceAndCheck(x, y, x, y_end, player))
									return true;
							else
								break;
						}

						// capture
						if (x > 4 && moveIsValid(x, y, x-1, y+1, player))
							if (!movePieceAndCheck(x, y, x-1, y+1, player))
									return true;
						if (x < 11 && moveIsValid(x, y, x+1, y+1, player))
							if (!movePieceAndCheck(x, y, x+1, y+1, player))
									return true;
					}

					// black
					else
					{
						// move
						for (int y_end = y-1; y_end >= y-2; y_end--)
						{
							if (moveIsValid(x, y, x, y_end, player))
								if (!movePieceAndCheck(x, y, x, y_end, player))
									return true;
							else
								break;
						}

						// capture
						if (x > 4 && moveIsValid(x, y, x-1, y-1, player))
							if (!movePieceAndCheck(x, y, x-1, y-1, player))
									return true;
						if (x < 11 && moveIsValid(x, y, x+1, y-1, player))
							if (!movePieceAndCheck(x, y, x+1, y-1, player))
									return true;
					}
				}

				// ROOK OR QUEEN
				else if (curr_piece.piece_type == ROOK || curr_piece.piece_type == QUEEN)
				{
					// right
					for (int x_end = x+1; x_end < 12; x_end++)
						if (moveIsValid(x, y, x_end, y, player))
							if (!movePieceAndCheck(x, y, x_end, y, player))
								return true;

					// left
					for (int x_end = x-1; x_end >= 4; x_end--)
						if (moveIsValid(x, y, x_end, y, player))
							if (!movePieceAndCheck(x, y, x_end, y, player))
								return true;

					// up
					for (int y_end = y+1; y_end < 12; y_end++)
						if (moveIsValid(x, y, x, y_end, player))
							if (!movePieceAndCheck(x, y, x, y_end, player))
								return true;

					// down
					for (int y_end = y-1; y_end >= 4; y_end--)
						if (moveIsValid(x, y, x, y_end, player))
							if (!movePieceAndCheck(x, y, x, y_end, player))
								return true;
				}

				// KNIGHT
				else if (curr_piece.piece_type == KNIGHT)
				{
					// right one
					if (x < 11 && y < 6 && moveIsValid(x, y, x+1, y+2, player))
						if (!movePieceAndCheck(x, y, x+1, y+2, player))
							return true;
					if (x < 11 && y > 1 && moveIsValid(x, y, x+1, y-2, player))
						if (!movePieceAndCheck(x, y, x+1, y-2, player))
							return true;

					// left one
					if (x > 4 && y < 6 && moveIsValid(x, y, x-1, y+2, player))
						if (!movePieceAndCheck(x, y, x-1, y+2, player))
							return true;
					if (x > 4 && y > 1 && moveIsValid(x, y, x-1, y-2, player))
						if (!movePieceAndCheck(x, y, x-1, y-2, player))
							return true;

					// right two
					if (x < 10 && y < 7 && moveIsValid(x, y, x+2, y+1, player))
						if (!movePieceAndCheck(x, y, x+2, y+1, player))
							return true;
					if (x < 10 && y > 0 && moveIsValid(x, y, x+2, y-1, player))
						if (!movePieceAndCheck(x, y, x+2, y-1, player))
							return true;

					// left two
					if (x > 5 && y < 7 && moveIsValid(x, y, x-2, y+1, player))
						if (!movePieceAndCheck(x, y, x-2, y+1, player))
							return true;
					if (x > 5 && y > 0 && moveIsValid(x, y, x-2, y-1, player))
						if (!movePieceAndCheck(x, y, x-2, y-1, player))
							return true;
				}

				// BISHOP OR QUEEN
				else if (curr_piece.piece_type == BISHOP || curr_piece.piece_type == QUEEN)
				{
					for (int increment = 0; increment < 8; increment++)
					{
						// right
						if (x+increment < 12)
						{
							if (y+increment < 8 && moveIsValid(x, y, x+increment, y+increment, player))
								if (!movePieceAndCheck(x, y, x+increment, y+increment, player))
									return true;

							if (y-increment >= 0 && moveIsValid(x, y, x+increment, y-increment, player))
								if (!movePieceAndCheck(x, y, x+increment, y-increment, player))
									return true;
						}
						// left
						if (x-increment >= 0)
						{
							if (y+increment < 8 && moveIsValid(x, y, x-increment, y+increment, player))
								if (!movePieceAndCheck(x, y, x-increment, y+increment, player))
									return true;

							if (y-increment >= 0 && moveIsValid(x, y, x-increment, y-increment, player))
								if (!movePieceAndCheck(x, y, x-increment, y-increment, player))
									return true;
						}
					}
				}

				// KING (DOES CASTLING NEED TO BE CHECKED?)
				else if (curr_piece.piece_type == KING)
				{
					// up-center
					if (y < 8 && moveIsValid(x, y, x, y+1, player))
						if (!movePieceAndCheck(x, y, x, y+1, player))
							return true;

					// up-right
					if (y < 8 && x < 12 && moveIsValid(x, y, x+1, y+1, player))
						if (!movePieceAndCheck(x, y, x+1, y+1, player))
							return true;

					// up-left
					if (y < 8 && x > 4 && moveIsValid(x, y, x-1, y+1, player))
						if (!movePieceAndCheck(x, y, x-1, y+1, player))
							return true;

					// right
					if (x < 12 && moveIsValid(x, y, x+1, y, player))
						if (!movePieceAndCheck(x, y, x+1, y, player))
							return true;

					// left
					if (x > 4 && moveIsValid(x, y, x-1, y, player))
						if (!movePieceAndCheck(x, y, x-1, y, player))
							return true;

					// down-center
					if (y > 0 && moveIsValid(x, y, x, y-1, player))
						if (!movePieceAndCheck(x, y, x, y-1, player))
							return true;

					// down-right
					if (y > 0 && x < 12 && moveIsValid(x, y, x+1, y-1, player))
						if (!movePieceAndCheck(x, y, x+1, y-1, player))
							return true;

					// down-left
					if (y > 0 && x > 4 && moveIsValid(x, y, x-1, y-1, player))
						if (!movePieceAndCheck(x, y, x-1, y-1, player))
							return true;
				}
			}
		}
	}
	return false;
}

/*
 * Checks whether the player is in check (1), checkmate (2), or stalemate (3).
 * Returns 0 otherwise.
 *
 * CINDY
 */
int checkmate(bool player)
{
	bool relieveCheck = canRelieveCheck(player);

	// check or checkmate
	if (check(player))
	{
		if (relieveCheck)
			return 1;
		else
			return 2;
	}

	// stalemate or nothing
	else
	{
		if (relieveCheck)
			return 0;
		else
			return 3;
	}
}

/*
 * Returns the corresponding x coordinate to a letter.
 * A = 4
 * B = 5
 * C = 6
 * D = 7
 * E = 8
 * F = 9
 * G = 10
 * H = 11
 *
 * CINDY
 */
int x_coord(char letter)
{
	if (letter == 'A')
		return 4;
	else if (letter == 'B')
		return 5;
	else if (letter == 'C')
		return 6;
	else if (letter == 'D')
		return 7;
	else if (letter == 'E')
		return 8;
	else if (letter == 'F')
		return 9;
	else if (letter == 'G')
		return 10;
	else if (letter == 'H')
		return 11;
	else
		return -1;
}

/*
 * Returns the corresponding letter to an x coordinate.
 * A = 4
 * B = 5
 * C = 6
 * D = 7
 * E = 8
 * F = 9
 * G = 10
 * H = 11
 *
 * CINDY
 */
char letter(int x)
{
	if (x == 4)
		return 'A';
	else if (x == 5)
		return 'B';
	else if (x == 6)
		return 'C';
	else if (x == 7)
		return 'D';
	else if (x == 8)
		return 'E';
	else if (x == 9)
		return 'F';
	else if (x == 10)
		return 'G';
	else if (x == 11)
		return 'H';
	return '!';
}

/*
 * Records the chess move to a file.
 *
 * HANK
 */
void recordMove(TFileHandle fout, int x_start, int y_start, int x_end, int y_end)
{
	writeLongPC(fout, x_start);
	writeEndlPC(fout);
	writeLongPC(fout, y_start);
	writeEndlPC(fout);
	writeLongPC(fout, x_end);
	writeEndlPC(fout);
	writeLongPC(fout, y_end);
	writeEndlPC(fout);
}

/*
 * Reads a chess move from a file.
 *
 * HANK
 */
void readMove(TFileHandle fin, int & x_start, int & y_start, int & x_end, int & y_end)
{
	readIntPC(fin, x_start);
	readIntPC(fin, y_start);
	readIntPC(fin, x_end);
	readIntPC(fin, y_end);
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
	// file IO
	TFileHandle fin, fout;
	bool fileOkay = openReadPC(fin, INPUT_FILE);
	//bool fileOkay = openWritePC(fout, OUTPUT_FILE);


	// initialize motor multiplexer and sensors
	SensorType[S1] = sensorI2CCustom;
	MSMMUXinit();
	SensorType[touch_x] = sensorEV3_Touch;
	SensorType[touch_y] = sensorEV3_Touch;

	wait1Msec(50);

	// reset motor encoders
	nMotorEncoder(z_motor) = 0;
	nMotorEncoder(claw_motor) = 0;

	// testing
	/*
	piece test;
	test.piece_type = PAWN;
	test.extend_dist = PAWN_EXTEND_DIST;
	test.close_dist = PAWN_CLOSE_DIST;
	test.colour = true;

	openClaw();
	for(int i = 4; i < 12; i++)
	{
		movePiece(i, 6, i, 4, test);
	}
	closeClaw(0);
	*/
	int x_start = 0, y_start = 0, x_end = 1, y_end = 1;
	readMove(fin, x_start, y_start, x_end, y_end);
	displayString(1, "%d %d %d %d", x_start, y_start, x_end, y_end);
	wait1Msec(5000);

	closeFilePC(fin);
	//closeFilePC(fout);
}
