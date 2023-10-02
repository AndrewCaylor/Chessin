#include <board.hpp>

class BoardNode
{

private:
  Board *board;
  vector<BoardNode *> possible;
  vector<float> evals;
  BoardNode *prev;
  float eval;
  int depth;
  int maxDepth;
  bool whitesTurn;
  

  Move best;
public:
  BoardNode(Board board, int depth, int maxDepth, bool whitesTurn);
  BoardNode(Board board, Move move, int depth, int maxDepth, bool whitesTurn);

  ~BoardNode();
  void calc();
  Move bestMove();
};
