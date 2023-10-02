#include <piece.hpp>

char Piece::toChar(ID piece)
{
  switch (piece)
  {
  case W_KING:
    return 'K';
  case W_QUEEN:
    return 'Q';
  case W_PAWN_1:
  case W_PAWN_2:
  case W_PAWN_3:
  case W_PAWN_4:
  case W_PAWN_5:
  case W_PAWN_6:
  case W_PAWN_7:
  case W_PAWN_8:
    return 'P';
  case W_KNIGHT_1:
  case W_KNIGHT_2:
    return 'N';
  case W_BISHOP_1:
  case W_BISHOP_2:
    return 'B';
  case W_ROOK_1:
  case W_ROOK_2:
    return 'R';
  case B_KING:
    return 'k';
  case B_QUEEN:
    return 'q';
  case B_PAWN_1:
  case B_PAWN_2:
  case B_PAWN_3:
  case B_PAWN_4:
  case B_PAWN_5:
  case B_PAWN_6:
  case B_PAWN_7:
  case B_PAWN_8:
    return 'p';
  case B_KNIGHT_1:
  case B_KNIGHT_2:
    return 'n';
  case B_BISHOP_1:
  case B_BISHOP_2:
    return 'b';
  case B_ROOK_1:
  case B_ROOK_2:
    return 'r';
  case ID_EMPTY:
    return '.';
  default:
    return '?';
  }
}

Piece::Color Piece::getColor(ID piece)
{
  if (piece == ID_EMPTY)
  {
    return NONE;
  }
  if (piece < 16)
  {
    return WHITE;
  }
  return BLACK;
}

Piece::Type Piece::getType(ID piece)
{
  piece = (ID)(piece % 16);
  switch (piece)
  {
  case W_KING:
    return KING;
  case W_QUEEN:
    return QUEEN;
  case W_PAWN_1:
  case W_PAWN_2:
  case W_PAWN_3:
  case W_PAWN_4:
  case W_PAWN_5:
  case W_PAWN_6:
  case W_PAWN_7:
  case W_PAWN_8:
    return PAWN;
  case W_KNIGHT_1:
  case W_KNIGHT_2:
    return KNIGHT;
  case W_BISHOP_1:
  case W_BISHOP_2:
    return BISHOP;
  case W_ROOK_1:
  case W_ROOK_2:
    return ROOK;
  default:
    return TYPE_EMPTY;
  }
}
