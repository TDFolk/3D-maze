#pragma once
#include <vector>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <fstream>
#include "PathfinderInterface.h"

class Pathfinder : public PathfinderInterface {
private:
	static const int X = 5;
	static const int Y = 5;
	static const int Z = 5;
	int maze[X][Y][Z];
	int temp_maze[X][Y][Z];
	vector<string> solve;

public:
	Pathfinder();
	virtual ~Pathfinder();

	virtual string getMaze();

	virtual void createRandomMaze();

	virtual bool importMaze(string file_name);

	virtual vector<string> solveMaze();

	bool find_maze_path(int r, int c, int g);

	void change_back();
};

