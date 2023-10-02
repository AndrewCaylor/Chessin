#include <tuple>
#include <string>
#include <vector>
#include <iostream>
#include <board.hpp>
#include <piece.hpp>

#define Empty ID_EMPTY
using namespace Piece;

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
  else{
    throw "Color cannot be NONE";
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

  x += incx;
  y += incy;
  while (x < 8 && y < 8 && x >= 0 && y >= 0)
  {
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
    x += incx;
    y += incy;
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
  if (color == NONE)
    throw "Color cannot be NONE";

  bool isWhite = color;

  // get king location
  Location kingLoc = this->pieces[isWhite * 16];

  // get all pieces of the opposite color
  int startID = (!isWhite) * 16;
  int endID = (!isWhite) * 16 + 16;

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
  this->pieces = vector<Location>(32);
  for (int i = 0; i < 8; i++)
  {
    this->pieces[i] = make_tuple(i, 0);
    this->pieces[i + 8] = make_tuple(i, 1);
    this->pieces[i + 16] = make_tuple(i, 7);
    this->pieces[i + 24] = make_tuple(i, 6);
  }

  this->board = BoardVec(8, RowVec(8, Empty));
  for(int id = MIN_W_ID; id <= MAX_B_ID; id++) {
    Location loc = this->pieces[id];
    this->boardSet(loc, toID(id));
  }
}

string Board::toString()
{
  string boardStr = "";
  for (int y = 7; y >= 0; y--)
  {
    for (int x = 0; x < 8; x++)
    {
      boardStr += Piece::toChar(this->boardGet(make_tuple(x, y)));
    }
    boardStr += " " + to_string(y + 1);
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
  this->board[get<1>(location)][get<0>(location)] = piece;
}

bool Board::move(Move move)
{
  Location from = get<0>(move);
  Location to = get<1>(move);

  ID piece = this->boardGet(from);
  this->boardSet(from, Empty);
  this->boardSet(to, piece);

  this->pieces[piece] = to;

  return true;
}

vector<Location> Board::listPossibleMoves(Location loc){

  ID piece = this->boardGet(loc);
  if (piece == Empty)
  {
    return {};
  }
  Type type = getType(piece);
  vector<Location> validMoves = {};
  switch (type)
  {
  case PAWN:
    validMoves = listPossiblePawnMoves(loc);
    break;
  case KNIGHT:
    validMoves = listPossibleKnightMoves(loc);
    break;
  case BISHOP:
    validMoves = listPossibleBishopMoves(loc);
    break;
  case ROOK:
    validMoves = listPossibleRookMoves(loc);
    break;
  case QUEEN:
    validMoves = listPossibleQueenMoves(loc);
    break;
  case KING:
    validMoves = listPossibleKingMoves(loc);
    break;
  default:
    break;
  }

  return validMoves;
}

bool Board::isValid(Move move)
{
  Location from = get<0>(move);
  Location to = get<1>(move);

  vector<Location> validMoves = listPossibleMoves(from);

  for (Location move : validMoves)
  {
    if (move == to)
    {
      return true;
    }
  }
  // cout << "Invalid. Valid moves are: " << endl;
  // for (Location move : validMoves)
  // {
  //   cout << (int) get<0>(move) << ", " << (int) get<1>(move) << endl;
  // }
  return false;
};

bool Board::moveIfAble(Move move){
  if (isValid(move))
  {
    Board::move(move);
    return true;
  }
  return false;
}