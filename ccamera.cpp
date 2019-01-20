#include "ccamera.h"

//CCamera::CCamera(QObject *parent) : QObject(parent)
//{

//}

CCamera::CCamera(QVector3D position, QVector3D up, float yaw, float pitch) : Front(QVector3D(0.0f, 0.0f, -4.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}

//CCamera::CCamera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(QVector3D(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
//{
//    Position = QVector3D(posX, posY, posZ);
//    WorldUp = QVector3D(upX, upY, upZ);
//    Yaw = yaw;
//    Pitch = pitch;
//    updateCameraVectors();
//}

QMatrix4x4 CCamera::GetViewMatrix()
{
    QMatrix4x4 t_cam;
    t_cam.lookAt(Position, Position + Front, Up);
//    qDebug()<<Front;
    return t_cam;
}

void CCamera::ProcessKeyboard(CCamera::Camera_Movement direction, float deltaTime)
{
    float velocity = MovementSpeed * deltaTime;
    if (direction == FORWARD)
        Position += Front * velocity;
    if (direction == BACKWARD)
        Position -= Front * velocity;
    if (direction == LEFT)
        Position -= Right * velocity;
    if (direction == RIGHT)
        Position += Right * velocity;
}

void CCamera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw   += xoffset;
    Pitch += yoffset;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    // Update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}

void CCamera::ProcessMouseScroll(float yoffset)
{
    if (Zoom >= 1.0f && Zoom <= 45.0f)
        Zoom -= yoffset;
    if (Zoom <= 1.0f)
        Zoom = 1.0f;
    if (Zoom >= 45.0f)
        Zoom = 45.0f;
}

void CCamera::setTargetPosition(QVector3D fornt)
{
    Position=fornt-Front;
    qDebug()<<Position<<Position+Front<<Front;
//    Front=fornt;
//    qDebug()<<Front<<Position + Front;
}

void CCamera::updateCameraVectors()
{
    // Calculate the new Front vector
    QVector3D front;
    front.setX(-cos(qDegreesToRadians(Yaw)) * cos(qDegreesToRadians(Pitch)));
    front.setY(sin(qDegreesToRadians(Pitch)));
    front.setZ(-sin(qDegreesToRadians(Yaw)) * cos(qDegreesToRadians(Pitch)));
    Front =  3*front.normalized();
    // Also re-calculate the Right and Up vector
    Right = QVector3D::crossProduct(Front, WorldUp).normalized();  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Up    = QVector3D::crossProduct(Right, Front).normalized();
}
