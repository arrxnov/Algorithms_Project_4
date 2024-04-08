#include <iostream>
#include <string>
#include <queue>
#include <map>
#include <stack>

#define GRADEL_SUBMISSION 1

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
		if (board[12 + i] > '1')
		{
			return false;
		}
		else if (board[12 + i] == '1')
		{
			return true;
		}
		else continue;
	}
	return false;
}

int checkAbove(std::string& board, int i)
{
	// Get car above empty space //
	//    Move to car
	int spaceToCheck = i, dist = 0;
	while (board[spaceToCheck] == '0'
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
		board[spaceToCheck] = '0';
		board[spaceToCheck - 6] = '0';
		board[i] = truck;
		board[i - 6] = truck;

		if (spaceToCheck >= 12)
		{
			if (board[i] == board[spaceToCheck - 12])
			{
				// Zero out old extra
				board[spaceToCheck - 12] = '0';

				// Fill in new extra
				board[i - 12] = truck;
			}
		}
	}

	return dist;
}

int checkBelow(std::string& board, int i)
{
	// Get car below empty space //
	//    Move to car
	int spaceToCheck = i, dist = 0;
	while (board[spaceToCheck] == '0'
		&& spaceToCheck < 30
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
		board[spaceToCheck] = '0';
		board[spaceToCheck + 6] = '0';
		board[i] = truck;
		board[i + 6] = truck;

		if (spaceToCheck < 24)
		{
			if (board[i] == board[spaceToCheck + 12])
			{
				// Zero out old extra
				board[spaceToCheck + 12] = '0';

				// Fill in new extra
				board[i + 12] = truck;
			}
		}
	}

	return dist;
}

int checkLeft(std::string& board, int i)
{
	// Get car left of empty space //
	//    Move to car
	int spaceToCheck = i, dist = 0;
	while (board[spaceToCheck] == '0'
		&& spaceToCheck % 6 >= 1
		)
	{
		spaceToCheck--;
		dist++;
	}

	// Check orientation
	if (spaceToCheck % 6 >= 1 && board[spaceToCheck] == board[spaceToCheck - 1])
	{
		// Move cars
		int truck = board[spaceToCheck];
		board[spaceToCheck] = '0';
		board[spaceToCheck - 1] = '0';
		board[i] = truck;
		board[i - 1] = truck;

		if (spaceToCheck % 6 >= 2)
		{
			if (board[i] == board[spaceToCheck - 2])
			{
				// Zero out old extra
				board[spaceToCheck - 2] = '0';

				// Fill in new extra
				board[i - 2] = truck;
			}
		}
	}

	return dist;
}

int checkRight(std::string& board, int i)
{
	// Get car right of empty space //
	//    Move to car
	int spaceToCheck = i, dist = 0;
	while (board[spaceToCheck] == '0'
		&& spaceToCheck % 6 <= 4
		)
	{
		spaceToCheck++;
		dist++;
	}

	// Check orientation
	if (spaceToCheck % 6 <= 4 && board[spaceToCheck] == board[spaceToCheck + 1])
	{
		// Move cars
		int truck = board[spaceToCheck];
		board[spaceToCheck] = '0';
		board[spaceToCheck + 1] = '0';
		board[i] = truck;
		board[i + 1] = truck;

		if (spaceToCheck % 6 <= 3)
		{
			if (board[i] == board[spaceToCheck + 2])
			{
				// Zero out old extra
				board[spaceToCheck + 2] = '0';

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
	string board = "";
	for (int i = 0; i < 36; i++) board.append("0");
	queue<Move*> moves;
	map<string, int> previousPositions;
	map<char, string> colorsToNums;

	cin >> numCars;

	for (int i = 1; i <= numCars; i++)
	{
		string type, color;
		char orientation;
		int col, row;
		cin >> type >> color >> orientation >> row >> col;
		
		row--;
		col--;

		colorsToNums['0' + i] = color;

		if (type == "car")
		{
			board[row * 6 + col] = '0' + i;
			if (orientation == 'h')
			{
				board[row * 6 + col + 1] = '0' + i;
			}
			else if (orientation == 'v')
			{
				board[row * 6 + col + 6] = '0' + i;
			}
		}
		else if (type == "truck")
		{
			board[row * 6 + col] = '0' + i;
			if (orientation == 'h')
			{
				board[row * 6 + col + 1] = '0' + i;
				board[row * 6 + col + 2] = '0' + i;
			}
			else if (orientation == 'v')
			{
				board[row * 6 + col + 6] = '0' + i;
				board[row * 6 + col + 12] = '0' + i;
			}
		}
		else
		{
			cout << "[X] Malformed input on car " << i << endl;
			return 1;
		}
	}

	Move* currentMove = new Move(board, "", NULL);

	// Main loop, solving and adding moves
	do 
	{
		// Old position case
		
		if (previousPositions.find(board) != previousPositions.end())
		{
			if (moves.empty())
			{
				goto empty;
			}
			currentMove = moves.front();
			moves.pop();
			board = currentMove->board;
			continue;
		}
		else
		{
			previousPositions[board] = 1;
		}

		// Solution case

		if (isSolved(board))
		{
			stack<Move*> moveStack;
			// Add the solution move as final
			int d = 1;
			for (int i = 17; board[i] == '0'; i--) 
			{
				d++;
			}
			string finalString = colorsToNums[board[17 - d]];
			finalString.append(" ");
			finalString.append(to_string(d));
			finalString.append(" R");
			Move* finalMove = new Move("", finalString, currentMove);
			moveStack.push(finalMove);
			if (currentMove->moveString != "")
			{
				moveStack.push(currentMove);
			}

			while (currentMove->parent != NULL && currentMove->parent->parent != NULL)
			{
				currentMove = currentMove->parent;
				moveStack.push(currentMove);
			}

			if (moveStack.size() != 1)
			{
				cout << moveStack.size() << " moves";
			}
			else
			{
				cout << "1 move";
			}
			if (!GRADEL_SUBMISSION)
			{
				cout << ":";
			}
			cout << endl;

			while (!moveStack.empty())
			{
				currentMove = moveStack.top();
				moveStack.pop();
				if (!GRADEL_SUBMISSION)
				{
					cout << currentMove->moveString << endl;
				}
				delete currentMove;
			}

			return 0;
		}

		// Standard case

		for (int i = 0; i < 36; i++)
		{
			int left = 0, right = 0, up = 0, down = 0;
			string tempBoard;
			if (board[i] == '0')
			{
				int dist;
				tempBoard = board;
				dist = checkAbove(tempBoard, i);
				if (tempBoard != board)
				{
					string moveString = colorsToNums[tempBoard[i]];
					moveString.append(" ");
					moveString.append(to_string(dist));
					moveString.append(" D");
					Move* newMove = new Move(tempBoard, moveString, currentMove);
					moves.push(newMove);
					down++;
				}
				tempBoard = board;
				dist = checkBelow(tempBoard, i);
				if (tempBoard != board)
				{
					string moveString = colorsToNums[tempBoard[i]];
					moveString.append(" ");
					moveString.append(to_string(dist));
					moveString.append(" U");
					Move* newMove = new Move(tempBoard, moveString, currentMove);
					moves.push(newMove);
					up++;
				}
				tempBoard = board;
				dist = checkLeft(tempBoard, i);
				if (tempBoard != board)
				{
					string moveString = colorsToNums[tempBoard[i]];
					moveString.append(" ");
					moveString.append(to_string(dist));
					moveString.append(" R");
					Move* newMove = new Move(tempBoard, moveString, currentMove);
					moves.push(newMove);
					right++;
				}
				tempBoard = board;
				dist = checkRight(tempBoard, i);
				if (tempBoard != board)
				{
					string moveString = colorsToNums[tempBoard[i]];
					moveString.append(" ");
					moveString.append(to_string(dist));
					moveString.append(" L");
					Move* newMove = new Move(tempBoard, moveString, currentMove);
					moves.push(newMove);
					left++;
				}
			}
		}

		// Unsolvable case

		if (moves.empty())
		{
		empty:
			cout << "[X] No solution" << endl;
			return 1;
		}

		currentMove = moves.front();
		moves.pop();
		board = currentMove->board;

	} while (1);
}