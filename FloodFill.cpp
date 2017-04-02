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
const int TOP_WALL = 1;
const int RIGHT_WALL = 2;
const int BOTTOM_WALL = 4;
const int LEFT_WALL = 8;

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
 * mouseX - the current x position of the mouse.
 * mouseY - the current y position of the mosue.
 * Description: This function has the ability to print the maze using the ASCII
 * characters to represent the walls and the mouse with the given maze
 * representation as defined.
 */
void printMaze(cell theMaze[SIZE][SIZE], int mouseX, int mouseY) {

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

      printf("(%2d, %2d)", theMaze[i][j].wall, theMaze[i][j].distance);
    }

    cout << endl;
  }
}

int main(int argc, char* argv[]) {

  // initially all cells has no wall and 0 distance
  //cell theMaze[SIZE][SIZE] = {{0, 0}};

  // sample maze
  int T = TOP_WALL;
  int R = RIGHT_WALL;
  int B = BOTTOM_WALL;
  int L = LEFT_WALL;
  cell theMaze[SIZE][SIZE] = {
    {{L|T, 0},    {L|T, 0},   {R|T|B, 0},   {L|R|T, 0}},
    {{L, 0},      {0, 0},     {T|B, 0},     {R, 0}},
    {{L|R, 0},    {L|R, 0},   {L|T, 0},     {R, 0}},
    {{L|R|B, 0},  {L|R|B, 0}, {L|B, 0},     {R|B, 0}}
  };

  int mouseX = 2;
  int mouseY = 2;

  // sample maze print out
  printMaze(theMaze, mouseX, mouseY);

  // flood the maze with initial distance
  floodMaze(theMaze);

  printArray(theMaze);

}