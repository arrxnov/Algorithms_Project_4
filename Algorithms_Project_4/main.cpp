#include <iostream>
#include <string>
#include <queue>
#include <map>
#include <stack>

#define GRADEL_SUBMISSION 0
#define DEBUG 1

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
	/*if (DEBUG)
	{
		for (int i = 0; i < 6; i++)
		{
			for (int j = 0; j < 6; j++) std::cout << board[i * 6 + j];
			std::cout << std::endl;
		}
	}*/
	
	for (int i = 5; i >= 0; i--)
	{
		if (board[18 + i] > '1')
		{
			if (DEBUG) std::cout << "[!] Unsolved" << std::endl;
			return false;
		}
		else if (board[18 + i] == '1')
		{
			if (DEBUG) std::cout << "[+] Solved!" << std::endl;
			return true;
		}
		else continue;
	}
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

		if (spaceToCheck >= 12 && spaceToCheck <= 24)
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
	// Get car above empty space //
	//    Move to car
	int spaceToCheck = i, dist = 0;
	while (board[spaceToCheck] == '0'
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
	// Get car above empty space //
	//    Move to car
	int spaceToCheck = i, dist = 0;
	while (board[spaceToCheck] == '0'
		&& spaceToCheck % 6 > 0
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
	// Get car above empty space //
	//    Move to car
	int spaceToCheck = i, dist = 0;
	while (board[spaceToCheck] == '0'
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
		board[spaceToCheck] = '0';
		board[spaceToCheck + 1] = '0';
		board[i] = truck;
		board[i + 1] = truck;

		if (spaceToCheck % 6 < 4)
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
	map<int, string> colorsToNums;

	// read in all input
	cin >> numCars;

	for (int i = 1; i <= numCars; i++)
	{
		string type, color;
		char orientation;
		int col, row;
		cin >> type >> color >> orientation >> row >> col;
		row--;
		col--;

		// Insert color into the convenience map
		colorsToNums.insert(pair<int, string>(i, color));

		// Insert automobile onto the board
		if (type == "car")
		{
			if (DEBUG) cout << color << " " << type << " at " << row << ", " << col << endl;
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
			if (DEBUG) cout << color << " " << type << " at " << row << ", " << col << endl;
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
			/*if (DEBUG) cout << "[!] Skipping previously seen board" << endl;*/
			if (moves.empty()) goto empty;
			currentMove = moves.front();
			moves.pop();
			board = currentMove->board;
			continue;
		}
		else
		{
			if (DEBUG) cout << "[+] New position!" << endl;
			previousPositions.insert(pair<string, int>(board, 1));
		}

		// add derivative moves to queue
		for (int i = 0; i < 36; i++)
		{
			if (board[i] == '0')
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
				else
				{
					/*if (DEBUG) cout << "[-] No down move for " << i << endl;*/
				}

				dist = checkBelow(tempBoard, i);
				if (tempBoard != board)
				{
					string moveString = "" + colorsToNums[board[i]] + " " + to_string(dist) + " U";
					Move* newMove = new Move(tempBoard, moveString, currentMove);
					moves.push(newMove);
				}
				else
				{
					/*if (DEBUG) cout << "[-] No up move for " << i << endl;*/
				}

				// dist = checkLeft(tempBoard, i);
				if (tempBoard != board)
				{
					string moveString = "" + colorsToNums[board[i]] + " " + to_string(dist) + " R";
					Move* newMove = new Move(tempBoard, moveString, currentMove);
					moves.push(newMove);
				}
				else
				{
					// if (DEBUG) cout << "[-] No right move for " << i << endl;
				}

				// dist = checkRight(tempBoard, i);
				if (tempBoard != board)
				{
					string moveString = "" + colorsToNums[board[i]] + " " + to_string(dist) + " L";
					Move* newMove = new Move(tempBoard, moveString, currentMove);
					moves.push(newMove);
				}
				else
				{
					// if (DEBUG) cout << "[-] No left move for " << i << endl;
				}
			}
			else
			{
				// if (DEBUG) cout << "[!] Space at " << i << " not empty" << endl;
			}
		}

		// Set up for next loop
		if (moves.empty())
		{
			empty:
			if (DEBUG)
			{
				cout << "[?] Finished queue without finishing puzzle" << endl;
				cout << "--- Was testing board: " << endl;
				for (int i = 0; i < 6; i++)
				{
					for (int j = 0; j < 6; j++) cout << board[i * 6 + j];
					cout << endl;
				}
			}
			return 1;
		}
		else
		{
			if (DEBUG) cout << "[+] Added moves!" << endl;
		}
		currentMove = moves.front();
		moves.pop();
		board = currentMove->board;

	} while (1);
}