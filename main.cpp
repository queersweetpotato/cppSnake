#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>

using namespace std;
//make a class snake which has a field of the snake (array) and the board(dimensions???) and methods move() and eat()
//field for apple coordinates
//make a method for printing the snake

class Snake {
  private:
    vector<vector<int>> snake;
    vector<int> appleCoordinates = {-1, -1};
    //coordinates are (x, y) and are 1-indexed
    int boardSize;
    int score; //the number of apples eaten
  public:
    Snake() {
      int size = 0;
      //set board size, checking for invalid input
      while(!setBoardSize(size)) {
	cout << "Please input a board size between 10 and 30\n";
	if(!(cin >> size)) {
	  cout << "Please enter a number\n";
	  cin.clear();
          cin.ignore(10000, '\n');
	  continue;
	}
	cin.clear();
	cin.ignore(10000, '\n');
        continue;
      }

       //initialise snake placement, he is 3 long and in the middle
      int middle = ceil(boardSize/2);
      snake.push_back({2, middle});
      snake.push_back({3, middle});
      snake.push_back({4, middle});
      
      generateApple();
      printSnake();

      score = 0;
    }
    //getters and setters
    bool setBoardSize(int size) {
      //check input to see that it is positive and at least SOME CONSTANT --> return false if not
      if(size>=10 && size<=30) {
        boardSize = size;
        return true;
      }
      return false;
    }

    int getScore() {
      return score;
    }

    int move() {
      char direction;
      while(direction!='w' && direction!='a' && direction!='s' && direction!='d') {
        cout << "Please enter a direction (w,a,s,d)\n";
        cin >> direction;
	cin.clear();
	cin.ignore(10000, '\n');
      }
      //check that it is either w, a, s, or d
      //turn the different directions into arrays e.g. (1,0) for later checks and actual movement
      int movement[2];
      switch(direction) {
        case 'w':
	  movement[0] = 0;
	  movement[1] = 1;
	  cout << "moving up!\n";
	  break;
	case 'a':
	  movement[0] = 1;
	  movement[1] = 0;
	  cout << "moving left!\n";
	  break;
	case 's':
	  movement[0] = 0;
	  movement[1] = -1;
	  cout << "moving down!\n";
	  break;
	case 'd':
	  movement[0] = -1;
	  movement[1] = 0;
	  cout << "moving right!\n";
	  break;
      }

      vector<int> nextCoords = {snake.front().at(0)+movement[0], snake.front().at(1)+movement[1]};

      //check if the snake would hit the wall
      if(nextCoords.at(0) == 0 || nextCoords.at(0)> boardSize || nextCoords.at(1) == 0 || nextCoords.at(1) > boardSize) {
        cout << "You've hit the wall!\n"
      }

      //check if the snake would hit itself
      if(!verifyCoordinates(nextCoords)) { //ADD COORDS INTO THIS
        cout << "You've hit yourself!\n"
	return 1;
      }
      
      //if it's legal, move the goober (HARD PART)
      return 0;
    }

    void eat() {
      //eat apple -> add to the end of the snake
      score++;
    }

    void generateApple() {
      //generates apple location, changes the field of apple coordinate
      do{
        appleCoordinates.at(0) = rand() % (boardSize + 1); 
        appleCoordinates.at(1) = rand() % (boardSize + 1);
      } while(!verifyCoordinates(appleCoordinates));
      //must verify that the apple isn't inside the snake
    }

    //method to verify that a given coordinate pair isn't inside the snake
    bool verifyCoordinates(vector<int> coords) {
      for(vector<int> v : snake) {
        if(v==coords) {
	  return false;
	}
      }
      return true;
    }

    void printSnake() {
      //print the snake/apple onto the board
      cout << "snake: ";
      for(vector<int> v : snake) {
        cout << "[" << v.at(0) << ", " << v.at(1) << "]";
      }
      cout << "\nApple: [" << appleCoordinates.at(0) << ", " << appleCoordinates.at(1) << "]\n";
      //if you want to be efficient, don't generate a new board every time --> EXTRA TASK LATER
    }
};

int main() {
  srand(time(0));
  Snake mySnake = Snake();
  mySnake.move();
  //infinite loop of move() and printSnake() while snake is alive
  return 0;
}
