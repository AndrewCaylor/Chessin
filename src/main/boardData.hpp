#include <vector>
#include <tuple>
#include <piece.hpp>
#include <iostream>
#include <string>
#include <piece.hpp>

#pragma once

using namespace std;
using namespace util;

typedef vector<vector<Square *>> Board;

class BoardData
{
private:
  Board board = {};

  // pinned pieces?? (for detecting in_check early)

public:
  vector<Piece *> whitePieces = {};
  vector<Piece *> blackPieces = {};

  Piece *wKing = nullptr;
  Piece *bKing = nullptr;

  BoardData();
  BoardData(vector<string> board);
  ~BoardData();

  string toString();

  Piece *getPiece(Location location);
  void setPiece(Location location, Piece *piece);

  vector<tuple<Piece *, ViewInd>> getViews(Location location, PieceColor c);
  int getNumViewers(Location location, PieceColor c);

  // used for backtracking during AI search
  void revertSquare(Location location);

  // before a piece is removed from the board, remove lines of sight
  void removeVision(Piece *piece);
  // after a piece is added to the board, add lines of sight
  void setVision(Piece *piece);

  void removeVision(Piece *piece, ViewInd viewInd, uint8_t start = 0, uint8_t end = 255);
  void setVision(Piece *piece, ViewInd viewInd, uint8_t start = 0, uint8_t end = 255);

  // check if a location is on the board
  static bool isValidLocation(Location location);

  bool isInCheck(PieceColor color);

  bool isCheckmated(PieceColor color);
};