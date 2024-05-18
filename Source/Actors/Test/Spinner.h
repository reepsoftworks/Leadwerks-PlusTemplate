#include "pch.h"
#include "../../App/App.h"

class_actor Spinner : public App::SceneActor
{
public:
	// Start Enabled
	exposed_bool enabled = true; //"Start Enabled"

	// Spin Speed 
	exposed_vec3 spinspeed;	//"Spin Speed"

	virtual void Start();
	virtual void UpdateWorld();
	virtual void ReceiveSignal(const std::string& inputname, Leadwerks::Entity* sender);

	virtual void Load(nlohmann::json properties = NULL);
	virtual nlohmann::json Save();

	exposed_input Enable(Leadwerks::Entity* sender);
	exposed_input Disable(Leadwerks::Entity* sender);
	exposed_output OnTurn();
};