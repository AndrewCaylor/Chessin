#include <piece.hpp>
#include <iostream>

char Piece::toChar()
{
  char c = ' ';
  switch (this->type)
  {
  case KING:
    c = 'K';
    break;
  case QUEEN:
    c = 'Q';
    break;
  case PAWN:
    c = 'P';
    break;
  case KNIGHT:
    c = 'N';
    break;
  case BISHOP:
    c = 'B';
    break;
  case ROOK:
    c = 'R';
    break;
  default:
    return '.';
  }

  if (this->color == BLACK)
  {
    c = c + 32;
  }
  return c;
}

Piece::Piece(PieceType type, PieceColor color, Location location)
{
  this->type = type;
  this->color = color;
  this->location = location;
  this->isOnBoard = true;
  this->views = {};
  this->moves = {};

  this->init(&views);
  this->init(&moves);
}

void Piece::init(vector<Vector> *vec)
{

  switch (this->type)
  {
  case QUEEN:
    this->value = 9;

    vec->push_back(Vector(&this->location, 0, 0, 1));
    vec->push_back(Vector(&this->location, 0, 0, -1));
    vec->push_back(Vector(&this->location, 0, 1, 0));
    vec->push_back(Vector(&this->location, 0, -1, 0));
    vec->push_back(Vector(&this->location, 0, 1, 1));
    vec->push_back(Vector(&this->location, 0, 1, -1));
    vec->push_back(Vector(&this->location, 0, -1, 1));
    vec->push_back(Vector(&this->location, 0, -1, -1));
    break;
  case KING:
    this->value = 0;
    vec->push_back(Vector(&this->location, 0, 0, 1));
    vec->push_back(Vector(&this->location, 0, 0, -1));
    vec->push_back(Vector(&this->location, 0, 1, 0));
    vec->push_back(Vector(&this->location, 0, -1, 0));
    vec->push_back(Vector(&this->location, 0, 1, 1));
    vec->push_back(Vector(&this->location, 0, 1, -1));
    vec->push_back(Vector(&this->location, 0, -1, 1));
    vec->push_back(Vector(&this->location, 0, -1, -1));

    break;
  case PAWN:
    this->value = 1;
    if (this->color == WHITE)
    {
      vec->push_back(Vector(&this->location, 0, 0, 1));
      vec->push_back(Vector(&this->location, 0, -1, 1));
      vec->push_back(Vector(&this->location, 0, 1, 1));
    }
    else
    {
      vec->push_back(Vector(&this->location, 0, 0, -1));
      vec->push_back(Vector(&this->location, 0, -1, -1));
      vec->push_back(Vector(&this->location, 0, 1, -1));
    }

    break;
  case KNIGHT:
    this->value = 3;
    vec->push_back(Vector(&this->location, 0, 1, 2));
    vec->push_back(Vector(&this->location, 0, 1, -2));
    vec->push_back(Vector(&this->location, 0, -1, 2));
    vec->push_back(Vector(&this->location, 0, -1, -2));
    vec->push_back(Vector(&this->location, 0, 2, 1));
    vec->push_back(Vector(&this->location, 0, 2, -1));
    vec->push_back(Vector(&this->location, 0, -2, 1));
    vec->push_back(Vector(&this->location, 0, -2, -1));

    break;
  case BISHOP:
    this->value = 3;
    vec->push_back(Vector(&this->location, 0, 1, 1));
    vec->push_back(Vector(&this->location, 0, 1, -1));
    vec->push_back(Vector(&this->location, 0, -1, 1));
    vec->push_back(Vector(&this->location, 0, -1, -1));

    break;
  case ROOK:
    this->value = 5;
    vec->push_back(Vector(&this->location, 0, 0, 1));
    vec->push_back(Vector(&this->location, 0, 0, -1));
    vec->push_back(Vector(&this->location, 0, 1, 0));
    vec->push_back(Vector(&this->location, 0, -1, 0));

    break;
  default:
    throw std::runtime_error("Invalid piece type");
  }
}

Piece::Piece(char c, Location location)
{
  this->location = location;
  this->isOnBoard = true;
  this->views = {};
  this->moves = {};

  if (c >= 'a' && c <= 'z')
  {
    this->color = BLACK;
  }
  else
  {
    this->color = WHITE;
    c = c + 32;
  }

  switch (c)
  {
  case 'k':
    this->type = KING;
    break;
  case 'q':
    this->type = QUEEN;
    break;
  case 'p':
    this->type = PAWN;
    break;
  case 'n':
    this->type = KNIGHT;
    break;
  case 'b':
    this->type = BISHOP;
    break;
  case 'r':
    this->type = ROOK;
    break;
  default:
    throw std::runtime_error("Invalid piece type");
  }

  this->init(&views);
  this->init(&moves);
}

Square::Square()
{
  this->piece = nullptr;
  this->viewsMapBlack = {};
  this->viewsMapWhite = {};
}

Location util::XY(uint8_t x, uint8_t y)
{
  Location location;
  location.x = x;
  location.y = y;
  return location;
}

Move util::newMove(Location from, Location to)
{
  Move move;
  move.from = from;
  move.to = to;
  return move;
}

Vector::Vector(Location *start, u_int8_t len, int8_t incx, int8_t incy)
{
  this->start = start;
  this->len = len;
  this->incx = incx;
  this->incy = incy;
}

Location Vector::operator[](u_int8_t i)
{
  Location loc;
  loc.x = start->x + incx * (i + 1);
  loc.y = start->y + incy * (i + 1);
  return loc;
}

PieceColor operator!(PieceColor& c)
{
  return static_cast<PieceColor>(!static_cast<bool>(c));
}