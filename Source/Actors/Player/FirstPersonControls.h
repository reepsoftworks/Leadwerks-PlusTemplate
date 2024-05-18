#include "pch.h"
#include "../../SampleApp.h"

class_actor FirstPersonControls : public App::SceneActor
{
	float currentheight;
	Leadwerks::Vec2 GetMovementAxis();
	bool crouchingstate;
	void HandleMovement();
	void HandleCrouching();

	static void PauseCallback(Leadwerks::Object* source, Leadwerks::Object* extra);
	virtual bool ProcessEvent(const Leadwerks::Event& e);

public:
	exposed_float eyeheight = 1.35f;
	exposed_float croucheyeheight = 0.64f;
	exposed_bool allowspinning = true;
	exposed_bool allowzooming = true;

	virtual void Start();
	virtual void UpdateWorld();
	virtual void UpdatePhysics();
	virtual void UpdateMatrix();
	virtual void UpdateInput();

	virtual void PostRender(Leadwerks::Context* context);

	virtual void Load(nlohmann::json properties = NULL);
	virtual nlohmann::json Save();
};