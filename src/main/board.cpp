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
  Color color = getColor(board.getID(loc));

  // TODO: handle promotion
  // TODO: handle en passant

  if (color == WHITE)
  {
    Location oneup = make_tuple(x, y + 1);
    Location twoup = make_tuple(x, y + 2);
    Location leftside = make_tuple(x - 1, y + 1);
    Location rightside = make_tuple(x + 1, y + 1);

    bool canMoveOne = board.isValidLocation(oneup) && board.getID(oneup) == Empty;
    bool canMoveTwo = canMoveOne && y == 1 && board.isValidLocation(twoup) && board.getID(twoup) == Empty;
    bool canMoveLeft = board.isValidLocation(leftside) && board.getID(leftside) != Empty && getColor(board.getID(leftside)) != color;
    bool canMoveRight = board.isValidLocation(rightside) && board.getID(rightside) != Empty && getColor(board.getID(rightside)) != color;

    if (canMoveOne)
    {
      validMoves.push_back(oneup);
    }
    if (canMoveTwo)
    {
      validMoves.push_back(twoup);
    }
    if (canMoveLeft)
    {
      validMoves.push_back(leftside);
    }
    if (canMoveRight)
    {
      validMoves.push_back(rightside);
    }
  }
  else if (color == BLACK)
  {
    Location onedown = make_tuple(x, y - 1);
    Location twodown = make_tuple(x, y - 2);
    Location leftside = make_tuple(x - 1, y - 1);
    Location rightside = make_tuple(x + 1, y - 1);

    bool canMoveOne = board.isValidLocation(onedown) && board.getID(onedown) == Empty;
    bool canMoveTwo = canMoveOne && y == 6 && board.isValidLocation(twodown) && board.getID(twodown) == Empty;
    bool canMoveLeft = board.isValidLocation(leftside) && board.getID(leftside) != Empty && getColor(board.getID(leftside)) != color;
    bool canMoveRight = board.isValidLocation(rightside) && board.getID(rightside) != Empty && getColor(board.getID(rightside)) != color;

    if (canMoveOne)
    {
      validMoves.push_back(onedown);
    }
    if (canMoveTwo)
    {
      validMoves.push_back(twodown);
    }
    if (canMoveLeft)
    {
      validMoves.push_back(leftside);
    }
    if (canMoveRight)
    {
      validMoves.push_back(rightside);
    }
  }
  else
  {
    throw runtime_error("Invalid color");
  }

  return validMoves;
}

vector<Location> Board::listPossiblePawnAttacks(Location loc)
{
  vector<Location> validMoves = {};
  int x = get<0>(loc);
  int y = get<1>(loc);
  Color color = getColor(board.getID(loc));

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
    if (!BoardData::isValidLocation(move))
    {
      validMoves.erase(validMoves.begin() + i);
      i--;
    }
  }

  // reject moves that are blocked by a piece of the same color
  Color color = getColor(board.getID(loc));
  for (size_t i = 0; i < validMoves.size(); i++)
  {
    Location move = validMoves[i];
    ID piece = board.getID(move);
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
    ID piece = board.getID(make_tuple(x, y));
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
  Color color = getColor(board.getID(loc));

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
  Color color = getColor(board.getID(loc));

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

  // reject moves that are off the board
  for (size_t i = 0; i < validMoves.size(); i++)
  {
    Location move = validMoves[i];
    if (!BoardData::isValidLocation(move))
    {
      validMoves.erase(validMoves.begin() + i);
      i--;
    }
  }

  return validMoves;
}

// TODO (efficiency): process pinned pieces first, determine axis they are pinned on
// now we can prune the moves beforehand so we dont need to check them for every possiuble move
// idk this is explained poorly but you get tht eidea

bool Board::isInCheck(Color color)
{
  if (color == NONE)
    throw runtime_error("Invalid color");

  // get king location
  Location kingLoc;
  if (color == WHITE)
  {
    kingLoc = board.getLocation(W_KING);
  }
  else
  {
    kingLoc = board.getLocation(B_KING);
  }

  // get all pieces of the opposite color
  int startID;
  int endID;

  if (color == WHITE)
  {
    startID = MIN_B_ID;
    endID = MAX_B_ID;
  }
  else
  {
    startID = MIN_W_ID;
    endID = MAX_W_ID;
  }

  for (int piece = startID; piece <= endID; piece++)
  {
    Location loc = board.getLocation(toID(piece));
    bool exists = BoardData::isValidLocation(loc);

    if (!exists)
    {
      continue;
    }

    vector<Location> moves = listPossibleMoves(loc);

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

vector<Location> Board::pruneMoves(Location loc, vector<Location> moves, Color color)
{
  vector<Location> validMoves = {};
  for (size_t i = 0; i < moves.size(); i++)
  {
    // test out new move
    Location move = moves[i];
    ID coveredPiece = board.getID(move);

    board.movePiece(make_tuple(loc, move));
    bool inCheck = isInCheck(color);
    board.movePiece(make_tuple(move, loc));
    board.setPiece(move, coveredPiece);

    if (!inCheck)
    {
      validMoves.push_back(move);
    }
  }
  return validMoves;
}

Board::Board()
{
  this->board = BoardData();
}

vector<Location> Board::listPossibleMoves(Location loc)
{

  ID piece = board.getID(loc);
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
  return false;
};

bool Board::moveIfAble(Move move)
{
  if (isValid(move))
  {
    board.movePiece(move);
    return true;
  }
  return false;
}

Board::Board(const Board &board)
{
  this->board = board.board;
}

Board::Board(const BoardData &board)
{
  this->board = board;
}

float Board::getEval()
{
  bool isCheckmate = isCheckmated(WHITE);
  if (isCheckmate)
  {
    return -1000000;
  }
  isCheckmate = isCheckmated(BLACK);
  if (isCheckmate)
  {
    return 1000000;
  }

  // loop through all pieces and add up their values
  float eval = 0;
  for (int i = MIN_W_ID; i <= MAX_W_ID; i++)
  {

    Location loc = board.getLocation(toID(i));
    if (board.isValidLocation(loc))
    {
      eval += getValue(getType(toID(i)));
    }
  }

  for (int i = MIN_B_ID; i <= MAX_B_ID; i++)
  {
    Location loc = board.getLocation(toID(i));
    if (board.isValidLocation(loc))
    {
      eval -= getValue(getType(toID(i)));
    }
  }

  return eval;
}

bool Board::isCheckmated(Color c){
  bool inCheck = isInCheck(c);
  if (!inCheck){
    return false;
  }

  vector<tuple<Location, Location>> moves = findAllMoves(c);
  for (size_t i = 0; i < moves.size(); i++)
  {
    Board newBoard = Board(board);
    newBoard.moveIfAble(moves[i]);
    if (!newBoard.isInCheck(c)){
      return false;
    }
  }
}

vector<tuple<Location, Location>> Board::findAllMoves(Color color)
{
  vector<tuple<Location, Location>> moves = {};
  int startID = color * 16;
  int endID = color * 16 + 16;

  for (int piece = startID; piece < endID; piece++)
  {
    Location from = board.getLocation(toID(piece));
    bool exists = BoardData::isValidLocation(from);

    if (!exists)
    {
      continue;
    }

    vector<Location> possibleMoves = listPossibleMoves(from);

    // prune moves that are invalid
    possibleMoves = pruneMoves(from, possibleMoves, color);

    for (size_t i = 0; i < possibleMoves.size(); i++)
    {
      Location to = possibleMoves[i];
      moves.push_back(make_tuple(from, to));
    }
  }
  return moves;
}