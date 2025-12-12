////////////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "cameraclass.h"


CameraClass::CameraClass()
{
	m_position.x = 0.0f;
	m_position.y = 0.0f;
	m_position.z = 0.0f;

	m_rotation.x = 0.0f;
	m_rotation.y = 0.0f;
	m_rotation.z = 0.0f;

	m_yaw = 0.0f;
	m_pitch = 0.0f;

	m_mouseSensitivity = 3.0f;
}


CameraClass::CameraClass(const CameraClass& other)
{
}


CameraClass::~CameraClass()
{
}


void CameraClass::SetPosition(float x, float y, float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}


void CameraClass::SetRotation(float x, float y, float z)
{
	m_rotation.x = x;
	m_rotation.y = y;
	m_rotation.z = z;
}


XMFLOAT3 CameraClass::GetPosition()
{
	return m_position;
}


XMFLOAT3 CameraClass::GetRotation()
{
	//return m_rotation;
	return XMFLOAT3(m_pitch, m_yaw, 0.0f);
}

void CameraClass::UpdateRotation(float deltaX, float deltaY)
{
	// 마우스 감도
	float sensitivity = 0.1f;

	// 마우스 이동량 반영
	m_rotation.y += deltaX * sensitivity; // Yaw (좌우)
	m_rotation.x += deltaY * sensitivity; // Pitch (상하)

	if (m_rotation.x > 89.0f)
		m_rotation.x = 89.0f;
	if (m_rotation.x < -89.0f)
		m_rotation.x = -89.0f;
}

XMFLOAT3 CameraClass::GetForwardVector()
{
	float pitch = XMConvertToRadians(m_rotation.x);
	float yaw = XMConvertToRadians(m_rotation.y);

	XMFLOAT3 forward;
	forward.x = cosf(pitch) * sinf(yaw);
	forward.y = sinf(pitch);
	forward.z = cosf(pitch) * cosf(yaw);

	XMVECTOR vec = XMVector3Normalize(XMLoadFloat3(&forward));
	XMStoreFloat3(&forward, vec);

	return forward;
}

XMFLOAT3 CameraClass::GetRightVector()
{
	float yaw = XMConvertToRadians(m_rotation.y);

	XMFLOAT3 right;
	right.x = sinf(yaw - XM_PIDIV2);
	right.y = 0.0f;
	right.z = cosf(yaw - XM_PIDIV2);

	XMVECTOR vec = XMVector3Normalize(XMLoadFloat3(&right));
	XMStoreFloat3(&right, vec);

	return right;
}

// This uses the position and rotation of the camera to build and to update the view matrix.
void CameraClass::Render()
{
	XMVECTOR up, position, lookAt;
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;

	// Setup the vector that points upwards.
	up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	// Setup the position of the camera in the world.
	position = XMLoadFloat3(&m_position);

	// Setup where the camera is looking by default.
	lookAt = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = m_rotation.x * 0.0174532925f;
	yaw   = m_rotation.y * 0.0174532925f;
	roll  = m_rotation.z * 0.0174532925f;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	lookAt = XMVector3TransformCoord(lookAt, rotationMatrix);
	up = XMVector3TransformCoord(up, rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	lookAt = position + lookAt;

	// Finally create the view matrix from the three updated vectors.
	m_viewMatrix = XMMatrixLookAtLH(position, lookAt, up);

	return;
}




void CameraClass::GetViewMatrix(XMMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;
}