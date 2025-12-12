////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;

	m_TextureShader = 0;
	m_Bitmap = 0;
	m_BillboardBitmap = 0;
	m_TitleBitmap = 0;

	m_DrivedCar = nullptr;
	m_isDriveKey = false;
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	XMMATRIX baseViewMatrix;

	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if(!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if(!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 2.0f, -10.0f);

	const wchar_t* modelFiles[] = {
			L"./data/Car01.obj", L"./data/Car02.obj", L"./data/Car03.obj", L"./data/Car04.obj", L"./data/Car05.obj",
			L"./data/Car06.obj", L"./data/Car073.obj", L"./data/Car08.obj", L"./data/Car09.obj", L"./data/Car10.obj",
			L"./data/Car10.obj", L"./data/Car10.obj", L"./data/Car10.obj", L"./data/Car10.obj", L"./data/Car10.obj",
			L"./data/Car10.obj", L"./data/Car10.obj", L"./data/Car10.obj", L"./data/Car10.obj", L"./data/Car10.obj",
			L"./data/Parking2.obj"
	};

	const wchar_t* textureFiles[] = {
		L"./data/Car01.dds", L"./data/Car02.dds", L"./data/Car03.dds", L"./data/Car04.dds", L"./data/Car05.dds",
		L"./data/Car06.dds", L"./data/Car07.dds", L"./data/Car08.dds", L"./data/Car09.dds", L"./data/Car10.dds",
		L"./data/Car10.dds", L"./data/Car10.dds", L"./data/Car10.dds", L"./data/Car10.dds", L"./data/Car10.dds",
		L"./data/Car10.dds", L"./data/Car10.dds", L"./data/Car10.dds", L"./data/Car10.dds", L"./data/Car10.dds",
		L"./data/Parking.dds"
	};

	// Create the model object.
	const int modelCount = sizeof(modelFiles) / sizeof(modelFiles[0]);
	static int xPos = -10;
	static int zPos = 10;
	for (int i = 0; i < modelCount; i++) {
		ModelClass* model = new ModelClass;
		model->position.x = xPos;
		model->position.z = zPos;
		if (i == modelCount - 1) {
			model->position.x = 0;
			model->position.y = 7.25f;
			model->position.z = 0;
		}
		if (xPos >= 40) {
			xPos = -15;
			zPos = 35;
		}
		xPos += 5;
		zPos -= 5;
		if (!model)
			return false;
		
		if (i >= 0 && i < modelCount) {
			result = model->Initialize(m_D3D->GetDevice(), modelFiles[i], textureFiles[i]);
		}

		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
			return false;
		}
		m_Model.push_back(model);
	}

	// Create the texture shader object.
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
	{
		return false;
	}

	// Initialize the texture shader object.
	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}


	// Create the bitmap object.
	m_Bitmap = new BitmapClass;
	if (!m_Bitmap)
	{
		return false;
	}

	// Initialize the bitmap object.
	result = m_Bitmap->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"./data/back.dds", 1200, 800);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	m_BillboardBitmap = new BitmapClass;
	if (!m_BillboardBitmap)
	{
		return false;
	}

	result = m_BillboardBitmap->Initialize(m_D3D->GetDevice(), 10, 10, L"./data/Billboard.dds", 10, 10);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	m_TitleBitmap = new BitmapClass;
	if (!m_TitleBitmap)
	{
		return false;
	}

	result = m_TitleBitmap->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"./data/title2.dds", 800, 600);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	// Initialize a base view matrix with the camera for 2D user interface rendering.
	m_Camera->Render();
	m_Camera->GetViewMatrix(baseViewMatrix);

	// Create the text object.
	for (int i = 0; i < 6; i++) {
		TextClass* text = new TextClass();
		// Initialize the text object.
		result = text->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix);
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
			return false;
		}
		m_Text.push_back(text);
	}

	return true;
}


void GraphicsClass::Shutdown()
{
	// Release the model object.
	for (size_t i = 0; i < m_Model.size(); i++)
	{
		if (m_Model[i])
		{
			m_Model[i]->Shutdown();
			delete m_Model[i];
			m_Model[i] = 0;
		}
	}
	m_Model.clear();

	// Release the camera object.
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the D3D object.
	if(m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	// Release the bitmap object.
	if (m_Bitmap)
	{
		m_Bitmap->Shutdown();
		delete m_Bitmap;
		m_Bitmap = 0;
	}

	if (m_BillboardBitmap)
	{
		m_BillboardBitmap->Shutdown();
		delete m_BillboardBitmap;
		m_BillboardBitmap = 0;
	}

	// Release the texture shader object.
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	return;
}

bool GraphicsClass::Frame()
{
	bool result;

	static float rotation = 0.0f;

	// Update the rotation variable each frame.
	rotation += XM_PI * 0.005f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}
	
	// Render the graphics scene.
	result = Render(rotation, rotation, rotation);
	
	if(!result)
	{
		return false;
	}

	return true;
}

bool GraphicsClass::Frame(string sceneName, int fps, int cpu, int numofobjects, int numofpolygons, float horizontal, float vertical)
{
	bool result;
	static float rotation = 0.0f;

	// Update the rotation variable each frame.
	rotation += (float)XM_PI * 0.0005f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	// Set the frames per second.
	result = m_Text[0]->SetFPS(fps, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	// Set the cpu usage.
	result = m_Text[1]->SetCPU(cpu, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	result = m_Text[2]->SetNumOfObjects(numofobjects, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}
	
	result = m_Text[3]->SetNumOfPolygons(numofpolygons, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}
	
	result = m_Text[4]->SetScreenResolution(m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	result = m_Text[5]->SetSceneInfo(sceneName, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	// Render the graphics scene.
	if (sceneName == "Title") {
		result = RenderTitleScene();
	} else if (sceneName == "Game") {
		result = Render(rotation, horizontal, vertical);
	}

	if (!result)
	{
		return false;
	}

	return true;
}


bool GraphicsClass::Render(float rotation, float horizontal, float vertical)
{
	XMMATRIX viewMatrix, projectionMatrix, orthoMatrix;
	vector<XMMATRIX*> worldMatrix;
	bool result;

	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// 카메라
	XMFLOAT3 newPos = m_Camera->GetPosition();
	XMFLOAT3 camForward = m_Camera->GetForwardVector();
	XMFLOAT3 camRight = m_Camera->GetRightVector();
	camForward.y = 0;
	camRight.y = 0;
	XMVECTOR forwardVec = XMVector3Normalize(XMLoadFloat3(&camForward));
	XMVECTOR rightVec = XMVector3Normalize(XMLoadFloat3(&camRight));
	XMVECTOR moveVec = XMVectorAdd(
		XMVectorScale(forwardVec, vertical),
		XMVectorScale(-1 * rightVec, horizontal)
	);

	XMVECTOR currentPosVec = XMLoadFloat3(&newPos);
	XMVECTOR newPosVec = XMVectorAdd(currentPosVec, moveVec);
	XMStoreFloat3(&newPos, newPosVec);
	m_Camera->SetPosition(newPos.x, newPos.y, newPos.z);

	// 마우스
	POINT mousePos;
	GetCursorPos(&mousePos);
	static POINT lastMousePos = mousePos;
	float deltaX = (float)(mousePos.x - lastMousePos.x);
	float deltaY = (float)(mousePos.y - lastMousePos.y);
	lastMousePos = mousePos;

	// 카메라 회전
	m_Camera->UpdateRotation(deltaX, deltaY);
	m_Camera->Render();

	// world matrix 생성
	for (size_t i = 0; i < m_Model.size() + 2; i++) { // 1개는 배경, 1개는 빌보딩
		XMMATRIX* worldmatrix = new XMMATRIX(XMMatrixIdentity());
		worldMatrix.push_back(worldmatrix);
	}

	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);

	// 비트맵 렌더 - Z버퍼 OFF, 알파 블렌딩 ON
	m_D3D->TurnZBufferOff();
	m_D3D->TurnOnAlphaBlending();
	*worldMatrix[m_Model.size()] *= XMMatrixTranslation(0, 0, 100); // 비트맵 사라짐 방지
	result = m_Bitmap->Render(m_D3D->GetDeviceContext(), -100, -100);
	if (!result) return false;

	// 배경 이미지 고정
	XMMATRIX identityMatrix = XMMatrixIdentity();
	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Bitmap->GetIndexCount(),
		*worldMatrix[m_Model.size()], identityMatrix, orthoMatrix, m_Bitmap->GetTexture());
	if (!result) return false;

	// 빌보드 이미지
	XMFLOAT3 billboardPos = { 0.0f, 10.0f, 100.0f };
	XMFLOAT3 cameraPos = m_Camera->GetPosition();
	float dx = cameraPos.x - billboardPos.x;
	float dz = cameraPos.z - billboardPos.z;
	float rotation2 = atan2f(dx, dz);
	*worldMatrix[m_Model.size() + 1] = XMMatrixRotationY(rotation2) * XMMatrixTranslation(billboardPos.x, billboardPos.y, billboardPos.z);
	result = m_BillboardBitmap->Render(m_D3D->GetDeviceContext(), 0, 0);
	if (!result) return false;
	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_BillboardBitmap->GetIndexCount(),
		*worldMatrix[m_Model.size() + 1], viewMatrix, projectionMatrix, m_BillboardBitmap->GetTexture());

	// 3D 모델 렌더링 - Z버퍼 ON, 알파 블렌딩 OFF
	m_D3D->TurnZBufferOn();
	m_D3D->TurnOffAlphaBlending();

	if (this->m_DrivedCar == nullptr) {
		ModelClass* nearest = nullptr;
		if (m_isDriveKey) {
			float dist;
			nearest = GetClosestObjectToCamera(dist);
			if (nearest != nullptr) {
				m_DrivedCar = nearest;
				m_isDriveKey = false;
			}
		}
	} else {
		if (m_isDriveKey) {
			this->m_DrivedCar = nullptr;
			m_isDriveKey = false;
		}
	}

	m_Model[7]->position.z += 0.01;
	m_Model[8]->position.x += 0.01;
	m_Model[9]->position.z += 0.01;
	for (size_t i = 0; i < m_Model.size(); i++) {
		if (i != m_Model.size() - 1) {
			if (m_Model[i] == this->m_DrivedCar) {
				m_Model[i]->position.x = cameraPos.x;
				m_Model[i]->position.y = cameraPos.y - 2;
				m_Model[i]->position.z = cameraPos.z + 5;
				*worldMatrix[i] *= XMMatrixRotationY(45);
			} else {
				*worldMatrix[i] *= XMMatrixRotationY(rotation);
			}
			
		}

		XMFLOAT3 modelPos = m_Model[i]->position;
		*worldMatrix[i] *= XMMatrixTranslation(modelPos.x, modelPos.y, modelPos.z);

		m_Model[i]->Render(m_D3D->GetDeviceContext());

		result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Model[i]->GetIndexCount(),
			*worldMatrix[i], viewMatrix, projectionMatrix, m_Model[i]->GetTexture());
		if (!result) return false;
	}

	// 2D 렌더링 - Z버퍼 OFF, 알파 블렌딩 ON
	m_D3D->TurnZBufferOff();
	m_D3D->TurnOnAlphaBlending();

	// 텍스트 렌더
	result = m_Text[0]->Render(m_D3D->GetDeviceContext(), identityMatrix, orthoMatrix);
	if (!result) return false;

	result = m_Text[1]->Render(m_D3D->GetDeviceContext(), identityMatrix, orthoMatrix);
	if (!result) return false;

	result = m_Text[2]->Render(m_D3D->GetDeviceContext(), identityMatrix, orthoMatrix);
	if (!result) return false;

	result = m_Text[3]->Render(m_D3D->GetDeviceContext(), identityMatrix, orthoMatrix);
	if (!result) return false;

	result = m_Text[4]->Render(m_D3D->GetDeviceContext(), identityMatrix, orthoMatrix);
	if (!result) return false;

	result = m_Text[5]->Render(m_D3D->GetDeviceContext(), identityMatrix, orthoMatrix);
	if (!result) return false;

	m_D3D->TurnOffAlphaBlending();
	m_D3D->TurnZBufferOn();
	

	// 메모리 해제
	for (auto mat : worldMatrix) delete mat;
	worldMatrix.clear();

	m_D3D->EndScene();

	return true;
}


bool GraphicsClass::RenderTitleScene()
{
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	XMMATRIX identityMatrix = XMMatrixIdentity();
	XMMATRIX orthoMatrix, viewMatrix;;
	m_D3D->GetOrthoMatrix(orthoMatrix);

	m_Camera->SetPosition(0.0f, 2.0f, -10.0f);
	m_Camera->Render();
	m_Camera->GetViewMatrix(viewMatrix);

	m_D3D->TurnZBufferOff();
	m_D3D->TurnOnAlphaBlending();

	bool result = m_TitleBitmap->Render(m_D3D->GetDeviceContext(), 0, 0);
	if (!result) return false;
	
	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_TitleBitmap->GetIndexCount(),
		identityMatrix, viewMatrix, orthoMatrix, m_TitleBitmap->GetTexture());
	if (!result) return false;

	m_D3D->TurnOffAlphaBlending();
	m_D3D->TurnZBufferOn();

	m_D3D->EndScene();
	return true;
}

ModelClass* GraphicsClass::GetClosestObjectToCamera(float& outDistance)
{
	if (m_Model.empty()) {
		outDistance = -1.0f; // 오브젝트 없음
		return nullptr;
	}

	XMFLOAT3 camPos = m_Camera->GetPosition();
	ModelClass* closestModel = nullptr;
	float minDistSq = FLT_MAX;

	for (auto model : m_Model) {
		XMFLOAT3 pos = model->position;
		float dx = pos.x - camPos.x;
		float dy = pos.y - camPos.y;
		float dz = pos.z - camPos.z;
		float distSq = dx * dx + dy * dy + dz * dz;
		if (distSq < minDistSq) {
			minDistSq = distSq;
			closestModel = model;
		}
	}

	outDistance = sqrtf(minDistSq);
	return closestModel;
}