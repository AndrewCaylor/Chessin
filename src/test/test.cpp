#include <gtest/gtest.h>
#include <board.hpp>
#include <boardnode.hpp>

// TEST(Tests, test_get) {
//   Board board = Board();
//   ID id = board.boardGet(make_tuple(7, 6));
//   EXPECT_EQ(id, B_PAWN_8);
// }

// TEST(Tests, test_set_get) {
//   Board board = Board();
//   Location loc = make_tuple(1, 1);
//   for (int i = MIN_W_ID; i <= MAX_B_ID; i++)
//   {
//     board.boardSet(loc, toID(i));
//     EXPECT_EQ(board.boardGet(loc), toID(i));
//   }
// }

TEST(Tests, test_valid_pawn_move)
{
  Board board = Board();
  vector<Location> locs = board.listPossibleMoves(make_tuple(1, 1));

  EXPECT_EQ(locs.size(), 2);
  EXPECT_EQ(locs[0], make_tuple(1, 2));
  EXPECT_EQ(locs[1], make_tuple(1, 3));

  bool result = board.moveIfAble(make_tuple(make_tuple(1, 1), make_tuple(1, 2)));

  EXPECT_TRUE(result);
}

TEST(Tests, test_valid_rook_move)
{
  Board board = Board();
  bool result = board.moveIfAble(make_tuple(make_tuple(1, 1), make_tuple(1, 2)));
  EXPECT_TRUE(result);

  vector<Location> locs = board.listPossibleMoves(make_tuple(7, 0));
  cout << locs.size() << endl;

  EXPECT_EQ(locs.size(), 0);

  result = board.moveIfAble(make_tuple(make_tuple(1, 1), make_tuple(1, 2)));

  EXPECT_FALSE(result);
}

TEST(Tests, test_best_move)
{
  BoardData b = BoardData({"rnb.kbnr",
                           "pppppppp",
                           "........",
                           "........",
                           "........",
                           "q.......",
                           "P.PPPPPP",
                           "RNBQKBNR"});
  Board board = Board(b);
  BoardNode node = BoardNode(board, 0, 2, true);
  node.calc();

  Move move = node.bestMove();
  board.moveIfAble(move);
  cout << board.board.toString() << endl;
  EXPECT_EQ(move, make_tuple(make_tuple(1, 0), make_tuple(0, 2)));
}

TEST(Tests, in_check)
{
  {
    BoardData bd = BoardData({"rnb.kbnr",
                              "ppp.pppp",
                              "........",
                              "........",
                              "....q...",
                              "........",
                              "PPPP.PPP",
                              "RNBQKBNR"});

    Board b = Board(bd);

    EXPECT_TRUE(b.isInCheck(Piece::WHITE));
    EXPECT_FALSE(b.isInCheck(Piece::BLACK));
  }
  {
    BoardData bd = BoardData({"rnb.kbnr",
                              "ppp.pPpp",
                              "........",
                              "........",
                              "........",
                              "........",
                              "PPPP.PPP",
                              "RNBQKBNR"});

    Board b = Board(bd);

    EXPECT_TRUE(b.isInCheck(Piece::BLACK));
    EXPECT_FALSE(b.isInCheck(Piece::WHITE));
  }
}

TEST(Tests, test_string_conttructor)
{
  BoardData b = BoardData({"rnbqkbnr",
                           "pppppppp",
                           "........",
                           "........",
                           "........",
                           "........",
                           "PPPPPPPP",
                           "RNBQKBNR"});

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
