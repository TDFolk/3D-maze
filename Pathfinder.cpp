#include "Pathfinder.h"

Pathfinder::Pathfinder() {
	srand(time(0));
}
Pathfinder::~Pathfinder() {}

/*
* getMaze
*
* Returns a string representation of the current maze. Returns a maze of all 1s if no maze
* has yet been generated or imported.
*
* A valid string representation of a maze consists only of 125 1s and 0s (each separated
* by spaces) arranged in five 5x5 grids (each separated by newlines). A valid maze must
* also have 1s in the entrance cell (0, 0, 0) and in the exit cell (4, 4, 4).
*
* Cell (0, 0, 0) is represented by the first number in the string representation of the
* maze. Increasing in x means moving toward the east, meaning cell (1, 0, 0) is the second
* number. Increasing in y means moving toward the south, meaning cell (0, 1, 0) is the
* sixth number. Increasing in z means moving downward to a new grid, meaning cell
* (0, 0, 1) is the twenty-sixth number. Cell (4, 4, 4) is represented by the last number.
*
* Returns:		string
*				A single string representing the current maze
*/
string Pathfinder::getMaze() {
	string maze_string = "";
	if (maze[0][0][0] != 1) {
		for (int i = 0; i < X; i++) {
			for (int j = 0; j < Y; j++) {
				for (int w = 0; w < Z; w++) {
					maze[i][j][w] = 1;
				}
			}
		}
	}
	stringstream ss;
	string converter;
	for (int i = 0; i < X; i++) {
		for (int j = 0; j < Y; j++) {
			for (int w = 0; w < Z; w++) {
				ss << maze[i][j][w];
				ss >> converter;
				maze_string += converter + " ";
				converter = "";
				ss.clear();
				ss.str("");
			}
			maze_string += "\n";
		}
		maze_string += "\n";
	}
	return maze_string;
}

/*
* createRandomMaze
*
* Generates a random maze and stores it as the current maze.
*
* The generated maze must contain a roughly equal number of 1s and 0s and must have a 1
* in the entrance cell (0, 0, 0) and in the exit cell (4, 4, 4).  The generated maze may be
* solvable or unsolvable, and this method should be able to produce both kinds of mazes.
*/
void Pathfinder::createRandomMaze() {
	int cell_to_add;
	int zero_counter = 0;
	int one_counter = 0;
	for (int i = 0; i < X; i++) {
		for (int j = 0; j < Y; j++) {
			for (int w = 0; w < Z; w++) {
				if ((i == 0 && j == 0 && w == 0) || (i == 4 && j == 4 && w == 4)) {
					cell_to_add = 1;
				}
				else {
					if ((one_counter - zero_counter) > 2) {
						cell_to_add = 0;
					}
					else if ((zero_counter - one_counter) > 2) {
						cell_to_add = 1;
					}
					else {
						cell_to_add = rand() % 2;
					}
					if (cell_to_add == 1) {
						one_counter++;
					}
					else {
						zero_counter++;
					}
				}
				maze[i][j][w] = cell_to_add;
			}
		}
	}
}

/*
* importMaze
*
* Reads in a maze from a file with the given file name and stores it as the current maze.
* Does nothing if the file does not exist or if the file's data does not represent a valid
* maze.
*
* The file's contents must be of the format described above to be considered valid.
*
* Parameter:	file_name
*				The name of the file containing a maze
* Returns:		bool
*				True if the maze is imported correctly; false otherwise
*/
bool Pathfinder::importMaze(string file_name) {
	ifstream in_file;
	in_file.open(file_name.c_str());
	// Prints error if bad file name is given
	if (in_file.fail()) {
		return false;
	}
	int cell;
	for (int i = 0; i < X; i++) {
		for (int j = 0; j < Y; j++) {
			for (int w = 0; w < Z; w++) {
				if (in_file >> cell) {
					if (cell == 0 || cell == 1) {
						temp_maze[i][j][w] = cell;
					}
					else {
						return false;
					}
				}
				else {
					return false;
				}
			}
		}
	}
	if (in_file >> cell) {
		return false;
	}
	else if (temp_maze[0][0][0] == 0 || temp_maze[4][4][4] == 0) {
		return false;
	}
	else {
		for(int i = 0; i < X; i++) {
			for(int j = 0; j < Y; j++) {
				for(int w = 0 ; w < Z; w++) {
					maze[i][j][w] = temp_maze[i][j][w];
				}
			}
		}
	}
	return true;
}

/*
* solveMaze
*
* Attempts to solve the current maze and returns a solution if one is found.
*
* A solution to a maze is a list of coordinates for the path from the entrance to the exit
* (or an empty vector if no solution is found). This list cannot contain duplicates, and
* any two consecutive coordinates in the list can only differ by 1 for only one
* coordinate. The entrance cell (0, 0, 0) and the exit cell (4, 4, 4) should be included
* in the solution. Each string in the solution vector must be of the format "(x, y, z)",
* where x, y, and z are the integer coordinates of a cell.
*
* Understand that most mazes will contain multiple solutions
*
* Returns:		vector<string>
*				A solution to the current maze, or an empty vector if none exists
*/
vector<string> Pathfinder::solveMaze() {
	solve.clear();
	if (find_maze_path(0, 0, 0)) {
		change_back();
	}
	else {
		solve.clear();
		change_back();
	}
	return solve;
}

bool Pathfinder::find_maze_path(int r, int c, int g) {
	if (r < 0 || c < 0 || g < 0 || r >= X || c >= Y || g >= Z) {
		return false;
	}
	else if (maze[r][c][g] != 1) {
		return false;
	}
	else if (r == (X - 1) && c == (Y - 1) && g == (Z - 1)) {
		solve.push_back("(4, 4, 4)");
		return true;
	}
	else {
		maze[r][c][g] = 2;
		if (find_maze_path(r - 1, c, g)
			|| find_maze_path(r + 1, c, g)
			|| find_maze_path(r, c - 1, g)
			|| find_maze_path(r, c + 1, g) 
			|| find_maze_path(r, c, g - 1)
			|| find_maze_path(r, c, g + 1)) {
			stringstream ss1;
			ss1 << r;
			string x = ss1.str();
			stringstream ss2;
			ss2 << c;
			string y = ss2.str();
			stringstream ss3;
			ss3 << g;
			string z = ss3.str();
			string path = "(" + x + ", " + y + ", " + z + ")";
			solve.insert(solve.begin(), path);
			return true;
		}
		else {
			return false;
		}
	}
}

void Pathfinder::change_back() {
	for(int i = 0; i < X; i++) {
		for(int j = 0; j < Y; j++) {
			for(int w = 0 ; w < Z; w++) {
				if (maze[i][j][w] == 2) {
					maze[i][j][w] = 1;
				}
			}
		}
	}
}