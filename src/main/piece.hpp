#pragma once
#include <map>
#include <stack>
#include <vector>
#include <cstdint>
using namespace std;

typedef uint8_t ViewInd;

enum PieceType
{
  KING,
  QUEEN,
  PAWN,
  KNIGHT,
  BISHOP,
  ROOK,
};

enum PieceColor
{
  BLACK,
  WHITE,
};

PieceColor operator!(PieceColor& c);

struct Location
{
  char x;
  char y;
};

struct Move
{
  Location from;
  Location to;
};

class Vector
{
  Location *start;
  int8_t incx;
  int8_t incy;

public:
  uint8_t len;
  Vector(Location *start, uint8_t len, int8_t incx, int8_t incy);
  Location operator[](uint8_t i);
};

class Piece
{
  void init(vector<Vector> *views);

public:
  PieceType type;
  PieceColor color;
  Location location;
  int value;
  vector<Vector> views;
  vector<Vector> moves;
  bool isOnBoard;

  Piece(PieceType type, PieceColor color, Location location);
  Piece(char c, Location location);
  int getValue();
  char toChar();
};

namespace util
{
  Location XY(uint8_t x, uint8_t y);
  Move newMove(Location from, Location to);
}

/**
 * A square on the board.
 * Holds a piece. If the square is empty, piece is nullptr.
 * Holds a map of pieces that can see this square. Maps them to the index of the view they see it from.
 * Holds a stack of pieces that have been on this square. Used for backtracking.
 */
class Square
{
public:
  Piece *piece;
  std::map<Piece *, ViewInd> viewsMapWhite;
  std::map<Piece *, ViewInd> viewsMapBlack;
  std::stack<Piece *> pieceHistory;

  Square();
};
