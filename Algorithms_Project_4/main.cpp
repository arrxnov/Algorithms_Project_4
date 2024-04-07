#include <iostream>
#include <string>
#include <queue>
#include <map>
#include <stack>

#define GRADEL_SUBMISSION 0

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

int checkAbove(std::string& board, int i)
{
	// Get car above empty space //
	//    Move to car
	int spaceToCheck = i, dist = 0;
	while (board[spaceToCheck] == 0
		&& spaceToCheck >= 6
		)
	{
		spaceToCheck -= 6;
		dist++;
	}

	// Check orientation
	if (spaceToCheck >= 6 && board[spaceToCheck] == board[spaceToCheck - 6])
	{
		// Move cars
		int truck = board[spaceToCheck];
		board[spaceToCheck] = 0;
		board[spaceToCheck - 6] = 0;
		board[i] = truck;
		board[i - 6] = truck;

		if (spaceToCheck >= 12 && spaceToCheck <= 24)
		{
			if (board[i] == board[spaceToCheck - 12])
			{
				// Zero out old extra
				board[spaceToCheck - 12] = 0;

				// Fill in new extra
				board[i - 12] = truck;
			}
		}
	}

	return dist;
}

int checkBelow(std::string& board, int i)
{
	// Get car above empty space //
	//    Move to car
	int spaceToCheck = i, dist = 0;
	while (board[spaceToCheck] == 0
		&& spaceToCheck <= 30
		)
	{
		spaceToCheck += 6;
		dist++;
	}

	// Check orientation
	if (spaceToCheck < 30 && board[spaceToCheck] == board[spaceToCheck + 6])
	{
		// Move cars
		int truck = board[spaceToCheck];
		board[spaceToCheck] = 0;
		board[spaceToCheck + 6] = 0;
		board[i] = truck;
		board[i + 6] = truck;

		if (spaceToCheck >= 12)
		{
			if (board[i] == board[spaceToCheck + 12])
			{
				// Zero out old extra
				board[spaceToCheck + 12] = 0;

				// Fill in new extra
				board[i + 12] = truck;
			}
		}
	}

	return dist;
}

int checkLeft(std::string& board, int i)
{
	// Get car above empty space //
	//    Move to car
	int spaceToCheck = i, dist = 0;
	while (board[spaceToCheck] == 0
		&& spaceToCheck % 6 > 0
		)
	{
		spaceToCheck--;
		dist++;
	}

	// Check orientation
	if (spaceToCheck % 6 > 1 && board[spaceToCheck] == board[spaceToCheck - 1])
	{
		// Move cars
		int truck = board[spaceToCheck];
		board[spaceToCheck] = 0;
		board[spaceToCheck - 1] = 0;
		board[i] = truck;
		board[i - 1] = truck;

		if (spaceToCheck % 6 > 2)
		{
			if (board[i] == board[spaceToCheck - 2])
			{
				// Zero out old extra
				board[spaceToCheck - 2] = 0;

				// Fill in new extra
				board[i - 2] = truck;
			}
		}
	}

	return dist;
}

int checkRight(std::string& board, int i)
{
	// Get car above empty space //
	//    Move to car
	int spaceToCheck = i, dist = 0;
	while (board[spaceToCheck] == 0
		&& spaceToCheck % 6 < 5
		)
	{
		spaceToCheck++;
		dist++;
	}

	// Check orientation
	if (spaceToCheck % 6 < 5 && board[spaceToCheck] == board[spaceToCheck - 1])
	{
		// Move cars
		int truck = board[spaceToCheck];
		board[spaceToCheck] = 0;
		board[spaceToCheck + 1] = 0;
		board[i] = truck;
		board[i + 1] = truck;

		if (spaceToCheck % 6 > 2)
		{
			if (board[i] == board[spaceToCheck + 2])
			{
				// Zero out old extra
				board[spaceToCheck + 2] = 0;

				// Fill in new extra
				board[i + 2] = truck;
			}
		}
	}

	return dist;
}

//======================//
//         MAIN         //
//======================//

int main()
{
	using namespace std;
	
	int numCars;
	string board = "", startingBoard = "";
	queue<Move*> moves;
	Move* currentMove = new Move("", "", NULL);
	map<string, int> previousPositions;
	map<int, string> colorsToNums;

	// read in all input
	cin >> numCars;

	for (int i = 1; i <= numCars; i++)
	{
		string type, color;
		char orientation;
		int col, row;
		cin >> type >> color >> orientation >> col >> row;

		// Insert color into the convenience map
		colorsToNums.insert(pair<int, string>(i, color));

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
			stack<Move*> moveStack;
			moveStack.push(currentMove);
			while (currentMove->parent != NULL)
			{
				currentMove = currentMove->parent;
				moveStack.push(currentMove);
			}

			if (moveStack.size() != 1) cout << moveStack.size() << " moves";
			else cout << "1 move";
			if (!GRADEL_SUBMISSION) cout << ":";
			cout << endl;

			while (!moveStack.empty())
			{
				currentMove = moveStack.top();
				moveStack.pop();
				if (!GRADEL_SUBMISSION) cout << currentMove->moveString << endl;
			}

			return 0;
		}

		if (previousPositions.find(board) != previousPositions.end())
		{
			// if we've been here before skip it and move on
			continue;
		}

		// add derivative moves to queue
		for (int i = 0; i < 36; i++)
		{
			if (board[i] == 0)
			{
				string tempBoard = board;
				int dist;
				dist = checkAbove(tempBoard, i);
				if (tempBoard != board)
				{
					string moveString = "" + colorsToNums[board[i]] + " " + to_string(dist) + " D";
					Move* newMove = new Move(tempBoard, moveString, currentMove);
					moves.push(newMove);
				}

				dist = checkBelow(tempBoard, i);
				if (tempBoard != board)
				{
					string moveString = "" + colorsToNums[board[i]] + " " + to_string(dist) + " U";
					Move* newMove = new Move(tempBoard, moveString, currentMove);
					moves.push(newMove);
				}

				// dist = checkLeft(tempBoard, i);
				if (tempBoard != board)
				{
					string moveString = "" + colorsToNums[board[i]] + " " + to_string(dist) + " R";
					Move* newMove = new Move(tempBoard, moveString, currentMove);
					moves.push(newMove);
				}

				// dist = checkRight(tempBoard, i);
				if (tempBoard != board)
				{
					string moveString = "" + colorsToNums[board[i]] + " " + to_string(dist) + " L";
					Move* newMove = new Move(tempBoard, moveString, currentMove);
					moves.push(newMove);
				}
				
			}
		}
		// blackMagic();

		// Set up for next loop
		currentMove = moves.front();
		moves.pop();
		board = currentMove->board;

	} while (!moves.empty());
}