#include <iostream>
#include <string>
#include <vector>
#include <list>

using namespace std;

const int gridSize = 3;
const int puzzleSize = gridSize * gridSize;
const int goalState[gridSize][gridSize] =
{
	0,1,2,
	3,4,5,
	6,7,8
};

string output;

struct Point
{
	int row;
	int col;

	Point()
	{
		row = 0;
		col = 0;
	}

	Point(int r, int c)
	{
		row = r;
		col = c;
	}

	Point indexToPoint(int index)
	{
		Point pos;

		for (int row = 0; row < gridSize; row++)
		{
			for (int col = 0; col < gridSize; col++)
			{
				if (goalState[row][col] == index)
				{
					pos.row = row;
					pos.col = col;
					break;
				}
			}
		}
		return pos;
	}

	int pointToIndex()
	{
		return row * gridSize + col;
	}
};

struct Tile
{
	int value;
	int goalValue;

	Tile()
	{
		value = -1;
		goalValue = -1;
	}
};

Tile puzzle[gridSize][gridSize];

// Store the puzzle state
class State
{
public:
	Tile grid[gridSize][gridSize];
	State* parent; // Pointer to parent node in the close set
	Point blankPos;
	vector<Point> neighbours;
	int gScore;
	int hScore;
	int fScore;

	State()
	{
		parent = NULL;
		blankPos = Point(-1, -1);
		gScore = 0;
		hScore = 0;
		fScore = INFINITY;
	}

	void setGrid(Tile inputGrid[gridSize][gridSize])
	{
		for (int row = 0; row < gridSize; row++)
		{
			for (int col = 0; col < gridSize; col++)
			{
				grid[row][col].value = inputGrid[row][col].value;
				grid[row][col].goalValue = inputGrid[row][col].goalValue;

				// Set the blank tile's position
				if (grid[row][col].value == 0)
				{
					blankPos.row = row;
					blankPos.col = col;
				}
			}
		}

		setNeighbour();
	}

	void slide(Point newPos)
	{
		grid[blankPos.row][blankPos.col].value = grid[newPos.row][newPos.col].value;
		grid[newPos.row][newPos.col].value = 0;
		blankPos = newPos;

		setNeighbour();
	}

	void updateScore()
	{
		// Calculate scores
		if (parent)
		{
			gScore = parent->gScore + 1;
		}

		calHScore();

		fScore = gScore + hScore;
	}

	void updateParent(State* state)
	{
		parent = state;
	}

	bool isStateSame(State compareState)
	{
		for (int row = 0; row < gridSize; row++)
		{
			for (int col = 0; col < gridSize; col++)
			{
				if (grid[row][col].value != compareState.grid[row][col].value) return false;
			}
		}
		return true;
	}

	bool isTarget()
	{
		return hScore == 0;
	}

	void debug(bool showExtra)
	{
		for (int row = 0; row < gridSize; row++)
		{
			for (int col = 0; col < gridSize; col++)
			{
				cout << grid[row][col].value;
			}
		}
		cout << "~~~" << endl;
		if (showExtra)
		{
			//printf("parent: index %i\n", parent);
			printf("blankPos: (%i,%i)\n", blankPos.row, blankPos.col);
			printf("neighbours: ");
			for (auto v : neighbours)
			{
				printf("(%i,%i)", v.row, v.col);
			}
			cout << endl;
			printf("gScore: %i\n", gScore);
			printf("hScore: %i\n", hScore);
			printf("fScore: %i\n", fScore);
			cout << "~~~~~~~~~" << endl;
		}
	}

private:
	void setNeighbour()
	{
		neighbours.clear();

		// Check left
		if (blankPos.col - 1 >= 0)
			neighbours.push_back(Point(blankPos.row, blankPos.col - 1));

		// Check right
		if (blankPos.col + 1 < gridSize)
			neighbours.push_back(Point(blankPos.row, blankPos.col + 1));

		// Check up
		if (blankPos.row - 1 >= 0)
			neighbours.push_back(Point(blankPos.row - 1, blankPos.col));

		// Check down
		if (blankPos.row + 1 < gridSize)
			neighbours.push_back(Point(blankPos.row + 1, blankPos.col));
	}

	void calHScore()
	{
		hScore = 0;
		int distance = 0;

		for (int row = 0; row < gridSize; row++)
		{
			for (int col = 0; col < gridSize; col++)
			{
				// find h score by finding the distance between current position and goal position (exclude blank)
				Point tileGoalPos = tileGoalPos.indexToPoint(grid[row][col].value);
				if (grid[row][col].value != 0)
					distance += findDistance(Point(row, col), tileGoalPos);
			}
		}

		hScore = distance;
	}

	int findDistance(Point currentPos, Point goalPos)
	{
		// Manhattan distance
		return abs(currentPos.row - goalPos.row) + abs(currentPos.col - goalPos.col);
	}
};

void getValues()
{
	// Get input
	int input;
	cout << "input puzzle = ";
	cin >> input;


	int inputArray[puzzleSize];

	// Get each digit from the input and store into array
	for (int i = 8; i >= 0; i--)
	{
		inputArray[i] = input % 10;
		input /= 10;
	}

	// Set the puzzle grid
	for (int row = 0; row < gridSize; row++)
	{
		for (int col = 0; col < gridSize; col++)
		{
			puzzle[row][col].value = inputArray[row * gridSize + col];
			puzzle[row][col].goalValue = goalState[row][col];
		}
	}
}

void storeMove(Point pos)
{
	char move = pos.pointToIndex() + 'A';
	output += move;
}

void reconstructPath(State* current)
{
	if (current->parent)
	{
		storeMove(current->blankPos);
		reconstructPath(current->parent);
	}
}

// A* search
void search()
{
	State currentState;
	State nextState;
	list<State> openSet;
	list<State> closeSet;
	list<State>::iterator iter;

	// Use the input as starting state
	State startState;
	startState.setGrid(puzzle);
	startState.updateScore();
	openSet.push_back(startState);

	while (!openSet.empty())
	{
		// Get the state with lowest f score
		currentState = openSet.front();
		list<State>::iterator lowest = openSet.begin(); // Store a iterator of the lowest f score state
		for (iter = openSet.begin(); iter != openSet.end(); ++iter)
		{
			if (iter->fScore < currentState.fScore)
			{
				currentState = *iter;
				lowest = iter;
			}
		}

		// Add that state to close set and remove it from open set
		closeSet.push_back(currentState);
		openSet.erase(lowest);

		// If current state is target state
		if (currentState.isTarget())
			break;

		// Cheak all neighbours
		for (auto& neighbour : currentState.neighbours)
		{
			// Simulate the next move
			nextState.setGrid(currentState.grid);
			nextState.slide(neighbour);

			// Check if the neighbour is in close set already
			bool isInCloseSet = false;
			for (auto& state : closeSet)
			{
				if (nextState.isStateSame(state))
				{
					isInCloseSet = true;
					break;
				}
			}

			if (!isInCloseSet)
			{
				// Check if the neighbour is in open set already
				State* stateRef = NULL;
				for (auto& state : openSet)
				{
					if (nextState.isStateSame(state))
					{
						stateRef = &state;
						break;
					}
				}

				if (nextState.hScore < currentState.hScore || !stateRef)
				{
					iter = --closeSet.end(); // Get the last element in the close set
					nextState.updateParent(&*iter);
					nextState.updateScore();

					if (!stateRef)
						openSet.push_back(nextState);
					else
						stateRef->updateScore();
				}
			}
		}
	}

	reconstructPath(&closeSet.back());
}

void printOutput()
{
	// Revearse the output path
	for (int i = output.length() - 1; i >= 0; i--)
		cout << output[i];
}

int main(int argc, char** argv)
{
	getValues();
	search();
	printOutput();

	return 0;
}