#pragma once
#include <iostream>
#include <iomanip>

/*
    Components for representing the maze, both for human viewing and data representation
*/
enum direction : short {
    // uses each bit to encode information about one of the cardinal directions.
    N = 1, // 0001
    E = 2, // 0010
    S = 4, // 0100
    W = 8, // 1000
    STOP = 0
};

const static char symbols[16]{
       ' ',         // 0000
       '^',         // 000N
       '>',         // 00E0
       (char)(192), // 00EN
       'v',         // 0S00
       (char)(179), // 0S0N
       (char)(218), // 0SE0
       (char)(195), // 0SEN
       '<',         // W000
       (char)(217), // W00N
       (char)(196), // W0E0
       (char)(193), // W0EN
       (char)(191), // WS00
       (char)(180), // WS0N
       (char)(194), // WSE0
       (char)(197), // WSEN
};

class MazeNode {
protected:


public:
    short leads = 0; // which directions are connected to this node

    int flag = 0; // helpful tracker for a variety of algorithms, has no internal use

    MazeNode() = default;
    MazeNode(const MazeNode& copyFrom) = default;
    MazeNode(short directions) :leads(directions) { };

    bool hasLeads(short direction) { return (direction & ~leads) == 0; } // no bits are in direction but not leads

    void addLeads(short directions) { leads |= directions; }

    friend std::ostream& operator<<(std::ostream& os, MazeNode& node);
};


template <int mazeWidth, int mazeHeight>
void printMaze(MazeNode maze[mazeWidth][mazeHeight], int curX = -1, int curY = -1) {
    // print top row with x-indicator
    std::cout << std::endl;
    if (curX >= 0) {
        std::cout << "\t";
        // maze display
        for (int i = 0; i < curX; i++) {
            std::cout << " ";
        }
        std::cout << "v";

        // flag display
        std::cout << "     ";
        for (int i = 0; i < mazeWidth-1; i++) {
            std::cout << " ";
        }
        std::cout << "v";

        std::cout << std::endl;
    }
    for (int y = 0; y < mazeHeight; y++) {
        std::cout << "\t"; // give a little bit of spacing on each line
        for (int x = 0; x < mazeWidth; x++) {
            std::cout << maze[x][y];
        }
        std::cout << "     "; // give a little bit of spacing between displays
        for (int x = 0; x < mazeWidth; x++) {

            std::cout << std::setw(4);
            if (x == curX && y == curY) {
                std::cout << -maze[x][y].flag;
            }
            else {
                std::cout << maze[x][y].flag;
            }
        }
        if (y == curY) std::cout << " <";
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

/*
    Components for traversing the maze while knowing only what's already been seen
*/
template <int mazeWidth, int mazeHeight>
class MazeTraverser {
protected:
    MazeNode maze[2 * mazeWidth - 1][2 * mazeHeight - 1]; // since we don't know maze start postition accomodate for extremes in both directions
    int curX = -1, curY = -1; // current positions in the maze. Default to impossible value of -1
public:
    MazeTraverser() = default;
    virtual direction decideDirection() = 0;
    virtual void addCurrentNodeInformation(MazeNode current) = 0;
    virtual void travelInDirection(direction d) = 0;

    void display();
};



/*
    Implementation details
*/
std::ostream& operator<<(std::ostream& os, MazeNode& node) {
    os << symbols[node.leads];
    return os;
};

template <int mazeWidth, int mazeHeight>
void MazeTraverser<mazeWidth, mazeHeight>::display() {
    printMaze<mazeWidth * 2 - 1, mazeHeight * 2 - 1>(maze, curX, curY);
}


