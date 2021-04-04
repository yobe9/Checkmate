#include "ass4.h"
//main
void main() {
    char board[SIZE][SIZE];
    // char fen[] = "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR";
    char fen[] = "rnbqkbnr/8/8/8/8/8/8/RNBQKBNR";
    char* pgn[] = {
        "Qe2+", "Qe7",
        "Rxa8", "Qxe2+",
        "Bxe2", "Rxh1",
        "Be3", "Bd6",
        "Nc3", "Bb7",
        "Ra7", "Bc6",
        "Kd2", "Ne7",
        "Bf3", "Rh2+",
        "Kd3", "Bd7",
        "Ra8", "Nc6",
        "Nb5", "Ne5+",
        "Kd4", "Bxb5",
        "Bf4", "Nxf3+",
        "Nxf3", "Bxf4",
        "Nxh2", "Bxh2",
        "Kc5", "Bd7",
        "Kb6", "Ke7",
        "Kb7", "Bc6+",
        "Kc8", "Nd7",
        "Ra7", "Ke8",
        "Ra2", "Nb6#"
    };

    int gameLength = sizeof(pgn) / sizeof(char*);
    // printBoardFromFEN(fen);
    createBoard(board, fen);
    printBoard(board);
    for (int i = 0; i < gameLength; i++) {
        makeMove(board, pgn[i], (i+1) % 2);
    }
}
