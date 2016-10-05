#pragma once


//this will have a SceneRenderer, containing light info and such?
//also this will have a list of all objects in scene, and call BaseDrawable::Draw(SceneRenderer)
//each frame. Or that could be within the scenerenderer, which passes relevant information to 
//BaseDrawable::Draw(lights, etc)
class Scene
{
public:
	Scene();
	~Scene();
};

