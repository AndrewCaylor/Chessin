#include <vector>
#include <tuple>
#include <piece.hpp>
#include <iostream>
#include <string>

using namespace std;
using namespace Piece;
typedef vector<ID> RowVec;
typedef vector<RowVec> BoardVec;

typedef tuple<char, char> Location;
typedef tuple<Location, Location> Move;

class BoardData
{
private:
  BoardVec board = {};
  vector<Location> pieces;

public:
  BoardData();
  BoardData(const BoardData &board);
  string toString();

  ID getID(Location location);
  
  // getLocation may return -1, -1 if the piece is not on the board
  Location getLocation(ID id);
  void setPiece(Location location, ID piece);

  void movePiece(Move move);

  static bool isValidLocation(Location location);
};