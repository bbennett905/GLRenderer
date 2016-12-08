#pragma once

#include <vector>

/*
 * Used to hold all objects being used, and update/draw them every tick
 */

class CSceneRenderer;
class IDrawable;
class IObject;
class CBaseUIElement;
class CCamera;

class CScene
{
public:
	CScene(CCamera* camera);
	~CScene();

	void AddObjectToScene(IObject* obj);
	void AddUIElementToScene(CBaseUIElement* element);

	void PrepareScene();

	void Update(double delta_time);

private:
	CSceneRenderer* _scene_renderer;
	std::vector<IObject*> _object_list;
};

