#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <string>

using namespace std;
//make a class snake which has a field of the snake (vector of vectors) and the board and methods move() and eat()
//field for apple coordinates
//make a method for printing the snake

class Snake {
  private:
    vector<vector<int>> snake;
    vector<int> appleCoordinates = {-1, -1};
    //coordinates are (x, y) and are 1-indexed
    int boardSize;
    int score = 0; //the number of apples eaten
    string board[32]; 
    //array of strings for board --> max board size is 30, 2 extra strings for horizontal top and bottom border
    //each string is one horizontal line of the board

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

       //initialise snake placement, he is 3 long and in the middle, snake head is the first entry
      int middle = ceil(boardSize/2);
      snake.push_back({4, middle});
      snake.push_back({3, middle});
      snake.push_back({2, middle});
     
      //set up empty board
      for(int i = 0; i<boardSize; i++) {
        board[i] = "";
      }

      //fill board line by line
      for(int i = 0; i<boardSize+2; i++) {
	for(int j = 0; j<boardSize+2; j++) {
          //top and bottom row are ----- horizontal borders
          if(j==0 || j==boardSize+1) {
	    board[j] = board[j].append("-"); 
	  }

	  //first and last column are | vertical borders
	  else if(i==0 || i==boardSize+1) {
            board[j] = board[j].append("|");
	  }

	  else {
	    board[j] = board[j].append(" ");
	  }
        }
      }
     
      //when graphically editing the board, coordinates are in y,x format

      //place snake --> indicated with #
      for(vector<int> v : snake) {
        board[v.at(1)][v.at(0)] = '#'; 
      }

      //place apple --> indicated with o
      generateApple();
      printSnake();
    }
    //getters and setters
    bool setBoardSize(int size) {
      //check input to see that it is positive and within bounds
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
	  movement[1] = -1;
	  cout << "moving up!\n";
	  break;
	case 'a':
	  movement[0] = -1;
	  movement[1] = 0;
	  board[appleCoordinates.at(1)][appleCoordinates.at(0)] = 'o';cout << "moving left!\n";
	  break;
	case 's':
	  movement[0] = 0;
	  movement[1] = 1;
	  cout << "moving down!\n";
	  break;
	case 'd':
	  movement[0] = 1;
	  movement[1] = 0;
	  cout << "moving right!\n";
	  break;
      }

      vector<int> nextCoords = {snake.front().at(0)+movement[0], snake.front().at(1)+movement[1]};

      //check if the snake would hit itself or the wall
      if(!verifyCoordinates(nextCoords)) { 
        cout << "You've died!\n";
	return 1;
      }

      //check if the snake is eating an apple
      if(nextCoords == appleCoordinates) {
        cout << "Eating apple!\n";
	eat();
      }

      //move the snake on the board
      board[nextCoords.at(1)][nextCoords.at(0)] = '#';
      board[snake.at(snake.size()-1).at(1)][snake.at(snake.size()-1).at(0)] = ' ';

      //move the snake
      for(int i=snake.size()-1; i>0; i--) {
        snake.at(i) = snake.at(i-1);
      }
      snake.at(0) = nextCoords; 
      return 0;
    }

    void eat() {
      //eat apple -> add to the end of the snake
      score++;
      generateApple();

      //SHOULD figure out which direction to place the new place (e.g. if going up, place it at the bottom), currently will just put it in the first place that isn't in the wall or the snake
      snake.push_back(snake.at(snake.size()-1));
      
      //default add it to the left side of the snake
      if(verifyCoordinates({snake.at(snake.size()-1).at(0)-1, snake.at(snake.size()-1).at(1)})) {
        snake.at(snake.size()-1).at(0)--;
	return;
      }

      //if left side doesn't work, try below
      if(verifyCoordinates({snake.at(snake.size()-1).at(0), snake.at(snake.size()-1).at(1)+1})) {
        snake.at(snake.size()-1).at(1)++;
        return;
      }

      //then try right
      if(verifyCoordinates({snake.at(snake.size()-1).at(0)+1, snake.at(snake.size()-1).at(1)})) {
        snake.at(snake.size()-1).at(0)++;
        return;
      }

      //finally try above
      if(verifyCoordinates({snake.at(snake.size()-1).at(0),snake.at(snake.size()-1).at(1)-1})) {
        snake.at(snake.size()-1).at(1)--;
        return;
      }

      //else print that the board is full
      cout << "the board is full\n";
    }

    void generateApple() {
      //generates apple location, changes the field of apple coordinate
      do{
        appleCoordinates.at(0) = rand() % (boardSize + 1); 
        appleCoordinates.at(1) = rand() % (boardSize + 1);
      } while(!verifyCoordinates(appleCoordinates));
      //must verify that the apple isn't inside the snake

      board[appleCoordinates.at(1)][appleCoordinates.at(0)] = 'o';
    }

    //method to verify that a given coordinate pair isn't inside the snake or the wall
    bool verifyCoordinates(vector<int> coords) {
      for(vector<int> v : snake) {
        if(v==coords) {
	  return false;
	}
      }
      if(coords.at(0) == 0 || coords.at(0)> boardSize || coords.at(1) == 0 || coords.at(1) > boardSize) {
        return false;
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
      cout << "Score: " << score << "\n";

      //print board
      for(int i=0; i<boardSize+2; i++) {
        cout << board[i] << "\n";
      }
      
    }
};

int main() {
  srand(time(0));
  Snake mySnake = Snake();
  while(!mySnake.move()) {
    mySnake.printSnake();
  }
  //infinite loop of move() and printSnake() while snake is alive
  return 0;
}
