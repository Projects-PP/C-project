#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

typedef enum {
    EMPTY = 0,
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING
} PieceType;

typedef enum {
    COLOR_NONE = 0,
    WHITE = 1,
    BLACK = 2
} PieceColor;

typedef struct {
    PieceType type;
    PieceColor color;
} ChessPiece;

typedef struct {
    ChessPiece board[8][8];
    PieceColor turn;
    int castling_rights[3][2];
} ChessState;

typedef struct HistoryNode {
    ChessState state;
    struct HistoryNode* prev;
    struct HistoryNode* next;
} HistoryNode;

void setup_board(ChessState* state);
int is_move_valid(ChessState* state, int from_x, int from_y, int to_x, int to_y);
int is_king_checked(ChessState* state, PieceColor king_color);
int apply_move(ChessState* state, int from_x, int from_y, int to_x, int to_y);

void init_history(const ChessState* initial_state);
void push_history(const ChessState* state);
int undo_move(ChessState* state);
int redo_move(ChessState* state);
void free_history(void);

#endif // GAME_LOGIC_H
