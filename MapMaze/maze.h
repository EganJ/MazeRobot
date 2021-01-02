/**
   Code for abstracting the maze details.

   I'd really like this to be called maze.tpp but the Arduino IDE isn't seeing it
*/
#pragma once

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

static const direction allDirections[4] { direction::N, direction::E, direction::S, direction::W };

/*
    Changes values of referenced coordinate variables as if stepping in given direction once. (i.e north of 0,0 is 0,-1)
*/
void directionToCoordinate(direction d, int& x, int& y);

/*
   Returns the direction to the right (clockwise) of supplied direction
*/
direction directionToRightOf(direction d);

/*
   Returns the direction to the left (counter-clockwise) of supplied direction
*/
direction directionToLeftOf(direction d);

/*
    Returns the opposite direction of given. Opposite of stop is stop.
*/
direction invertDirection(direction d);


class MazeNode {
    /** Represents a single intersection or cooridor in the maze.

       Contains a variable (leads) for encoding flags in each bit on which directions lead to/from the maze
       Simple enough so implementation done inline

    */
  public:
    short leads = 0; // which directions are connected to this node

    int flag = 0; // helpful tracker for a variety of algorithms, has no internal use

    MazeNode() = default;
    MazeNode(const MazeNode& copyFrom) = default;
    MazeNode(short directions) : leads(directions) { };

    /**
       Returns whether each of the leads encoded in the parameter is present in this node.
       The
    */
    bool hasLeads(short directions);

    void addLeads(short directions);
};


enum TremauxFlag { unvisited = 0, visited = 1, deadend = 2 };

template <int mazeWidth, int mazeHeight>
class TremauxTraverser {
  private:

    MazeNode maze[2 * mazeWidth - 1][2 * mazeHeight - 1]; // since we don't know maze start postition accomodate for extremes in both directions
    int curX = mazeWidth - 1, curY = mazeHeight - 1;      // current positions in the maze. Default to center, so regardless of start position
    // the maze can fit in array
    direction lastTraveledDirection = direction::STOP;

    /**
        Returns the TreumauxFlag encoded inside the MazeNode's flag for a given direction
    */
    TremauxFlag decodeFlag(MazeNode & n, direction d);

    /*
        Encodes a TreumaxFlag inside a given MazeNode's flag for a given direction
    */
    void encodeFlag(MazeNode & n, direction d, TremauxFlag f);

  public:
    TremauxTraverser() = default;

    // algorithm implementation functions

    /**
       Based on currently known information, decide which direction to move in next
    */
    direction decideDirection();

    /**
       Update information to account for the node just arrived at.
       @param current- a new node object containing the lead information of
        current intersection
    */
    void addCurrentNodeInformation(MazeNode current);

    /**
       Update internal algorithm resources to reflect a step in given direction.
       Robot movement is accounted for elsewhere.
    */
    void travelInDirection(direction d);

    void displayMaze();
};


/**
   Implementation for abstracting the maze details.

*/

#include <Arduino.h>

/**
   Returns floor of log2(n)
*/
int log2(int n) {
  int result = 0;
  while ( n >> result) {
    result++;
  }
  return result;
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

direction invertDirection(direction d) {
  switch (d) {
    case direction::N: return direction::S;
    case direction::E: return direction::W;
    case direction::S: return direction::N;
    case direction::W: return direction::E;
  }
  return direction::STOP;
}

direction directionToRightOf(direction d) {
  switch (d) {
    case direction::N: return direction::E;
    case direction::E: return direction::S;
    case direction::S: return direction::W;
    case direction::W: return direction::N;
  }
  return direction::STOP;
}

direction directionToLeftOf(direction d) {
  switch (d) {
    case direction::N: return direction::W;
    case direction::E: return direction::N;
    case direction::S: return direction::E;
    case direction::W: return direction::S;
  }
  return direction::STOP;
}


bool MazeNode::hasLeads(short direction) {
  return (direction & ~leads) == 0;  // no bits are in direction but not leads
}

void MazeNode::addLeads(short directions) {
  leads |= directions;
}

const static char symbolCodes[16] {
  char(32),  // 0000
  char(94),  // 000N
  char(62),  // 00E0
  char(192), // 00EN
  char(118), // 0S00
  char(179), // 0S0N
  char(218), // 0SE0
  char(195), // 0SEN
  char(60),  // W000
  char(217), // W00N
  char(196), // W0E0
  char(193), // W0EN
  char(191), // WS00
  char(180), // WS0N
  char(194), // WSE0
  char(197), // WSEN
};

template<int mazeWidth, int mazeHeight>
TremauxFlag TremauxTraverser<mazeWidth, mazeHeight>::decodeFlag(MazeNode& n, direction d) {
  int shift = 2 * static_cast<int>(log2(d));
  return static_cast<TremauxFlag>((n.flag >> shift) & 3); // mask the two bits in the directions position with 3 (000000011)
}

template<int mazeWidth, int mazeHeight>
void TremauxTraverser<mazeWidth, mazeHeight>::encodeFlag(MazeNode& n, direction d, TremauxFlag f) {
  int shift = 2 * static_cast<int>(log2(d)); // directions are encoded in powers of 2. 2 bits are needed for a flag.

  int resetMask = ~(3 << shift);  // creates a binary mask to erase the previous flag (3 = 00000011)
  n.flag &= resetMask;            // apply mask

  n.flag |= static_cast<int>(f) << shift;           // apply new flag

  n.flag;
}

template<int mazeWidth, int mazeHeight>
direction TremauxTraverser<mazeWidth, mazeHeight>::decideDirection() {
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

template<int mazeWidth, int mazeHeight>
void TremauxTraverser<mazeWidth, mazeHeight>::addCurrentNodeInformation(MazeNode current) {

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

template<int mazeWidth, int mazeHeight>
void TremauxTraverser<mazeWidth, mazeHeight>::travelInDirection(direction d) {
  directionToCoordinate(d, this->curX, this->curY);
  lastTraveledDirection = d;
}

template<int mazeWidth, int mazeHeight>
void TremauxTraverser<mazeWidth, mazeHeight>::displayMaze() {
  Serial.println();
  for (int y = 0; y < mazeHeight; y++) {
    for (int x = 0; x < mazeWidth; x++) {
      Serial.write(symbolCodes[maze[x][y].leads]);
    }
    Serial.println();
  }
  Serial.println();
}
