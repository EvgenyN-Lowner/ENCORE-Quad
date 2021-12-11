#include "debugENCORE.h"
#include "raylib.h"
#include "raymath.h"

void CreateDebugWindow(Camera3D *camera) {

	DrawText(TextFormat("CAMERA.POS: %.01f", camera->position.x), 10, 10, 25, BLUE);
	DrawText(TextFormat(",%.01f", camera->position.y), 250, 10, 25, BLUE);
	DrawText(TextFormat(",%.01f", camera->position.z), 320, 10, 25, BLUE);
	DrawText(TextFormat("CAMERA.TAR: %.01f", camera->target.x), 10, 40, 25, DARKGREEN);
	DrawText(TextFormat(", %.01f", camera->target.y), 250, 40, 25, DARKGREEN);
	DrawText(TextFormat(", %.01f", camera->target.z), 320, 40, 25, DARKGREEN);

	DrawText(TextFormat("CAMERA.UP: %.01f", camera->up.x), 10, 70, 25, DARKGREEN);
	DrawText(TextFormat(", %.01f", camera->up.y), 250, 70, 25, DARKGREEN);
	DrawText(TextFormat(", %.01f", camera->up.z), 320, 70, 25, DARKGREEN);

}