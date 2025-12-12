////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "textureshaderclass.h"
#include "bitmapclass.h"
#include "textclass.h"
#include "vector"

/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 10000.0f;
const float SCREEN_NEAR = 0.001f;


////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////
class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();
	bool Frame(string, int, int, int, int, float, float);
	ModelClass* GetClosestObjectToCamera(float& outDistance);

	vector<ModelClass*> m_Model;
	ModelClass* m_DrivedCar;
	bool m_isDriveKey;

private:
	bool Render(float, float, float);
	bool RenderTitleScene();

private:
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	
	TextureShaderClass* m_TextureShader;
	BitmapClass* m_Bitmap;
	BitmapClass* m_BillboardBitmap;
	BitmapClass* m_TitleBitmap;

	vector<TextClass*> m_Text;
};

#endif