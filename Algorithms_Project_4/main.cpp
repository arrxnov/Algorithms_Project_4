#include <iostream>
#include <string>
#include <queue>
#include <map>

class Move
{
public:
	std::string board;
	std::string moveString;
	Move* parent;
};

bool isSolved(std::string board)
{
	for (int i = 5; i >= 0; i--)
	{
		if (board[i*6+3] > 1) return false;
		else if (board[i*6+3] == 1) return true;
		else continue;
	}
}

int main()
{
	using namespace std;
	
	string board, startingBoard;
	queue<string> moves;
	map<string, int> previousPositions;
	map<string, int> colorsToNums; // convenience really

	// read in all input

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
	} while (!moves.empty());
}