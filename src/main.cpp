#include "main.hpp"
#include <iostream>
using namespace std;
#include <SFML/Graphics.hpp>

const unsigned int WINDOW_WIDTH = 1500;
const unsigned int WINDOW_HEIGHT = 800;
const unsigned int CELL_SIZE = 10; // Size of each cell in pixels
const unsigned int GRID_WIDTH = WINDOW_WIDTH / CELL_SIZE;
const unsigned int GRID_HEIGHT = WINDOW_HEIGHT / CELL_SIZE;

unsigned int cells[GRID_HEIGHT][GRID_WIDTH];
unsigned int currCellRow[GRID_WIDTH];
unsigned int prevCellRow[GRID_WIDTH];

const unsigned int RULE[8] = {0,0,1,1,1,1,0,0};

unsigned int counter;

// TODO: refactor cell into class

void clearRow(unsigned int * row) {
    for (int i = 0; i < GRID_WIDTH; i++) {
        row[i] = 0;
    }
}

void drawCell(sf::RenderWindow &window, unsigned int x, unsigned int y, int state) {
    sf::RectangleShape cell(sf::Vector2f(CELL_SIZE, CELL_SIZE));
    cell.setPosition(x, y);
    if (!state) {
        cell.setFillColor(sf::Color::Black);
    } else {
        cell.setFillColor(sf::Color::White);
    }
    window.draw(cell);
}

void drawGridLines(sf::RenderWindow& window) {
    // Draw vertical lines
    for (int i = 0; i <= GRID_WIDTH; ++i) {
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(i * CELL_SIZE, 0)),
            sf::Vertex(sf::Vector2f(i * CELL_SIZE, WINDOW_HEIGHT))
        };
        window.draw(line, 2, sf::Lines);
    }
    // Draw horizontal lines
    for (int i = 0; i <= GRID_HEIGHT; ++i) {
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(0, i * CELL_SIZE)),
            sf::Vertex(sf::Vector2f(WINDOW_WIDTH, i * CELL_SIZE))
        };
        window.draw(line, 2, sf::Lines);
    }
}

void drawCellGrid(sf::RenderWindow& window) {
    for (int i = 0; i < GRID_HEIGHT; i++) {
        for (int j = 0; j < GRID_WIDTH; j++) {
            drawCell(window, j * CELL_SIZE, i * CELL_SIZE, cells[i][j]);
        }
    }
}

void computeNextState() {
    if (counter == 0) {
        clearRow(currCellRow);
        currCellRow[GRID_WIDTH/2] = 1;
    } else {
        for (int i = 0; i < GRID_WIDTH; i++) {
            int leftPrevIndex = (i - 1 + GRID_WIDTH) % GRID_WIDTH;
            int centerPrevIndex = i;
            int rightPrevIndex = (i + 1 + GRID_WIDTH) % GRID_WIDTH;

            int derivedRule = (prevCellRow[leftPrevIndex] << 2) + (prevCellRow[centerPrevIndex] << 1) + prevCellRow[rightPrevIndex];
            int nextState = RULE[derivedRule];
            currCellRow[i] = nextState;

        }
    }
}

void copyRow(unsigned int * src, unsigned int * dest) {
    for (int i = 0; i < GRID_WIDTH; i++) {
        dest[i] = src[i];
    }
}

void updateCellsGrid() {
    if (counter <= GRID_HEIGHT) {
        copyRow((unsigned int *) &currCellRow, (unsigned int *) &cells[counter]);
    } else {
        for (int i = 0; i < GRID_HEIGHT - 1; i++) {
            copyRow((unsigned int *) &cells[i+1], (unsigned int *) &cells[i]);
        }
        copyRow((unsigned int *) &currCellRow, (unsigned int *) & cells[GRID_HEIGHT-1]);
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Wolfram 1-D Cellular Automata");

    counter = 0; 

    sf::Clock clock; 
    
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        // Draw cellular automata simulation here

        drawGridLines(window);  // This doesn't work...fix this

        //wait to update (1 step/second)
        sf::Time elapsed = clock.getElapsedTime();
        if (elapsed > sf::milliseconds(50)) {
            // next state
            computeNextState();
            updateCellsGrid();
            drawCellGrid(window);
            window.display();

            std::cout << elapsed.asSeconds() << std::endl;
            std::cout << counter << std::endl;
            copyRow((unsigned int *) &currCellRow, (unsigned int *) &prevCellRow);
            counter++;
            clock.restart();
        }
        
        

        
        
    }

    return 0;
}