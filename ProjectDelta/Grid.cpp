#include "raylib.h"
#include <vector>
#include <iostream>

void CreateGrid(int width, int height, int cellSize, Vector3 initPos) {
	//std::vector<std::vector<int>> gridArray(width, std::vector<int> (height,0));
	for (float i = 0; i < width; i++) {
		for (float j = 0; j < height; j++) {
			DrawCube({i, 0,j}, 0.1f, 0.1f, 0.1f, DARKGREEN);
		}
	}
}