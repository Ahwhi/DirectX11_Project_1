#ifndef _SCENECLASS_H_
#define _SCENECLASS_H_

//#include <directxmath.h>
#include <string> 
using namespace std;
//using namespace DirectX;

class SceneClass {
public:
	SceneClass();
	SceneClass(const SceneClass&);
	~SceneClass();

	void SetSceneName(const string);
	void SetSceneNumber(int);

	string m_sceneName;

private:
	int m_sceneNumber;
};

#endif