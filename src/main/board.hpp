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
class BoardManager
{
private:
  void calcPawnVision(Piece* piece);
  void calcKVision(Piece* piece);

  vector<Location> kingCast(Piece* piece);

  void cast(uint8_t startInd, Piece *piece, ViewInd viewInd, uint8_t endInd = 255);
  void pawnCast(Piece *piece, ViewInd ind);

  /**
   * Update a view for a piece when another piece is moved
  */
  void updateView(Piece* piece, char viewInd);

  void createVision(Piece* piece);

  bool isValid(Move move);

  void cutView(Piece *piece, ViewInd viewInd, Piece *interpos);
  void extendView(Piece *piece, ViewInd viewInd, Location interpos);

  int getDistance(Location from, Location to);

public:
  BoardData board;

  BoardManager();
  BoardManager(const BoardData &board);

  bool isCheckmated(PieceColor color);

  void movePiece(Piece* piece, Location location);

  float getEval();
};
