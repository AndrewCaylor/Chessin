#include <boardnode.hpp>

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