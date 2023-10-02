#include <boardData.hpp>

ID BoardData::getID(Location location)
{
  if (get<0>(location) >= 8 || get<1>(location) >= 8 || get<0>(location) < 0 || get<1>(location) < 0)
  {
    throw runtime_error("GetID: Invalid location");
  }

  return board[get<1>(location)][get<0>(location)];
}

Location BoardData::getLocation(ID id)
{
  if (id < MIN_W_ID || id > MAX_B_ID)
  {
    throw runtime_error("GetLocation: Invalid ID");
  }
  return pieces[id];
}

void BoardData::setPiece(Location location, ID piece)
{
  board[get<1>(location)][get<0>(location)] = piece;
  if (piece == ID_EMPTY)
  {
    return;
  }
  pieces[piece] = location;
}

void BoardData::movePiece(Move move)
{
  Location from = get<0>(move);
  Location to = get<1>(move);

  if (get<0>(from) >= 8 || get<1>(from) >= 8)
  {
    throw runtime_error("MovePiece: Invalid location (from)");
  }
  if (get<0>(to) >= 8 || get<1>(to) >= 8)
  {
    throw runtime_error("MovePiece: Invalid location (to)");
  }

  ID piece = getID(from);

  ID capturedPiece = getID(to);
  if (capturedPiece != ID_EMPTY)
  {
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
  for (int id = MIN_W_ID; id <= MAX_B_ID; id++)
  {
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

BoardData::BoardData(vector<string> boardstr)
{
  board = BoardVec(8, RowVec(8, ID_EMPTY));
  pieces = vector<Location>(32);

  int numWhiteRooks = 0;
  int numWhiteKnights = 0;
  int numWhiteBishops = 0;
  int numWhiteQueens = 0;
  int numWhiteKings = 0;
  int numWhitePawns = 0;

  int numBlackRooks = 0;
  int numBlackKnights = 0;
  int numBlackBishops = 0;
  int numBlackQueens = 0;
  int numBlackKings = 0;
  int numBlackPawns = 0;

  // technically constructing this upside down, but it doesn't matter
  for (int y = 0; y < 8; y++)
  {
    for (int x = 0; x < 8; x++)
    {
      const char c = boardstr[7-y][x];
      Type type = toType(c);
      Color color;
      // if lowercase, black
      if (c >= 'a' && c <= 'z')
      {
        color = BLACK;
      }
      // if uppercase, white
      else
      {
        color = WHITE;
      }

      Location loc = make_tuple(x, y);

      switch (type)
      {
      case KING:
        if (color == WHITE)
        {
          numWhiteKings++;
          if (numWhiteKings < 1)
          {
            throw runtime_error("Invalid board: Too many white kings");
          }
          setPiece(loc, W_KING);
        }
        else
        {
          numBlackKings++;
          if (numBlackKings > 1)
          {
            throw runtime_error("Invalid board: Too many black kings");
          }
          setPiece(loc, B_KING);
        }
        break;
      case QUEEN:
        if (color == WHITE)
        {
          numWhiteQueens++;
          if (numWhiteQueens > 1)
          {
            throw runtime_error("Invalid board: Too many white queens");
          }
          setPiece(loc, W_QUEEN);
        }
        else
        {
          numBlackQueens++;
          if (numBlackQueens > 1)
          {
            throw runtime_error("Invalid board: Too many black queens");
          }
          setPiece(loc, B_QUEEN);
        }
        break;
      case PAWN:
        if (color == WHITE)
        {
          numWhitePawns++;
          if (numWhitePawns > 8)
          {
            throw runtime_error("Invalid board: Too many white pawns");
          }
          setPiece(loc, (ID)(W_PAWN_1 + numWhitePawns - 1));
        }
        else
        {
          numBlackPawns++;
          if (numBlackPawns > 8)
          {
            throw runtime_error("Invalid board: Too many black pawns");
          }
          setPiece(loc, (ID)(B_PAWN_1 + numBlackPawns - 1));
        }
        break;
      case KNIGHT:
        if (color == WHITE)
        {
          numWhiteKnights++;
          if (numWhiteKnights > 2)
          {
            throw runtime_error("Invalid board: Too many white knights");
          }
          setPiece(loc, numWhiteKnights == 1 ? W_KNIGHT_1 : W_KNIGHT_2);
        }
        else
        {
          numBlackKnights++;
          if (numBlackKnights > 2)
          {
            throw runtime_error("Invalid board: Too many black knights");
          }
          setPiece(loc, numBlackKnights == 1 ? B_KNIGHT_1 : B_KNIGHT_2);
        }
        break;
      case BISHOP:
        if (color == WHITE)
        {
          numWhiteBishops++;
          if (numWhiteBishops > 2)
          {
            throw runtime_error("Invalid board: Too many white bishops");
          }
          setPiece(loc, numWhiteBishops == 1 ? W_BISHOP_1 : W_BISHOP_2);
        }
        else
        {
          numBlackBishops++;
          if (numBlackBishops > 2)
          {
            throw runtime_error("Invalid board: Too many black bishops");
          }
          setPiece(loc, numBlackBishops == 1 ? B_BISHOP_1 : B_BISHOP_2);
        }
        break;
      case ROOK:
        if (color == WHITE)
        {
          numWhiteRooks++;
          if (numWhiteRooks > 2)
          {
            throw runtime_error("Invalid board: Too many white rooks");
          }
          setPiece(loc, numWhiteRooks == 1 ? W_ROOK_1 : W_ROOK_2);
        }
        else
        {
          numBlackRooks++;
          if (numBlackRooks > 2)
          {
            throw runtime_error("Invalid board: Too many black rooks");
          }
          setPiece(loc, numBlackRooks == 1 ? B_ROOK_1 : B_ROOK_2);
        }
        break;
      default:
        setPiece(loc, ID_EMPTY);
        break;
      }
    }
  }
}
