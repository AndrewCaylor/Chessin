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
  ~Piece();
  int getValue();
  char toChar();
};

namespace util
{
  Location XY(uint8_t x, uint8_t y);
  Move newMove(Location from, Location to);
}

class Square
{
public:
  bool occupied;
  Piece *piece;
  std::map<Piece *, ViewInd> viewsMapWhite;
  std::map<Piece *, ViewInd> viewsMapBlack;
  std::stack<Piece *> pieceHistory;

  Square();
};
