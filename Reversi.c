//Reversi game, with Minimax (only 2 iterations)   ---Qiyao Wei
//Magic chars in this program are 'B', 'W', and 'U'
//Could include GetBoard for formatting
#include <stdio.h>
#include <stdbool.h>
#define MAXDIM 26   //Maximum dimension of board array

//Called in Main
void InitializeBoard(char board[MAXDIM][MAXDIM], int dimension);                          //Set the 4 at center
void PrintBoard(char board[MAXDIM][MAXDIM], int dimension);                               //Prints the board
bool TakeMoves(char board[MAXDIM][MAXDIM], int dimension, char humanPlays);               //Read input moves & check validity
void Minimax(char board[MAXDIM][MAXDIM], int dimension, char compPlays, char humanPlays); //Computer chooses moves
int FinalCount(char board[MAXDIM][MAXDIM], int dimension, char color);                    //Counts who wins

//Helper functions
int ValidMoves(char board[MAXDIM][MAXDIM], int dimension, char color);                                                     //Number of valid moves
void SetBoard(char board[MAXDIM][MAXDIM], char row, char col, char color);                                                 //Set the color of a position
void Reverse(char board[MAXDIM][MAXDIM], int dimension, char row, char col, char color);                                   //Reverse colors!
void FlipInDirection(char board[MAXDIM][MAXDIM], int dimension, char row, char col, char color, int dirX, int dirY);       //Helps Reverse, could also reuse CheckLegal
bool IsValid(char board[MAXDIM][MAXDIM], int dimension, char row, char col, char color);                                   //Helps ValidMoves
bool CheckLegalInDirection(char board[MAXDIM][MAXDIM], int dimension, char row, char col, char color, int dirX, int dirY); //Helps IsValid
bool InBounds(int dimension, char row, char col);                                                                          //Is a position within dimension?

int main(int argc, char **argv)
{
	char board[MAXDIM][MAXDIM];
	char compPlays = '0', humanPlays = '0';
	int dimension  = 0;
	
	//Get dimension
	printf("Enter the board dimension: ");
	scanf("%d", &dimension);
	
	//Decide B/W
	printf("Computer plays (B/W) : ");
	scanf(" %c", &compPlays);
	
	//Initialize board
	InitializeBoard(board, dimension);
	PrintBoard(board, dimension);
	
	//Differentiates color, could just have a boolean and not if-else loops
	if (compPlays == 'W'){
		humanPlays = 'B';
		
		while (ValidMoves(board, dimension, compPlays) != 0 || ValidMoves(board, dimension, humanPlays) != 0){   //when moves are available
			
			if (ValidMoves(board, dimension, humanPlays) == 0){   //if human can't move
				printf("%c player has no valid move.\n", humanPlays);
			} else {
				if (!TakeMoves(board, dimension, humanPlays)){   //execute if human move is valid
					printf("%c player wins.\n", compPlays);
					return 0;
				} else {
					PrintBoard(board, dimension);
				}
			}
			
			//Check again to avoid incorrect output error
			if (ValidMoves(board, dimension, compPlays) == 0 && ValidMoves(board, dimension, humanPlays) == 0)
				break;
				
			if (ValidMoves(board, dimension, compPlays) == 0){   //if computer can't move
				printf("%c player has no valid move.\n", compPlays);
			} else {
				Minimax(board, dimension, compPlays, humanPlays);
				PrintBoard(board, dimension);
			}
		}
		
		//End of game
		if (FinalCount(board, dimension, compPlays) > FinalCount(board, dimension, humanPlays))
			printf("%c player wins.\n", compPlays);
		else if (FinalCount(board, dimension, humanPlays) > FinalCount(board, dimension, compPlays))
			printf("%c player wins.\n", humanPlays);
		else
			printf("Tie.\n");
	} else if (compPlays == 'B'){
		humanPlays = 'W';
		
		while (ValidMoves(board, dimension, compPlays) != 0 || ValidMoves(board, dimension, humanPlays) != 0){   //when moves are available
			
			if (ValidMoves(board, dimension, compPlays) == 0){   //if computer can't move
				printf("%c player has no valid move.\n", compPlays);
			} else {
				Minimax(board, dimension, compPlays, humanPlays);
				PrintBoard(board, dimension);
			}
			
			//Check again to avoid incorrect output error
			if (ValidMoves(board, dimension, compPlays) == 0 && ValidMoves(board, dimension, humanPlays) == 0)
				break;
				
			if (ValidMoves(board, dimension, humanPlays) == 0){   //if human can't move
				printf("%c player has no valid move.\n", humanPlays);
			} else {
				if (!TakeMoves(board, dimension, humanPlays)){   //execute if human move is valid
					printf("%c player wins.\n", compPlays);
					return 0;
				} else {
					PrintBoard(board, dimension);
				}
			}
		}
		
		//End of game
		if (FinalCount(board, dimension, compPlays) > FinalCount(board, dimension, humanPlays))
			printf("%c player wins.\n", compPlays);
		else if (FinalCount(board, dimension, humanPlays) > FinalCount(board, dimension, compPlays))
			printf("%c player wins.\n", humanPlays);
		else
			printf("Tie.\n");
	}
	
	return 0;
}

void Minimax(char board[MAXDIM][MAXDIM], int dimension, char compPlays, char humanPlays){
	int humanMoves = 0, compMoves = 0, minimax = -dimension * dimension;
	char imag[MAXDIM][MAXDIM];
	char row = '0', col = '0';
	
	//Configure imaginary board, and use only imag afterwards in this function
	for (int i = 0; i < dimension; i++)
		for (int j = 0; j < dimension; j++)
			imag[i][j] = board[i][j];

	//Heuristic shortcut to avoid the loop
	if (IsValid(imag, dimension, 'a', 'a', compPlays)){
		printf("Computer places %c at %c%c.\n", compPlays, 'a', 'a');
		Reverse(board, dimension, 'a', 'a', compPlays);
		return;
	} else if (IsValid(imag, dimension, 'a' + dimension - 1, 'a', compPlays)){
		printf("Computer places %c at %c%c.\n", compPlays, 'a' + dimension - 1, 'a');
		Reverse(board, dimension, 'a' + dimension - 1, 'a', compPlays);
		return;
	} else if (IsValid(imag, dimension, 'a', 'a' + dimension - 1, compPlays)){
		printf("Computer places %c at %c%c.\n", compPlays, 'a', 'a' + dimension - 1);
		Reverse(board, dimension, 'a', 'a' + dimension - 1, compPlays);
		return;
	} else if (IsValid(imag, dimension, 'a' + dimension -1 , 'a' + dimension - 1, compPlays)){
		printf("Computer places %c at %c%c.\n", compPlays, 'a' + dimension -1, 'a' + dimension - 1);
		Reverse(board, dimension, 'a' + dimension -1, 'a' + dimension - 1, compPlays);
		return;
	}
				
	//compMoves minus humanMoves, achieve overall max
	for (int i = 0; i < dimension; i++){
		for (int j = 0; j < dimension; j++){
			if (IsValid(imag, dimension, i + 'a', j + 'a', compPlays)){
				Reverse(imag, dimension, i + 'a', j + 'a', compPlays);
				humanMoves = ValidMoves(imag, dimension, humanPlays);
				
				//Heuristic shortcut to quit the loop
				if (humanMoves == 0){
					row = i + 'a';
					col = j + 'a';
					printf("Computer places %c at %c%c.\n", compPlays, row, col);
					Reverse(board, dimension, row, col, compPlays);
					return;
				}
				
				for (int m = 0; m < dimension; m++){
					for (int n = 0; n < dimension; n++){
						if (IsValid(imag, dimension, m + 'a', n + 'a', humanPlays)){
							Reverse(imag, dimension, m + 'a', n + 'a', humanPlays);
							compMoves = ValidMoves(imag, dimension, compPlays);
							if (compMoves - humanMoves > minimax){
								minimax = compMoves - humanMoves;
								row     = i + 'a';
								col     = j + 'a';
							}
						
						//reset imag
						for (int i = 0; i < dimension; i++)
							for (int j = 0; j < dimension; j++)
								imag[i][j] = board[i][j];
						Reverse(imag, dimension, i + 'a', j + 'a', compPlays);
						}
					}
				}
				
				//reset imag
				for (int i = 0; i < dimension; i++)
					for (int j = 0; j < dimension; j++)
						imag[i][j] = board[i][j];
			}
		}
	}

	printf("Computer places %c at %c%c.\n", compPlays, row, col);
	Reverse(board, dimension, row, col, compPlays);
}

void InitializeBoard(char board[MAXDIM][MAXDIM], int dimension){
	
	for (int a = 0; a < dimension; a++)
		for (int b = 0; b < dimension; b++)
			SetBoard(board, a + 'a', b + 'a', 'U');
	SetBoard(board, dimension/2 - 1 + 'a', dimension/2 - 1 + 'a', 'W');
	SetBoard(board, dimension/2 - 1 + 'a', dimension/2 + 'a', 'B');
	SetBoard(board, dimension/2 + 'a', dimension/2 - 1 + 'a', 'B');
	SetBoard(board, dimension/2 + 'a', dimension/2 + 'a', 'W');
}

void PrintBoard(char board[MAXDIM][MAXDIM], int dimension){
	printf("  ");//Formatting
	for (int i = 0; i < dimension; i++)
		printf("%c", 'a' + i);
	printf("\n");
	for (int a = 0; a < dimension; a++){
		printf("%c", 'a' + a);
		printf(" ");
		for (int b = 0; b < dimension; b++){
			printf("%c", board[a][b]);
		}
		printf("\n");
	}
}

int ValidMoves(char board[MAXDIM][MAXDIM], int dimension, char color){
	int result = 0;
	
	for (int i = 0; i < dimension; i++)
		for (int j = 0; j < dimension; j++)
			if (IsValid(board, dimension, i + 'a', j + 'a', color))
				result++;
	return result;
}

bool TakeMoves(char board[MAXDIM][MAXDIM], int dimension, char humanPlays){
	
	char row = '0', col = '0';
	printf("Enter move for colour %c (RowCol): ", humanPlays);
	scanf(" %c%c", &row, &col);
	
	//Check if human moves are valid
	if (!InBounds(dimension, row, col)){
		printf("Invalid move.\n");
		return false;
	}
	
	if(!IsValid(board, dimension, row, col, humanPlays)){
	printf("Invalid move.\n");
		return false;
	} else {
		Reverse(board, dimension, row, col, humanPlays);
	}
	
	return true;
}

int FinalCount(char board[MAXDIM][MAXDIM], int dimension, char color){
	int count = 0;
	
	for (int i = 0; i < dimension; i++)
		for (int j = 0; j < dimension; j++)
			if (board[i][j] == color)
				count++;
	return count;
}

void SetBoard(char board[MAXDIM][MAXDIM], char row, char col, char color){
	board[row - 'a'][col - 'a'] = color;
}

void Reverse(char board[MAXDIM][MAXDIM], int dimension, char row, char col, char color){
	int dirX = 0, dirY = 0;
	
	//Flip in all directions
	FlipInDirection(board, dimension, row, col, color, dirX - 1, dirY - 1);
	FlipInDirection(board, dimension, row, col, color, dirX - 1, dirY);
	FlipInDirection(board, dimension, row, col, color, dirX - 1, dirY + 1);
	FlipInDirection(board, dimension, row, col, color, dirX, dirY - 1);
	FlipInDirection(board, dimension, row, col, color, dirX, dirY + 1);
	FlipInDirection(board, dimension, row, col, color, dirX + 1, dirY - 1);
	FlipInDirection(board, dimension, row, col, color, dirX + 1, dirY);
	FlipInDirection(board, dimension, row, col, color, dirX + 1, dirY + 1);
}

bool IsValid(char board[MAXDIM][MAXDIM], int dimension, char row, char col, char color){
	int dirX = 0, dirY = 0;

	//If the position already has a color then it's obviously not valid
	if (board[row - 'a'][col - 'a'] != 'U')
		return false;

	//Check in all directions
	return (CheckLegalInDirection(board, dimension, row, col, color, dirX - 1, dirY - 1) ||
			CheckLegalInDirection(board, dimension, row, col, color, dirX - 1, dirY)     ||
			CheckLegalInDirection(board, dimension, row, col, color, dirX - 1, dirY + 1) ||
			CheckLegalInDirection(board, dimension, row, col, color, dirX, dirY - 1)     ||
			CheckLegalInDirection(board, dimension, row, col, color, dirX, dirY + 1)     ||
			CheckLegalInDirection(board, dimension, row, col, color, dirX + 1, dirY - 1) ||
			CheckLegalInDirection(board, dimension, row, col, color, dirX + 1, dirY)     ||
			CheckLegalInDirection(board, dimension, row, col, color, dirX + 1, dirY + 1));
}

bool CheckLegalInDirection(char board[MAXDIM][MAXDIM], int dimension, char row, char col, char color, int dirX, int dirY){
	//Loop by simply referring to dirX and dirY, not their actual values
	//Store initial value for second loop
	char tempRow = row, tempCol = col;
	
	//Color white
	if (color == 'W' && board[row - 'a' + dirY][col - 'a' + dirX] == 'B'){
		while (InBounds(dimension, row, col)){
			row += dirY;
			col += dirX;
			if (board[row - 'a'][col - 'a'] == 'U')   //If the sequence "breaks"
				return false;
			if (board[row - 'a'][col - 'a'] == color) //Continues the same color until "destination"
				return true;
		}
	}
	//Reset for the other color
	row = tempRow;
	col = tempCol;
	
	//Color black
	if (color == 'B' && board[row - 'a' + dirY][col - 'a' + dirX] == 'W'){
		while (InBounds(dimension, row, col)){
			row += dirY;
			col += dirX;
			if (board[row - 'a'][col - 'a'] == 'U')   //If the sequence "breaks"
				return false;
			if (board[row - 'a'][col - 'a'] == color) //Continues the same color until "destination"
				return true;
		}
	}
	
	return false;
}

void FlipInDirection(char board[MAXDIM][MAXDIM], int dimension, char row, char col, char color, int dirX, int dirY){
	//Similar to CheckLegal
	//Store initial value for second loop
	char tempRow = row, tempCol = col;
	if (color == 'W' && board[row - 'a' + dirY][col - 'a' + dirX] == 'B'){
		while (InBounds(dimension, row + dirY, col + dirX)){
			row += dirY;
			col += dirX;
			if (board[row - 'a'][col - 'a'] == color){   //check the other end
				int i = tempRow;
				int j = tempCol;
				while (i != row || j != col){   //Increase and flip all 
					SetBoard(board, i, j, color);
					i += dirY;
					j += dirX;
				}
			return;   //Only required to flip until first "self-color"
			}
		}
	}
	row = tempRow;
	col = tempCol;
	if (color == 'B' && board[row - 'a' + dirY][col - 'a' + dirX] == 'W'){
		while (InBounds(dimension, row + dirY, col + dirX)){
			row += dirY;
			col += dirX;
			if (board[row - 'a'][col - 'a'] == color){   //check the other end
				int i = tempRow;
				int j = tempCol;
				while (i != row || j != col){   //Increase and flip all 
					SetBoard(board, i, j, color);
					i += dirY;
					j += dirX;
				}
			return;   //Only required to flip until first "self-color"
			}
		}
	}
}

bool InBounds(int dimension, char row, char col){
	return (row - 'a' < dimension && col - 'a' < dimension && row - 'a' >= 0 && col - 'a' >= 0);
}
