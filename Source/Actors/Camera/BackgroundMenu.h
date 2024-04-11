#include "pch.h"
#include "../../App/App.h"

class_actor BackgroundMenu : public App::SceneActor
{
	float alpha;
public:
	exposed_float fadeoutspeed = 0.008f; // "Fadeout Speed"

	virtual void Start();
	virtual void PostRender(Leadwerks::Framebuffer* framebuffer);
};