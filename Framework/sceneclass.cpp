#include "sceneclass.h"

SceneClass::SceneClass() {
	m_sceneNumber = 0;
	m_sceneName = "Untitled";
}

SceneClass::SceneClass(const SceneClass& other) {
}

SceneClass::~SceneClass() {
}

void SceneClass::SetSceneName(const string sceneName) {
	this->m_sceneName = sceneName;
}

void SceneClass::SetSceneNumber(int sceneNumber) {
	this->m_sceneNumber = sceneNumber;
}