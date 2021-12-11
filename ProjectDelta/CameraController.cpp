#include "CameraController.h"

void ResizeENCORECameraView(ENCORECamera* camera) {
	if (camera == NULL)
		return;

	float width = (float)GetScreenWidth();
	float height = (float)GetScreenHeight();

	camera->FOV.y = camera->ViewCamera.fovy;

	if (height != 0)
		camera->FOV.x = camera->FOV.y * (width / height);
}

void InitENCORECamera(ENCORECamera* camera, float fovY, Vector3 position) {
	if (camera == NULL)
		return;

    camera->ControlKeys[0] = 'W';
    camera->ControlKeys[1] = 'S';
    camera->ControlKeys[2] = 'D';
    camera->ControlKeys[3] = 'A';
    camera->ControlKeys[4] = 'E';
    camera->ControlKeys[5] = 'Q';
    camera->ControlKeys[6] = KEY_LEFT;
    camera->ControlKeys[7] = KEY_RIGHT;
    camera->ControlKeys[8] = KEY_UP;
    camera->ControlKeys[9] = KEY_DOWN;
    camera->ControlKeys[10] = KEY_LEFT_SHIFT;

    camera->MoveSpeed = { 3,3,3 };
    camera->TurnSpeed = { 90,90 };
    camera->MouseSensitivity = 600;
    
    camera->MinimumViewY = 1.0f;
    camera->MaximumViewY = 89.0f;

    camera->PreviousMousePosition = GetMousePosition();
    camera->Focused = IsWindowFocused();
    camera->CameraPullbackDistance = 5;
    camera->ViewAngles = { 0,0 };

    camera->CameraPosition = position;
    camera->FOV.y = fovY;

    camera->ViewCamera.target = position;
    camera->ViewCamera.position = Vector3Add(camera->ViewCamera.target, { 0,0,camera->CameraPullbackDistance });
    camera->ViewCamera.up = { 0.0f, 1.0f, 0.0f };
    camera->ViewCamera.fovy = fovY;
    camera->ViewCamera.projection = CAMERA_PERSPECTIVE;

    ResizeENCORECameraView(camera);
    UseENCORECameraMouse(camera, true, 2);
}

void UseENCORECameraMouse(ENCORECamera* camera, bool useMouse, int button) {
    if (camera == NULL)
        return;

    camera->UseMouse = useMouse;
    camera->UseMouseButton = button;

    bool showCursor = !useMouse || button >= 0;

    if (!showCursor && IsWindowFocused())
        DisableCursor();
    else if (showCursor && IsWindowFocused())
        EnableCursor();
}

Vector3 GetENCORECameraPosition(ENCORECamera* camera) {
    return camera->CameraPosition;
}

static float GetSpeedForAxis(ENCORECamera* camera, CameraControls axis, float speed) {
    if (camera == NULL)
        return 0;

    int key = camera->ControlKeys[axis];
    if (key == -1)
        return 0;
    
    float factor = 1.0f;
    if (IsKeyDown(camera->ControlKeys[SPRINT]))
        factor = 2;
    if (IsKeyDown(camera->ControlKeys[axis]))
        return speed * GetFrameTime() * factor;

    return 0.0f;
}

void UpdateENCORECamera(ENCORECamera* camera) {
    if (camera == NULL)
        return;

    if (IsWindowResized())
        ResizeENCORECameraView(camera);

    bool showCursor = !camera->UseMouse || camera->UseMouseButton >= 0;

    if (IsWindowFocused() != camera->Focused && !showCursor) {
        camera->Focused = IsWindowFocused();
        if (camera->Focused) {
            DisableCursor();
            camera->PreviousMousePosition = GetMousePosition();
        }
        else {
            EnableCursor();
        }
    }

    Vector2 mousePositionDelta = { 0.0f, 0.0f };
    Vector2 mousePosition = GetMousePosition();
    float mouseWheelMove = -GetMouseWheelMove();

    float direction[MOVE_DOWN + 1] = {
        -GetSpeedForAxis(camera,MOVE_FRONT, camera->MoveSpeed.z),
        -GetSpeedForAxis(camera,MOVE_BACK, camera->MoveSpeed.z),
        GetSpeedForAxis(camera,MOVE_RIGHT, camera->MoveSpeed.x),
        GetSpeedForAxis(camera,MOVE_LEFT, camera->MoveSpeed.x),
        GetSpeedForAxis(camera,MOVE_UP, camera->MoveSpeed.y),
        GetSpeedForAxis(camera,MOVE_DOWN, camera->MoveSpeed.y),
    };

    mousePositionDelta.x = mousePosition.x - camera->PreviousMousePosition.x;
    mousePositionDelta.y = mousePosition.y - camera->PreviousMousePosition.y;

    camera->PreviousMousePosition = mousePosition;

    bool useMouse = camera->UseMouse && (camera->UseMouseButton < 0 || IsMouseButtonDown(camera->UseMouseButton));
    
    float turnRotation = GetSpeedForAxis(camera, TURN_RIGHT, camera->TurnSpeed.x) - GetSpeedForAxis(camera, TURN_LEFT, camera->TurnSpeed.x);
    float tiltRotation = GetSpeedForAxis(camera, TURN_UP, camera->TurnSpeed.y) - GetSpeedForAxis(camera, TURN_DOWN, camera->TurnSpeed.y);

    if (turnRotation != 0)
        camera->ViewAngles.x -= turnRotation * DEG2RAD;
    else if (useMouse && camera->Focused)
        camera->ViewAngles.x -= (mousePositionDelta.x / -camera->MouseSensitivity);

    if (tiltRotation)
        camera->ViewAngles.y -= tiltRotation * DEG2RAD;
    else if (useMouse && camera->Focused)
        camera->ViewAngles.y -= (mousePositionDelta.y / -camera->MouseSensitivity);

    if (camera->ViewAngles.y < camera->MinimumViewY * DEG2RAD)
        camera->ViewAngles.y = camera->MinimumViewY * DEG2RAD;
    else if (camera->ViewAngles.y > camera->MaximumViewY * DEG2RAD)
        camera->ViewAngles.y = camera->MaximumViewY * DEG2RAD;

    Vector3 moveVec = { 0,0,0 };
    moveVec.z = direction[MOVE_FRONT] - direction[MOVE_BACK];
    moveVec.x = direction[MOVE_RIGHT] - direction[MOVE_LEFT];

    camera->CameraPullbackDistance += GetMouseWheelMove();
    if (camera->CameraPullbackDistance < 1)
        camera->CameraPullbackDistance = 1;

    Vector3 camPos = { 0,0, camera->CameraPullbackDistance };

    Matrix tiltMat = MatrixRotateX(camera->ViewAngles.y);
    Matrix rotMat = MatrixRotateY(camera->ViewAngles.x);
    Matrix mat = MatrixMultiply(tiltMat, rotMat);

    camPos = Vector3Transform(camPos, mat);
    moveVec = Vector3Transform(moveVec, rotMat);

    camera->CameraPosition = Vector3Add(camera->CameraPosition, moveVec);

    camera->ViewCamera.target = camera->CameraPosition;
    camera->ViewCamera.position = Vector3Add(camera->CameraPosition, camPos);
}