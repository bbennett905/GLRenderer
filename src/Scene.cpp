#include "Scene.h"

#include "BaseDrawable.h"
#include "BaseObject.h"
#include "SceneRenderer.h"
#include "Model.h"
#include "Lights.h"

Scene::Scene(Window * window, Camera * camera)
{
	_scene_renderer = new SceneRenderer(window, camera);
}

Scene::~Scene()
{
}

void Scene::AddObjectToScene(BaseObject * obj)
{
	_object_list.push_back(obj);

	BaseDrawable * drawable = dynamic_cast<BaseDrawable *>(obj);
	if (drawable)
		_scene_renderer->AddDrawable(drawable);
	

	Model * model = dynamic_cast<Model *>(obj);
	if (model)
	{
		for (auto mesh : model->Meshes)
			_scene_renderer->AddDrawable(mesh);
	}

	LightPoint * point_light = dynamic_cast<LightPoint *>(obj);
	if (point_light)
		_scene_renderer->AddPointLight(point_light);

	LightSpot * spot_light = dynamic_cast<LightSpot *>(obj);
	if (spot_light)
		_scene_renderer->AddSpotLight(spot_light);

	LightDirectional * directional_light = dynamic_cast<LightDirectional *>(obj);
	if (directional_light)
		_scene_renderer->SetDirectionalLight(directional_light);
}

void Scene::PrepareScene()
{
	_scene_renderer->BuildShaders();
}

void Scene::Update(double delta_time)
{
	//TODO implement BaseObject::Update
	for (auto obj : _object_list)
	{
		//obj->Update(delta_time);
	}

	_scene_renderer->Draw();
}
