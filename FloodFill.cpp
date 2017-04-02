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

// walls representations
const int TOP_WALL = 1;
const int RIGHT_WALL = 2;
const int BOTTOM_WALL = 4;
const int LEFT_WALL = 8;

/**
bool notCenterAndOut(int x, int y) {

  // check if it's one of the center
  if (x < 0 || x >= SIZE) {

    return false;
  }

  else if (y < 0 || y >= SIZE) {

    return false;
  }

  else if (x >= CENTER_MIN && x <= CENTER_MAX) {

    if (y >= CENTER_MIN && y <= CENTER_MAX) {

      return false;
    }
  }

  return true;
}*/

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
void floodMaze(int theMaze[SIZE][SIZE]) {

  for (int i = 0; i < SIZE; i++) {

    for (int j = 0; j < SIZE; j++) {

      theMaze[i][j] = 0;
    }
  }

  point center1 = make_pair(CENTER_MIN, CENTER_MIN);
  point center2 = make_pair(CENTER_MIN, CENTER_MAX);
  point center3 = make_pair(CENTER_MAX, CENTER_MIN);
  point center4 = make_pair(CENTER_MAX, CENTER_MAX);

  queue<point> myQueue;

  int current = 0;
  int count = 4;
  int tempCount = 0;

  myQueue.push(center1);
  myQueue.push(center2);
  myQueue.push(center3);
  myQueue.push(center4);

  while (!myQueue.empty()) {

    point currentPoint = myQueue.front();
    myQueue.pop();
    theMaze[currentPoint.first][currentPoint.second] = current;

    int currentX = currentPoint.first;
    int currentY = currentPoint.second;

    cout << "current X: " << currentX << " current Y: " << currentY << endl;

    int tempX = 0;
    int tempY = 0;

    // examine neighbors
    for (int i = 0; i < 4; i++) {

      switch (i) {

        // up
        case 0:
          tempX = currentX - 1;
          tempY = currentY;
          break;

        // right
        case 1:
          tempX = currentX;
          tempY = currentY + 1;
          break;

        // down
        case 2:
          tempX = currentX + 1;
          tempY = currentY;
          break;

        // left
        case 3:
          tempX = currentX;
          tempY = currentY - 1;
          break;
      }

      // check if it's not center or outside
      if (notCenterAndOut(tempX, tempY)) {

        // if not, then check if it's 0 or pending to be changed -1
        if (theMaze[tempX][tempY] == 0 && theMaze[tempX][tempY] != -1) {

          cout << "temp X: " << tempX << " temp Y: " << tempY << endl;
          // add this point to the queue
          myQueue.push(make_pair(tempX, tempY));
          theMaze[tempX][tempY] = -1;
          tempCount++;
        }
      }
    }

    cout << "count: "<< count << endl;
    count--;

    if (count == 0) {

      cout << "hello" << current << endl;
      count = tempCount;
      tempCount = 0;
      current++;
    }
  }

  for (int i = 0; i < SIZE; i++) {

    for (int j = 0; j < SIZE; j++) {

      printf("%2d",theMaze[i][j]);
    }

    cout << endl;
  }
}*/

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
 * Parameters: myArray - the 2D array to be printed.
 * Description: This function will print the given 2D array, where each element
 * is a cell, which represents walls and distance.
 */
void printArray(cell myArray[SIZE][SIZE]) {

  for (int i = 0; i < SIZE; i++) {

    for (int j = 0; j < SIZE; j++) {

      cout << "(" << myArray[i][j].wall << ", ";
      cout << myArray[i][j].distance << ") ";
    }

    cout << endl;
  }
}

int main(int argc, char* argv[]) {

  // initially all cells has no wall and 0 distance
  // cell theMaze[SIZE][SIZE];

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

  printArray(theMaze);

  // floodMaze(distanceMaze);
}












