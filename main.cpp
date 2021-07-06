#include <SFML/Graphics.hpp>
#include <iostream>
#include <stack>
#include <vector>
#include <random>

struct directions {
    bool up;
    bool down;
    bool left;
    bool right;
    bool visited;
};

    const int lineWidth = 3;
int main()
{
    int steps = 0;
    const int width = 1000;
    const int height = 800;
    const int cellSize = 25;
    const int delay = 0;
    bool ssTaken = 0;
    directions maze[width / cellSize][height / cellSize];
    int visited;
    std::stack<std::pair<int, int>> mazeStack;

    for (int x = 0; x < width / cellSize; x++) {
        for (int y = 0; y < height / cellSize; y++) {
            maze[x][y].up = 0;
            maze[x][y].down = 0;
            maze[x][y].left = 0;
            maze[x][y].right = 0;
            maze[x][y].visited = 0;
        }
    }

    // create the window
    sf::RenderWindow window(sf::VideoMode(width + lineWidth, height + lineWidth), "Maze");

    //Create texture
    sf::Texture texture;
    texture.create(window.getSize().x, window.getSize().y);

    //Make Start and End
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> sdist(0, height / cellSize);
    int start = sdist(mt);
    std::uniform_int_distribution<int> edist(0, height / cellSize);
    int end = edist(mt);

    //Start from Start
    mazeStack.push(std::make_pair(0, start));
    maze[0][start].visited = 1;
    visited = 1;

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // clear the window with black color
        window.clear(sf::Color::Red);

        // draw everything here...
        // window.draw(...);

        //Maze Creation
        if (visited < (width / cellSize) * (height / cellSize)) {
            //Check unvisited neighbours
            std::vector<int> neighbours;
            //Up
            if ((mazeStack.top().second > 0) && (!maze[mazeStack.top().first][mazeStack.top().second - 1].visited)) {
                neighbours.push_back(0);
            }
            //Right
            if ((mazeStack.top().first < (width / cellSize) - 1) && (!maze[mazeStack.top().first + 1][mazeStack.top().second].visited)) {
                neighbours.push_back(1);
            }
            //Down
            if ((mazeStack.top().second < (height / cellSize) - 1) && (!maze[mazeStack.top().first][mazeStack.top().second + 1].visited)) {
                neighbours.push_back(2);
            }
            //Left
            if ((mazeStack.top().first > 0) && (!maze[mazeStack.top().first - 1][mazeStack.top().second].visited)) {
                neighbours.push_back(3);
            }

            //Check neighbours
            if (!neighbours.empty()) {

                std::random_device rd;
                std::mt19937 mt(rd());
                std::uniform_int_distribution<int> dist(0, 301);
                int nextCellDir = neighbours[dist(mt) % neighbours.size()];
                //int nextCellDir = neighbours[rand() % neighbours.size()];
                //std::cout << nextCellDir << '\n';

                //Make path
                switch (nextCellDir) {
                    //Up
                case 0:
                    maze[mazeStack.top().first][mazeStack.top().second].up = 1;
                    maze[mazeStack.top().first][mazeStack.top().second - 1].down = 1;
                    mazeStack.push(std::make_pair((mazeStack.top().first), (mazeStack.top().second - 1)));
                    break;
                    //Right
                case 1:
                    maze[mazeStack.top().first][mazeStack.top().second].right = 1;
                    maze[mazeStack.top().first + 1][mazeStack.top().second].left = 1;
                    mazeStack.push(std::make_pair((mazeStack.top().first + 1), (mazeStack.top().second)));
                    break;
                    //Down
                case 2:
                    maze[mazeStack.top().first][mazeStack.top().second].down = 1;
                    maze[mazeStack.top().first][mazeStack.top().second + 1].up = 1;
                    mazeStack.push(std::make_pair((mazeStack.top().first), (mazeStack.top().second + 1)));
                    break;
                    //Left
                case 3:
                    maze[mazeStack.top().first][mazeStack.top().second].left = 1;
                    maze[mazeStack.top().first - 1][mazeStack.top().second].right = 1;
                    mazeStack.push(std::make_pair((mazeStack.top().first - 1), (mazeStack.top().second)));
                    break;
                }

                visited++;
                maze[mazeStack.top().first][mazeStack.top().second].visited = 1;

            }
            else {
                mazeStack.pop();
            }

            sf::RectangleShape stackHead(sf::Vector2f(cellSize - lineWidth, cellSize - lineWidth));
            stackHead.setPosition(sf::Vector2f((mazeStack.top().first * cellSize) + lineWidth, (mazeStack.top().second * cellSize) + lineWidth));
            stackHead.setFillColor(sf::Color::Red);
            window.draw(stackHead);
        }
        else {
            //Grid
            //horizontal
            sf::RectangleShape hline(sf::Vector2f(width + lineWidth, lineWidth));
            hline.setFillColor(sf::Color(0, 0, 0));
            for (int y = 0; y < height + lineWidth; y += cellSize) {
                hline.setPosition(0, y);
                window.draw(hline);
            }
            //vertical
            sf::RectangleShape vline(sf::Vector2f(lineWidth, height + lineWidth));
            vline.setFillColor(sf::Color(0, 0, 0));
            for (int x = 0; x < width + lineWidth; x += cellSize) {
                vline.setPosition(x, 0);
                window.draw(vline);
            }
            //Display Maze
            sf::RectangleShape cell(sf::Vector2f(cellSize - lineWidth, cellSize - lineWidth));
            for (int x = 0; x < width / cellSize; x++) {
                for (int y = 0; y < height / cellSize; y++) {
                    cell.setPosition(sf::Vector2f((x * cellSize) + lineWidth, (y * cellSize) + lineWidth));
                    if (maze[x][y].visited) {
                        cell.setFillColor(sf::Color(0, 0, 255));
                        //window.draw(cell);
                    }
                    else { cell.setFillColor(sf::Color(255, 255, 255)); }
                    window.draw(cell);

                    //Walls
                    cell.setFillColor(sf::Color(0, 0, 255));
                    if (maze[x][y].up) {
                        cell.setPosition(sf::Vector2f((x * cellSize) + lineWidth, ((y * cellSize) + lineWidth) - cellSize / 2));
                        window.draw(cell);
                    }
                    if (maze[x][y].down) {
                        cell.setPosition(sf::Vector2f((x * cellSize) + lineWidth, ((y * cellSize) + lineWidth) + cellSize / 2));
                        window.draw(cell);
                    }
                    if (maze[x][y].left) {
                        cell.setPosition(sf::Vector2f(((x * cellSize) + lineWidth) - cellSize / 2, (y * cellSize) + lineWidth));
                        window.draw(cell);
                    }
                    if (maze[x][y].right) {
                        cell.setPosition(sf::Vector2f(((x * cellSize) + lineWidth) + cellSize / 2, (y * cellSize) + lineWidth));
                        window.draw(cell);
                    }
                }
            }

            //Start Point
            sf::RectangleShape se(sf::Vector2f(cellSize - lineWidth, cellSize - lineWidth));
            se.setFillColor(sf::Color::Green);
            se.setPosition(sf::Vector2f(lineWidth, (start * cellSize) + lineWidth));
            window.draw(se);

            //End Point
            se.setFillColor(sf::Color::Red);
            se.setPosition(sf::Vector2f(((width / cellSize) - 1) * cellSize + lineWidth, (end * cellSize) + lineWidth));
            window.draw(se);

            if (!ssTaken) {
                ssTaken = 1;
                texture.update(window);
                if (texture.copyToImage().saveToFile("maze.png"))
                {
                    std::cout << "screenshot saved to " << "maze.png" << '\n';
                }
            }

        }

        // end the current frame
        window.display();
    }
}