#include <tuple>
#include <string>
#include <vector>
#include <iostream>
#include <board.hpp>
#include <piece.hpp>

void BoardManager::calcPawnVision(Piece *piece)
{
  // TODO: handle en passant

  for (ViewInd i = 0; i < piece->views.size(); i++)
  {
    pawnCast(piece, i);
  }
}

void BoardManager::pawnCast(Piece *piece, ViewInd ind)
{
  if (ind == 0)
  {
    int y = piece->location.y;
    bool isStarting = (piece->color == WHITE && y == 1) || (piece->color == BLACK && y == 6);

    Location oneup = piece->views[0][0];
    Location twoup = piece->views[0][1];

    bool canMoveOne = board.isValidLocation(oneup) && board.getPiece(oneup) == nullptr;
    bool canMoveTwo = canMoveOne && isStarting && board.getPiece(twoup) == nullptr;

    if (canMoveTwo)
    {
      piece->moves[0].len = 2;
      piece->views[0].len = 2;
    }
    else if (canMoveOne)
    {
      piece->moves[0].len = 1;
      if (isStarting)
      {
        piece->views[0].len = 2;
      }
      else
      {
        piece->views[0].len = 1;
      }
    }
    else
    {
      piece->moves[0].len = 0;
      piece->views[0].len = 1;
    }
  }
  else if (ind == 1 || ind == 2)
  {
    Location side = piece->views[ind][0];

    bool isValid = board.isValidLocation(side);
    bool canMove = isValid && board.getPiece(side) != nullptr && board.getPiece(side)->color != piece->color;

    piece->views[ind].len = isValid ? 1 : 0;
    piece->moves[ind].len = canMove ? 1 : 0;
  }
  else
  {
    throw std::runtime_error("Invalid view index");
  }
}

void BoardManager::calcKVision(Piece *piece)
{
  for (size_t i = 0; i < piece->views.size(); i++)
  {
    Location loc = piece->views[i][0];

    if (!BoardData::isValidLocation(loc))
    {
      piece->views[i].len = 0;
      piece->moves[i].len = 0;
      continue;
    }

    piece->views[i].len = 1;
    Piece *viewed = board.getPiece(loc);
    if (viewed == nullptr || viewed->color != piece->color)
    {
      piece->moves[i].len = 1;
    }
  }
}

/**
 * @brief Cast a piece's vision in a direction
 * Starts from "startInd" and goes until it hits a piece or the edge of the board
 * Updates the piece's "views" and "moves" vectors
 */
void BoardManager::cast(uint8_t startInd, Piece *piece, ViewInd viewInd, uint8_t endInd)
{
  Vector view = piece->views[viewInd];
  Location loc = view[startInd];
  int i = startInd;
  PieceColor color = piece->color;

  while (BoardData::isValidLocation(loc) && i <= endInd)
  {
    Piece *viewed = board.getPiece(loc);
    if (viewed != nullptr)
    {
      piece->views[viewInd].len = i + 1;
      if (color == viewed->color)
      {
        piece->moves[viewInd].len = i;
      }
      else
      {
        piece->moves[viewInd].len = i + 1;
      }
      return;
    }
    i++;
    loc = view[i];
  }

  piece->views[viewInd].len = i;
  piece->moves[viewInd].len = i;
}

// TODO (efficiency): process pinned pieces first, determine axis they are pinned on
// now we can prune the moves beforehand so we dont need to check them for every possiuble move
// idk this is explained poorly but you get tht eidea

BoardManager::BoardManager()
{
  this->board = BoardData();
}

int BoardManager::getDistance(Location from, Location to)
{
  int dx = to.x - from.x;
  int dy = to.y - from.y;
  return max(abs(dx), abs(dy));
}

void BoardManager::extendView(Piece *piece, ViewInd viewInd, Location interpos)
{
  PieceType type = piece->type;

  switch (type)
  {
  case KNIGHT:
    piece->moves[viewInd].len = 1;
    return;
  case KING:
    piece->moves[viewInd].len = 1;
    return;
  case PAWN:
    pawnCast(piece, viewInd);
    return;
  default:
    break;
  }

  int extendFromInd = getDistance(piece->location, interpos);
  cast(extendFromInd, piece, viewInd);
  board.setVision(piece, viewInd, extendFromInd);
}

void BoardManager::cutView(Piece *piece, ViewInd viewInd, Piece *interpos)
{
  PieceType type = piece->type;
  bool interposSameColor = interpos->color == piece->color;

  switch (type)
  {
  case KNIGHT:
  case KING:
    if (interposSameColor)
    {
      piece->moves[viewInd].len = 0;
    }
    return;
  case PAWN:
    pawnCast(piece, viewInd);
    return;
  default:
    break;
  }

  int cutInd = getDistance(piece->location, interpos->location);
  board.removeVision(piece, viewInd, cutInd);
  piece->views[viewInd].len = cutInd;

  // if the interposing piece is the same color as the piece, then we need to cut one more
  if (interposSameColor)
  {
    cutInd--;
  }
  piece->moves[viewInd].len = cutInd;
}

void BoardManager::createVision(Piece *piece)
{
  PieceType type = piece->type;
  switch (type)
  {
  case KNIGHT:
  case KING:
    calcKVision(piece);
    board.setVision(piece);

    break;
  case PAWN:
    calcPawnVision(piece);
    board.setVision(piece);

    break;
  default:
    for (size_t i = 0; i < piece->views.size(); i++)
    {
      cast(0, piece, i);
    }

    board.setVision(piece);
    break;
  }
}

BoardManager::BoardManager(const BoardData &board)
{
  this->board = board;

  for (size_t i = 0; i < board.whitePieces.size(); i++)
  {
    Piece *piece = board.whitePieces[i];
    createVision(piece);
  }

  for (size_t i = 0; i < board.blackPieces.size(); i++)
  {
    Piece *piece = board.blackPieces[i];
    createVision(piece);
  }
}

void BoardManager::movePiece(Piece *piece, Location loc)
{
  // remove the piece and its vision from the board
  board.setPiece(piece->location, nullptr);

  // recalculate vision for all pieces that can see this piece
  vector<tuple<Piece *, ViewInd>> fromViewersWhite = board.getViews(piece->location, PieceColor::WHITE);
  for (size_t i = 0; i < fromViewersWhite.size(); i++)
  {
    Piece *viewer = get<0>(fromViewersWhite[i]);
    char viewInd = get<1>(fromViewersWhite[i]);
    extendView(viewer, viewInd, piece->location);
  }
  vector<tuple<Piece *, ViewInd>> fromViewersBlack = board.getViews(piece->location, PieceColor::BLACK);
  for (size_t i = 0; i < fromViewersBlack.size(); i++)
  {
    Piece *viewer = get<0>(fromViewersBlack[i]);
    char viewInd = get<1>(fromViewersBlack[i]);
    extendView(viewer, viewInd, piece->location);
  }

  // also check around the old location for pieces that were

  // move the piece
  board.setPiece(loc, piece);
  createVision(piece);

  // recalculate vision for all pieces that can see this piece
  vector<tuple<Piece *, ViewInd>> toViewersWhite = board.getViews(piece->location, PieceColor::WHITE);
  for (size_t i = 0; i < toViewersWhite.size(); i++)
  {
    Piece *viewer = get<0>(toViewersWhite[i]);
    char viewInd = get<1>(toViewersWhite[i]);
    cutView(viewer, viewInd, piece);
  }
  vector<tuple<Piece *, ViewInd>> toViewersBlack = board.getViews(piece->location, PieceColor::BLACK);
  for (size_t i = 0; i < toViewersBlack.size(); i++)
  {
    Piece *viewer = get<0>(toViewersBlack[i]);
    char viewInd = get<1>(toViewersBlack[i]);
    cutView(viewer, viewInd, piece);
  }
}
