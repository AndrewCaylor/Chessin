#include <board.hpp>

class BoardNode
{
public:
    BoardNode(Board board, int depth, int maxDepth, bool whitesTurn);
    ~BoardNode();
    Board* board;
    vector<BoardNode*> possible;
    BoardNode *prev;
    int eval;
    int depth;
    int maxDepth;
    bool whitesTurn;
};
