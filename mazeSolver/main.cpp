#include <SFML/Graphics.hpp>
#include <iostream>
#include <stack>
#include <vector>
#include <random>
#include <string>

const int width = 1000;
const int height = 800;
const int cellSize = 10;
bool maze[(width / cellSize)][(height / cellSize)];
bool wasHere[(width / cellSize)][(height / cellSize)];
bool correctPath[(width / cellSize)][(height / cellSize)];
int startX = 1;
int startY = 1;
int endX;
int endY;
bool screenshot = 0;
int visited = 0;

void generateMaze() {
    for (int x = 0; x < width / cellSize; x++) {
        for (int y = 0; y < height / cellSize; y++) {
            maze[x][y] = 0;
        }
    }

    std::stack<std::pair<int, int>> mazeStack;
    
    //Start from Start
    mazeStack.push(std::make_pair(1, 1));
    maze[1][1] = 1;
    visited = 1;

    while (mazeStack.size() > 0){
        //Check unvisited neighbours
        std::vector<int> neighbours;
        //Up
        if ((mazeStack.top().second > 1) && (!maze[mazeStack.top().first][mazeStack.top().second - 2])) {
            neighbours.push_back(0);
        }
        //Right
        if ((mazeStack.top().first < (width / cellSize) - 2) && (!maze[mazeStack.top().first + 2][mazeStack.top().second])) {
            neighbours.push_back(1);
        }
        //Down
        if ((mazeStack.top().second < (height / cellSize) - 2) && (!maze[mazeStack.top().first][mazeStack.top().second + 2])) {
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
                maze[mazeStack.top().first][mazeStack.top().second - 1] = 1;
                maze[mazeStack.top().first][mazeStack.top().second - 2] = 1;
                mazeStack.push(std::make_pair((mazeStack.top().first), (mazeStack.top().second - 2)));
                break;
                //Right
            case 1:
                maze[mazeStack.top().first + 1][mazeStack.top().second] = 1;
                maze[mazeStack.top().first + 2][mazeStack.top().second] = 1;
                mazeStack.push(std::make_pair((mazeStack.top().first) + 2, (mazeStack.top().second)));
                break;
                //Down
            case 2:
                maze[mazeStack.top().first][mazeStack.top().second + 1] = 1;
                maze[mazeStack.top().first][mazeStack.top().second + 2] = 1;
                mazeStack.push(std::make_pair((mazeStack.top().first), (mazeStack.top().second + 2)));
                break;
                //Left
            case 3:
                maze[mazeStack.top().first - 1][mazeStack.top().second] = 1;
                maze[mazeStack.top().first - 2][mazeStack.top().second] = 1;
                mazeStack.push(std::make_pair((mazeStack.top().first) - 2, (mazeStack.top().second)));
                break;
            }
            visited++;
        }
        else {
            mazeStack.pop();
        }
    }

}

bool recursiveSolve(int x, int y) {
    //Reached the end
    if (x == endX && y == endY) return 1;

    // If you are on a wall or already were here
    if (!maze[x][y] || wasHere[x][y]) return 0;

    wasHere[x][y] = 1;

    // Checks if not on left edge
    if (x != 0){
        // Recalls method one to the left
        if (recursiveSolve((x - 1), y)) {
            // Sets that path value to true;
            correctPath[x][y] = 1;
            return 1;
        }
    }
    // Checks if not on right edge
    if (x != width - 1) {
        // Recalls method one to the right
        if (recursiveSolve((x + 1), y)) {
            correctPath[x][y] = 1;
            return 1;
        }
    }

    // Checks if not on top edge
    if (y != 0) {
        // Recalls method one up
        if (recursiveSolve(x, (y - 1))) {
            correctPath[x][y] = 1;
            return 1;
        }
    }

    // Checks if not on bottom edge
    if (y != height - 1) {
        // Recalls method one down
        if (recursiveSolve(x, (y + 1))) {
            correctPath[x][y] = 1;
            return 1;
        }
    }

    return 0;
}

void solveMaze() {
    generateMaze();

    for (int y = (height / cellSize) - 1; y > 0; y--) {
        if (maze[(width / cellSize) - 1][y]) {
            endX = (width / cellSize) - 1;
            endY = y;
            break;
        }
    }

    for (int x = 0; x < width / cellSize; x++) {
        for (int y = 0; y < height / cellSize; y++) {
            wasHere[x][y] = 0;
            correctPath[x][y] = 0;
        }
    }
    bool b = recursiveSolve(startX, startY);
}

int main()
{

    solveMaze();


    // create the window
    sf::RenderWindow window(sf::VideoMode(width, height), "Maze Solver");

    sf::Texture texture;
    texture.create(window.getSize().x, window.getSize().y);

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // clear the window with black color
        window.clear(sf::Color::Black);

        // draw everything here...
        // window.draw(...);
        //Display Maze
        sf::RectangleShape cell(sf::Vector2f(cellSize, cellSize));
        for (int x = 0; x < (width / cellSize); x++) {
            for (int y = 0; y < (height / cellSize); y++) {
                cell.setPosition(sf::Vector2f(x * cellSize, y * cellSize));
                if (correctPath[x][y]) {
                    cell.setFillColor(sf::Color(0, 255, 0));
                }
                else if ((x == width / cellSize - 1) && (y == height / cellSize - 1)) {
                    cell.setFillColor(sf::Color(255, 0, 0));
                }
                else if (maze[x][y]) {
                    cell.setFillColor(sf::Color(0, 0, 255));
                }
                else { cell.setFillColor(sf::Color(0, 0, 0)); }
                window.draw(cell);
            }
        }

        texture.update(window);
        if (!screenshot){
            std::string fileLocation = "solvedMaze" + std::to_string(visited) + ".png";
            if (texture.copyToImage().saveToFile(fileLocation))
            {
                std::cout << "screenshot saved to " << fileLocation << '\n';
            }
            screenshot = 1;
        }


        // end the current frame
        window.display();
    }
}
