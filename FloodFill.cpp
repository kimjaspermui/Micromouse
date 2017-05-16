// #include <queue>
#include <stack>
#include <iostream>

using namespace std;

struct location {

  int x;
  int y;
};

// need visited member?
struct cell {

  int wall;
  int distance;
  bool visited;
};

// NOTE: need static allocation instead of dynamic
const int SIZE = 16;

// even
const int CENTER_MIN = SIZE / 2 - 1;
const int CENTER_MAX = SIZE / 2;
const int NUM_CENTERS = 4;

const int START_X = 15;
const int START_Y = 0;

/**
// odd
const int CENTER_MIN = SIZE / 2;
const int CENTER_MAX = SIZE / 2;
const int NUM_CENTERS = 1;*/

// walls representations
const int WALLS = 4;
const int TOP_WALL = 1;
const int RIGHT_WALL = 2;
const int BOTTOM_WALL = 4;
const int LEFT_WALL = 8;
const int ALL_WALLS = 15;
const int walls[WALLS] = {1, 2, 4, 8};

const int DEAD_WALLS = 3;
const int DEAD_WALLS_NEIGHBOR = 2;

// directions representations
const int DIRECTIONS = 4;
const int NORTH = 0;
const int EAST = 1;
const int SOUTH = 2;
const int WEST = 3;
const string directions[DIRECTIONS] = {"North", "East", "South", "West"};
const char directionSymbols[DIRECTIONS] = {'^', '>', 'v', '<'};

//------------------------------------------------------------------------------
// Helper functions start here

/**
 * Name: isCenter()
 * Parameters: x - this is the x coordinate to be checked.
 * y - this is the y coordinate to be checked.
 * Description: This function will check whether or not the given coordinates
 * combine is one of the centers.
 * Return: A boolean indicating that the given coordinates combine is one of the
 * centers.
 */
bool isCenter(int x, int y) {

  // check if it's one of the centers
  if (x >= CENTER_MIN && x <= CENTER_MAX) {

    if (y >= CENTER_MIN && y <= CENTER_MAX) {

      return true;
    }
  }

  // otherwise, return false
  return false;
}

/**
 * Name: isOut()
 * Parameters: x - this is the x coordinate to be checked.
 * y - this is the y coordinate to be checked.
 * Description: This function will check whether or not the given coordinates
 * are out of bounds of the 2D array.
 * Return: A boolean indicating that the given coordinates are out of bounds.
 */
bool isOut(int x, int y) {

  // check if x and y is out of bounds of the maze array
  if (x < 0 || x >= SIZE) {

    return true;
  }

  else if (y < 0 || y >= SIZE) {

    return true;
  }

  // otherwise, return false
  return false;
}

/**
 * Name: numWalls()
 * Parameters: walls - this is the walls to be counted.
 * Description: This function will be able to count the number of walls from the
 * given walls.
 * Return: An int representing the number of walls around this cell.
 */
int numWalls(int walls) {

  // the mask to get the first bit
  int mask = 1;

  // the counter of 1's
  int count = 0;

  // for loop to count the number of walls
  for (int i = 0; i < DIRECTIONS; i++) {

    if ((walls >> i & mask) == 1) {

      count++;
    }
  }

  return count;
}

/**
 * Name: enterableCells()
 * Parameters: currentX - the x position of the current location.
 * currentY - they y position of the current location.
 * theMaze - the 2D array representing the maze, where each element
 * is a cell that has a wall and distance member.
 * enterableNeighbors - the array to be populated with distances if the cell
 * is enterable from the current location.
 * Description: This function will be able to determine which neighbors are
 * enterable and populate the given array with the distances. It will keep the
 * element as -1 if there is a wall.
 */
void enterableCells(int currentX, int currentY, cell theMaze[SIZE][SIZE],
int enterableNeighbors[DIRECTIONS]) {

  int currentWall = theMaze[currentX][currentY].wall;

  // check all directions whether this neighbor is enterable
  for (int i = 0; i < DIRECTIONS; i++) {

    // the i represents the current direction being examined, if there is no
    // wall then add the distance to the array
    switch (i) {

      case NORTH:

        if ((currentWall & TOP_WALL) == 0) {

          enterableNeighbors[i] = theMaze[currentX - 1][currentY].distance;
        }
        break;

      case EAST:

        if ((currentWall & RIGHT_WALL) == 0) {

          enterableNeighbors[i] = theMaze[currentX][currentY + 1].distance;
        }
        break;

      case SOUTH:

        if ((currentWall & BOTTOM_WALL) == 0) {

          enterableNeighbors[i] = theMaze[currentX + 1][currentY].distance;
        }
        break;

      case WEST:

        if ((currentWall & LEFT_WALL) == 0) {

          enterableNeighbors[i] = theMaze[currentX][currentY - 1].distance;
        }
        break;
    }
  }
}

/**
 * Name: findMinDistance()
 * Parameters: enterableNeighbors() - this is the array that contains the
 * distances of the neighbors.
 * Description: This function will find the least distance to center, except if
 * the distance is -1, unenterable.
 * Return: an integer representing the smallest distance in given array.
 */
int findMinDistance(int enterableNeighbors[DIRECTIONS]) {

  // this will keep track of the smallest distance
  int min = SIZE * SIZE;

  // check all distances
  for (int i = 0; i < DIRECTIONS; i++) {

    int currentDistance = enterableNeighbors[i];

    if (currentDistance != -1 && currentDistance < min) {

      min = currentDistance;
    }
  }

  return min;
}

/**
 * Name: stepAtDirection()
 * Parameters: currentLocation - the current location of the mouse.
 * direction - the direction to take a step to.
 * Description: This function will update the current location to a neighbor for
 * a step given the specified direction.
 */
void stepAtDirection(location* currentLocation, int direction) {

    switch (direction) {

      case NORTH:
        currentLocation->x -= 1;
        break;
      
      case EAST:
        currentLocation->y +=1;
        break;

      case SOUTH:
        currentLocation->x += 1;
        break;

      case WEST:
        currentLocation->y -= 1;
        break;
    }
}

/**
 * Name: addWall()
 * Parameters: currentX - the x position of the neighbor.
 * currentY - the y position of the neighbor.
 * theMaze - the 2D array representing the maze, where each element
 * is a cell that has a wall and distance member.
 * wall - the side of the wall to add.
 * Description: This function will update the wall of the given location with
 * the side of the wall.
 */
void addWall(int currentX, int currentY, cell theMaze[SIZE][SIZE], int wall) {

  // add the wall at the given location
  if (!isOut(currentX, currentY)) {

    int *tempWall = &(theMaze[currentX][currentY].wall);
    *tempWall = *tempWall | wall;
  }  
}

/**
 * Name: updateNeighborWall()
 * Parameters: currentX - the x position of the current location.
 * currentY - the y position of the current location.
 * theMaze - the 2D array representing the maze, where each element
 * is a cell that has a wall and distance member.
 * direction - the direction of the neighbor to add wall.
 * Description: This function will be able to add the corresponding wall of the
 * the neighbor with the corresponding direction.
 */
void updateNeighborWall(int currentX, int currentY, cell theMaze[SIZE][SIZE],
int direction) {

  // update the wall of the neighbor
  switch (direction) {

    // north side neighbor
    case NORTH:

      addWall(currentX - 1, currentY, theMaze, BOTTOM_WALL);
      break;

    // east side neighbor
    case EAST:

      addWall(currentX, currentY + 1, theMaze, LEFT_WALL);
      break;

    // south side neighbor
    case SOUTH:

      addWall(currentX + 1, currentY, theMaze, TOP_WALL);
      break;

    // west side neighbor
    case WEST:

      addWall(currentX, currentY - 1, theMaze, RIGHT_WALL);
      break;
  }
}

/**
 * Name: populateStack()
 * Parameters: currentX - the x position of the current cell.
 * currentY - the y position of the current cell.
 * direction - the direction of the neighbor to be examined.
 * myStack - the stack to populate.
 * Description: This function will be able to populate the stack with the given
 * neighbor that is not out of bounds.
 */
void populateStack(int currentX, int currentY, int direction,
stack<location>* myStack) {

  switch (direction) {

    case NORTH:
      
      if (!isOut(currentX - 1, currentY)) {

        myStack->push({currentX - 1, currentY});
      }
      break;

    case EAST:
      
      if (!isOut(currentX, currentY + 1)) {

        myStack->push({currentX, currentY + 1});
      }
      break;

    case SOUTH:
      
      if (!isOut(currentX + 1, currentY)) {

        myStack->push({currentX + 1, currentY});
      }
      break;

    case WEST:
      
      if (!isOut(currentX, currentY - 1)) {

        myStack->push({currentX, currentY - 1});
      }
      break;
  }
}

/**
 * Name: printMaze()
 * Parameters: theMaze - the 2D array representing the maze, where each element
 * is a cell that has a wall and distance member.
 * currentLocation - the current location of the mouse.
 * currentDirection - the current direction of the mouse.
 * Description: This function has the ability to print the maze using the ASCII
 * characters to represent the walls and the mouse with the given maze
 * representation as defined.
 */
void printMaze(cell theMaze[SIZE][SIZE], location currentLocation,
int currentDirection) {

  // get the x and y of the mouse's location
  int mouseX = currentLocation.x;
  int mouseY = currentLocation.y;

  // print the walls on the top row
  for (int i = 0; i < SIZE; i++) {

    cout << ' ';
    cout << '=';
  }

  // the extra space at the end of top row
  cout << ' ' << endl;

  // print the left or bottom walls of the current cell if any
  for (int i = 0; i < SIZE; i++) {

    // iterate to print left walls, if any, then print the car if at the current
    // cell
    for (int j = 0; j < SIZE; j++) {

      // get the current wall
      int currentWall = theMaze[i][j].wall;

      // check if there is a left wall, if so, print it
      if ((currentWall & LEFT_WALL) != 0) {

        cout << "|";
      }

      else {

        cout << " ";
      }

      // check if the mouse is present at this cell, if so, print it
      if (i == mouseX && j == mouseY) {

        cout << directionSymbols[currentDirection];
      }

      else {

        cout << " ";
      }
    }

    // the wall at the very end, then new line
    cout << "|" << endl;

    // iterate to print the bottom walls, if any
    for (int j = 0; j < SIZE; j++) {

      // get the current wall
      int currentWall = theMaze[i][j].wall;

      // check if there is a bottom wall, if so, print it
      if ((currentWall & BOTTOM_WALL) != 0) {

        cout << " =";
      }

      else {

        cout << "  ";
      }
    }

    // the space at the very end, then new line
    cout << " " << endl;
  }
}

/**
 * Name: printArray()
 * Parameters: theMaze - the 2D array to be printed.
 * Description: This function will print the given 2D array, where each element
 * is a cell, which represents walls and distance.
 */
void printArray(cell theMaze[SIZE][SIZE]) {

  for (int i = 0; i < SIZE; i++) {

    for (int j = 0; j < SIZE; j++) {

      printf("(%2d,%2d,%d)", theMaze[i][j].wall, theMaze[i][j].distance,
      theMaze[i][j].visited);
    }

    cout << endl;
  }
}

/**
 * Name: checkStatus()
 * Parameters: theMaze - the 2D array representing the maze, where each element
 * is a cell that has a wall and distance member.
 * currentLocation - the current location of the mouse.
 * currentDirection - the current direction of the mouse.
 * Description: This function will be used to check the current state of the
 * maze and mouse for debugging purposes.
 */
void checkStatus(cell theMaze[SIZE][SIZE], location currentLocation,
int currentDirection) {

  // check status of the maze and mouse
  printMaze(theMaze, currentLocation, currentDirection);
  printArray(theMaze);
}

/**
 * Name: fillWalls()
 * Parameters: currentX - the x position of the location to be isolated.
 * currentY - the y position of the location to be isolated.
 * theMaze - the 2D array representing the maze, where each element
 * is a cell that has a wall and distance member.
 * Description: This function will be able to isolate the cell from other cells,
 * by filling it with walls on all sides because it is a dead end.
 */
void fillWalls(int currentX, int currentY, cell theMaze[SIZE][SIZE]) {

  int enterableNeighbors[DIRECTIONS] = {-1, -1, -1, -1};

  enterableCells(currentX, currentY, theMaze, enterableNeighbors);

  for (int i = 0; i < DIRECTIONS; i++) {

    if (enterableNeighbors[i] != -1) {

      updateNeighborWall(currentX, currentY, theMaze, i);
    }
  }

  theMaze[currentX][currentY].wall = ALL_WALLS;
}
//------------------------------------------------------------------------------
// Core functions start here

void printEnter(int enter[DIRECTIONS], location myElement) {

  printf("(%2d,%2d) Enterable: ", myElement.x, myElement.y);
  for (int i = 0; i < DIRECTIONS; i++) {

    cout << enter[i] << ", ";
  }

  cout << endl;
}

/**
 * Name: findExit()
 * Parameters: theMaze - the 2D array representing the maze, where each element
 * is a cell that has a wall and distance member.
 * currentLocation - the current location of the mouse.
 * currentDirection - the current direction of the mouse.
 * Description: This function will be used only when the mouse is at the center,
 * and when it is trying to find an exit rather than where it entered.
 */
void findExit(cell theMaze[SIZE][SIZE], location* currentLocation,
int* currentDirection) {

  // this will represent whether an exit has been found, not center and backward
  bool found = false;

  // this will keep track of the next center to go
  int centerDirection = *currentDirection;

  // while an exit has not been found, keep going
  while (!found) {

    // move one step forward (gurantee it's another center for the first time)
    stepAtDirection(currentLocation, centerDirection);
    *currentDirection = centerDirection;

    // TODO: turn if necessary then move the mouse forward

    // get the x and y positions of the current location
    int currentX = currentLocation->x;
    int currentY = currentLocation->y;

    // check which neighbor is enterable
    int enterableNeighbors[DIRECTIONS] = {-1, -1, -1, -1};
    enterableCells(currentX, currentY, theMaze, enterableNeighbors);

    // can't go backward center
    enterableNeighbors[(*currentDirection + 2) % DIRECTIONS] = -1;

    // for loop to check all directions
    for (int i = 0; i < DIRECTIONS; i++) {

      // if it is enterable, check if it is not center, if so, then an exit has
      // been found
      if (enterableNeighbors[i] != -1) {

        if (enterableNeighbors[i] != 0) {

          found = true;
        }

        else {

          centerDirection = i;
        }
      }
    }
    
    checkStatus(theMaze, *currentLocation, *currentDirection);
  }
}

/**
 * Name: updateDistances()
 * Parameters: currentLocation - the location of the current that is being
 * examined.
 * theMaze - the 2D array representing the maze, where each element is a cell
 * that has a wall and distance member.
 * neighbors - this is an array containing the neighbors that are enterable,
 * represented by a value of 1, 0 otherwise.
 * Description: This function will be able to update the distances of the cells
 * starting from the current cell and the neighbors that has new walls added to
 * them.
 */
void updateDistances(location currentLocation, cell theMaze[SIZE][SIZE],
int neighbors[DIRECTIONS]) {

  // get the x and y position of the current location
  int currentX = currentLocation.x;
  int currentY = currentLocation.y;

  // stack to store the potentially changing cell's distance
  stack<location> myStack;

  // push the current cell
  myStack.push(currentLocation);

  // iterate through all directions
  for (int i = 0; i < DIRECTIONS; i++) {

    // if this neighbor has been set to 1, then put it to stack
    if (neighbors[i] == 1) {

      // push the neighbor if it's valid, if so, add it to stack
      populateStack(currentX, currentY, i, &myStack);
    }
  }

  // this will track the element being examined from the stack
  location myElement;

  // update while there is element in stack
  while (!myStack.empty()) {

    // get the top element
    myElement = myStack.top();
    myStack.pop();

    // the x and y positions of the current element
    int tempX = myElement.x;
    int tempY = myElement.y;

    // get the distance of the current element
    int* currentDistance = &(theMaze[tempX][tempY].distance);

    // array to keep track which neighbor is enterable (open)
    int enterableNeighbors[DIRECTIONS] = {-1, -1, -1, -1};

    // find out which neighbors can be entered (no wall in between)
    enterableCells(tempX, tempY, theMaze, enterableNeighbors);
    // printEnter(enterableNeighbors, myElement);

    // find the minimum distance among the neighbors
    int minDistance = findMinDistance(enterableNeighbors);

    // the correct distance value of the current cell is minimum distance
    // neighbor + 1
    int correctDistance = minDistance + 1;

    // check if the distance of the current element is not correct and it is not
    // the center, if so, update the distance and push the open neighbors to the
    // stack
    if (*currentDistance != correctDistance && *currentDistance != 0) {

      // update the distance of the current element
      *currentDistance = correctDistance;

      // push all reachable neighbors to stack
      for (int i = 0; i < DIRECTIONS; i++) {

        // if it's not -1, then it's reachable
        if (enterableNeighbors[i] != -1) {

          populateStack(tempX, tempY, i, &myStack);
        }
      }
    }
  }
}

/**
 * Name: evaluateCell()
 * Parameters: theMaze - the 2D array representing the maze, where each element
 * is a cell that has a wall and distance member.
 * virtualMaze - the testing maze to see where are the walls.
 * currentLocation - the current location of the mouse.
 * deadOn - the status of a dead end to populate it to other cells.
 * Description: This function will first examine if there are new walls
 * discovered around the cell, it will update the cell's wall status
 * accordingly. Then, it wiill call the helper function to update the distances
 * starting from the current cell and the neighbor cells that have added new
 * walls. It will update the deadOn status when the cell is a dead end.
 */
void evaluateCell(cell theMaze[SIZE][SIZE], cell virtualMaze[SIZE][SIZE],
location currentLocation, bool* deadOn) {

  // get the x and y position of the current location
  int currentX = currentLocation.x;
  int currentY = currentLocation.y;

  // TODO: get wall status from input (replace virtual maze)
  // get the status of the wall at the current location
  int *currentWalls = &(theMaze[currentX][currentY].wall);
  int actualWalls = virtualMaze[currentX][currentY].wall;

  // this will keep track which neighbors should be put into the stack for
  // distance update
  int neighbors[DIRECTIONS] = {0};

  // for loop to check if there is new wall discovered
  for (int i = 0; i < DIRECTIONS; i++) {

    // check if there is a wall at this direction
    if ((actualWalls & walls[i]) != 0) {

      // if there is, check if the wall not exist in theMaze, add it if that's
      // the case (new wall discovered)
      if ((*currentWalls & walls[i]) == 0) {

        // add the wall
        *currentWalls = *currentWalls | walls[i];

       // update the neighbor wall
       updateNeighborWall(currentX, currentY, theMaze, i);

       // set this neighbor to be put in stack for distance update
       neighbors[i] = 1;
      }
    }
  }

  // update the distances starting from the current cell and neighbor cells if
  // applicable
  updateDistances(currentLocation, theMaze, neighbors);

  // check if it has three walls and not the start, if so, it's dead
  if (numWalls(theMaze[currentX][currentY].wall) == DEAD_WALLS) {

    if (currentX != START_X || currentY != START_Y) {

      *deadOn = true;
    }
  }

  else {

    *deadOn = false;
  }
}

/**
 * Name: move()
 * Parameters: theMaze - the 2D array representing the maze, where each element
 * is a cell that has a wall and distance member.
 * currentLocation - the current location of the mouse.
 * currentDirection - the current direction of the mouse.
 * Description: This function will be able to decide which way to turn and move.
 * It will first attempt to go straight to the cell that is in the current
 * direction with minimum distance. If not, it will turn the mouse in the order
 * of N, E, S, W that has minimum distance.
 */
void move(cell theMaze[SIZE][SIZE], location* currentLocation,
int* currentDirection) {

  // TODO: make this function returnable, return a direction to step in

  // get the x and y position of the current location
  int currentX = currentLocation->x;
  int currentY = currentLocation->y;

  // initially, assume neighbors are not enterable
  int enterableNeighbors[DIRECTIONS] = {-1, -1, -1, -1};

  // check which neighbors can be entered (no wall in between)
  enterableCells(currentX, currentY, theMaze, enterableNeighbors);

  // find the minimum distance
  int minDistance = findMinDistance(enterableNeighbors);

  // check if the next cell of current direction has minimum distance
  if (enterableNeighbors[*currentDirection] == minDistance) {

    // TODO: Move mouse straight one step

    // check the current direction then go straight
    stepAtDirection(currentLocation, *currentDirection);
  }

  // otherwise, find the earliest direction then turn that way and move
  else {

    // find the direction that has the minimum distance, then go there
    int index = 0;
    for (; index < DIRECTIONS; index++) {

      if (enterableNeighbors[index] == minDistance) {

        break;
      }
    }

    // take a step to that direction
    stepAtDirection(currentLocation, index);

    // update the direction
    *currentDirection = index;

    // TODO: turn to this direction, then take a step
  }

  theMaze[currentLocation->x][currentLocation->y].visited = true;
}

/**
 * Name: floodMaze()
 * Parameters: theMaze - the 2D array representing the maze, where each element
 * is a cell that has a wall and distance member.
 * Description: This function will be able to flood the distances of each cell
 * of the given maze with initial distances.
 */
void floodMaze(cell theMaze[SIZE][SIZE]) {

  // constants for neighbors searching
  /**
  const int NUM_NEIGHBORS = 4;
  const int TOP = 0;
  const int RIGHT = 1;
  const int BOTTOM = 2;
  const int LEFT = 3; 

  // define all of the centers
  location center1 = {CENTER_MIN, CENTER_MIN};
  location center2 = {CENTER_MIN, CENTER_MAX};
  location center3 = {CENTER_MAX, CENTER_MIN};
  location center4 = {CENTER_MAX, CENTER_MAX};

  // the queue for storing the neighbors to be updated
  queue<location> myQueue;

  // current distance starts with 0
  int currentDistance = 0;

  // number of cells for this distance 
  int count = NUM_CENTERS;

  int tempCount = 0;

  // push all of the centers to queue
  myQueue.push(center1);
  myQueue.push(center2);
  myQueue.push(center3);
  myQueue.push(center4);

  // while loop to update all of the cell's distaces
  while (!myQueue.empty()) {

    // get the next element from the queue
    location currentPoint = myQueue.front();
    myQueue.pop();

    // update the distance at this location with the current distance
    theMaze[currentPoint.x][currentPoint.y].distance = currentDistance;

    // get the x and y of the current location
    int currentX = currentPoint.x;
    int currentY = currentPoint.y;

    // temporary x and y for the neighbors
    int tempX = 0;
    int tempY = 0;

    // try all of the neighbors
    for (int i = 0; i < NUM_NEIGHBORS; i++) {

      switch (i) {

        // top neighbor
        case TOP:
          tempX = currentX - 1;
          tempY = currentY;
          break;

        // right neighbor
        case RIGHT:
          tempX = currentX;
          tempY = currentY + 1;
          break;

        // bottom neighbor
        case BOTTOM:
          tempX = currentX + 1;
          tempY = currentY;
          break;

        // left neighbor
        case LEFT:
          tempX = currentX;
          tempY = currentY - 1;
          break;
      }

      // check if it's not center or outside
      if (!isCenter(tempX, tempY) && !isOut(tempX, tempY)) {

        // get the distance of this neighbor cell
        int neighborDistance = theMaze[tempX][tempY].distance;

        // check if the distance is 0 and not pending to be changed -1
        if (neighborDistance == 0 && neighborDistance != -1) {

          // add this location to the queue
          myQueue.push({tempX, tempY});
          theMaze[tempX][tempY].distance = -1;
          tempCount++;
        }
      }
    }

    // decrement it to keep track of the remaining cell for currentDistance
    count--;

    // if count becomes 0, then time to reset it
    if (count == 0) {

      // assign tempCount to count, then reset tempCount
      count = tempCount;
      tempCount = 0;

      // increment the currentDistance
      currentDistance++;
    }
  }*/
}

int main(int argc, char* argv[]) {

  // the current direction and position of the mouse
  int currentDirection = NORTH;
  location currentLocation = {START_X, START_Y};

  // sample maze
  int T = TOP_WALL;
  int R = RIGHT_WALL;
  int B = BOTTOM_WALL;
  int L = LEFT_WALL;
  
  /**
  // 4 by 4 maze
  cell theMaze[SIZE][SIZE] = {
    {{L|T, 0, false},   {T, 0, false},   {T, 0, false}, {R|T, 0, false}},
    {{L, 0, false},     {0, 0, false},   {0, 0, false}, {R, 0, false}},
    {{L, 0, false},     {0, 0, false},   {0, 0, false}, {R, 0, false}},
    {{L|R|B, 0, false}, {L|B, 0, false}, {B, 0, false}, {R|B, 0, false}}
  };
  cell virtualMaze[SIZE][SIZE] = {
    {{L|T, 0, false},    {T|B, 0, false},   {T|B, 0, false},   {R|T, 0, false}},
    {{L|R, 0, false},    {L|T, 0, false},   {R|T, 0, false},   {L|R, 0, false}},
    {{L|R, 0, false},    {L, 0, false},     {R, 0, false},     {L|R, 0, false}},
    {{L|R|B, 0, false},  {L|B, 0, false},   {B, 0, false},     {R|B, 0, false}}
  };*/

  /**
  // 5 by 5 maze
  cell theMaze[SIZE][SIZE] = {
    {{L|T, 0},   {T, 0},   {T, 0}, {T,0}, {R|T, 0}},
    {{L, 0},     {0, 0},   {0, 0}, {0,0}, {R, 0}},
    {{L, 0},     {0, 0},   {0, 0}, {0,0}, {R, 0}},
    {{L,0},      {0, 0},   {0, 0}, {0,0}, {R, 0}},
    {{L|R|B, 0}, {L|B, 0}, {B, 0}, {B,0}, {R|B, 0}}
  };

  cell virtualMaze[SIZE][SIZE] = {
    {{L|T|B, 0},  {T|B, 0},   {T|R, 0},   {L|T, 0}, {R|T, 0}},
    {{L|T, 0},    {T, 0},     {B, 0},     {0, 0},   {R, 0}},
    {{L|R, 0},    {L|R, 0},   {L|T, 0},   {B, 0},   {R, 0}},
    {{L, 0},      {R, 0},     {L, 0},     {T, 0},   {R, 0}},
    {{L|R|B, 0},  {L|B, 0},   {R|B, 0},   {L|B, 0}, {R|B, 0}}
  };*/

  // 16 by 16
  /**
  cell theMaze[SIZE][SIZE] = {
    {{L|T, 0, false},   {T, 0, false},   {T, 0, false}, {T, 0, false},   {T, 0, false}, {T, 0, false},   {T, 0, false}, {T, 0, false},   {T, 0, false}, {T, 0, false},   {T, 0, false}, {T, 0, false},   {T, 0, false}, {T, 0, false},   {T, 0, false}, {R|T, 0, false}},
    {{L, 0, false},     {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {R, 0, false}},
    {{L, 0, false},     {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {R, 0, false}},
    {{L, 0, false},     {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {R, 0, false}},
    {{L, 0, false},     {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {R, 0, false}},
    {{L, 0, false},     {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {R, 0, false}},
    {{L, 0, false},     {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {R, 0, false}},
    {{L, 0, false},     {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {R, 0, false}},
    {{L, 0, false},     {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {R, 0, false}},
    {{L, 0, false},     {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {R, 0, false}},
    {{L, 0, false},     {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {R, 0, false}},
    {{L, 0, false},     {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {R, 0, false}},
    {{L, 0, false},     {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {R, 0, false}},
    {{L, 0, false},     {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {R, 0, false}},
    {{L, 0, false},     {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {0, 0, false},   {0, 0, false}, {R, 0, false}},
    {{L|R|B, 0, false}, {L|B, 0, false}, {B, 0, false}, {B, 0, false},   {B, 0, false}, {B, 0, false},   {B, 0, false}, {B, 0, false},   {B, 0, false}, {B, 0, false},   {B, 0, false}, {B, 0, false},   {B, 0, false}, {B, 0, false},   {B, 0, false}, {R|B, 0, false}}
  };*/
  cell theMaze[SIZE][SIZE] = {
    {{9,14,0}, {1,13,0}, {1,12,0}, {1,11,0}, {1,10,0}, {1,9,0}, {1,8,0}, {1,7,0}, {1,7,0}, {1,8,0}, {1,9,0}, {1,10,0}, {1,11,0}, {1,12,0}, {1,13,0}, {3,14,0}},
    {{8,13,0}, {0,12,0}, {0,11,0}, {0,10,0}, {0,9,0}, {0,8,0}, {0,7,0}, {0,6,0}, {0,6,0}, {0,7,0}, {0,8,0}, {0,9,0}, {0,10,0}, {0,11,0}, {0,12,0}, {2,13,0}},
    {{8,12,0}, {0,11,0}, {0,10,0}, {0,9,0}, {0,8,0}, {0,7,0}, {0,6,0}, {0,5,0}, {0,5,0}, {0,6,0}, {0,7,0}, {0,8,0}, {0,9,0}, {0,10,0}, {0,11,0}, {2,12,0}},
    {{8,11,0}, {0,10,0}, {0,9,0}, {0,8,0}, {0,7,0}, {0,6,0}, {0,5,0}, {0,4,0}, {0,4,0}, {0,5,0}, {0,6,0}, {0,7,0}, {0,8,0}, {0,9,0}, {0,10,0}, {2,11,0}},
    {{8,10,0}, {0,9,0}, {0,8,0}, {0,7,0}, {0,6,0}, {0,5,0}, {0,4,0}, {0,3,0}, {0,3,0}, {0,4,0}, {0,5,0}, {0,6,0}, {0,7,0}, {0,8,0}, {0,9,0}, {2,10,0}},
    {{8,9,0}, {0,8,0}, {0,7,0}, {0,6,0}, {0,5,0}, {0,4,0}, {0,3,0}, {0,2,0}, {0,2,0}, {0,3,0}, {0,4,0}, {0,5,0}, {0,6,0}, {0,7,0}, {0,8,0}, {2,9,0}},
    {{8,8,0}, {0,7,0}, {0,6,0}, {0,5,0}, {0,4,0}, {0,3,0}, {0,2,0}, {0,1,0}, {0,1,0}, {0,2,0}, {0,3,0}, {0,4,0}, {0,5,0}, {0,6,0}, {0,7,0}, {2,8,0}},
    {{8,7,0}, {0,6,0}, {0,5,0}, {0,4,0}, {0,3,0}, {0,2,0}, {0,1,0}, {0,0,0}, {0,0,0}, {0,1,0}, {0,2,0}, {0,3,0}, {0,4,0}, {0,5,0}, {0,6,0}, {2,7,0}},
    {{8,7,0}, {0,6,0}, {0,5,0}, {0,4,0}, {0,3,0}, {0,2,0}, {0,1,0}, {0,0,0}, {0,0,0}, {0,1,0}, {0,2,0}, {0,3,0}, {0,4,0}, {0,5,0}, {0,6,0}, {2,7,0}},
    {{8,8,0}, {0,7,0}, {0,6,0}, {0,5,0}, {0,4,0}, {0,3,0}, {0,2,0}, {0,1,0}, {0,1,0}, {0,2,0}, {0,3,0}, {0,4,0}, {0,5,0}, {0,6,0}, {0,7,0}, {2,8,0}},
    {{8,9,0}, {0,8,0}, {0,7,0}, {0,6,0}, {0,5,0}, {0,4,0}, {0,3,0}, {0,2,0}, {0,2,0}, {0,3,0}, {0,4,0}, {0,5,0}, {0,6,0}, {0,7,0}, {0,8,0}, {2,9,0}},
    {{8,10,0}, {0,9,0}, {0,8,0}, {0,7,0}, {0,6,0}, {0,5,0}, {0,4,0}, {0,3,0}, {0,3,0}, {0,4,0}, {0,5,0}, {0,6,0}, {0,7,0}, {0,8,0}, {0,9,0}, {2,10,0}},
    {{8,11,0}, {0,10,0}, {0,9,0}, {0,8,0}, {0,7,0}, {0,6,0}, {0,5,0}, {0,4,0}, {0,4,0}, {0,5,0}, {0,6,0}, {0,7,0}, {0,8,0}, {0,9,0}, {0,10,0}, {2,11,0}},
    {{8,12,0}, {0,11,0}, {0,10,0}, {0,9,0}, {0,8,0}, {0,7,0}, {0,6,0}, {0,5,0}, {0,5,0}, {0,6,0}, {0,7,0}, {0,8,0}, {0,9,0}, {0,10,0}, {0,11,0}, {2,12,0}},
    {{8,13,0}, {0,12,0}, {0,11,0}, {0,10,0}, {0,9,0}, {0,8,0}, {0,7,0}, {0,6,0}, {0,6,0}, {0,7,0}, {0,8,0}, {0,9,0}, {0,10,0}, {0,11,0}, {0,12,0}, {2,13,0}},
    {{14,14,1}, {12,13,0}, {4,12,0}, {4,11,0}, {4,10,0}, {4,9,0}, {4,8,0}, {4,7,0}, {4,7,0}, {4,8,0}, {4,9,0}, {4,10,0}, {4,11,0}, {4,12,0}, {4,13,0}, {6,14,0}}
  };

  cell virtualMaze[SIZE][SIZE] = {
    {{L|T, 0, false},   {T|B, 0, false},   {T|B, 0, false}, {T|B, 0, false},   {T|B, 0, false}, {T|B, 0, false},   {T, 0, false}, {T, 0, false},   {T|B, 0, false}, {T|B, 0, false},   {T|B, 0, false}, {T, 0, false},   {T, 0, false}, {T, 0, false},   {T, 0, false}, {R|T, 0, false}},
    {{L, 0, false},     {T|R, 0, false},   {T|L, 0, false}, {T, 0, false},   {T|B, 0, false}, {T|R|B, 0, false},   {L|R, 0, false}, {L, 0, false},   {T|R, 0, false}, {T|L, 0, false},   {T|R, 0, false}, {L|R|B, 0, false},   {L|R|B, 0, false}, {L|R, 0, false},   {L|R, 0, false}, {L|R, 0, false}},
    {{L|R, 0, false},     {L|R, 0, false},   {L|R, 0, false}, {L|B, 0, false},   {T|B, 0, false}, {T|R, 0, false},   {L, 0, false}, {R, 0, false},   {L, 0, false}, {R|B, 0, false},   {L, 0, false}, {T, 0, false},   {T|B, 0, false}, {0, 0, false},   {R, 0, false}, {L|R, 0, false}},
    {{L|R, 0, false},     {L|R, 0, false},   {L|B, 0, false}, {T, 0, false},   {T|R|B, 0, false}, {L|R, 0, false},   {L|R, 0, false}, {L, 0, false},   {R|B, 0, false}, {T|L, 0, false},   {R|B, 0, false}, {L, 0, false},   {T, 0, false}, {R|B, 0, false},   {L|R, 0, false}, {L|R, 0, false}},
    {{L|R, 0, false},     {L, 0, false},   {T|R|B, 0, false}, {L|R, 0, false},   {T|L|B, 0, false}, {R, 0, false},   {L|B, 0, false}, {B, 0, false},   {T|B, 0, false}, {R|B, 0, false},   {T|L, 0, false}, {R, 0, false},   {L|R, 0, false}, {T|L, 0, false},   {R|B, 0, false}, {L|R, 0, false}},
    {{L|B, 0, false},     {0, 0, false},   {T|R|B, 0, false}, {L|B, 0, false},   {T|R, 0, false}, {L|R|B, 0, false},   {T|L, 0, false}, {T|B, 0, false},   {T|B, 0, false}, {T, 0, false},   {R, 0, false}, {L|R, 0, false},   {L, 0, false}, {R|B, 0, false},   {T|L, 0, false}, {R|B, 0, false}},
    {{L|T|B, 0, false},     {0, 0, false},   {T|B, 0, false}, {T|B, 0, false},   {R|B, 0, false}, {T|L, 0, false},   {B, 0, false}, {T|B, 0, false},   {T|B, 0, false}, {R, 0, false},   {L|B, 0, false}, {0, 0, false},   {R, 0, false}, {T|L|B, 0, false},   {R, 0, false}, {T|L|R|B, 0, false}},
    {{L|T|B, 0, false},     {0, 0, false},   {T|R|B, 0, false}, {T|L, 0, false},   {T|R, 0, false}, {L, 0, false},   {T|R, 0, false}, {T|L, 0, false},   {T|R, 0, false}, {L|R|B, 0, false},   {T|L, 0, false}, {R, 0, false},   {L|R|B, 0, false}, {T|L|B, 0, false},   {B, 0, false}, {T|R, 0, false}},
    {{L|T|B, 0, false},     {0, 0, false},   {T|R|B, 0, false}, {L|R, 0, false},   {L|R, 0, false}, {L|R, 0, false},   {L|R, 0, false}, {L, 0, false},   {R|B, 0, false}, {T|L, 0, false},   {R, 0, false}, {L|R|B, 0, false},   {T|L, 0, false}, {T|R, 0, false},   {T|L, 0, false}, {R, 0, false}},
    {{L|T|R, 0, false},     {L|B, 0, false},   {T|R, 0, false}, {L|R, 0, false},   {L|R, 0, false}, {L|B, 0, false},   {0, 0, false}, {R|B, 0, false},   {T|L, 0, false}, {R, 0, false},   {L|R|B, 0, false}, {T|L, 0, false},   {R, 0, false}, {L|B, 0, false},   {R|B, 0, false}, {L|R, 0, false}},
    {{L|R, 0, false},     {T|L|B, 0, false},   {B, 0, false}, {R, 0, false},   {L, 0, false}, {T|R, 0, false},   {L|B, 0, false}, {T|B, 0, false},   {R|B, 0, false}, {L|R|B, 0, false},   {T|L, 0, false}, {R, 0, false},   {L|R|B, 0, false}, {T|L|B, 0, false},   {T|B, 0, false}, {R, 0, false}},
    {{L|R, 0, false},     {T|L, 0, false},   {T|R|B, 0, false}, {L|B, 0, false},   {R, 0, false}, {L|R, 0, false},   {T|L|R, 0, false}, {T|L|R, 0, false},   {T|L, 0, false}, {T|R, 0, false},   {L|R, 0, false}, {L|R, 0, false},   {T|L, 0, false}, {T, 0, false},   {T|B, 0, false}, {R, 0, false}},
    {{L|R, 0, false},     {L, 0, false},   {T|R|B, 0, false}, {T|L, 0, false},   {R, 0, false}, {L|B, 0, false},   {R, 0, false}, {L|R, 0, false},   {L|R, 0, false}, {L|B, 0, false},   {R|B, 0, false}, {L, 0, false},   {R, 0, false}, {L|R|B, 0, false},   {T|L|B, 0, false}, {R, 0, false}},
    {{L, 0, false},     {R|B, 0, false},   {T|L, 0, false}, {R|B, 0, false},   {L|R, 0, false}, {T|L|R, 0, false},   {L, 0, false}, {R|B, 0, false},   {L|R, 0, false}, {T|L|R, 0, false},   {T|L, 0, false}, {R|B, 0, false},   {L|B, 0, false}, {T|R, 0, false},   {T|L|B, 0, false}, {R, 0, false}},
    {{L, 0, false},     {T, 0, false},   {B, 0, false}, {T|R|B, 0, false},   {L|R, 0, false}, {L|R, 0, false},   {L|R, 0, false}, {T|L, 0, false},   {R|B, 0, false}, {L|R, 0, false},   {L|B, 0, false}, {T|R, 0, false},   {T|L, 0, false}, {B, 0, false},   {T, 0, false}, {R|B, 0, false}},
    {{L|R|B, 0, false}, {L|B, 0, false}, {T|B, 0, false}, {T|B, 0, false},   {B, 0, false}, {B, 0, false},   {B, 0, false}, {B, 0, false},   {T|B, 0, false}, {B, 0, false},   {T|B, 0, false}, {B, 0, false},   {B, 0, false}, {T|B, 0, false},   {B, 0, false}, {T|R|B, 0, false}}
  };

  // true when we need to fill the cell with all walls
  bool deadOn = false;

  int previousX = 0;
  int previousY = 0;

  // sample maze print out
  cout << "Virtual Maze:" << endl;
  printMaze(virtualMaze, currentLocation, currentDirection);

  // Manually Initialized Maze
  // flood the maze with initial distance
  // floodMaze(theMaze);
  
  cout << endl << "Initial Maze: " << endl;
  checkStatus(theMaze, currentLocation, currentDirection);

  // keep moving until 0 has been found
  while (theMaze[currentLocation.x][currentLocation.y].distance != 0) {

    // move to a cell, smaller value
    move(theMaze, &currentLocation, &currentDirection);

    if (deadOn) {

      fillWalls(previousX, previousY, theMaze);
    }

    // evaluate the cell to see if there are new walls, then update the
    // distances accordingly
    evaluateCell(theMaze, virtualMaze, currentLocation, &deadOn);
    
    if (deadOn) {

      previousX = currentLocation.x;
      previousY = currentLocation.y;
    }

    // check the status through print outs
    checkStatus(theMaze, currentLocation, currentDirection);
  }

  // go back to starting point
  // findExit(theMaze, &currentLocation, &currentDirection);

  /**
  while(!(currentLocation->x == start->x && currentLocation->y == start->y)) {
    
    // move to a cell, smaller value
    move(theMaze, &currentLocation, &currentDirection);

    // evaluate the cell to see if there are new walls, then update the
    // distances accordingly
    evaluateCell(theMaze, virtualMaze, currentLocation);
    
    // check the status through print outs
    checkStatus(theMaze, currentLocation, currentDirection);
  }*/
}