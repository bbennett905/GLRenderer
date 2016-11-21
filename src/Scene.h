#pragma once

#include <vector>

/*
 * Used to hold all objects being used, and update/draw them every tick
 */

class SceneRenderer;
class IDrawable;
class IObject;
class CBaseUIElement;
class Camera;
class Window;

class Scene
{
public:
	Scene(Window * window, Camera * camera);
	~Scene();

	void AddObjectToScene(IObject * obj);
	void AddUIElementToScene(CBaseUIElement * element);

	void PrepareScene();

	void Update(double delta_time);

private:
	SceneRenderer * _scene_renderer;
	std::vector<IObject *> _object_list;
};

