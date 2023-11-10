#include <gtest/gtest.h>
#include <boardData.hpp>
#include <board.hpp>

TEST(Tests, test_string_constructor)
{
  BoardData b = BoardData({"rnbqkbnr",
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

string getView(BoardData b, PieceColor color, bool doNewline = false)
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
    if (doNewline) out += "\n";
  }
  if (doNewline) out += "\n";
  return out;
}

TEST(Tests, test_in_check)
{
  BoardData b = BoardData({"....k...",
                           "........",
                           "........",
                           "........",
                           "....Rr..",
                           "........",
                           "........",
                           "....K..."});

  BoardManager bm = BoardManager(b);
  EXPECT_TRUE(b.isInCheck(PieceColor::BLACK));
  EXPECT_FALSE(b.isInCheck(PieceColor::WHITE));
}

TEST(Tests, test_vision_king)
{
  BoardData b = BoardData({"....k...",
                           "........",
                           "........",
                           "........",
                           "....R...",
                           "........",
                           "........",
                           "....K..."});

  BoardManager bm = BoardManager(b);

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
  BoardData b = BoardData({"........",
                           ".P......",
                           "....K...",
                           "........",
                           "....q...",
                           "........",
                           "........",
                           "k......."});

  BoardManager bm = BoardManager(b);

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
  BoardData b = BoardData({"........",
                           ".P......",
                           "....K...",
                           "........",
                           "........",
                           ".......n",
                           "........",
                           "k......."});

  BoardManager bm = BoardManager(b);

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
  BoardData b = BoardData({"...k....",
                           "........",
                           "........",
                           "........",
                           "........",
                           "....P...",
                           ".......P",
                           "K......."});

  BoardManager bm = BoardManager(b);

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
  BoardData b = BoardData({".....k..",
                           "........",
                           ".....r..",
                           "........",
                           "...R....",
                           "........",
                           "........",
                           "....K..."});

  BoardManager bm = BoardManager(b);
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
