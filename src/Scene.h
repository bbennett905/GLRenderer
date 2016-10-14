#pragma once

#include <vector>
//this will have a SceneRenderer, containing light info and such?
//also this will have a list of all objects in scene, and call BaseDrawable::Draw(SceneRenderer)
//each frame. Or that could be within the scenerenderer, which passes relevant information to 
//BaseDrawable::Draw(lights, etc)

class SceneRenderer;
class BaseDrawable;
class BaseObject;
class Camera;
class Window;

class Scene
{
public:
	Scene(Window * window, Camera * camera);
	~Scene();

	void AddObjectToScene(BaseObject * obj);

	void PrepareScene();

	void Update(double delta_time);

private:
	SceneRenderer * _scene_renderer;
	std::vector<BaseObject *> _object_list;
};

