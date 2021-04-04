/***********************
* Yoav Berger
* 
* Group 01
* ass4
**********************/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <assert.h>

#define SIZE 8

// FEN separator for strtok()
const char SEP[] = "/";

// Board characters
const char EMPTY = ' ';

// board's first coloumn
const char FIRST_COL = 'a';

// PGN characters
const char PAWN = 'P';
const char ROOK = 'R';
const char KNIGHT = 'N';
const char BISHOP = 'B';
const char QUEEN = 'Q';
const char KING = 'K';
const char CAPTURE = 'x';
const char PROMOTION = '=';
const char CHECK = '+';
const char MATE = '#';

/******************
* Function Name: printColumns
* Input: null
* Output: void
* Function Operation: prints row with alphabetic indexes and spaces in the amount of varible
******************/

void printColumns() {
	char column = toupper(FIRST_COL);
	printf("* |");
	for (int i = 0; i < SIZE; i++) {
		if (i) {
			printf(" ");
		}
		printf("%c", column);
		column++;
	}
	printf("| *\n");
}

/******************
* Function Name: printSpacers
* Input: null
* Output: void
* Function Operation: prints row starting with *,
					  continuing with -- in the amount of varible and ending with *
******************/
void printSpacers()
{
	printf("* -");
	for (int i = 0; i < SIZE; i++) {
		printf("--");
	}
	printf(" *\n");
}

/******************
* Function Name: toDigit
* Input: char c
* Output: c - '0'
* Function Operation: convert char to digit
******************/
int toDigit(char c) {
	assert('0' <= c && c <= '9');
	return c - '0';
}

/******************
* Function Name: createBoard
* Input: char board[][SIZE], char fen[]
* Output: void
* Function Operation: insert string array into 2D array according to FEN rules
******************/
void createBoard(char board[][SIZE], char fen[])
{
	//strtok on the input
	char* fenPtr = strtok(fen, SEP);
	int spaces = 0;
	//iterating threw the 2D array
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			//in case it a number of spaces
			if (*fenPtr >= '0' && *fenPtr <= '9')
			{
				spaces = toDigit(*fenPtr);
				for (int k = 0; k < spaces && j < SIZE; k++)
				{
					board[i][j] = EMPTY;
					j++;
				}
				//need to set one back because we increasd him
				j--;
			}
			//regular cases
			else
			{
				board[i][j] = *fenPtr;
			}
			//moving to the next char
			fenPtr++;
		}
		//moving to the next /
		fenPtr = strtok(NULL, SEP);
	}

}

/******************
* Function Name: printBoard
* Input: char board[][SIZE]
* Output: void
* Function Operation: takes 2D array and print it in chess board style
******************/
void printBoard(char board[][SIZE])
{
	printColumns();
	printSpacers();
	for (int i = 0; i < SIZE; i++)
	{
		printf("%d |", SIZE - i);
		for (int j = 0; j < SIZE; j++)
		{
			printf("%c|", board[i][j]);
		}
		printf(" %d\n", SIZE - i);
	}
	printSpacers();
	printColumns();
}

//***************************mission 2*******************

// Move logical representation
typedef struct {
	char srcPiece, srcRow, srcCol, destPiece, destRow, destCol, promotionPiece;
	int iSrc, jSrc, iDest, jDest;
	int isWhite, isCapture, isPromotion, isCheck, isMate, isLegal;
} Move;

/******************
* Function Name: checkMate
* Input: char pgn[]
* Output: 1/0
* Function Operation: check if there is mate in the string
******************/
int checkMate(char pgn[])
{
	for (int i = strlen(pgn); i >= 0; i--)
	{
		if (pgn[i] == MATE)
		{
			return 1;
		}
	}
	return 0;
}

/******************
* Function Name: checkCheck
* Input: char pgn[]
* Output: 1/0
* Function Operation: check if there is mate/ check in the string
******************/
int checkCheck(char pgn[])
{
	for (int i = strlen(pgn); i >= 0; i--)
	{
		if ((pgn[i] == CHECK) || (pgn[i] == MATE))
		{
			return 1;
		}
	}
	return 0;
}

/******************
* Function Name: checkPromotion
* Input: char pgn[]
* Output: 1/0
* Function Operation: check if there is promotion in the string
******************/
int checkPromotion(char pgn[])
{
	for (int i = strlen(pgn); i >= 0; i--)
	{
		if (pgn[i] == PROMOTION)
		{
			return 1;
		}
	}
	return 0;
}

/******************
* Function Name: checkPromotionPiece
* Input: char pgn[]
* Output: 1/0
* Function Operation: check if there is mate in the string
******************/
int checkPromotionPiece(char pgn[])
{
	for (int i = strlen(pgn); i >= 0; i--)
	{
		if (pgn[i] == PROMOTION)
		{
			return pgn[i + 1];
		}
	}
	return 'w';
}

/******************
* Function Name: checkCapture
* Input: char pgn[]
* Output: 1/0
* Function Operation: check if there is capture in the string
******************/
int checkCapture(char pgn[])
{
	int pgnLength = strlen(pgn);
	for (int i = 0; i < pgnLength; i++)
	{
		if (pgn[i] == CAPTURE)
		{
			return 1;
		}
	}
	return 0;
}

/******************
* Function Name: checkPiece
* Input: char pgn[]
* Output: 1/0
* Function Operation: check what piece in the string
******************/
char checkPiece(char pgn[])
{
	int i = 0;
	if (pgn[i] == ROOK)
	{
		return ROOK;
	}
	if (pgn[i] == QUEEN)
	{
		return QUEEN;
	}
	if (pgn[i] == KING)
	{
		return KING;
	}
	if (pgn[i] == BISHOP)
	{
		return BISHOP;
	}
	if (pgn[i] == KNIGHT)
	{
		return KNIGHT;
	}
	//if there is none its pawn
	return PAWN;
}

/******************
* Function Name: checkSourceColumn
* Input: char pgn[]
* Output: 1/0
* Function Operation: check if there is source column in the string
******************/
char checkSourceColumn(char pgn[])
{
	int isThereSourceColumn = 0, sourceColumnSpot;
	int pgnLength = strlen(pgn);
	for (int i = 0; i < pgnLength; i++)
	{
		if (pgn[i] == 'a' || pgn[i] == 'b' || pgn[i] == 'c' || pgn[i] == 'd' ||
			pgn[i] == 'e' || pgn[i] == 'f' || pgn[i] == 'g' || pgn[i] == 'h')
		{
			isThereSourceColumn++;
			if (isThereSourceColumn == 1)
			{
				sourceColumnSpot = i;
			}
		}
	}
	if (isThereSourceColumn == 2)
	{
		return pgn[sourceColumnSpot];
	}

	//acts like a flag to inform there in no column
	return 'w';
}

/******************
* Function Name: checkSourceRow
* Input: char pgn[]
* Output: 1/0
* Function Operation: check if there is source row in the string
******************/
char checkSourceRow(char pgn[])
{
	int isThereSourceRow = 0, sourceRowSpot;
	int pgnLength = strlen(pgn);
	for (int i = 0; i < pgnLength; i++)
	{
		if (pgn[i] == '1' || pgn[i] == '2' || pgn[i] == '3' || pgn[i] == '4' ||
			pgn[i] == '5' || pgn[i] == '6' || pgn[i] == '7' || pgn[i] == '8')
		{
			isThereSourceRow++;
			if (isThereSourceRow == 1)
			{
				sourceRowSpot = i;
			}
		}
	}
	if (isThereSourceRow == 2)
	{
		return pgn[sourceRowSpot];
	}

	//acts like a flag to inform there in no column
	return 'w';
}

/******************
* Function Name: convertColumnChar
* Input: char col
* Output: int
* Function Operation: converts character to its chess index
******************/
int convertColumnChar(char col)
{
	if (col != 'w')
	{
		//convert char column to numeric column according to ascii table
		return col - 'a';
	}
	else
	{
		//setting to not exist
		return -1;
	}
}

/******************
* Function Name: convertRowChar
* Input: char row
* Output: int
* Function Operation: converts character to its chess index
******************/
int convertRowChar(char row)
{
	if (row != 'w')
	{
		//converts char row to numeric row according to ascii table
		return ((row - '0' - SIZE) * -1);
	}
	else
	{
		//setting to not exist
		return -1;
	}
}

/******************
* Function Name: checkDestColumn
* Input: char pgn[]
* Output: char
* Function Operation: check dest column in the string
******************/
char checkDestColumn(char pgn[])
{
	for (int i = (strlen(pgn) - 1); i >= 0; i--)
	{
		if (pgn[i] == 'a' || pgn[i] == 'b' || pgn[i] == 'c' || pgn[i] == 'd' ||
			pgn[i] == 'e' || pgn[i] == 'f' || pgn[i] == 'g' || pgn[i] == 'h')
		{
			return pgn[i];
		}
	}

	//never gets to it but have to write return
	return -1;
}

/******************
* Function Name: printBoard
* Input: char pgn[]
* Output: 1/0
* Function Operation: check dest row in the string
******************/
char checkDestRow(char pgn[])
{
	for (int i = (strlen(pgn) - 1); i >= 0; i--)
	{
		if (pgn[i] == '1' || pgn[i] == '2' || pgn[i] == '3' || pgn[i] == '4' ||
			pgn[i] == '5' || pgn[i] == '6' || pgn[i] == '7' || pgn[i] == '8')
		{
			return pgn[i];
		}
	}
	return -1;
}

/******************
* Function Name: likePGNSource
* Input: int optionalj, int optionali, Move currentTurn
* Output: int
* Function Operation: checks if piece indexes are equals to pgn source indexes
******************/
int likePGNSource(int optionalj, int optionali, Move currentTurn)
{
	//there is no source indexes in pgn
	if ((currentTurn.iSrc == -1) && (currentTurn.jSrc == -1))
	{
		return 1;
	}
	
	//there are 2 source indexes in pgn
	if ((currentTurn.iSrc == optionali) && (currentTurn.jSrc == optionalj))
	{
		return 1;
	}

	//there is only 1 source index in pgn
	if ((currentTurn.iSrc == -1) && (currentTurn.jSrc == optionalj))
	{
		return 1;
	}
	if ((currentTurn.iSrc == optionali) && (currentTurn.jSrc == -1))
	{
		return 1;
	}
	return 0;
}

/******************
* Function Name: checkDestPiece
* Input: char board[][SIZE], Move currentTurn
* Output: char
* Function Operation: returns char in dest
******************/
char checkDestPiece(char board[][SIZE], Move currentTurn)
{
	return board[currentTurn.iDest][currentTurn.jDest];
}

/******************
* Function Name: destClear
* Input: char board[][SIZE], Move currentTurn
* Output: int
* Function Operation: check if dest is clear
******************/
int destClear(char board[][SIZE], Move currentTurn)
{
	
	//if dest is empty and there is no capture
	if ((currentTurn.destPiece == EMPTY) && (currentTurn.isCapture == 0))
	{
		return 1;
	}

	//black can eat white
	if ((currentTurn.isCapture == 1) && (currentTurn.isWhite == 1) &&
		((currentTurn.destPiece > 'a') && (currentTurn.destPiece < 'z')))
	{
		return 1;
	}

	//white can eat black
	if ((currentTurn.isCapture == 1) && (currentTurn.isWhite == 0) &&
		((currentTurn.destPiece > 'A') && (currentTurn.destPiece < 'Z')))
	{
		return 1;
	}
	return 0;
}

/******************
* Function Name: canMove
* Input: Move currentTurn, int optionalj, int optionali
* Output: int
* Function Operation: checks if piece can move from source to destination by piece kind
******************/
int canMove(Move currentTurn, int optionalj, int optionali)
{
	//in case its black turn
	char upperSrcPiece = toupper(currentTurn.srcPiece);

	if (upperSrcPiece == BISHOP)
	{

		//bisop can move only if source i+j = dest i+j or src i-j = dest i-j
		if (currentTurn.jDest + currentTurn.iDest == optionalj + optionali)
		{
			return 1;
		}
		if (currentTurn.jDest - currentTurn.iDest == optionalj - optionali)
		{
			return 1;
		}
		return 0;
	}

	//rook can move to dest only if src col == dest col OR src row == dest row
	if (upperSrcPiece == ROOK)
	{
		if (currentTurn.jDest == optionalj)
		{
			return 1;
		}
		if (currentTurn.iDest == optionali)
		{
			return 1;
		}
		return 0;
	}

	//queen can move to dest with the rules of rook and bisop combine
	if (upperSrcPiece == QUEEN)
	{
		if (currentTurn.jDest == optionalj)
		{
			return 1;
		}
		if (currentTurn.iDest == optionali)
		{
			return 1;
		}
		if (currentTurn.jDest + currentTurn.iDest == optionalj + optionali)
		{
			return 1;
		}
		if (currentTurn.jDest - currentTurn.iDest == optionalj - optionali)
		{
			return 1;
		}
		return 0;
	}

	//king can move to dest from 8 difrent positions
	if (upperSrcPiece == KING)
	{
		//same row, column+1/-1
		if (currentTurn.iDest == optionali)
		{
			if ((currentTurn.jDest == optionalj + 1) || (currentTurn.jDest == optionalj - 1))
			{
				return 1;
			}
		}

		//row above, column -/+/=
		if (currentTurn.iDest + 1 == optionali)
		{
			if (currentTurn.jDest == optionalj || (currentTurn.jDest == optionalj + 1) ||
				(currentTurn.jDest == optionalj - 1))
			{
				return 1;
			}
		}

		//row below, column -/+/=
		if (currentTurn.iDest - 1 == optionali)
		{
			if (currentTurn.jDest == optionalj || (currentTurn.jDest == optionalj + 1) ||
				(currentTurn.jDest == optionalj - 1))
			{
				return 1;
			}
		}
		return 0;
	}
	if (upperSrcPiece == KNIGHT)
	{

		//8 optional places can arrive to point
		if ((currentTurn.iDest == optionali + 1) && (currentTurn.jDest == optionalj + 2))
		{
			return 1;
		}
		if ((currentTurn.iDest == optionali - 1) && (currentTurn.jDest == optionalj + 2))
		{
			return 1;
		}
		if ((currentTurn.iDest == optionali - 2) && (currentTurn.jDest == optionalj + 1))
		{
			return 1;
		}
		if ((currentTurn.iDest == optionali + 2) && (currentTurn.jDest == optionalj + 1))
		{
			return 1;
		}
		if ((currentTurn.iDest == optionali - 1) && (currentTurn.jDest == optionalj - 2))
		{
			return 1;
		}
		if ((currentTurn.iDest == optionali + 1) && (currentTurn.jDest == optionalj - 2))
		{
			return 1;
		}
		if ((currentTurn.iDest == optionali + 2) && (currentTurn.jDest == optionalj - 1))
		{
			return 1;
		}
		if ((currentTurn.iDest == optionali - 2) && (currentTurn.jDest == optionalj - 1))
		{
			return 1;
		}
		return 0;
	}
	if (upperSrcPiece == PAWN)
	{

		//in case we dont have capture
		if (currentTurn.isCapture == 0)
		{

			//in case its white turn and dest row is row that can jump 2 steps and the columns are the same
			if ((currentTurn.isWhite == 1) && (currentTurn.iDest == SIZE - 4) && (currentTurn.jDest == optionalj))
			{
				if ((currentTurn.iDest == optionali - 1) || (currentTurn.iDest == optionali - 2))
				{
					return 1;
				}
			}

			//in case dest is regular row
			if ((currentTurn.isWhite == 1) && (currentTurn.iDest == optionali - 1) && (currentTurn.jDest == optionalj))
			{
				return 1;
			}

			//in case its black turn and dest is row that can jump 2 steps and the columns are the same
			if ((currentTurn.isWhite == 0) && (currentTurn.iDest == 3) && (currentTurn.jDest == optionalj))
			{
				if ((currentTurn.iDest == optionali + 1) || (currentTurn.iDest == optionali + 2))
				{
					return 1;
				}
			}

			//in case dest is regular row
			if ((currentTurn.isWhite == 0) && (currentTurn.iDest == optionali + 1) && (currentTurn.jDest == optionalj))
			{
				return 1;
			}
		}

		//in case we have capture
		else
		{
			//white turn up left and right
			if ((currentTurn.isWhite == 1) && (currentTurn.iDest == optionali - 1) && (currentTurn.jDest == optionalj - 1))
			{
				return 1;
			}
			if ((currentTurn.isWhite == 1) && (currentTurn.iDest == optionali - 1) && (currentTurn.jDest == optionalj + 1))
			{
				return 1;
			}

			//black ture up left and right
			if ((currentTurn.isWhite == 0) && (currentTurn.iDest == optionali + 1) && (currentTurn.jDest == optionalj - 1))
			{
				return 1;
			}
			if ((currentTurn.isWhite == 0) && (currentTurn.iDest == optionali + 1) && (currentTurn.jDest == optionalj + 1))
			{
				return 1;
			}
		}
		return 0;
	}
	return 0;
}

/******************
* Function Name: checkCross
* Input: char board[][SIZE], Move currentTurn, int optionalj, int optionali
* Output: int
* Function Operation: checks if cross directions are clear
******************/
int checkCross(char board[][SIZE], Move currentTurn, int optionalj, int optionali)
{
	int start, end;

	//checks in the row, seperate to source column smaller and bigger then dest col
	if (currentTurn.iDest == optionali)
	{
		if (currentTurn.jDest > optionalj)
		{
			start = optionalj + 1;
			end = currentTurn.jDest;
		}
		else
		{
			start = currentTurn.jDest + 1;
			end = optionalj;
		}

		//checks if not empty
		for (int j = start; j < end; j++)
		{
			if (board[optionali][j] != EMPTY)
			{
				return 0;
			}
		}
	}

	//checks in the col, seperate to source row smaller and bigger then dest row
	else if (currentTurn.jDest == optionalj)
	{
		if (currentTurn.iDest > optionali)
		{
			start = optionali + 1;
			end = currentTurn.iDest;
		}
		else
		{
			start = currentTurn.iDest + 1;
			end = optionali;
		}

		//checks if not empty
		for (int i = start; i < end; i++)
		{
			if (board[i][optionalj] != EMPTY)
			{
				return 0;
			}
		}
	}

	//when cross is not the right option, special for queen checks
	if ((currentTurn.iDest != optionali) && (currentTurn.jDest != optionalj))
	{
		return 0;
	}
	return 1;
}

/******************
* Function Name: checkDiagonal
* Input: char board[][SIZE], Move currentTurn, int optionalj, int optionali
* Output: int
* Function Operation: check if diagonal directions are clear
******************/
int checkDiagonal(char board[][SIZE], Move currentTurn, int optionalj, int optionali)
{
	//checks from optional source to dest down right
	if ((optionali < currentTurn.iDest) && (optionalj < currentTurn.jDest))
	{
		int i = optionali + 1;
		int j = optionalj + 1;
		while ((i < currentTurn.iDest) && (j < currentTurn.jDest))
		{
			if (board[i][j] != EMPTY)
			{
				return 0;
			}
			i++;
			j++;
		}
	}

	//checks from optional source to dest up left
	if ((optionali > currentTurn.iDest) && (optionalj > currentTurn.jDest))
	{
		int i = optionali - 1;
		int j = optionalj - 1;
		while ((i > currentTurn.iDest) && (j > currentTurn.jDest))
		{
			if (board[i][j] != EMPTY)
			{
				return 0;
			}
			i--;
			j--;
		}
	}

	//checks from optional source to dest down right
	if ((optionali > currentTurn.iDest) && (optionalj < currentTurn.jDest))
	{
		int i = optionali - 1;
		int j = optionalj + 1;
		while (i > currentTurn.iDest && (j < currentTurn.jDest))
		{
			if (board[i][j] != EMPTY)
			{
				return 0;
			}
			i--;
			j++;
		}
	}

	//checks from optional source to dest down left
	if ((optionali < currentTurn.iDest) && (optionalj > currentTurn.jDest))
	{
		int i = optionali + 1;
		int j = optionalj - 1;
		while (i < currentTurn.iDest && (j > currentTurn.jDest))
		{
			if (board[i][j] != EMPTY)
			{
				return 0;
			}
			i++;
			j--;
		}
	}

	//when diagonal is not the right option, special for queen checks
	if ((optionali == currentTurn.iDest) || (optionalj == currentTurn.jDest))
	{
		return 0;
	}
	return 1;
}

/******************
* Function Name: isClear
* Input: char board[][SIZE], Move currentTurn, int optionalj, int optionali
* Output: int
* Function Operation: gets optional sources and return if the way from them to dest is clear
					  depends in piece kind
******************/
int isClear(char board[][SIZE], Move currentTurn, int optionalj, int optionali)
{

	//in case its black turn
	char upperSrcPiece = toupper(currentTurn.srcPiece);

	if (upperSrcPiece == PAWN)
	{
		//in case moving to row that pawn can jump to there by 2 steps
		if ((currentTurn.isWhite == 1) && (currentTurn.iDest == SIZE - 4) && (optionalj == currentTurn.jDest) &&
			(optionali - 2 == currentTurn.iDest) && (board[optionali - 1][optionalj] == EMPTY))
		{
			return 1;
		}
		
		// in case moving one step to 2jump row dest, no need to check
		if ((currentTurn.isWhite == 1) && (currentTurn.iDest == SIZE - 4) && (optionali - 1 == currentTurn.iDest))
		{
			return 1;
		}

		//in case moving to row that pawn can jump to there by 2 steps but walks 1 step
		if ((currentTurn.isWhite == 1) && (currentTurn.iDest != SIZE - 4))
		{
			return 1;
		}

		//in case moving to row that pawn can jump to there by 2 steps
		if ((currentTurn.isWhite == 0) && (currentTurn.iDest == 3) && (optionalj == currentTurn.jDest) &&
			(optionali + 2 == currentTurn.iDest) && (board[optionali + 1][optionalj] == EMPTY))
		{
			return 1;
		}
		
		// in case moving one step to 2jump row dest, no need to check
		if ((currentTurn.isWhite == 0) && (currentTurn.iDest == 3) && (optionali + 1 == currentTurn.iDest))
		{
			return 1;
		}

		//in case moving to row that pawn can jump to there by 2 steps but walks 1 step
		if ((currentTurn.isWhite == 0) && (currentTurn.iDest != 3))
		{
			return 1;
		}
	}

	//rook need to check cross directions
	if (upperSrcPiece == ROOK)
	{
		if (checkCross(board, currentTurn, optionalj, optionali))
		{
			return 1;
		}
	}

	//bishop need to check diagonal directions
	if (upperSrcPiece == BISHOP)
	{
		if (checkDiagonal(board, currentTurn, optionalj, optionali))
		{
			return 1;
		}
	}

	//queen need to check both cross and diagonal directions
	if (upperSrcPiece == QUEEN)
	{
		int isCrossClear = checkCross(board, currentTurn, optionalj, optionali);
		int isDiagonalClear = checkDiagonal(board, currentTurn, optionalj, optionali);
		if ((isCrossClear == 1) || (isDiagonalClear == 1))
		{
			return 1;
		}
	}

	//knigt only jump so way always clear
	if (upperSrcPiece == KNIGHT)
	{
		return 1;
	}

	//king only walks one step so way always clear
	if (upperSrcPiece == KING)
	{
		return 1;
	}
	return 0;
}

/******************
* Function Name: legalPromotion
* Input: char board[][SIZE], Move currentTurn, int optionalj, int optionali
* Output: int
* Function Operation: checks if promotion is legal
******************/
int legalPromotion(char board[][SIZE], Move currentTurn, int optionalj, int optionali)
{
	//in case its black turn
	char upperSrcPiece = toupper(currentTurn.srcPiece);

	//can promote only pawn, when he at the end of the board and promotion is turn on
	if ((upperSrcPiece == PAWN) && (currentTurn.isWhite == 1) && (currentTurn.iDest == 0) &&
		(currentTurn.isPromotion == 1) && (currentTurn.promotionPiece != 'w'))
	{
		return 1;
	}
	if ((upperSrcPiece == PAWN) && (currentTurn.isWhite == 0) && (currentTurn.iDest == (SIZE - 1)) &&
		(currentTurn.isPromotion == 1) && (currentTurn.promotionPiece != 'w'))
	{
		return 1;
	}

	//if promotion turn off return 1
	if ((upperSrcPiece != PAWN) && (currentTurn.isPromotion == 0))
	{
		return 1;
	}
	if ((upperSrcPiece == PAWN) && (currentTurn.isWhite == 1) && (currentTurn.isPromotion == 0) &&
		(currentTurn.iDest != 0))
	{
		return 1;
	}
	if ((upperSrcPiece == PAWN) && (currentTurn.isWhite == 0) && (currentTurn.isPromotion == 0) &&
		(currentTurn.iDest != SIZE - 1))
	{
		return 1;
	}
	return 0;
}

/******************
* Function Name: pgnMove
* Input: char board[][SIZE], Move currentTurn, int optionalj, int optionali
* Output: void
* Function Operation: moves the pieces on the board
******************/
void pgnMove(char board[][SIZE], Move currentTurn, int optionalj, int optionali)
{
	char blackPromotionPiece;

	//in cases of promotions
	if ((currentTurn.isPromotion == 1) && (currentTurn.isWhite == 1))
	{
		board[currentTurn.iDest][currentTurn.jDest] = currentTurn.promotionPiece;
		board[optionali][optionalj] = EMPTY;
	}
	if ((currentTurn.isPromotion == 1) && (currentTurn.isWhite == 0))
	{
		blackPromotionPiece = tolower(currentTurn.promotionPiece);
		board[currentTurn.iDest][currentTurn.jDest] = blackPromotionPiece;
		board[optionali][optionalj] = EMPTY;
	}

	//in cases regular move
	if (currentTurn.isPromotion == 0)
	{
		board[currentTurn.iDest][currentTurn.jDest] = currentTurn.srcPiece;
		board[optionali][optionalj] = EMPTY;
	}
}

/******************
* Function Name: isCheck
* Input: char board[][SIZE], Move currentTurn, int optionalj, int optionali
* Output: int
* Function Operation: check if black and white king are under threat. once before the move and
					  after the move. Acorrding to answers checks if move will be legal.
******************/
int isCheck(char board[][SIZE], Move currentTurn, int optionalj, int optionali)
{
	int lastTurnWhiteCheck = 0, lastTurnBlackCheck = 0, nextTurnWhiteCheck = 0, nextTurnBlackCheck = 0;
	char boardCpy[SIZE][SIZE];

	//coping the board so we can check future move
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			boardCpy[i][j] = board[i][j];
		}
	}

	//will handle bothe current board and future board status
	Move lastTurn;

	//checking chess status of current board, before the move
	//finding white king index
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			if (board[i][j] == KING)
			{
				lastTurn.iDest = i;
				lastTurn.jDest = j;
			}
		}
	}

	//iterating to find a threat
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{

			//in case its pawn
			if (board[i][j] == 'p')
			{
				lastTurn.srcPiece = 'p';
				
				//pawn can threat when he eats, and walks difrently when eats so capture is on
				lastTurn.isCapture = 1;

				//checks if rival can move to king dest
				lastTurnWhiteCheck = canMove(lastTurn, j, i);
				lastTurn.isCapture = 0;
				if (lastTurnWhiteCheck == 0)
				{
					continue;
				}

				//checks if rival's way is clear to king dest
				lastTurnWhiteCheck = isClear(board, lastTurn, j, i);
				if (lastTurnWhiteCheck == 0)
				{
					continue;
				}
				else
				{
					break;
				}
			}

			//in case its not pawn
			if ((board[i][j] > 'a') && (board[i][j] < 'z') && (board[i][j] != 'p'))
			{
				lastTurn.srcPiece = board[i][j];

				//checks if rival can move to king dest
				lastTurnWhiteCheck = canMove(lastTurn, j, i);
				if (lastTurnWhiteCheck == 0)
				{
					continue;
				}

				//checks if rival's way is clear to king dest
				lastTurnWhiteCheck = isClear(board, lastTurn, j, i);
				if (lastTurnWhiteCheck == 0)
				{
					continue;
				}
				else
				{
					break;
				}
			}
		}
		if (lastTurnWhiteCheck == 0)
		{
			continue;
		}
		else
		{
			break;
		}
	}

	//finding black king index
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			if (board[i][j] == 'k')
			{
				lastTurn.iDest = i;
				lastTurn.jDest = j;
			}
		}
	}
	//iterating to find a threat
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{

			//in case its pawn
			if (board[i][j] == PAWN)
			{
				lastTurn.srcPiece = PAWN;
				lastTurn.isCapture = 1;

				//checks if rival can move to king dest
				lastTurnBlackCheck = canMove(lastTurn, j, i);
				lastTurn.isCapture = 0;
				if (lastTurnBlackCheck == 0)
				{
					continue;
				}

				//checks if rival's way is clear to king dest
				lastTurnBlackCheck = isClear(board, lastTurn, j, i);
				if (lastTurnBlackCheck == 0)
				{
					continue;
				}
				else
				{
					break;
				}
			}

			//in case its not pawn
			if ((board[i][j] > 'A') && (board[i][j] < 'Z') && (board[i][j] != PAWN))
			{
				lastTurn.srcPiece = board[i][j];

				//checks if rival can move to king dest
				lastTurnBlackCheck = canMove(lastTurn, j, i);
				if (lastTurnBlackCheck == 0)
				{
					continue;
				}

				//checks if rival's way is clear to king dest
				lastTurnBlackCheck = isClear(board, lastTurn, j, i);
				if (lastTurnBlackCheck == 0)
				{
					continue;
				}
				else
				{
					break;
				}
			}
		}
		if (lastTurnBlackCheck == 0)
		{
			continue;
		}
		else
		{
			break;
		}
	}

	//moving the piece******************************************************************
	pgnMove(boardCpy, currentTurn, optionalj, optionali);

	//finding white king index
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			if (boardCpy[i][j] == KING)
			{
				lastTurn.iDest = i;
				lastTurn.jDest = j;
			}
		}
	}
	//iterating to find a threat
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{

			//in case its pawn
			if (boardCpy[i][j] == 'p')
			{
				lastTurn.srcPiece = 'p';
				lastTurn.isCapture = 1;

				//checks if rival can move to king dest
				nextTurnWhiteCheck = canMove(lastTurn, j, i);
				lastTurn.isCapture = 0;
				if (nextTurnWhiteCheck == 0)
				{
					continue;
				}

				//checks if rival's way is clear to king dest
				nextTurnWhiteCheck = isClear(boardCpy, lastTurn, j, i);
				if (nextTurnWhiteCheck == 0)
				{
					continue;
				}
				else
				{
					break;
				}
			}

			//in case its not pawn
			if ((boardCpy[i][j] > 'a') && (boardCpy[i][j] < 'z') && (boardCpy[i][j] != 'p'))
			{
				lastTurn.srcPiece = boardCpy[i][j];

				//checks if rival can move to king dest
				nextTurnWhiteCheck = canMove(lastTurn, j, i);
				if (nextTurnWhiteCheck == 0)
				{
					continue;
				}

				//checks if rival's way is clear to king dest
				nextTurnWhiteCheck = isClear(boardCpy, lastTurn, j, i);
				if (nextTurnWhiteCheck == 0)
				{
					continue;
				}
				else
				{
					break;
				}
			}
		}
		//breaking the seond for in case of chess
		if (nextTurnWhiteCheck == 0)
		{
			continue;
		}
		else
		{
			break;
		}
	}

	//finding black king index
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			if (boardCpy[i][j] == 'k')
			{
				lastTurn.iDest = i;
				lastTurn.jDest = j;
			}
		}
	}
	//iterating to find a threat
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{

			//in case its pawn
			if (boardCpy[i][j] == PAWN)
			{
				lastTurn.srcPiece = PAWN;
				lastTurn.isCapture = 1;

				//checks if rival can move to king dest
				nextTurnBlackCheck = canMove(lastTurn, j, i);
				lastTurn.isCapture = 0;
				if (nextTurnBlackCheck == 0)
				{
					continue;
				}

				//checks if rival's way is clear to king dest
				nextTurnBlackCheck = isClear(boardCpy, lastTurn, j, i);
				if (nextTurnBlackCheck == 0)
				{
					continue;
				}
				else
				{
					break;
				}
			}

			//in case its not pawn
			if ((boardCpy[i][j] > 'A') && (boardCpy[i][j] < 'Z') && (boardCpy[i][j] != PAWN))
			{
				lastTurn.srcPiece = boardCpy[i][j];

				//checks if rival can move to king dest
				nextTurnBlackCheck = canMove(lastTurn, j, i);
				if (nextTurnBlackCheck == 0)
				{
					continue;
				}

				//checks if rival's way is clear to king dest
				nextTurnBlackCheck = isClear(boardCpy, lastTurn, j, i);
				if (nextTurnBlackCheck == 0)
				{
					continue;
				}
				else
				{
					break;
				}
			}
		}
		if (nextTurnBlackCheck == 0)
		{
			continue;
		}
		else
		{
			break;
		}
	}

	//conditions*******************************************************
	//white check conditions that confirm the move legal
	if ((currentTurn.isWhite == 1) && (lastTurnWhiteCheck == 1) && (nextTurnWhiteCheck == 0)
		&& (nextTurnBlackCheck == 1) && (currentTurn.isCheck == 1))
	{
		return 1;
	}
	if ((currentTurn.isWhite == 1) && (lastTurnWhiteCheck == 1) && (nextTurnWhiteCheck == 0)
		&& (nextTurnBlackCheck == 0) && (currentTurn.isCheck == 0))
	{
		return 1;
	}
	if ((currentTurn.isWhite == 1) && (lastTurnWhiteCheck == 0) && (nextTurnWhiteCheck == 0)
		&& (nextTurnBlackCheck == 1) && (currentTurn.isCheck == 1))
	{
		return 1;
	}
	if ((currentTurn.isWhite == 1) && (lastTurnWhiteCheck == 0) && (nextTurnWhiteCheck == 0)
		&& (nextTurnBlackCheck == 0) && (currentTurn.isCheck == 0))
	{
		return 1;
	}

	//black check conditions that confirm the move legal
	if ((currentTurn.isWhite == 0) && (lastTurnBlackCheck == 1) && (nextTurnBlackCheck == 0) &&
		(nextTurnWhiteCheck == 1) && (currentTurn.isCheck == 1))
	{
		return 1;
	}
	if ((currentTurn.isWhite == 0) && (lastTurnBlackCheck == 1) && (nextTurnBlackCheck == 0) &&
		(nextTurnWhiteCheck == 0) && (currentTurn.isCheck == 0))
	{
		return 1;
	}
	if ((currentTurn.isWhite == 0) && (lastTurnBlackCheck == 0) && (nextTurnBlackCheck == 0) &&
		(nextTurnWhiteCheck == 1) && (currentTurn.isCheck == 1))
	{
		return 1;
	}
	if ((currentTurn.isWhite == 0) && (lastTurnBlackCheck == 0) && (nextTurnBlackCheck == 0) &&
		(nextTurnWhiteCheck == 0) && (currentTurn.isCheck == 0))
	{
		return 1;
	}

	return 0;
}

/******************
* Function Name: makeMove
* Input: char board[][SIZE], char pgn[], int isWhiteTurn
* Output: int
* Function Operation: braking down pgn string into bool flags, and checks if the move can accour
******************/
int makeMove(char board[][SIZE], char pgn[], int isWhiteTurn)
{
	//need to create a new move sturct
	Move turn;
	turn.isWhite = isWhiteTurn;
	turn.srcPiece = checkPiece(pgn);

	//low casing if its black turn
	if (turn.isWhite == 0)
	{
		turn.srcPiece = tolower(turn.srcPiece);
	}

	//check if there is a source information and converting chess index to array index if not get -1
	turn.srcCol = checkSourceColumn(pgn);
	turn.jSrc = convertColumnChar(turn.srcCol);
	turn.srcRow = checkSourceRow(pgn);
	turn.iSrc = convertRowChar(turn.srcRow);

	//chek the dest information and converting chess index to array index
	turn.destCol = checkDestColumn(pgn);
	turn.jDest = convertColumnChar(turn.destCol);
	turn.destRow = checkDestRow(pgn);
	turn.iDest = convertRowChar(turn.destRow);

	//checks what piece is in destination
	turn.destPiece = checkDestPiece(board, turn);

	//checks boolean conditions
	turn.isPromotion = checkPromotion(pgn);
	turn.promotionPiece = checkPromotionPiece(pgn);
	turn.isCheck = checkCheck(pgn);
	turn.isMate = checkMate(pgn);
	turn.isCapture = checkCapture(pgn);


	//chek destination spot clear
	turn.isLegal = destClear(board, turn);
	if (turn.isLegal == 0)
	{
		return 0;
	}

	//will save the indexes of optional moving piece
	int optionaljSrc, optionaliSrc;

	//iterate threw board and looking for piece which fits pgn directions
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{

			//checks for simillar pieces to pgn
			if (board[i][j] == turn.srcPiece)
			{
				optionaljSrc = j;
				optionaliSrc = i;
				turn.isLegal = canMove(turn, optionaljSrc, optionaliSrc);
				if (turn.isLegal == 0)
				{
					continue;
				}
				turn.isLegal = likePGNSource(optionaljSrc, optionaliSrc, turn);
				if (turn.isLegal == 0)
				{
					continue;
				}
				turn.isLegal = isClear(board, turn, optionaljSrc, optionaliSrc);
				if (turn.isLegal == 0)
				{
					continue;
				}
				turn.isLegal = legalPromotion(board, turn, optionaljSrc, optionaliSrc);
				if (turn.isLegal == 0)
				{
					continue;
				}
				turn.isLegal = isCheck(board, turn, optionaljSrc, optionaliSrc);
				if (turn.isLegal == 0)
				{
					continue;
				}

				//if the piece pass all the tests moving it according to pgn
				pgnMove(board, turn, optionaljSrc, optionaliSrc);
				return 1;
			}
		}
	}
	return 0;
}
