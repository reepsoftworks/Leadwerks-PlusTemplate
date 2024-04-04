#pragma once
#include "../App.h"

namespace App
{
	class TextureGadget : public Gadget
	{
		std::string imagepath;
		Leadwerks::iVec2 position;
		Leadwerks::iVec2 size;
		Leadwerks::Texture* texture;
	public:
		TextureGadget();

		virtual void SetImage(const std::string& path);
		virtual void SetImage(Leadwerks::Texture* texture);
		virtual void SetSize(const Leadwerks::iVec2& size);

		virtual void DrawUI();
		virtual void PostRender(Leadwerks::Framebuffer* context);
	};
}