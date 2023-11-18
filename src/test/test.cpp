#include <gtest/gtest.h>
#include <boardData.hpp>
#include <board.hpp>

TEST(Tests, StringConstructor)
{
  BoardData b({"rnbqkbnr",
               "pppppppp",
               "........",
               "........",
               "........",
               "........",
               "PPPPPPPP",
               "RNBQKBNR"});

  std::cout << b.toString() << std::endl;

  EXPECT_EQ(b.toString(),
            "rnbqkbnr 8\n"
            "pppppppp 7\n"
            "........ 6\n"
            "........ 5\n"
            "........ 4\n"
            "........ 3\n"
            "PPPPPPPP 2\n"
            "RNBQKBNR 1\n"
            "\n"
            "abcdefgh\n");
}

string getView(BoardData &b, PieceColor color, bool doNewline = false)
{
  string out = "";
  for (int y = 7; y >= 0; y--)
  {
    for (int x = 0; x < 8; x++)
    {
      auto viewers = b.getViews(XY(x, y), color);
      if (viewers.size() > 0)
      {
        out += to_string(viewers.size());
      }
      else
      {
        out += ".";
      }
    }
    if (doNewline)
      out += "\n";
  }
  if (doNewline)
    out += "\n";
  return out;
}

int numMoves(Piece *piece)
{
  int moves = 0;
  for (Vector v : piece->moves)
  {
    moves += v.len;
  }
  return moves;
}

int amtVision(Piece *piece)
{
  int moves = 0;
  for (Vector v : piece->views)
  {
    moves += v.len;
  }
  return moves;
}

TEST(Tests, test_in_check)
{
  BoardData b({"....k...",
               "........",
               "........",
               "........",
               "....Rr..",
               "........",
               "........",
               "....K..."});

  BoardManager bm(b);
  EXPECT_TRUE(b.isInCheck(PieceColor::BLACK));
  EXPECT_FALSE(b.isInCheck(PieceColor::WHITE));
}

TEST(Tests, test_vision_king)
{
  BoardData b({"....k...",
               "........",
               "........",
               "........",
               "....R...",
               "........",
               "........",
               "....K..."});

  BoardManager bm(b);

  string expectedVisionBlack =
      "...1.1.."
      "...111.."
      "........"
      "........"
      "........"
      "........"
      "........"
      "........";

  string view = getView(b, PieceColor::BLACK);

  EXPECT_EQ(view, expectedVisionBlack);
}

TEST(Tests, test_vision_queen)
{
  BoardData b({"........",
               ".P......",
               "....K...",
               "........",
               "....q...",
               "........",
               "........",
               "k......."});

  BoardManager bu(b);

  string expectedVisionBlack =
      "........"
      ".1.....1"
      "..1.1.1."
      "...111.."
      "1111.111"
      "...111.."
      "111.1.1."
      ".2..1..1";

  string view = getView(b, PieceColor::BLACK);

  EXPECT_EQ(view, expectedVisionBlack);
}

TEST(Tests, visionKnight)
{
  BoardData b({"........",
               ".P......",
               "....K...",
               "........",
               "........",
               ".......n",
               "........",
               "k......."});

  BoardManager bm(b);

  string expectedVisionBlack =
      "........"
      "........"
      "........"
      "......1."
      ".....1.."
      "........"
      "11...1.."
      ".1....1.";

  string view = getView(b, PieceColor::BLACK);

  EXPECT_EQ(view, expectedVisionBlack);
}

TEST(Tests, visionPawn)
{
  BoardData b({"...k....",
               "........",
               "........",
               "........",
               "........",
               "....P...",
               ".......P",
               "K......."});

  BoardManager bm(b);

  string expectedVisionWhite =
      "........"
      "........"
      "........"
      "........"
      "...111.1"
      "......11"
      "11......"
      ".1......";

  string view = getView(b, PieceColor::WHITE);

  EXPECT_EQ(view, expectedVisionWhite);
}

TEST(Tests, movePiece)
{
  BoardData b({".....k..",
               "........",
               ".....r..",
               "........",
               "...R....",
               "........",
               "........",
               "....K..."});

  BoardManager bm(b);
  Piece *bRook = b.getPiece(XY(5, 5));

  // cout << "WHITE" << endl;
  // cout << getView(b, PieceColor::WHITE, true);
  // cout << "BLACK" << endl;
  // cout << getView(b, PieceColor::BLACK, true);

  bm.movePiece(bRook, XY(5, 3));

  EXPECT_EQ(b.toString(),
            ".....k.. 8\n"
            "........ 7\n"
            "........ 6\n"
            "........ 5\n"
            "...R.r.. 4\n"
            "........ 3\n"
            "........ 2\n"
            "....K... 1\n"
            "\n"
            "abcdefgh\n");

  string expectedVisionBlack =
      "....111."
      "....121."
      ".....1.."
      ".....1.."
      "...11.11"
      ".....1.."
      ".....1.."
      ".....1..";

  string expectedVisionWhite =
      "...1...."
      "...1...."
      "...1...."
      "...1...."
      "111.11.."
      "...1...."
      "...211.."
      "...2.1..";

  string viewBlack = getView(b, PieceColor::BLACK);
  EXPECT_EQ(viewBlack, expectedVisionBlack);

  string viewWhite = getView(b, PieceColor::BLACK);
  EXPECT_EQ(viewWhite, expectedVisionBlack);

  // cout << "WHITE" << endl;
  // cout << getView(b, PieceColor::WHITE, true);
  // cout << "BLACK" << endl;
  // cout << getView(b, PieceColor::BLACK, true);
}


TEST(Tests, movePieceKnight)
{
  BoardData b({".....k..",
               "........",
               "........",
               "........",
               "........",
               "......p.",
               "........",
               "K....P.N"});

  BoardManager bm(b);
  Piece *wKnight = b.getPiece(XY(7, 0));
  Piece *wPawn = b.getPiece(XY(5, 0));

  EXPECT_EQ(numMoves(wKnight), 2);

  bm.movePiece(wPawn, XY(5, 1));

  EXPECT_EQ(numMoves(wKnight), 1);
}

TEST(Tests, attacksPawn)
{
  BoardData b({"...k....",
               "........",
               "........",
               "...p....",
               ".....Pp.",
               "....P...",
               ".......P",
               "K......."});

  BoardManager bm(b);

  Piece *p1W = b.getPiece(XY(4, 2));
  EXPECT_EQ(numMoves(p1W), 1);

  bm.movePiece(p1W, XY(4, 3));
  EXPECT_EQ(numMoves(p1W), 2);

  Piece *p2W = b.getPiece(XY(7, 1));
  Piece *p1B = b.getPiece(XY(6, 3));
  EXPECT_EQ(numMoves(p2W), 2);

  bm.movePiece(p1B, XY(6, 2));
  EXPECT_EQ(numMoves(p2W), 3);

  // b-but en passant you didnt test for en passant
  // shut
}

TEST(Tests, attacksBishop)
{
  BoardData b({"...k....",
               "........",
               "........",
               "........",
               "...p....",
               "R.......",
               ".B......",
               "K.R....B"});

  BoardManager bm(b);
  Piece *bishop1 = b.getPiece(XY(1, 1));
  Piece *bishop2 = b.getPiece(XY(7, 0));
  Piece *pawn = b.getPiece(XY(3, 3));

  EXPECT_EQ(numMoves(bishop1), 2);

  bm.movePiece(bishop2, XY(2, 2));
  EXPECT_EQ(numMoves(bishop1), 0);

  bm.movePiece(bishop2, XY(1, 3));
  bm.movePiece(pawn, XY(7, 7));
  EXPECT_EQ(numMoves(bishop1), 6);
}

TEST(Tests, rookCapture)
{
  BoardData b({".....k..",
               "........",
               "R....r..",
               "........",
               "........",
               "........",
               "........",
               "....K..."});

  BoardManager bm(b);
  Piece *bRook = b.getPiece(XY(5, 5));
  Piece *wRook = b.getPiece(XY(0, 5));

  bm.movePiece(bRook, XY(0, 5));

  EXPECT_EQ(b.toString(),
            ".....k.. 8\n"
            "........ 7\n"
            "r....... 6\n"
            "........ 5\n"
            "........ 4\n"
            "........ 3\n"
            "........ 2\n"
            "....K... 1\n"
            "\n"
            "abcdefgh\n");

  string expectedVisionWhite =
      "........"
      "........"
      "........"
      "........"
      "........"
      "........"
      "...111.."
      "...1.1..";

  string expectedVisionBlack =
      "1...1.1."
      "1...111."
      ".1111111"
      "1......."
      "1......."
      "1......."
      "1......."
      "1.......";

  string viewBlack = getView(b, PieceColor::BLACK);
  EXPECT_EQ(viewBlack, expectedVisionBlack);

  string viewWhite = getView(b, PieceColor::WHITE);
  EXPECT_EQ(viewWhite, expectedVisionWhite);

  EXPECT_EQ(amtVision(wRook), 0);
  EXPECT_EQ(numMoves(wRook), 0);
}