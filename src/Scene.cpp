#include "Scene.h"

#include "IDrawable.h"
#include "IObject.h"
#include "SceneRenderer.h"
#include "CModel.h"
#include "Lights.h"
#include "CMesh.h"
#include "Logging.h"
#include "CBaseUIElement.h"

Scene::Scene(Window * window, CCamera * camera)
{
	_scene_renderer = new SceneRenderer(window, camera);
	Logging::LogMessage(LogLevel_Debug, "Created Scene object");
}

Scene::~Scene()
{
	for (auto object : _object_list)
		delete object;

	delete _scene_renderer;
	Logging::LogMessage(LogLevel_Debug, "Destroyed Scene object");
}

void Scene::AddObjectToScene(IObject * obj)
{
	_object_list.push_back(obj);

	IDrawable * drawable = dynamic_cast<IDrawable *>(obj);
	if (drawable)
		_scene_renderer->AddDrawable(drawable);

	CModel * model = dynamic_cast<CModel *>(obj);
	if (model)
	{
		for (auto mesh : model->Meshes)
			_scene_renderer->AddDrawable(mesh);
	}

	CLightPoint * point_light = dynamic_cast<CLightPoint *>(obj);
	if (point_light)
		_scene_renderer->AddPointLight(point_light);

	CLightSpot * spot_light = dynamic_cast<CLightSpot *>(obj);
	if (spot_light)
		_scene_renderer->AddSpotLight(spot_light);

	CLightDirectional * directional_light = dynamic_cast<CLightDirectional *>(obj);
	if (directional_light)
		_scene_renderer->SetDirectionalLight(directional_light);
}

void Scene::AddUIElementToScene(CBaseUIElement * element)
{
	_scene_renderer->AddDrawable(element);
}

void Scene::PrepareScene()
{
	Logging::LogMessage(LogLevel_Debug, "Preparing scene");
	_scene_renderer->BuildShaders();
}

void Scene::Update(double delta_time)
{
	//TODO implement BaseObject::Update
	/*for (auto obj : _object_list)
	{
		//obj->Update(delta_time);
	}*/

	_scene_renderer->Draw();

}
