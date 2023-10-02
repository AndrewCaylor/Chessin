#pragma once

namespace Piece
{
  enum ID
  {
    W_KING,
    W_QUEEN,
    W_PAWN_1,
    W_PAWN_2,
    W_PAWN_3,
    W_PAWN_4,
    W_PAWN_5,
    W_PAWN_6,
    W_PAWN_7,
    W_PAWN_8,
    W_KNIGHT_1,
    W_KNIGHT_2,
    W_BISHOP_1,
    W_BISHOP_2,
    W_ROOK_1,
    W_ROOK_2,

    B_KING,
    B_QUEEN,
    B_PAWN_1,
    B_PAWN_2,
    B_PAWN_3,
    B_PAWN_4,
    B_PAWN_5,
    B_PAWN_6,
    B_PAWN_7,
    B_PAWN_8,
    B_KNIGHT_1,
    B_KNIGHT_2,
    B_BISHOP_1,
    B_BISHOP_2,
    B_ROOK_1,
    B_ROOK_2,

    ID_EMPTY = -1
  };

  enum Type
  {
    KING,
    QUEEN,
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,

    TYPE_EMPTY = -1
  };

  enum Color
  {
    WHITE = 0,
    BLACK = 1,
    NONE = 2
  };

  char toChar(ID piece);
  
  Color getColor(ID piece);

  Type getType(ID piece);
}