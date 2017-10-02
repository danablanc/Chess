#ifndef CHMINIMAXNODE_H_
#define CHMINIMAXNODE_H_

#include "ChessGame.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <assert.h>

// MACROS
#define max(a,b) ((a)>=(b) ? (a) : (b)) //macro that returns the maximum between two numbers
#define min(a,b) ((a)<=(b) ? (a) : (b)) //macro that returns the minimum between two numbers

#define VEC {1,3,3,5,9,100} ////macro that defines the weight vector
#define ALLOC_ERROR_FLAG -2
#define ALLOC_ERROR_VAL 15000
#define NO_MOVES_FLAG -1


typedef struct { //structure that represent the node in the minimax tree
	int best_move[CELL_SIZE*CELL_SIZE]; //the best col to choose for the current node according to the minimax algorithm
	char player; //the player
	CH_Game* game_copy; //copy of the game with the moves represented by the node
} Node;


/**** The use of assertions checks in sub function in order to check null pointers is approved by Moab ****/


/**
 * Creates the root of the minimax tree and calls the recursion function due to the desired level
 * which is represented by the maxDepth argument
 *
 * @param currentGame - The source game
 * @param maxDepth - The level of the game - represents the maximum Depth of the minimax algorithm
 * @return
 * null - if the game is NULL or there was an allocation erroe
 * suggested move - otherwise
 * assertion error if currentGame == null
 */
int* minMax(CH_Game* currentGame, unsigned int maxDepth);

/**
 * the recursion function that computes the best col move the player according to the minimax algorithm
 *
 * @param currentGame - The source game
 * @param maxDepth - The level of the game - represents the maximum Depth of the minimax algorithm
 * @return
 * best move for the minimax algorithm
 * assertion error if src == null
 */
int minMaxRec(Node* src, int level, int maxDepth, bool maximize,int a,int b);

/**
 * Creates a new node for the minimax tree. The node contains copy of the game, which she gets from it's
 * parent in the minimax tree or if it's the root - a copy of the current game.
 * The node also contains the player and the best column (best move) for the current node
 *
 * @param currentGame - The game with the hypothetical move
 *
 * @return
 * NULL if either a memory allocation failure occurs
 * Otherwise, a new node instant is returned.
 */
Node* nodeCreate(CH_Game* src);

/**
 * Frees all memory allocation associated with a given node. If src==NULL
 * the function does nothing.
 *
 * @param src - the game represent by the node
 */
void nodeDestroy(Node* src);

/**
 *	Creates a copy of a given node.
 *	The new copy has the same status as the src node.
 *
 *	@param src - the source node which will be copied
 *	@return
 *	NULL if either src is NULL or a memory allocation failure occurred.
 *	Otherwise, an new copy of the source node is returned.
 *
 */
Node* nodeCopy(Node* src);

/**
 *	Computes the scoring function according to the weight vector defined
 *
 *	@param arr - the array that represents the game's span calculation.
 *	@return
 *	INT_MIN - if the player wins
 *	INT_MAX - if the computer wins
 *	the sum of the weight vector - otherwise
 *	assertion error if arr == null
 *
 */
int scoreFunction(CH_Game* src);

/**
 *	checks if the player who called the func wins
 *
 *	@param src - the root node.
 *	@return
 *	INT_MIN - if the player wins
 *	INT_MAX - if the computer wins
 *	0 - otherwise
 *	assertion error if src == null
 *
 */
int callerWin(CH_Game* src);

/**
 *	decides if the recursion function depth currently computes the maximum of the values or the minimum
 *
 *	@param a and b - two values that need to be compared
 *  @param maximize - boolean that represent whether the max of the two values need to be computed or the minimum
 *
 *	@return
 *	Max(a,b) - if maximize == true
 *  Min(a,b) - if maximize == false
 *
 */
int value(int a, int b, bool maximize);

#endif
