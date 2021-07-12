# 8-Puzzle_Solver
My implementation of a 8-Puzzle Solver using A* search algorithm.

[![Dark Zone](https://img.youtube.com/vi/MXoERlO0tsA/0.jpg)](https://youtu.be/MXoERlO0tsA)

## How it works

The program uses A* algorithm to solve the puzzle. The g score each state is calculated by parent state’s total number of move + 1, while the heuristic value is the total Manhattan distance of all tiles from their current position to goal position, and f score is the sum of g score and h score.

The logic for the AI algorithm is as follows:

### 1.
There is a class call State, which stores the different states of the board/puzzle, the parent of that state, the blank’s position as well as its neighbors’ position, and also the g score, h score and f score of the state

### 2.
In the search function, two lists called openSet and closeSet is defined, and then the input puzzle is pushed into the openSet.

### 3.
Then, while the openSet is not empty, do the following:

▸ First, get the state with the lowest f score in the openSet, once find, set the currentState as that state, the push that state into the closeSet and remove it from the openSet.

▸ Check if the currentState is the same as the goal state (h score = 0), if yes, then quit out of the loop

▸ Then, check all the neighbors of the currentState (up, down, left, right), and swap the blank with the neighbor in each loop, and set nextState to that state.

▸ Then, check if nextState exists in closeSet already (already visited), and if true, skip this neighbor and move on to others.

▸ Else, check if the neighbour is in openSet already, if yes, get a reference of it. Then, check if the nextState’s h score is lower then currentState’s h score, or if it is in openSet already, if yes, then use the last element in the closeSet and set it as the parent of nextState. If nextState doesn’t exist in openSet, then push it into openSet, else just update the score of the reference.

### 4.
Once the search is complete, get the closeSet’s last element and backtrack to start using its parent, then convert the blank’s position into a char and store the path in a string.

### 5.
At last, print the string in reverse to get the move sequence.

## Code references
- Implementing A-star(A*) to solve N-Puzzle. Insight into programming algorithms. (2016, May 3). https://algorithmsinsight.wordpress.com/graph-theory-2/a-star-in-general/implementing-a-star-to-solve-n-puzzle/

- Solving 8-Puzzle using A* Algorithm https://blog.goodaudience.com/solving-8-puzzle-using-a-algorithm-7b509c331288

- A* Pathfinding (E01: algorithm explanation) https://youtu.be/-L-WgKMFuhE

- 8 puzzle problem using A* search algorithm in English\ Artificial intelligence tutorial English https://youtu.be/wJu3IZq1NFs
