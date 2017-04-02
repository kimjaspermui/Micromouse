#include <utility>
#include <queue>
#include <vector>
#include <iostream>

using namespace std;

//typedef pair<int, int> point;

struct location {

  int x;
  int y;
};

// need visited member?
struct cell {

  int wall;
  int distance;
};

// NOTE: need static allocation instead of dynamic
const int SIZE = 4;
const int CENTER_MIN = SIZE / 2 - 1;
const int CENTER_MAX = SIZE / 2;
const int NUM_CENTERS = 4;

// walls representations
const int WALLS = 4;
const int TOP_WALL = 1;
const int RIGHT_WALL = 2;
const int BOTTOM_WALL = 4;
const int LEFT_WALL = 8;
const int walls[WALLS] = {1, 2, 4, 8};

// directions representations
const int DIRECTIONS = 4;
const int NORTH = 0;
const int EAST = 1;
const int SOUTH = 2;
const int WEST = 3;
const string directions[DIRECTIONS] = {"North", "East", "South", "West"};

/**
 * Name: notCenter()
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
 * Name: notOut()
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
void floodFill(int xStart, int yStart) {

  // 2d array representing the maze size 16 x 16?
  int theMaze[SIZE][SIZE] = {0};
  int mazePath[SIZE][SIZE] = {0};

  // get the center position
  point center = make_pair(xCenter, yCenter);

  // flood the cells with values starting from center, no walls
  floodMaze(theMaze);

  // note that there will always have a wall on the right side of starting cell

  // while goal not found
    // move the mouse, choose a smaller value
    // if tie, do the direction decision here
    // if new wall found
      updateDistances(current, theMaze);
      printMaze(theMaze);
      printMazePath(mazePath);
}
*/

/**
void updateDistances(point current, &int theMaze[][]) {

  // stack to store the potentially changing points
  stack<point> myStack;

  myStack.push(current);

  // find adjacent cell that is beside the new wall

  myStack.push(adjacent);

  // update while there is element in stack
  while (!myStack.empty()) {

    // get the top element
    current = myStack.top();
    myStack.pop();

    int correct = // minimum reachable neighbor + 1

    //if (// current's value != correct && current's value != 0) {

      // current's value = correct;
      // push all reachable neighbors to stack
    }
  }
}*/

/**
 */
void evaluateCell(cell theMaze[SIZE][SIZE], cell virtualMaze[SIZE][SIZE],
location currentLocation) {

  // get the x and y position of the current location
  int currentX = currentLocation.x;
  int currentY = currentLocation.y;

  // TODO: request for new walls status
  // get the status of the wall at the current location
  int *currentWalls = &(theMaze[currentX][currentY].wall);
  int actualWalls = virtualMaze[currentX][currentY].wall;

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

       // update the distances starting from the current cell and neighbor cell
       // if applicable
       //updateDistances(currentX, currentY, theMaze, i);
      }
    }
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
  }
}

/**
 * Name: printMaze()
 * Parameters: theMaze - the 2D array representing the maze, where each element
 * is a cell that has a wall and distance member.
 * currentLocation - the current location of the mouse
 * Description: This function has the ability to print the maze using the ASCII
 * characters to represent the walls and the mouse with the given maze
 * representation as defined.
 */
void printMaze(cell theMaze[SIZE][SIZE], location currentLocation) {

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

        cout << "*";
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

      printf("(%2d,%2d)", theMaze[i][j].wall, theMaze[i][j].distance);
    }

    cout << endl;
  }
}

void checkStatus(cell theMaze[SIZE][SIZE], location currentLocation,
int currentDirection) {

 // check status of the maze and mouse
  printMaze(theMaze, currentLocation);
  printArray(theMaze);
  cout << "Current Direction: " << directions[currentDirection] << endl;
  cout << "X: " << currentLocation.x << " Y: " << currentLocation.y << endl;
}

int main(int argc, char* argv[]) {

  // the current direction and position of the mouse
  int currentDirection = NORTH;
  location currentLocation = {3, 0};

  // sample maze
  int T = TOP_WALL;
  int R = RIGHT_WALL;
  int B = BOTTOM_WALL;
  int L = LEFT_WALL;
  cell theMaze[SIZE][SIZE] = {
    {{L|T, 0},   {T, 0},   {T, 0}, {R|T, 0}},
    {{L, 0},     {0, 0},   {0, 0}, {R, 0}},
    {{L, 0},     {0, 0},   {0, 0}, {R, 0}},
    {{L|R|B, 0}, {L|B, 0}, {B, 0}, {R|B, 0}}
  };
  cell virtualMaze[SIZE][SIZE] = {
    {{L|T, 0},    {L|T, 0},   {R|T|B, 0},   {L|R|T, 0}},
    {{L, 0},      {0, 0},     {T|B, 0},     {R, 0}},
    {{L|R, 0},    {L|R, 0},   {L|T, 0},     {R, 0}},
    {{L|R|B, 0},  {L|R|B, 0}, {L|B, 0},     {R|B, 0}}
  };

  // sample maze print out
  printMaze(theMaze, currentLocation);

  // flood the maze with initial distance
  floodMaze(theMaze);

  // keep moving until 0 has been found
  while (theMaze[currentLocation.x][currentLocation.y].distance != 0) {

    // move to a cell, smaller value
    move(theMaze, &currentLocation, &currentDirection);

    // evaluate the cell to see if there are new walls, then update the
    // distances accordingly
    evaluateCell(theMaze, virtualMaze, currentLocation);
    
    // check the status through print outs
    checkStatus(theMaze, currentLocation, currentDirection);
  }
}