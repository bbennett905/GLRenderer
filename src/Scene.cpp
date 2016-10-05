#include "Scene.h"

#include "BaseDrawable.h"
#include "BaseObject.h"

Scene::Scene()
{
}


Scene::~Scene()
{
}

void Scene::AddObjectToScene(BaseObject * obj)
{
	//TODO test this! its very likely that this doesnt quite work this way
	_object_list.push_back(obj);

	BaseDrawable * drawable = dynamic_cast<BaseDrawable *>(obj);
	if (drawable)
	{
		_draw_list.push_back(drawable);
	}
}

void Scene::Update(double delta_time)
{
	//TODO implement BaseObject::Update
	for (auto obj : _object_list)
	{
		//obj->Update(delta_time);
	}
	for (auto draw : _draw_list)
	{
		draw->Draw(_scene_renderer);
	}
}
