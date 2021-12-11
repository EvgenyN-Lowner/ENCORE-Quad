#include "raylib.h"
#include "CameraController.h"
#include "Grid.h"
#include "debugENCORE.h"

#define GLSL_VERSION            330

#define RLIGHTS_IMPLEMENTATION
#include "rlights.h"

const int WindowWidth = 1920;
const int WindowHeight = 1080;
const Vector3 InitCamPos = { -10, 10, 0 };
const Vector3 InitTargetPos = { 0,0,0 };
const Vector3 InitRotation = { 0, 1,0 };

void InitShaders(Shader shader) {
		shader.locs[SHADER_LOC_MATRIX_MODEL] = GetShaderLocation(shader, "matModel");
		shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");

		int ambientLoc = GetShaderLocation(shader, "ambient");
		float delta[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
		SetShaderValue(shader, ambientLoc, delta, SHADER_UNIFORM_VEC4);
}

int main(void) {

	InitWindow(WindowWidth, WindowHeight, "ENCORE Engine");
	SetTargetFPS(60);
	ENCORECamera camera;

	InitENCORECamera(&camera, 45, InitTargetPos);

	Shader shader = LoadShader("resources/shaders/base_lighting.vs", "resources/shaders/lighting.fs");	
	Shader shader_default = { 0 };
	InitShaders(shader);
	Light light_1 = CreateLight(LIGHT_POINT, { 0, 2.f, 0 }, Vector3Zero(), WHITE, shader);
	UpdateLightValues(shader, light_1);

	float phaseLight = .0f;
	Vector3 phase = { 0 };
	Camera3D camera_shadow_map = { 0 };

	Model model = LoadModel("resources/delta.obj");
	Model plane = LoadModel("resources/plane_delta.obj");
	Texture2D texture = LoadTexture("resources/defalutMat.jpg");
	Texture2D texture2 = LoadTexture("resources/defalutMat2.jpg");
	Model torus = { 0 };


	RenderTexture2D render_texture = LoadRenderTexture(160, 100);

	camera_shadow_map.fovy = 20.0f;
	camera_shadow_map.target = { .0f, .0f, .0f };
	camera_shadow_map.position = { 0.0f, 10.0f, 0.0f };
	camera_shadow_map.up = { 0.0f, 0.0f,-1.0f };
	camera_shadow_map.projection = CAMERA_PERSPECTIVE;

	shader_default = model.materials[0].shader;
	model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = render_texture.texture;
	plane.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = render_texture.texture;

	while (!WindowShouldClose()) {
		phaseLight += 0.05f;
		camera_shadow_map.position.x = light_1.position.x = sinf(phaseLight) * 4.0f;
		UpdateLightValues(shader, light_1);

		phase = Vector3Add(phase, { 0.01f, 0.02f, 0.03f });
		UpdateENCORECamera(&camera);

		camera_shadow_map.fovy += IsKeyDown(KEY_KP_ADD) * 0.1f;
		camera_shadow_map.fovy -= IsKeyDown(KEY_KP_SUBTRACT) * 0.1f;

		model.materials[0].shader = shader_default;
		plane.materials[0].shader = shader_default;

		BeginTextureMode(render_texture); {
			ClearBackground(GRAY);
			BeginMode3D(camera_shadow_map); {
		        DrawCube(camera.CameraPosition, 0.1f, 0.1f, 0.1f, RED);
				DrawModel(model, { 3,0,0 }, 0.0254f, DARKGRAY);
				DrawModel(plane, InitTargetPos, 0.0254f, DARKGRAY);
			}
			EndMode3D();
		}EndTextureMode();

		model.materials[0].shader = shader;
		plane.materials[0].shader = shader;

		BeginDrawing(); {
			ClearBackground(BLACK);
			DrawFPS(10, 10);

			BeginMode3D(camera.ViewCamera); {
				DrawCube(camera.CameraPosition, 0.1f, 0.1f, 0.1f, RED);
				DrawModel(model, { 3,0,0 }, 0.0254f, GREEN);
				DrawModel(plane, InitTargetPos, 0.0254f, GREEN);
				//CreateGrid(50, 50, 1, InitTargetPos);
			}
			EndMode3D();
		}
		EndDrawing();
	}

	CloseWindow();
}