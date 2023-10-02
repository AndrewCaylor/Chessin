#include <tuple>
#include <string>
#include <vector>
#include <iostream>
#include <piece.hpp>

#pragma once

using namespace std;
using namespace Piece;
typedef vector<ID> RowVec;
typedef vector<RowVec> BoardVec;

typedef tuple<char, char> Location;
typedef tuple<Location, Location> Move;

class Board
{
private:
  BoardVec board = {};

  vector<Location> pieces;

  vector<Location> listPossiblePawnMoves(Location loc);

  vector<Location> listPossiblePawnAttacks(Location loc);

  vector<Location> listPossibleKnightMoves(Location loc);

  vector<Location> listPossibleBishopMoves(Location loc);

  vector<Location> listPossibleRookMoves(Location loc);

  vector<Location> listPossibleQueenMoves(Location loc);

  vector<Location> listPossibleKingMoves(Location loc);

  vector<Location> cast(Location loc, Color color, int incx, int incy);

  vector<Location> pruneMoves(Location loc, vector<Location> moves);

  bool isInCheck(Color color);

  bool canCapture(Location loc, Color color);

  vector<Location> findAllMoves(Color color);

public:
  Board();
  string toString();

  ID boardGet(Location location);

  void boardSet(Location location, ID piece);

  bool move(Move move);

  bool isValid(Move move);
};