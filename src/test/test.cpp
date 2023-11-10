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
  return out;
}

TEST(Tests, test_in_check)
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
  EXPECT_TRUE(b.isInCheck(PieceColor::BLACK));
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

TEST(Tests, test_move)
{
  BoardData b = BoardData({"....k...",
                           "........",
                           "........",
                           "......r.",
                           "..R.....",
                           "........",
                           "........",
                           "....K..."});

  BoardManager bm = BoardManager(b);
  cout << getView(b, PieceColor::WHITE);
  cout << getView(b, PieceColor::BLACK);

  Piece *bRook = b.getPiece(XY(6, 4));
  bm.movePiece(bRook, XY(6, 3));

  // printView(b, PieceColor::WHITE);
  // printView(b, PieceColor::BLACK);

  EXPECT_TRUE(b.isInCheck(PieceColor::BLACK));
}
