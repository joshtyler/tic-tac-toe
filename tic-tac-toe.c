/* Tic tac toe version 1.0
 * Created by Joshua Tyler, June 2014
 */

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>



/* Pass this macro a pointer and if it is null it will terminate the program giving the line number it was called from.
   It is used to check the pointers returned by malloc, calloc, fopen etc. */
#define checkPtr(pointer) if(pointer == NULL) { \
												fprintf(stderr,"NULL Pointer errror on line %d\n"                                        \
												                 "Suspected memory allocation error or file opening error.\n",__LINE__); \
												exit(EXIT_FAILURE); }
typedef struct coord coord;
struct coord
 {
	 int row;
	 int col;
 };

const coord boardMax = {3,3};
const char naughtChar = 'O', crossChar = 'X', emptyChar = ' ';
#define TRUE 1
#define FALSE 0

const int notOver = FALSE, win = TRUE, draw = 2;

char **createBoard(void);
void freeBoard(char **board);
void printBoard(char **board, FILE *stream);
int placeTurn(char **board, coord location, char character);
coord getInput(FILE *inStream);
int gameOver(char **board);

int main(void)
{
	char **board = createBoard();
	
	/* Begin main game */
	coord currentPos;
	char currentPlayer = crossChar;
	int i;
	for(i=0; gameOver(board) == FALSE; i++)
	{
		if(i == 0)
		{
			printBoard(board, stdout);
			printf("It's %c's turn.\nChoose co-ordinates: ", currentPlayer);
		} else {
			puts("Incorrect input, please try again");
		}
		currentPos = getInput(stdin);
		if(currentPos.row == -1)
			continue;

		if( placeTurn(board, currentPos, currentPlayer) == FALSE )
			continue;
		
		if(currentPlayer == crossChar)
			currentPlayer = naughtChar;
		else
			currentPlayer = crossChar;
		i = -1;
	}

	if(currentPlayer == crossChar)
		currentPlayer = naughtChar;
	else
		currentPlayer = crossChar;

	if(gameOver(board) == win)
		printf("Game over. %c wins.\n", currentPlayer);
	else
		puts("Game over. Draw.\n");

	printBoard(board, stdout);

	freeBoard(board);

	return EXIT_SUCCESS;
}

char **createBoard(void)
{
	/* Allocate board */
	char **board = (char **) malloc(boardMax.row * sizeof(char*));
	checkPtr(board);

	int i;
	for(i=0; i<boardMax.row; i++)
	{
		board[i] = (char *) malloc(boardMax.col * sizeof(char));
		checkPtr(board[i]);
	}

	/* Initialise board */
	int j;
	for(i=0; i<boardMax.row; i++)
	{
		for(j=0; j<boardMax.col; j++)
		{
			board[i][j] = emptyChar;
		}
	}

	return board;
}

void freeBoard(char **board)
{
	int i;
	for(i=0; i<boardMax.row; i++)
	{
		free(board[i]);
	}
	free(board);
}


void printBoard(char **board, FILE *stream)
{
	int i,j;

	fputs("    A  B  C\n",stream);

	for(i=0; i<boardMax.row; i++)
	{
		fprintf(stream,"%i [", i + 1 );
		for(j=0; j< boardMax.col -1; j++)
		{
			printf(" %c,",board[i][j]);
		}
		printf(" %c ]",board[i][j]);
		fputc('\n',stream);
	}
}

int placeTurn(char **board, coord location, char character)
{
	if(board[location.row][location.col] != emptyChar)
		return FALSE;
	
	board[location.row][location.col] = character;
	return TRUE;
}

coord getInput(FILE *inStream)
{
	coord returnVal = {-1,-1};

	char tempCol = '\0';
	
	fscanf(inStream,"%c%d", &tempCol, &returnVal.row);
	while ( fgetc(inStream) != '\n' ); /* Clear input stream */

	if(tempCol != '\0')
		returnVal.col = (int) tempCol - 'A';

	if(returnVal.row != -1)
		returnVal.row -= 1;

	if(returnVal.row < 0 || returnVal.row > 2 || returnVal.col < 0 || returnVal.col > 2)
	{
		returnVal.row = -1;
		returnVal.col = -1;
	}

	return returnVal;
}

int gameOver(char **board)
{
	int i;
	/* Horizontal wins */
	for(i=0; i<boardMax.row; i++)
	{
		if( board[i][0] == board[i][1] && board[i][0] == board[i][2] && board[i][0] != emptyChar)
			return win;
	}
	/* Vertrical wins */
	for(i=0; i<boardMax.col; i++)
	{
		if( board[0][i] == board[1][i] && board[0][i] == board[2][i] && board[0][i] != emptyChar)
			return win;
	}
	/* Diagonal wins */
	if( board[0][0] == board[1][1] && board[0][0] == board[2][2] && board[0][0] != emptyChar)
		return win;
	if( board[0][2] == board[1][1] && board[0][2] == board[2][0] && board[0][2] != emptyChar)
		return win;

	int j;
	for(i=0; i<boardMax.row; i++)
	{
		for(j=0; j<boardMax.col; j++)
		{
			if(board[i][j] == emptyChar)
				return notOver;
		}
	}
	return draw;
}