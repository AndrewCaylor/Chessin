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

TEST(Tests, test_valid_pawn_move) {
  Board board = Board();
  vector<Location> locs = board.listPossibleMoves(make_tuple(1, 1));

  EXPECT_EQ(locs.size(), 2);
  EXPECT_EQ(locs[0], make_tuple(1, 2));
  EXPECT_EQ(locs[1], make_tuple(1, 3));

  bool result = board.moveIfAble(make_tuple(make_tuple(1, 1), make_tuple(1, 2)));

  EXPECT_TRUE(result);
}

TEST(Tests, test_valid_rook_move) {
  Board board = Board();
  bool result = board.moveIfAble(make_tuple(make_tuple(1, 1), make_tuple(1, 2)));
  EXPECT_TRUE(result);

  vector<Location> locs = board.listPossibleMoves(make_tuple(7, 0));
  cout << locs.size() << endl;

  EXPECT_EQ(locs.size(), 0);

  result = board.moveIfAble(make_tuple(make_tuple(1, 1), make_tuple(1, 2)));

  EXPECT_FALSE(result);
}

TEST(Tests, test_best_move) {
  BoardNode node = BoardNode(Board(), 0, 2, true);
  node.calc();

  node.bestMove(); 
}


