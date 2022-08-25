#ifndef SAXA_HPP_INCLUDED
#define SAXA_HPP_INCLUDED

/**            SAXA

 Sistema Artificial de Xadrez Acad�mico.
 The greatest chess engine of all time,

 XUXA built in,


 **/


/* a bot that knows how to play chess.*/

#include <cctype>
#include <iostream>
#include <cstring>
#include <math.h>
#include "Board.hpp"
#include "Piece.hpp"

#define IMESUARABLE 1
#define WORST_THING_POSSIBLE 0
#define BEST_THING_POSSIBLE 1


/*global variables*/

int pawnValue = 1;
int horseValue = 3;
int bishopValue = 3;
int rookValue = 5;
int queenValue = 9;
int kingValue = IMESUARABLE;

float squareValue = 0.1;

bool saxacolor = 0; // 0 para black, 1 para white

int saxaDepht;

typedef struct {
    double grade;
    int to;
    int from;
} saxa_move;


/* gives value to a instant position*/

double evaluatePosition(Board board);

/* returns the best move of a position based on depth*/

saxa_move finalMoveGrade(Board board, int depth);

/* test values for position*/

double moveGrade(Board board, int from, int to, int depth);

/* this function returns a value between zero and one*/

double sigmoid(double grade) {
    return 1/(1+ pow(2.718281828,- grade));
}


saxa_move finalMoveGrade(Board board, int depth){
    saxa_move best_move = {0,-1,-1};

    int movecounter = 0;

    for(int i = 0; i < 64; i++) {
        for(int j = 0; j < 64; j++) {

            if(board.move.list[i][j] == true) {
                movecounter++;
                double auxGrade = moveGrade(board,i,j, depth);

                if(board.state.whoMoves == PIECE_BLACK){
                    if(auxGrade > best_move.grade || movecounter == 1) {
                        best_move.grade = auxGrade;
                        best_move.from = i;
                        best_move.to = j;
                    }
                }else{
                    if(auxGrade < best_move.grade || movecounter == 1) {
                        best_move.grade = auxGrade;
                        best_move.from = i;
                        best_move.to = j;
                    }

                }
            }
        }
    }

    return best_move;
}

double moveGrade(Board board, int from, int to, int depth){


    BoardMakeMove(board, from, to, true);

    //buscando mate
    if(BoardKingInMate(board,PIECE_WHITE)){
        return BEST_THING_POSSIBLE;
    }else if(BoardKingInMate(board,PIECE_BLACK)){
        return WORST_THING_POSSIBLE;
    }



    if(depth > 0){

        return finalMoveGrade(board, depth - 1).grade;

    } else {

        return evaluatePosition(board);
    }
}


double evaluatePosition(Board board) {

    double grade = 0;
    int sinal;
    //contando material
    for(int i = 0; i < 8; i ++) {
        for(int j = 0; j < 8; j ++) {

            int pieceType  = PieceGetType(board.squares[i*8 + j]);
            int pieceColor = PieceGetColor(board.squares[i*8 + j]);
            sinal = (pieceColor == PIECE_BLACK) ? 1: -1;


            switch (pieceType) {

            case PIECE_BISHOP:
                grade += bishopValue*sinal;
                break;
            case PIECE_KNIGHT:
                grade += horseValue*sinal;
                break;
            case PIECE_PAWN:
                grade += pawnValue*sinal;
                break;
            case PIECE_QUEEN:
                grade += queenValue*sinal;
                break;
            case PIECE_ROOK:
                grade += rookValue*sinal;
            default:
                break;

            }
        }
    }
    //contando quadrados de ataque dos inimigos
    for(int i = 0; i < 64; i ++){
        if(board.move.pseudoLegalMoves[i] == true)
         if(board.state.whoMoves == PIECE_WHITE){
            grade += squareValue;
        }else if(board.state.whoMoves == PIECE_BLACK){
            grade -= squareValue;
        }
    }

     return sigmoid(grade);

}


#endif // SAXA_HPP_INCLUDED
