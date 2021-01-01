/* A driver program for testing maze generation/exploration files for use in an arduino
   maze-solving robot. */

#include <iostream>
#include "maze.tpp"
#include <cassert>
#include <random>

using namespace std;

enum class TremauxFlag { unvisited = 0, visited = 1, deadend = 2 };

static const direction allDirections[4]{ direction::N, direction::E, direction::S, direction::W };

template<int mazeWidth, int mazeHeight>
class TremauxTraverser : public MazeTraverser<mazeWidth, mazeHeight> {
private:


    direction lastTraveledDirection = direction::STOP;

    /**
        Returns the TreumauxFlag encoded inside the MazeNode's flag for a given direction
    */
    TremauxFlag decodeFlag(MazeNode& n, direction d) {
        int shift = 2 * static_cast<int>(log2(d));
        return static_cast<TremauxFlag>((n.flag >> shift) & 3); // mask the two bits in the directions position with 3 (000000011)
    }

    /*
        Encodes a TreumaxFlag inside a given MazeNode's flag for a given direction
    */
    void encodeFlag(MazeNode& n, direction d, TremauxFlag f) {
        int shift = 2 * static_cast<int>(log2(d)); // directions are encoded in powers of 2. 2 bits are needed for a flag.

        int resetMask = ~(3 << shift);  // creates a binary mask to erase the previous flag (3 = 00000011)
        n.flag &= resetMask;            // apply mask

        n.flag |= static_cast<int>(f) << shift;           // apply new flag

        n.flag;
    }

    /*
        Returns the opposite direction of given. Opposite of stop is stop.
    */
    direction invertDirection(direction d) {
        switch (d) {
        case direction::N: return direction::S;
        case direction::E: return direction::W;
        case direction::S: return direction::N;
        case direction::W: return direction::E;
        }
        return direction::STOP;
    }

    /*
        Changes values of referenced coordinate variables as if stepping in given direction once. (i.e north of 0,0 is 0,-1)
    */
    void directionToCoordinate(direction d, int& x, int& y) {
        switch (d) {
        case direction::N: y--; break;
        case direction::E: x++; break;
        case direction::S: y++; break;
        case direction::W: x--; break;
        }
    }

public:

    TremauxTraverser() {
        this->curX = mazeWidth - 1;
        this->curY = mazeHeight - 1;
    }

    direction decideDirection() override {
        // Have just arrived at a potential intersection. Follow Tremaux maze algorithm rules, found
        // at https://en.wikipedia.org/wiki/Maze_solving_algorithm#Tr%C3%A9maux%27s_algorithm

        MazeNode& current = this->maze[this->curX][this->curY];

        direction decided = direction::STOP;
        TremauxFlag decidedFlag = TremauxFlag::deadend;

        for (direction canditate : allDirections) {
            if (!current.hasLeads(canditate)) continue;

            TremauxFlag canditateFlag = decodeFlag(current, canditate);
            if (canditateFlag < decidedFlag) {
                decided = canditate;
                decidedFlag = canditateFlag;
            }
        }

        return decided;
    }

    void addCurrentNodeInformation(MazeNode current) override {

        if (this->maze[this->curX][this->curY].flag == 0) {
            // visiting a completely new part of the maze

            // By Tremaux algorithm rules, any path that would create a loop (connects to an already 
            // investigated node other than the one just visited) is marked as a dead end.
            if (lastTraveledDirection != direction::STOP) {

                for (direction d : allDirections) {
                    if (d == invertDirection(lastTraveledDirection)) continue;

                    if (current.hasLeads(d)) {
                        int x = this->curX, y = this->curY; // coordinates of node in direction d
                        directionToCoordinate(d, x, y);

                        if (this->maze[x][y].flag != 0) {
                            // already visited, mark path 
                            encodeFlag(current, d, TremauxFlag::deadend);
                            encodeFlag(this->maze[x][y], invertDirection(d), TremauxFlag::deadend);
                        }
                    }
                }
            }
            // mark the path we entered by as traveled once
            encodeFlag(current, invertDirection(lastTraveledDirection), TremauxFlag::visited);

            this->maze[this->curX][this->curY] = current;
        }
        else {
            // re-visiting a node, which only happens after a deadend
            encodeFlag(this->maze[this->curX][this->curY], invertDirection(lastTraveledDirection), TremauxFlag::deadend);
        }
    }

    void travelInDirection(direction d) override {
        directionToCoordinate(d, this->curX, this->curY);
        lastTraveledDirection = d;
    }
};

int main() {
    // generate a random maze
    const int mazeSize = 10;
    const float density = 0.6f; // odds that a node will have a lead in a given direction
    MazeNode maze[mazeSize][mazeSize];

    srand(101);
    for (int x = 0; x < mazeSize; x++) {
        for (int y = 0; y < mazeSize; y++) {
            // roll for east lead
            float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
            if (r < density && x < mazeSize - 1) {
                maze[x][y].addLeads(direction::E);
                maze[x + 1][y].addLeads(direction::W);
            }
            // roll for south lead
            r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
            if (r < density && y < mazeSize - 1) {
                maze[x][y].addLeads(direction::S);
                maze[x][y + 1].addLeads(direction::N);
            }
            // north and west are inherited from other nodes
        }
    }

    cout << "Generated Maze: \n";
    printMaze<mazeSize, mazeSize>(maze);

    TremauxTraverser<mazeSize, mazeSize> traverser;
    int curX = mazeSize/2, curY = mazeSize/2;
    direction travel = STOP;
    do {
        traverser.addCurrentNodeInformation(maze[curX][curY]);
        traverser.display();
        travel = traverser.decideDirection();
        traverser.travelInDirection(travel);
        switch (travel) {
        case N:
            curY--;
            break;
        case S:
            curY++;
            break;
        case E:
            curX++;
            break;
        case W:
            curX--;
            break;
        }
    } while (travel != STOP);

}