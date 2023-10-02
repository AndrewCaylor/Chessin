#include <tuple>
#include <string>
#include <vector>
#include <iostream>
#include <board.hpp>
#include <piece.hpp>

#define Empty ID_EMPTY
using namespace Piece;
#define toID(i) static_cast<ID>(i)

vector<Location> Board::listPossiblePawnMoves(Location loc)
{
  vector<Location> validMoves = {};
  int x = get<0>(loc);
  int y = get<1>(loc);
  Color color = getColor(this->boardGet(loc));

  // TODO: handle promotion
  // TODO: handle en passant

  if (color == WHITE)
  {
    if (this->boardGet(make_tuple(x, y + 1)) == Empty)
    {
      validMoves.push_back(make_tuple(x, y + 1));

      if (y == 1 && this->boardGet(make_tuple(x, y + 2)) == Empty)
      {
        validMoves.push_back(make_tuple(x, y + 2));
      }
    }

    // handle captures
    ID piece = this->boardGet(make_tuple(x + 1, y + 1));
    if (piece != Empty && getColor(piece) != color)
    {
      validMoves.push_back(make_tuple(x + 1, y + 1));
    }

    piece = this->boardGet(make_tuple(x - 1, y + 1));
    if (piece != Empty && getColor(piece) != color)
    {
      validMoves.push_back(make_tuple(x - 1, y + 1));
    }
  }
  else if (color == BLACK)
  {
    if (this->boardGet(make_tuple(x, y - 1)) == Empty)
    {
      validMoves.push_back(make_tuple(x, y - 1));

      if (y == 6 && this->boardGet(make_tuple(x, y - 2)) == Empty)
      {
        validMoves.push_back(make_tuple(x, y - 2));
      }
    }

    // handle captures
    ID piece = this->boardGet(make_tuple(x + 1, y - 1));
    if (piece != Empty && getColor(piece) != color)
    {
      validMoves.push_back(make_tuple(x + 1, y - 1));
    }

    piece = this->boardGet(make_tuple(x - 1, y - 1));
    if (piece != Empty && getColor(piece) != color)
    {
      validMoves.push_back(make_tuple(x - 1, y - 1));
    }
  }

  return validMoves;
}

vector<Location> Board::listPossiblePawnAttacks(Location loc)
{
  vector<Location> validMoves = {};
  int x = get<0>(loc);
  int y = get<1>(loc);
  Color color = getColor(this->boardGet(loc));

  if (color == WHITE)
  {
    validMoves.push_back(make_tuple(x + 1, y + 1));
    validMoves.push_back(make_tuple(x - 1, y + 1));
  }
  else if (color == BLACK)
  {
    validMoves.push_back(make_tuple(x + 1, y - 1));
    validMoves.push_back(make_tuple(x - 1, y - 1));
  }
  return validMoves;
}

vector<Location> Board::listPossibleKnightMoves(Location loc)
{
  vector<Location> validMoves = {};
  int x = get<0>(loc);
  int y = get<1>(loc);
  validMoves.push_back(make_tuple(x + 1, y + 2));
  validMoves.push_back(make_tuple(x + 1, y - 2));
  validMoves.push_back(make_tuple(x - 1, y + 2));
  validMoves.push_back(make_tuple(x - 1, y - 2));
  validMoves.push_back(make_tuple(x + 2, y + 1));
  validMoves.push_back(make_tuple(x + 2, y - 1));
  validMoves.push_back(make_tuple(x - 2, y + 1));
  validMoves.push_back(make_tuple(x - 2, y - 1));

  // reject moves that are off the board
  for (size_t i = 0; i < validMoves.size(); i++)
  {
    Location move = validMoves[i];
    int x = get<0>(move);
    int y = get<1>(move);
    if (x < 0 || x >= 8 || y < 0 || y >= 8)
    {
      validMoves.erase(validMoves.begin() + i);
      i--;
    }
  }

  // reject moves that are blocked by a piece of the same color
  Color color = getColor(this->boardGet(loc));
  for (size_t i = 0; i < validMoves.size(); i++)
  {
    Location move = validMoves[i];
    ID piece = this->boardGet(move);
    if (getColor(piece) == color)
    {
      validMoves.erase(validMoves.begin() + i);
      i--;
    }
  }

  return validMoves;
}

vector<Location> Board::cast(Location loc, Color color, int incx, int incy)
{
  vector<Location> validMoves = {};
  int x = get<0>(loc);
  int y = get<1>(loc);

  while (x < 8 && y < 8 && x >= 0 && y >= 0)
  {
    x += incx;
    y += incy;
    ID piece = this->boardGet(make_tuple(x, y));
    if (piece != Empty)
    {
      if (color != getColor(piece))
      {
        validMoves.push_back(make_tuple(x, y));
      }
      break;
    }
    validMoves.push_back(make_tuple(x, y));
  }

  return validMoves;
}

vector<Location> Board::listPossibleBishopMoves(Location loc)
{
  vector<Location> validMoves = {};
  Color color = getColor(this->boardGet(loc));

  vector<Location> validMoves1 = this->cast(loc, color, 1, 1);
  vector<Location> validMoves2 = this->cast(loc, color, 1, -1);
  vector<Location> validMoves3 = this->cast(loc, color, -1, 1);
  vector<Location> validMoves4 = this->cast(loc, color, -1, -1);

  validMoves.insert(validMoves.end(), validMoves1.begin(), validMoves1.end());
  validMoves.insert(validMoves.end(), validMoves2.begin(), validMoves2.end());
  validMoves.insert(validMoves.end(), validMoves3.begin(), validMoves3.end());
  validMoves.insert(validMoves.end(), validMoves4.begin(), validMoves4.end());

  return validMoves;
}

vector<Location> Board::listPossibleRookMoves(Location loc)
{
  vector<Location> validMoves = {};
  Color color = getColor(this->boardGet(loc));

  vector<Location> validMoves1 = this->cast(loc, color, 1, 0);
  vector<Location> validMoves2 = this->cast(loc, color, -1, 0);
  vector<Location> validMoves3 = this->cast(loc, color, 0, 1);
  vector<Location> validMoves4 = this->cast(loc, color, 0, -1);

  validMoves.insert(validMoves.end(), validMoves1.begin(), validMoves1.end());
  validMoves.insert(validMoves.end(), validMoves2.begin(), validMoves2.end());
  validMoves.insert(validMoves.end(), validMoves3.begin(), validMoves3.end());
  validMoves.insert(validMoves.end(), validMoves4.begin(), validMoves4.end());

  return validMoves;
}

vector<Location> Board::listPossibleQueenMoves(Location loc)
{
  vector<Location> validMoves = {};
  vector<Location> bishopMoves = this->listPossibleBishopMoves(loc);
  vector<Location> rookMoves = this->listPossibleRookMoves(loc);

  validMoves.insert(validMoves.end(), bishopMoves.begin(), bishopMoves.end());
  validMoves.insert(validMoves.end(), rookMoves.begin(), rookMoves.end());
  return validMoves;
}

vector<Location> Board::listPossibleKingMoves(Location loc)
{
  vector<Location> validMoves = {};
  int x = get<0>(loc);
  int y = get<1>(loc);
  validMoves.push_back(make_tuple(x + 1, y + 1));
  validMoves.push_back(make_tuple(x + 1, y - 1));
  validMoves.push_back(make_tuple(x - 1, y + 1));
  validMoves.push_back(make_tuple(x - 1, y - 1));
  validMoves.push_back(make_tuple(x + 1, y));
  validMoves.push_back(make_tuple(x - 1, y));
  validMoves.push_back(make_tuple(x, y + 1));
  validMoves.push_back(make_tuple(x, y - 1));
  return validMoves;
}

// TODO (efficiency): process pinned pieces first, determine axis they are pinned on
// now we can prune the moves beforehand so we dont need to check them for every possiuble move
// idk this is explained poorly but you get tht eidea

bool Board::isInCheck(Color color)
{
  if(color == NONE) throw "Color cannot be NONE";

  bool isWhite = color;

  // get king location
  Location kingLoc = this->pieces[isWhite*16];

  // get all pieces of the opposite color
  int startID = (!isWhite)*16;
  int endID = (!isWhite)*16 + 16;

  for (int piece = startID; piece < endID; piece++)
  {
    Location loc = this->pieces[piece];

    vector<Location> moves = {};
    switch (getType(toID(piece)))
    {
    case PAWN:
      moves = this->listPossiblePawnAttacks(loc);
      break;
    case KNIGHT:
      moves = this->listPossibleKnightMoves(loc);
      break;
    case BISHOP:
      moves = this->listPossibleBishopMoves(loc);
      break;
    case ROOK:
      moves = this->listPossibleRookMoves(loc);
      break;
    case QUEEN:
      moves = this->listPossibleQueenMoves(loc);
      break;
    case KING:
      moves = this->listPossibleKingMoves(loc);
      break;
    default:
      continue;
    }
    
    for (Location move : moves)
    {
      if (move == kingLoc)
      {
        return true;
      }
    }
  }
  return false;
}

vector<Location> pruneMoves(Location loc, vector<Location> moves)
{
  vector<Location> validMoves = {};
  for (size_t i = 0; i < moves.size(); i++)
  {
    Location move = moves[i];
    int x = get<0>(move);
    int y = get<1>(move);
    if (x >= 0 && x < 8 && y >= 0 && y < 8)
    {
      validMoves.push_back(move);
    }
  }
  return validMoves;
}

Board::Board()
{
  this->board = BoardVec(8, RowVec(8, Empty));
  this->board[7] = {B_ROOK_1, B_KNIGHT_1, B_BISHOP_1, B_QUEEN, B_KING, B_BISHOP_1, B_KNIGHT_2, B_ROOK_2};
  this->board[6] = {B_PAWN_1, B_PAWN_2, B_PAWN_3, B_PAWN_4, B_PAWN_5, B_PAWN_6, B_PAWN_7, B_PAWN_8};

  this->board[1] = {W_PAWN_1, W_PAWN_2, W_PAWN_3, W_PAWN_4, W_PAWN_5, W_PAWN_6, W_PAWN_7, W_PAWN_8};
  this->board[0] = {W_ROOK_1, W_KNIGHT_1, W_BISHOP_1, W_QUEEN, W_KING, W_BISHOP_1, W_KNIGHT_2, W_ROOK_2};
}

string Board::toString()
{
  string boardStr = "";
  for (int i = 7; i >= 0; i--)
  {
    for (int j = 0; j < 8; j++)
    {
      boardStr += Piece::toChar(this->board[i][j]);
    }
    boardStr += " " + to_string(i + 1);
    boardStr += "\n";
  }
  boardStr += "\n";
  boardStr += "abcdefgh\n";
  return boardStr;
}

ID Board::boardGet(Location location)
{
  return this->board[get<1>(location)][get<0>(location)];
}

void Board::boardSet(Location location, ID piece)
{
  this->pieces[piece] = location;
  this->board[get<1>(location)][get<0>(location)] = piece;
}

bool Board::move(Move move)
{
  Location from = get<0>(move);
  Location to = get<1>(move);

  ID piece = this->boardGet(from);
  if (piece == Empty)
  {
    return false;
  }

  this->boardSet(from, Empty);
  this->boardSet(to, piece);

  return true;
}

bool Board::isValid(Move move)
{
  Location from = get<0>(move);
  // Location to = get<1>(move);

  ID piece = this->boardGet(from);
  if (piece == Empty)
  {
    return false;
  }
  return true;
};
