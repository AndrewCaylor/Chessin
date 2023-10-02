#include <boardnode.hpp>

BoardNode::BoardNode(Board board, Move move, int depth, int maxDepth, bool whitesTurn){
  this->board = new Board(board);
  this->board->board.movePiece(move);
  this->depth = depth;
  this->maxDepth = maxDepth;
  this->whitesTurn = whitesTurn;
}

BoardNode::BoardNode(Board board, int depth, int maxDepth, bool whitesTurn){
  this->board = new Board(board);
  this->depth = depth;
  this->maxDepth = maxDepth;
  this->whitesTurn = whitesTurn;
}

BoardNode::~BoardNode(){
  delete this->board;
  for (size_t i = 0; i < this->possible.size(); i++)
  {
    delete this->possible[i];
  }
}

void BoardNode::calc(){
  // if we are at the max depth, just evaluate the board
  if (this->depth == this->maxDepth)
  {
    this->eval = this->board->getEval();
    return;
  }

  // find all possible moves for this color
  vector<Move> moves = board->findAllMoves(whitesTurn ? Piece::WHITE : Piece::BLACK);
  for (size_t i = 0; i < moves.size(); i++)
  {
    BoardNode *newNode = new BoardNode(*board, moves[i], depth + 1, maxDepth, !whitesTurn);
    newNode->calc();
    possible.push_back(newNode);
    evals.push_back(newNode->eval);
  }

  // find best move for this color, and set it to this->best
  float bestEval = this->whitesTurn ? -1000000 : 1000000;
  for (size_t i = 0; i < this->possible.size(); i++)
  {
    if (this->whitesTurn)
    {
      if (this->possible[i]->eval > bestEval)
      {
        bestEval = this->possible[i]->eval;
        this->best = moves[i];
      }
    }
    else
    {
      if (this->possible[i]->eval < bestEval)
      {
        bestEval = this->possible[i]->eval;
        this->best = moves[i];
      }
    }
  }

  this->eval = bestEval;
}

Move BoardNode::bestMove(){
  return this->best;
}