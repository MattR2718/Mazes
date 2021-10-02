#include "Maze.h"

#include <SFML/Graphics.hpp>
#include <iostream>
#include <stack>
#include <vector>
#include <random>
#include <string>
#include <strstream>

const int width = 100;
const int height = 80;
const int cellSize = 10;

int startX = 1;
int startY = 1;

bool screenshot = 0;

int main()
{
    Maze maze(width, height, startX, startY);
    maze.generateMaze();
    maze.solveMaze();


    // create the window
    sf::RenderWindow window(sf::VideoMode(width * cellSize, height * cellSize), "Maze Solver");

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
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                cell.setPosition(sf::Vector2f(x * cellSize, y * cellSize));
                if (maze.correctPath[x][y]) {
                    cell.setFillColor(sf::Color(0, 255, 0));
                }
                else if ((x == maze.endX) && (y == maze.endY)) {
                    cell.setFillColor(sf::Color(255, 0, 0));
                }
                else if (maze.maze[x][y]) {
                    cell.setFillColor(sf::Color(0, 0, 255));
                }
                else { cell.setFillColor(sf::Color(0, 0, 0)); }
                window.draw(cell);
            }
        }

        texture.update(window);
        if (!screenshot) {
            srand(time(NULL));
            int num = rand() % 100000;

            std::string fileLocation = "images\\solvedMaze" + std::to_string(maze.visited) + "-" + std::to_string(num) + ".png";
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