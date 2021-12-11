#ifndef CAMERA_CONTROLLER_H
#define CAMERA_CONTROLLER_H

#include "raylib.h"
#include "raymath.h"

typedef enum {
    MOVE_FRONT = 0,
    MOVE_BACK,
    MOVE_RIGHT,
    MOVE_LEFT,
    MOVE_UP,
    MOVE_DOWN,
    TURN_LEFT,
    TURN_RIGHT,
    TURN_UP,
    TURN_DOWN,
    SPRINT,
    LAST_CONTROL
}CameraControls;

typedef struct Transform3D
{
    Vector3 position;
    Quaternion rotation;
    Vector3 scale;

    Matrix world;
}Transform3D;

typedef struct {
    //Клавиши для перемещения камеры
    int ControlKeys[LAST_CONTROL];

    /*Скорость ЮНИТЫ / СЕКУНДЫ для перемешения
    * X = sidestep | Y = jump/fall | Z = forward;
    */
    Vector3 MoveSpeed;
    //Скорость ГРАДУСЫ / СЕКУНДЫ для поворота
    Vector2 TurnSpeed;

    bool UseMouse;
    int UseMouseButton;

    float MouseSensitivity;

    float MinimumViewY;
    float MaximumViewY;

    //Позиция TARGET RIG
    Vector3 CameraPosition;

    //Длина RIG
    float CameraPullbackDistance;

    Camera ViewCamera;
    Vector3 ViewForward;
    Vector3 FOV;
    Vector2 PreviousMousePosition;
    Vector2 ViewAngles;

    bool Focused;

}ENCORECamera;

RLAPI void InitENCORECamera(ENCORECamera* camera, float fovY, Vector3 position);
RLAPI void UseENCORECameraMouse(ENCORECamera* camera, bool useMouse, int button);
RLAPI Vector3 GetENCORECameraPosition(ENCORECamera* camera);
RLAPI void UpdateENCORECamera(ENCORECamera* camera);

#endif