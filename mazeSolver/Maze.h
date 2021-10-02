#pragma once

#include <vector>
#include <stack>
#include <random>

class Maze
{
public:
	//Public Functions
	void generateMaze();
	void solveMaze();

	//Constructor and Destructor
	Maze(int w, int h, int sX, int sY);
	virtual ~Maze();

	//Public Variables
	int endX;
	int endY;
	int visited = 0;
	std::vector<std::vector<bool>> maze;
	std::vector<std::vector<bool>> correctPath;

private:
	//Private Functions
	bool recursiveSolve(int x, int y);

	//Private Variables
	int width;
	int height;
	int startX;
	int startY;
	std::vector<std::vector<bool>> wasHere;
	

	
};

