#include <tuple>
#include <string>
#include <vector>
#include <iostream>
#include <board.hpp>

using namespace std;

#ifdef UNIT_TESTS
#define MAIN not_main
#else
#define MAIN main
#endif

int MAIN()
{

  Board board = Board();


  // let two players play the game, using command line input
  while (true)
  {
    cout << board.getBoardData().toString() << endl;

    cout << "Enter move: " ;
    string moveStr;
    cin >> moveStr;

    if (moveStr == "q")
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

    board.moveIfAble(move);
  }

  return 0;
}