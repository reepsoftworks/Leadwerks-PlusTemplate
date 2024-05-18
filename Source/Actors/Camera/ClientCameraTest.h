#include "pch.h"
#include "../../SampleApp.h"

class_actor ClientCameraTest : public App::SceneActor
{
public:
	exposed_entity lookat;  //"Look At"

	virtual void Start();

	virtual void UpdateInput();
};