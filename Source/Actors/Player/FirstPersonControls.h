#include "pch.h"
#include "../../SampleApp.h"

class_actor FirstPersonControls : public App::SceneActor
{
	bool freelookstarted{ false };
	Leadwerks::Vec3 freelookmousepos;
	Leadwerks::Vec3 freelookrotation;
	Leadwerks::Vec2 lookchange;
	Leadwerks::Vec3 currentcameraposition;
	bool lockmouse;
	float topangle;
	float bottomangle;

	void HandleMouseLook();
	Leadwerks::Vec2 GetMovementAxis();
	void HandleMovement();

	static void PauseCallback(Leadwerks::Object* source, Leadwerks::Object* extra);
	virtual bool ProcessEvent(const Leadwerks::Event& e);

public:
	exposed_float eyeheight = 1.7f;
	//exposed_float croucheyeheight = 1.0f;
	exposed_float movespeed = 4.0f;
	exposed_float jumpforce = 8.0f;
	exposed_float jumplunge = 1.5f;
	exposed_bool allowspinning = true;
	exposed_bool allowzooming = true;

	virtual void Start();
	virtual void UpdateWorld();
	virtual void UpdatePhysics();
	virtual void UpdateMatrix();

	virtual void Load(nlohmann::json properties = NULL);
	virtual void Save(nlohmann::json properties = NULL);
};