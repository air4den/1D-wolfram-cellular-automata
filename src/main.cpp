#include "main.hpp"
#include <iostream>
using namespace std;
#include <SFML/Graphics.hpp>

const unsigned int WINDOW_WIDTH = 1600;
const unsigned int WINDOW_HEIGHT = 800;
const unsigned int CELL_SIZE = 10;
const unsigned int GRID_WIDTH = WINDOW_WIDTH / CELL_SIZE;
const unsigned int GRID_HEIGHT = WINDOW_HEIGHT / CELL_SIZE;

// change this value to [0, 256)
const unsigned int RULE_NUMBER = 110;

const unsigned int TIME_BETWEEN_STATE = 10;     // time between states (milliseconds)

sf::Color colors[2] = {
    sf::Color::White,       // state is 0
    sf::Color::Black,       // state is 1
};

void setRuleArray(unsigned int * rule) {
    int dec = RULE_NUMBER;
    int index = 0;
    while (dec != 0 && index < 8) {
        int currRule = dec % 2;
        rule[index] = currRule;
        dec /= 2;
        index++;
    }
}

void clearRow(unsigned int * row) {
    for (int i = 0; i < GRID_WIDTH; i++) {
        row[i] = 0;
    }
}

void clearCellGrid(unsigned int cells[][GRID_WIDTH]) {
    for (int i = 0; i < GRID_HEIGHT; i++) {
        clearRow(cells[i]);
    }
}

void drawCell(sf::RenderWindow &window, unsigned int x, unsigned int y, int state) {
    sf::RectangleShape cell(sf::Vector2f(CELL_SIZE, CELL_SIZE));
    cell.setPosition(x, y);
    cell.setFillColor(colors[state]);
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

void drawCellGrid(sf::RenderWindow& window, unsigned int cells[][GRID_WIDTH]) {
    for (int i = 0; i < GRID_HEIGHT; i++) {
        for (int j = 0; j < GRID_WIDTH; j++) {
            drawCell(window, j * CELL_SIZE, i * CELL_SIZE, cells[i][j]);    
        }
    }
}

void computeNextState(unsigned int * currCellRow, unsigned int * prevCellRow, unsigned int cells[][GRID_WIDTH], int counter, unsigned int * rule) {
    if (counter == 0) {
        clearCellGrid(cells);
        clearRow(currCellRow);
        currCellRow[GRID_WIDTH/2] = 1;
    } else {
        for (int i = 0; i < GRID_WIDTH; i++) {
            int leftPrevIndex = (i - 1 + GRID_WIDTH) % GRID_WIDTH;
            int centerPrevIndex = i;
            int rightPrevIndex = (i + 1 + GRID_WIDTH) % GRID_WIDTH;

            int derivedRule = (prevCellRow[leftPrevIndex] << 2) + (prevCellRow[centerPrevIndex] << 1) + prevCellRow[rightPrevIndex];
            int nextState = rule[derivedRule];
            currCellRow[i] = nextState;
        }
    }
}

void copyRow(unsigned int * src, unsigned int * dest) {
    for (int i = 0; i < GRID_WIDTH; i++) {
        dest[i] = src[i];
    }
}

void updateCellsGrid(unsigned int * currCellRow, unsigned int cells[][GRID_WIDTH], int counter) {
    if (counter < GRID_HEIGHT) {
        copyRow(currCellRow, cells[counter]);
    } else {
        for (int i = 0; i < GRID_HEIGHT - 1; i++) {
            copyRow(cells[i+1], cells[i]);
        }
        copyRow(currCellRow, cells[GRID_HEIGHT-1]);
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Wolfram 1-D Cellular Automata");

    // initialize current Row, previous Row, overall grid
    unsigned int cells[GRID_HEIGHT][GRID_WIDTH];
    unsigned int currCellRow[GRID_WIDTH];
    unsigned int prevCellRow[GRID_WIDTH];
    unsigned int counter = 0;

    clearCellGrid(cells);

    unsigned int rule[8];
    setRuleArray(rule);

    unsigned int currHue; 
    sf::Clock clock; // start the clock

    // begin automata loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.scancode == sf::Keyboard::Scan::Enter) {
                    counter = 0;
                }
            }
        }

        window.clear();

        // Draw cellular automata simulation here

        //wait to update
        sf::Time elapsed = clock.getElapsedTime();
        if (elapsed > sf::milliseconds(TIME_BETWEEN_STATE)) {
            // next state
            computeNextState(currCellRow, prevCellRow, cells, counter, rule);
            updateCellsGrid(currCellRow, cells, counter);

            drawCellGrid(window, cells);
            //drawGridLines(window);

            window.display();

            copyRow(currCellRow, prevCellRow);
            counter++;
            clock.restart();
        }   
    }

    return 0;
}