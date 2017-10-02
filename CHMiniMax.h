#ifndef CHMINIMAX_H_
#define CHMINIMAX_H_

#include "CHMiniMaxNode.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>


/**
 * Given a game state, this function evaluates the best move according to
 * the current player. The function initiates a MiniMax algorithm with alpha beta pruning up to a
 * specified length given by maxDepth. The current game state doesn't change
 * by this function including the history of previous moves.
 *
 * @param currentGame - The current game state
 * @param maxDepth - The maximum depth of the miniMax algorithm
 * @return
 * -1 if either currentGame is NULL or maxDepth <= 0.
 * On success the function returns the best move
 * which is the best move for the current player.
 */
int* miniMaxPcMove(CH_Game* currentGame, unsigned int maxDepth);

#endif
