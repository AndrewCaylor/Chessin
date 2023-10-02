#include <tuple>
#include <string>
#include <vector>
#include <iostream>
#include <piece.hpp>
#include <boardData.hpp>

#pragma once

class Board
{
private:
  BoardData board;

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

  bool move(Move move);

  bool isValid(Move move);

public:
  Board();
  Board(const Board& board);

  vector<Location> listPossibleMoves(Location loc);
  bool moveIfAble(Move move);

  BoardData getBoardData();

  float getEval();
};
