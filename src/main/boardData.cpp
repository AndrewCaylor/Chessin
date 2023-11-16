#include <boardData.hpp>
#include <piece.hpp>

Piece *BoardData::getPiece(Location loc)
{
  if (!isValidLocation(loc))
  {
    throw runtime_error("getPiece: Invalid location");
  }
  return board[loc.y][loc.x].piece;
}

void BoardData::setPiece(Location loc, Piece *piece)
{
  if (!isValidLocation(loc))
  {
    throw runtime_error("setPiece: Invalid location");
  }
  Square &square = board[loc.y][loc.x];

  // push whatever piece (or nullptr) was there onto the history stack
  square.pieceHistory.push(square.piece);
  // when a piece is removed from the board, remove lines of sight
  if (square.piece != nullptr)
  {
    removeVision(square.piece);
  }

  square.piece = piece;
  if (piece != nullptr)
    piece->location = loc;
}

void BoardData::revertSquare(Location loc)
{
  if (!isValidLocation(loc))
  {
    throw runtime_error("revertSquare: Invalid location");
  }
  Square &square = board[loc.y][loc.x];
  Piece *piece = square.pieceHistory.top();
  square.pieceHistory.pop();
  square.piece = piece;
}

string BoardData::toString()
{
  std::string boardStr = "";
  for (int y = 7; y >= 0; y--)
  {
    for (int x = 0; x < 8; x++)
    {
      Piece *piece = getPiece(XY(x, y));
      if (piece == nullptr)
      {
        boardStr += ".";
      }
      else
      {
        boardStr += piece->toChar();
      }
    }
    boardStr += " " + to_string(y + 1);
    boardStr += "\n";
  }
  boardStr += "\n";
  boardStr += "abcdefgh\n";
  return boardStr;
}

BoardData::BoardData()
{
  for (int y = 0; y < 8; y++)
  {
    vector<Square> row;
    for (int x = 0; x < 8; x++)
    {
      row.push_back(Square());
    }
    board.push_back(row);
  }
}

BoardData::~BoardData()
{
  for (size_t i = 0; i < whitePieces.size(); i++)
  {
    delete whitePieces[i];
  }
  for (size_t i = 0; i < blackPieces.size(); i++)
  {
    delete blackPieces[i];
  }
}

bool BoardData::isValidLocation(Location location)
{
  int x = location.x;
  int y = location.y;
  return x >= 0 && x < 8 && y >= 0 && y < 8;
}

BoardData::BoardData(vector<string> boardstr)
{
  for (int y = 0; y < 8; y++)
  {
    vector<Square> row;
    for (int x = 0; x < 8; x++)
    {
      row.push_back(Square());
    }
    board.push_back(row);
  }

  // technically constructing this upside down, but it doesn't matter
  for (int y = 0; y < 8; y++)
  {
    for (int x = 0; x < 8; x++)
    {
      const char c = boardstr[7 - y][x];
      if (c == '.')
      {
        continue;
      }

      Location loc = XY(x, y);
      Piece *piece = new Piece(c, loc);

      if (piece->type == KING)
      {
        if (piece->color == WHITE)
        {
          if (wKing != nullptr)
          {
            throw runtime_error("BoardData: Multiple white kings");
          }
          wKing = piece;
        }
        else
        {
          if (bKing != nullptr)
          {
            throw runtime_error("BoardData: Multiple black kings");
          }
          bKing = piece;
        }
      }

      if (piece->color == WHITE)
      {
        whitePieces.push_back(piece);
      }
      else
      {
        blackPieces.push_back(piece);
      }

      setPiece(loc, piece);
    }
  }

  if (wKing == nullptr)
  {
    throw runtime_error("BoardData: No white king");
  }
  if (bKing == nullptr)
  {
    throw runtime_error("BoardData: No black king");
  }
}

void BoardData::removeVision(Piece *piece, ViewInd viewInd, uint8_t start, uint8_t end)
{
  if (end == 255)
  {
    end = piece->views[viewInd].len;
  }

  if (piece->color == WHITE)
  {
    for (uint8_t j = start; j < end; j++)
    {
      Location loc = piece->views[viewInd][j];
      board[loc.y][loc.x].viewsMapWhite.erase(piece);
    }
  }
  else
  {
    for (uint8_t j = start; j < end; j++)
    {
      Location loc = piece->views[viewInd][j];
      board[loc.y][loc.x].viewsMapBlack.erase(piece);
    }
  }
}

void BoardData::setVision(Piece *piece, ViewInd viewInd, uint8_t start, uint8_t end)
{
  if (end == 255)
  {
    end = piece->views[viewInd].len;
  }

  if (piece->color == WHITE)
  {
    for (uint8_t j = start; j < end; j++)
    {
      Location loc = piece->views[viewInd][j];
      board[loc.y][loc.x].viewsMapWhite[piece] = viewInd;
    }
  }
  else
  {
    for (uint8_t j = start; j < end; j++)
    {
      Location loc = piece->views[viewInd][j];
      board[loc.y][loc.x].viewsMapBlack[piece] = viewInd;
    }
  }
}

void BoardData::removeVision(Piece *piece)
{
  if (piece == nullptr)
  {
    return;
  }

  vector<Vector> los = piece->views;
  for (size_t i = 0; i < los.size(); i++)
  {
    removeVision(piece, i);
    piece->views[i].len = 0;
    piece->moves[i].len = 0;
  }
}

void BoardData::setVision(Piece *piece)
{
  if (piece == nullptr)
  {
    return;
  }

  vector<Vector> los = piece->views;
  for (size_t i = 0; i < los.size(); i++)
  {
    setVision(piece, i);
  }
}

vector<tuple<Piece *, ViewInd>> BoardData::getViews(Location loc, PieceColor c)
{
  Square &square = board[loc.y][loc.x];
  if (c == WHITE)
  {
    return vector<tuple<Piece *, ViewInd>>(square.viewsMapWhite.begin(), square.viewsMapWhite.end());
  }
  else
  {
    return vector<tuple<Piece *, ViewInd>>(square.viewsMapBlack.begin(), square.viewsMapBlack.end());
  }
}

int BoardData::getNumViewers(Location loc, PieceColor c)
{
  Square &square = board[loc.y][loc.x];
  if (c == WHITE)
  {
    return square.viewsMapWhite.size();
  }
  else
  {
    return square.viewsMapBlack.size();
  }
}

bool BoardData::isInCheck(PieceColor color)
{
  Piece *king = color == WHITE ? wKing : bKing;
  return getNumViewers(king->location, !color) > 0;
}

bool BoardData::isCheckmated(PieceColor color)
{
  Piece *king = color == WHITE ? wKing : bKing;

  for (Vector vec : king->moves)
  {
    Location loc = vec[0];
    if (getPiece(loc) == nullptr)
    {
      return false;
    }
  }

  return true;
}