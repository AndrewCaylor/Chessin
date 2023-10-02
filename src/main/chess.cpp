#include <tuple>
#include <string>
#include <vector>
#include <iostream>
#include <board.hpp>

using namespace std;

int main()
{

  Board board = Board();

  cout << board.toString() << endl;

  cout << 1 << endl;
  cout << ~1 << endl;

  // let two players play the game, using command line input
  while (true)
  {
    cout << "Enter move: ";
    string moveStr;
    cin >> moveStr;

    if (moveStr == "quit")
    {
      break;
    }

    // parse move string
    int fromX = moveStr[0] - 'a';
    int fromY = moveStr[1] - '1';
    int toX = moveStr[2] - 'a';
    int toY = moveStr[3] - '1';

    Location from = make_tuple(fromX, fromY);
    Location to = make_tuple(toX, toY);
    Move move = make_tuple(from, to);

    board.move(move);

    cout << board.toString() << endl;
  }

  return 0;
}