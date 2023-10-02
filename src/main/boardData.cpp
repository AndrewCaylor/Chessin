#include <boardData.hpp>

ID BoardData::getID(Location location)
{
  if(get<0>(location) >= 8 || get<1>(location) >= 8 || get<0>(location) < 0 || get<1>(location) < 0){
    throw runtime_error("GetID: Invalid location");
  }

  return board[get<1>(location)][get<0>(location)];
}

Location BoardData::getLocation(ID id)
{
  if(id < MIN_W_ID || id > MAX_B_ID) {
    throw runtime_error("GetLocation: Invalid ID");
  }
  return pieces[id];
}

void BoardData::setPiece(Location location, ID piece)
{
  board[get<1>(location)][get<0>(location)] = piece;
  if(piece == ID_EMPTY) {
    return;
  }
  pieces[piece] = location;
}

void BoardData::movePiece(Move move)
{
  Location from = get<0>(move);
  Location to = get<1>(move);

  if(get<0>(from) >= 8 || get<1>(from) >= 8){
    throw runtime_error("MovePiece: Invalid location (from)");
  }
  if(get<0>(to) >= 8 || get<1>(to) >= 8){
    throw runtime_error("MovePiece: Invalid location (to)");
  }

  ID piece = getID(from);

  ID capturedPiece = getID(to);
  if(capturedPiece != ID_EMPTY) {
    pieces[capturedPiece] = make_tuple(-1, -1);
  }
  setPiece(from, ID_EMPTY);
  setPiece(to, piece);

  pieces[piece] = to;
}

BoardData::BoardData(const BoardData &board)
{
  this->board = BoardVec(board.board);
  pieces = std::vector<Location>(board.pieces);
}

string BoardData::toString()
{
  std::string boardStr = "";
  for (int y = 7; y >= 0; y--)
  {
    for (int x = 0; x < 8; x++)
    {
      boardStr += Piece::toChar(getID(make_tuple(x, y)));
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
  pieces = vector<Location>(32);
  for (int i = 0; i < 8; i++)
  {
    pieces[i] = make_tuple(i, 0);
    pieces[i + 8] = make_tuple(i, 1);
    pieces[i + 16] = make_tuple(i, 7);
    pieces[i + 24] = make_tuple(i, 6);
  }

  board = BoardVec(8, RowVec(8, ID_EMPTY));
  for(int id = MIN_W_ID; id <= MAX_B_ID; id++) {
    Location loc = pieces[id];
    setPiece(loc, toID(id));
  }
}

bool BoardData::isValidLocation(Location location)
{
  int x = get<0>(location);
  int y = get<1>(location);
  return x >= 0 && x < 8 && y >= 0 && y < 8;
}
