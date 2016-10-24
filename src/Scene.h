#pragma once

#include <vector>

/*
 * Used to hold all objects being used, and update/draw them every tick
 */

class SceneRenderer;
class BaseDrawable;
class BaseObject;
class BaseUIElement;
class Camera;
class Window;

class Scene
{
public:
	Scene(Window * window, Camera * camera);
	~Scene();

	void AddObjectToScene(BaseObject * obj);
	void AddUIElementToScene(BaseUIElement * element);

	void PrepareScene();

	void Update(double delta_time);

private:
	SceneRenderer * _scene_renderer;
	std::vector<BaseObject *> _object_list;
};

