#include <tuple>
#include <string>
#include <vector>
#include <iostream>
#include <piece.hpp>
#include <boardData.hpp>

#pragma once

/**
 * Board class
 * This is essentially a wrapper around BoardData
 * It provides a bunch of useful functions for manipulating the board
*/
class Board
{
private:
  vector<Location> listPossiblePawnMoves(Location loc);

  vector<Location> listPossiblePawnAttacks(Location loc);

  vector<Location> listPossibleKnightMoves(Location loc);

  vector<Location> listPossibleBishopMoves(Location loc);

  vector<Location> listPossibleRookMoves(Location loc);

  vector<Location> listPossibleQueenMoves(Location loc);

  vector<Location> listPossibleKingMoves(Location loc);

  vector<Location> cast(Location loc, Color color, int incx, int incy);

  vector<Location> pruneMoves(Location loc, vector<Location> moves, Color color);

  bool canCapture(Location loc, Color color);

  bool isInCheck(Color color);

  bool move(Move move);

  bool isValid(Move move);

public:
  BoardData board;

  Board();
  Board(const BoardData &board);
  Board(const Board& board);

  vector<Location> listPossibleMoves(Location loc);
  vector<Move> findAllMoves(Color color);

  // use for CLI, not for AI
  bool moveIfAble(Move move);

  float getEval();
};
