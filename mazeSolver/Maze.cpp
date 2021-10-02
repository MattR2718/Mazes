#include "Maze.h"


//Public
void Maze::generateMaze()
{

    for (int y = this->height-1; y > 0; y--) {
        if (this->maze[this->width - 1][y]) {
            this->endX = this->width - 1;
            this->endY = y;
            break;
        }
    }

    std::stack<std::pair<int, int>> mazeStack;

    //Start from Start
    mazeStack.push(std::make_pair(1, 1));
    maze[1][1] = 1;
    this->visited = 1;

    while (mazeStack.size() > 0) {
        //Check unvisited neighbours
        std::vector<int> neighbours;
        //Up
        if ((mazeStack.top().second > 1) && (!maze[mazeStack.top().first][mazeStack.top().second - 2])) {
            neighbours.push_back(0);
        }
        //Right
        if ((mazeStack.top().first < this->width - 2) && (!maze[mazeStack.top().first + 2][mazeStack.top().second])) {
            neighbours.push_back(1);
        }
        //Down
        if ((mazeStack.top().second < this->height - 2) && (!maze[mazeStack.top().first][mazeStack.top().second + 2])) {
            neighbours.push_back(2);
        }
        //Left
        if ((mazeStack.top().first > 1) && (!maze[mazeStack.top().first - 2][mazeStack.top().second])) {
            neighbours.push_back(3);
        }

        if (!neighbours.empty()) {
            std::random_device rd;
            std::mt19937 mt(rd());
            std::uniform_int_distribution<int> dist(0, 301);
            int nextCellDir = neighbours[dist(mt) % neighbours.size()];

            //Make path
            switch (nextCellDir) {
                //Up
            case 0:
                this->maze[mazeStack.top().first][mazeStack.top().second - 1] = 1;
                this->maze[mazeStack.top().first][mazeStack.top().second - 2] = 1;
                mazeStack.push(std::make_pair((mazeStack.top().first), (mazeStack.top().second - 2)));
                break;
                //Right
            case 1:
                this->maze[mazeStack.top().first + 1][mazeStack.top().second] = 1;
                this->maze[mazeStack.top().first + 2][mazeStack.top().second] = 1;
                mazeStack.push(std::make_pair((mazeStack.top().first) + 2, (mazeStack.top().second)));
                break;
                //Down
            case 2:
                this->maze[mazeStack.top().first][mazeStack.top().second + 1] = 1;
                this->maze[mazeStack.top().first][mazeStack.top().second + 2] = 1;
                mazeStack.push(std::make_pair((mazeStack.top().first), (mazeStack.top().second + 2)));
                break;
                //Left
            case 3:
                this->maze[mazeStack.top().first - 1][mazeStack.top().second] = 1;
                this->maze[mazeStack.top().first - 2][mazeStack.top().second] = 1;
                mazeStack.push(std::make_pair((mazeStack.top().first) - 2, (mazeStack.top().second)));
                break;
            }
            this->visited++;
        }
        else {
            mazeStack.pop();
        }
    }
}

void Maze::solveMaze()
{
    bool b = recursiveSolve(startX, startY);
}


//Constructor and Destructor
Maze::Maze(int w, int h, int sX, int sY)
{
    this->width = w;
    this->height = h;

	//Initialise maze, path and correct oath with 0
	std::vector<bool> temp;
	for (int j = 0; j < this->height; j++) {
		temp.push_back(0);
	}
	for (int i = 0; i < this->width; i++) {
		this->maze.push_back(temp);
		this->wasHere.push_back(temp);
		this->correctPath.push_back(temp);
	}
	//Initialise start position
	this->startX = sX;
	this->startY = sY;

    this->endX = this->width - 1;
    this->endY = this->height - 1;
}

Maze::~Maze()
{
}


//Private

bool Maze::recursiveSolve(int x, int y)
{
    //Reached the end
    if (x == endX && y == endY) return 1;

    // If you are on a wall or already were here
    if (!maze[x][y] || wasHere[x][y]) return 0;

    wasHere[x][y] = 1;

    // Checks if not on left edge
    if (x != 0) {
        // Recalls method one to the left
        if (this->recursiveSolve((x - 1), y)) {
            // Sets that path value to true;
            correctPath[x][y] = 1;
            return 1;
        }
    }
    // Checks if not on right edge
    if (x != width - 1) {
        // Recalls method one to the right
        if (this->recursiveSolve((x + 1), y)) {
            correctPath[x][y] = 1;
            return 1;
        }
    }

    // Checks if not on top edge
    if (y != 0) {
        // Recalls method one up
        if (this->recursiveSolve(x, (y - 1))) {
            correctPath[x][y] = 1;
            return 1;
        }
    }

    // Checks if not on bottom edge
    if (y != height - 1) {
        // Recalls method one down
        if (this->recursiveSolve(x, (y + 1))) {
            correctPath[x][y] = 1;
            return 1;
        }
    }

    return 0;
}
