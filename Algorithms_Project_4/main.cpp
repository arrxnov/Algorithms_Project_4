#include <iostream>
#include <string>
#include <queue>
#include <map>

//======================//
//        CLASSES       //
//======================//

class Move
{
public:
	std::string board;
	std::string moveString;
	Move* parent;
	Move() 
	{
		board = "";
		moveString = "";
		parent = NULL;
	};
	Move(std::string newBoard, std::string newMoveString, Move* newParent) : 
		board(newBoard),
		moveString(newMoveString),
		parent(newParent) {}
};

//======================//
//   HELPER FUNCTIONS   //
//======================//

bool isSolved(std::string board)
{
	for (int i = 5; i >= 0; i--)
	{
		if (board[18+i] > 1) return false;
		else if (board[18+i] == 1) return true;
		else continue;
	}
}

//======================//
//         MAIN         //
//======================//

int main()
{
	using namespace std;
	
	int numCars;

	string board = "", startingBoard = "";
	queue<Move> moves;
	Move currentMove;
	map<string, int> previousPositions;
	map<string, int> colorsToNums; // convenience really

	// read in all input
	cin >> numCars;

	for (int i = 1; i <= numCars; i++)
	{
		string type, color;
		char orientation;
		int col, row;
		cin >> type >> color >> orientation >> col >> row;

		// Insert color into the convenience map
		colorsToNums.insert(pair<string, int>(color, i));

		// Insert automobile onto the board
		if (type == "car")
		{
			board[row * 6 + col] = i;
			if (orientation == 'h')
			{
				board[row * 6 + col + 1] = i;
			}
			else if (orientation == 'v')
			{
				board[row * 6 + col + 6] = i;
			}
		}
		else if (type == "truck")
		{
			board[row * 6 + col] = i;
			if (orientation == 'h')
			{
				board[row * 6 + col + 6] = i;
				board[row * 6 + col + 12] = i;
			}
			else if (orientation == 'v')
			{
				board[row * 6 + col] = i;
			}
		}
		else
		{
			cout << "[X] Malformed input on car " << i << endl;
		}
	}

	// Main loop, solving and adding moves
	do 
	{
		if (isSolved(board))
		{
			// print the solution by going thru backards

			// exit gracefully
			return 0;
		}

		if (previousPositions.find(board) != previousPositions.end())
		{
			// if we've been here before skip it and move on
			continue;
		}

		// add derivative moves to queue
		moves.push(currentMove); // placeholder
		// blackMagic();

		// Set up for next loop
		currentMove = moves.front();
		moves.pop();
		board = currentMove.board;

	} while (!moves.empty());
}