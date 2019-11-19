#include <cmath>
#include <iostream>
#include <iomanip>

using namespace std;

// chess piece type
struct piece
{
	int piece_type;
	bool colour; // true = white, false = black
};

//combined chess board and extra / capture pieces array that matches with coordinate system
piece board[16][8];

// chess pieces
const int PAWN = 0;
const int ROOK = 1;
const int KNIGHT = 2;
const int BISHOP = 3;
const int QUEEN = 4;
const int KING = 5;
const int NULL_PIECE = 6;

bool check(bool);
bool movePieceAndCheck(int, int, int, int, bool);

void initBoard()
{
    // fill sides
    for (int j = 0; j < 8; j++)
    {
        for (int i = 0; i < 4; i++)
        {
            piece temp;
            temp.piece_type = NULL_PIECE;
            board[i][j] = temp;
        }
        for (int i = 12; i < 16; i++)
        {
            piece temp;
            temp.piece_type = NULL_PIECE;
            board[i][j] = temp;
        }
    }

    // fill middle
    for (int j = 2; j < 6; j++)
    {
        for (int i = 4; i < 12; i++)
        {
            piece temp;
            temp.piece_type = NULL_PIECE;
            board[i][j] = temp;
        }
    }

    // fill pawns
    for (int i = 4; i < 12; i++)
    {
        piece b_pawn, w_pawn;
        b_pawn.piece_type = PAWN;
        b_pawn.colour = false;
        w_pawn.piece_type = PAWN;
        w_pawn.colour = true;

        board[i][1] = w_pawn;
        board[i][6] = b_pawn;
    }

    // rooks
    piece b_rook1, b_rook2, w_rook1, w_rook2;
    b_rook1.piece_type = ROOK;
    b_rook1.colour = false;
    b_rook2.piece_type = ROOK;
    b_rook2.colour = false;
    w_rook1.piece_type = ROOK;
    w_rook1.colour = true;
    w_rook2.piece_type = ROOK;
    w_rook2.colour = true;

    board[4][0] = w_rook1;
    board[11][0] = w_rook2;
    board[4][7] = b_rook1;
    board[11][7] = b_rook2;

    // knights
    piece b_knight1, b_knight2, w_knight1, w_knight2;
    b_knight1.piece_type = KNIGHT;
    b_knight1.colour = false;
    b_knight2.piece_type = KNIGHT;
    b_knight2.colour = false;
    w_knight1.piece_type = KNIGHT;
    w_knight1.colour = true;
    w_knight2.piece_type = KNIGHT;
    w_knight2.colour = true;

    board[5][0] = w_knight1;
    board[10][0] = w_knight2;
    board[5][7] = b_knight1;
    board[10][7] = b_knight2;

    // bishops
    piece b_bishop1, b_bishop2, w_bishop1, w_bishop2;
    b_bishop1.piece_type = BISHOP;
    b_bishop1.colour = false;
    b_bishop2.piece_type = BISHOP;
    b_bishop2.colour = false;
    w_bishop1.piece_type = BISHOP;
    w_bishop1.colour = true;
    w_bishop2.piece_type = BISHOP;
    w_bishop2.colour = true;

    board[6][0] = w_bishop1;
    board[9][0] = w_bishop2;
    board[6][7] = b_bishop1;
    board[9][7] = b_bishop2;

    // queens
    piece b_queen, w_queen;
    b_queen.piece_type = QUEEN;
    b_queen.colour = false;
    w_queen.piece_type = QUEEN;
    w_queen.colour = true;

    board[7][0] = w_queen;
    board[7][7] = b_queen;

    // kings
    piece b_king, w_king;
    b_king.piece_type = KING;
    b_king.colour = false;
    w_king.piece_type = KING;
    w_king.colour = true;

    board[8][0] = w_king;
    board[8][7] = b_king;
}

void printBoard()
{
    for (int j = 7; j >= 0; j--)
    {
        cout << j << "  ";
        for (int i = 0; i < 16; i++)
        {
            piece temp = board[i][j];

            int c = 1;
            if (!temp.colour)
                c = -1;

            cout << fixed << setw(2) << c*board[i][j].piece_type << " ";
        }
        cout << endl;
    }
    cout << endl << "   ";
    for (int i = 0; i < 16; i++)
        cout << fixed << setw(2) << i << " ";
    
    cout << endl << endl;
}

bool moveIsValid(int x_start, int y_start, int x_end, int y_end, bool player, bool checkForCheck)
{
	// check that start and end are valid tiles
	if (x_start < 4 || x_start > 11 || x_end < 4 || x_end > 11 || y_start < 0 || y_start > 7 || y_end < 0 || y_end > 7)
		return false;

	// check that start and end are not the same tile
	if (x_start == x_end && y_start == y_end)
		return false;
	
	piece curr_piece = board[x_start][y_start];

	// check that piece at start tile is owned by the player
	if (curr_piece.piece_type == NULL_PIECE || curr_piece.colour != player)
		return false;

	// check that piece at end tile is not owned by player
	if (board[x_end][y_end].piece_type != NULL_PIECE && board[x_end][y_end].colour == player)
		return false;

	// PAWN (NEED TO CHECK DOUBLE STEP AND EN-PASSANT)
	if (curr_piece.piece_type == PAWN)
	{
		// white
		if (player)
		{
			// move
			if (x_start == x_end && y_end - y_start == 1 && board[x_end][y_end].piece_type == NULL_PIECE)
				return !checkForCheck || !(movePieceAndCheck(x_start, y_start, x_end, y_end, player));
			
			// capture
			else if (abs(x_start - x_end) == 1 && y_end - y_start == 1 && board[x_end][y_end].piece_type != NULL_PIECE && board[x_end][y_end].colour != player)
				return !checkForCheck || !(movePieceAndCheck(x_start, y_start, x_end, y_end, player));
			
			else
				return false;
		}

		// black
		else
		{
			// move
			if (x_start == x_end && y_start - y_end == 1 && board[x_end][y_end].piece_type == NULL_PIECE) 
				return !checkForCheck || !(movePieceAndCheck(x_start, y_start, x_end, y_end, player));
			
			// capture
			else if (abs(x_start - x_end) == 1 && y_start - y_end == 1 && board[x_end][y_end].piece_type != NULL_PIECE && board[x_end][y_end].colour != player)
				return !checkForCheck || !(movePieceAndCheck(x_start, y_start, x_end, y_end, player));
			
			else
				return false;
		}
	}

	// ROOK
	else if (curr_piece.piece_type == ROOK)
	{		
		// vertical
		if (x_start == x_end)
		{
			// up
			if (y_end > y_start)
			{
				for (int row = y_start+1; row < y_end; row++)
					if (board[x_end][row].piece_type != NULL_PIECE)
						return false;
			}

			// down
			else
			{
				for (int row = y_start-1; row > y_end; row--)
					if (board[x_end][row].piece_type != NULL_PIECE)
						return false;
			}	

			return !checkForCheck || !(movePieceAndCheck(x_start, y_start, x_end, y_end, player));		
		}

		// horizontal
		else if (y_start == y_end)
		{
			// left
			if (x_end < x_start)
			{
				for (int col = x_start-1; col > x_end; col--)
					if (board[col][y_end].piece_type != NULL_PIECE)
						return false;
			}

			// right
			else
			{
				for (int col = x_start+1; col < x_end; col++)
					if (board[col][y_end].piece_type != NULL_PIECE)
						return false;
			}
			
			return !checkForCheck || !(movePieceAndCheck(x_start, y_start, x_end, y_end, player));
		}

		return false;
	}

	// KNIGHT
	else if (curr_piece.piece_type == KNIGHT)
	{
		if ((abs(x_end - x_start) == 1 && abs(y_end - y_start) == 2) || (abs(y_end - y_start) == 1 && abs(x_end - x_start) == 2))
			return !checkForCheck || !(movePieceAndCheck(x_start, y_start, x_end, y_end, player));
		return false;
	}

	// BISHOP
	else if (curr_piece.piece_type == BISHOP)
	{
		// check diagonal movement
		if (abs(x_end - x_start) != abs(y_end - y_start))
			return false;

		int right = 1;
		if (x_end < x_start)
			right = -1;

		// up
		if (y_end > y_start)
		{
			for (int increment = 1; increment < y_end - y_start; increment++)
			{
				if (board[x_start + right*increment][y_start+increment].piece_type != NULL_PIECE)
					return false;
			}
		}

		// down
		else
		{
			for (int increment = 1; increment < y_start - y_end; increment++)
			{
				if (board[x_start + right*increment][y_start-increment].piece_type != NULL_PIECE)
					return false;
			}
		}
		
		return !checkForCheck || !(movePieceAndCheck(x_start, y_start, x_end, y_end, player));
	}

	// QUEEN
	else if (curr_piece.piece_type == QUEEN)
	{
		// rook style movement
		
		// vertical
		if (x_start == x_end)
		{
			// up
			if (y_end > y_start)
			{
				for (int row = y_start+1; row < y_end; row++)
					if (board[x_end][row].piece_type != NULL_PIECE)
						return false;
			}

			// down
			else
			{
				for (int row = y_start-1; row > y_end; row--)
					if (board[x_end][row].piece_type != NULL_PIECE)
						return false;
			}	

			return !checkForCheck || !(movePieceAndCheck(x_start, y_start, x_end, y_end, player));		
		}

		// horizontal
		else if (y_start == y_end)
		{
			// left
			if (x_end < x_start)
			{
				for (int col = x_start-1; col > x_end; col--)
					if (board[col][y_end].piece_type != NULL_PIECE)
						return false;
			}

			// right
			else
			{
				for (int col = x_start+1; col < x_end; col++)
					if (board[col][y_end].piece_type != NULL_PIECE)
						return false;
			}
			
			return !checkForCheck || !(movePieceAndCheck(x_start, y_start, x_end, y_end, player));
		}	

		// bishop style movement

		// check diagonal movement
		if (abs(x_end - x_start) != abs(y_end - y_start))
			return false;

		int right = 1;
		if (x_end < x_start)
			right = -1;

		// up
		if (y_end > y_start)
		{
			for (int increment = 1; increment < y_end - y_start; increment++)
			{
				if (board[x_start + right*increment][y_start+increment].piece_type != NULL_PIECE)
					return false;
			}
		}

		// down
		else
		{
			for (int increment = 1; increment < y_start - y_end; increment++)
			{
				if (board[x_start + right*increment][y_start-increment].piece_type != NULL_PIECE)
					return false;
			}
		}
		
		return !checkForCheck || !(movePieceAndCheck(x_start, y_start, x_end, y_end, player));
	}

	// KING (CHECK CASTLING)
	else
	{
		if ((abs(x_end - x_start) == 1 || x_end - x_start == 0) && (abs(y_end - y_start) == 1 || y_end - y_start == 0))
			return !checkForCheck || !(movePieceAndCheck(x_start, y_start, x_end, y_end, player));
		return false;
	}
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
			if (board[x][y].piece_type != NULL_PIECE && board[x][y].colour != player && moveIsValid(x, y, x_king, y_king, !player, false))
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
    bool isCheck = true;

	piece temp;
	temp = board[x_end][y_end];
	piece null_piece;
	null_piece.piece_type = NULL_PIECE;

	board[x_end][y_end] = board[x_start][y_start];
	board[x_start][y_start] = null_piece;

	if (!check(player))
		isCheck = false;

	board[x_start][y_start] = board[x_end][y_end];
	board[x_end][y_end] = temp;

	return isCheck;
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
							if (moveIsValid(x, y, x, y_end, player, true))
								return true;
							else
								break;
						}

						// capture
						if (x > 4 && moveIsValid(x, y, x-1, y+1, player, true))
								return true;
						if (x < 11 && moveIsValid(x, y, x+1, y+1, player, true))
								return true;
					}

					// black
					else
					{
						// move
						for (int y_end = y-1; y_end >= y-2; y_end--)
						{
							if (moveIsValid(x, y, x, y_end, player, true))
								return true;
							else
								break;
						}

						// capture
						if (x > 4 && moveIsValid(x, y, x-1, y-1, player, true))
								return true;
						if (x < 11 && moveIsValid(x, y, x+1, y-1, player, true))
								return true;
					}
				}

				// ROOK OR QUEEN
				else if (curr_piece.piece_type == ROOK || curr_piece.piece_type == QUEEN)
				{
					// right
					for (int x_end = x+1; x_end < 12; x_end++)
						if (moveIsValid(x, y, x_end, y, player, true))
							return true;

					// left
					for (int x_end = x-1; x_end >= 4; x_end--)
						if (moveIsValid(x, y, x_end, y, player, true))
							return true;

					// up
					for (int y_end = y+1; y_end < 12; y_end++)
						if (moveIsValid(x, y, x, y_end, player, true))
							return true;

					// down
					for (int y_end = y-1; y_end >= 4; y_end--)
						if (moveIsValid(x, y, x, y_end, player, true))
							return true;
				}

				// KNIGHT
				else if (curr_piece.piece_type == KNIGHT)
				{
					// right one
					if (x < 11 && y < 6 && moveIsValid(x, y, x+1, y+2, player, true))
						return true;
					if (x < 11 && y > 1 && moveIsValid(x, y, x+1, y-2, player, true))
						return true;

					// left one
					if (x > 4 && y < 6 && moveIsValid(x, y, x-1, y+2, player, true))
						return true;
					if (x > 4 && y > 1 && moveIsValid(x, y, x-1, y-2, player, true))
						return true;

					// right two
					if (x < 10 && y < 7 && moveIsValid(x, y, x+2, y+1, player, true))
						return true;
					if (x < 10 && y > 0 && moveIsValid(x, y, x+2, y-1, player, true))
						return true;

					// left two
					if (x > 5 && y < 7 && moveIsValid(x, y, x-2, y+1, player, true))
						return true;
					if (x > 5 && y > 0 && moveIsValid(x, y, x-2, y-1, player, true))
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
							if (y+increment < 8 && moveIsValid(x, y, x+increment, y+increment, player, true))
								return true;

							if (y-increment >= 0 && moveIsValid(x, y, x+increment, y-increment, player, true))
								return true;
						}
						// left
						if (x-increment >= 0)
						{
							if (y+increment < 8 && moveIsValid(x, y, x-increment, y+increment, player, true))
								return true;

							if (y-increment >= 0 && moveIsValid(x, y, x-increment, y-increment, player, true))
								return true;
						}
					}
				}

				// KING (DOES CASTLING NEED TO BE CHECKED?)
				else if (curr_piece.piece_type == KING)
				{
					// up-center
					if (y < 8 && moveIsValid(x, y, x, y+1, player, true))
						return true;

					// up-right
					if (y < 8 && x < 12 && moveIsValid(x, y, x+1, y+1, player, true))
						return true;

					// up-left
					if (y < 8 && x > 4 && moveIsValid(x, y, x-1, y+1, player, true))
						return true;

					// right
					if (x < 12 && moveIsValid(x, y, x+1, y, player, true))
						return true;

					// left
					if (x > 4 && moveIsValid(x, y, x-1, y, player, true))
						return true;

					// down-center
					if (y > 0 && moveIsValid(x, y, x, y-1, player, true))
						return true;

					// down-right
					if (y > 0 && x < 12 && moveIsValid(x, y, x+1, y-1, player, true))
						return true;

					// down-left
					if (y > 0 && x > 4 && moveIsValid(x, y, x-1, y-1, player, true))
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

void movePiece(int x_start, int y_start, int x_end, int y_end)
{
    piece temp;
    temp.piece_type = NULL_PIECE;

    board[x_end][y_end] = board[x_start][y_start];
    board[x_start][y_start] = temp;
}


int main()
{
    initBoard();
    
    bool player = true;
    while (true)
    {
        printBoard();
        bool valid = false;
        int x1, y1, x2, y2;
        do {
            cout << "Enter move: ";
            cin >> x1 >> y1 >> x2 >> y2;
            valid = moveIsValid(x1, y1, x2, y2, player, true);
        } while (!valid);

        cout << "VALID MOVE!" << endl;
        movePiece(x1, y1, x2, y2);
        
        player = !player;
        int status = checkmate(player);
        if (status == 1)
            cout << "CHECK" << endl;
        else if (status == 2)
        {
            cout << "CHECKMATE" << endl;
            break;
        }
        else if (status == 3)
        {
            cout << "STALEMATE" << endl;
            break;
        }
    }
}
