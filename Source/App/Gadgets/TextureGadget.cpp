#include "pch.h"
#include "../App.h"

namespace App
{
	using namespace Leadwerks;

	TextureGadget::TextureGadget()
	{
		imagepath.clear();
		position = iVec2(0, 0);
		size = iVec2(0, 0);
		texture = NULL;

		Gadget::Gadget();
	}

	void TextureGadget::SetImage(const std::string& path)
	{	
		if (imagepath.empty() || imagepath != path)
		{
			if (texture)
			{
				texture->Release();
				texture = NULL;
			}

			auto ext = FileSystem::ExtractExt(imagepath);
			texture = AssetLoader::LoadTexture(path);

			imagepath = path;
		}
	}

	void TextureGadget::SetImage(Leadwerks::Texture* texture)
	{
		if (this->texture)
		{
			this->texture->Release();
			this->texture = NULL;
		}

		this->texture = texture;
		imagepath = this->texture->path;
	}

	void TextureGadget::SetSize(const Leadwerks::iVec2& size)
	{
		this->size = size;
	}

	void TextureGadget::DrawUI()
	{
#if 0
		if (imagepath.empty()) return;

		static GLuint my_image_texture = 0;
		static int my_image_width = 0;
		static int my_image_height = 0;

		if (my_image_texture == NULL)
		{
			bool ret = ImageTexture::LoadTextureFromFile(imagepath.c_str(), &my_image_texture, &my_image_width, &my_image_height);
			IM_ASSERT(ret);
		}

		// If no size is provided, set the size to the image.
		auto drawsize = size;
		if (drawsize.x == 0) size.x = my_image_width;
		if (drawsize.y == 0) size.y = my_image_height;

		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!

		ImGui::SetNextWindowPos(ImVec2(position.x, position.y), ImGuiCond_None);
		ImGui::SetNextWindowSize(ImVec2(drawsize.x, drawsize.y), ImGuiCond_None);
		ImGui::Begin("Test", 0, ImGuiWindowFlags_NoDecoration
			| ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoBackground);

		//ImGui::Begin("OpenGL Texture Text", open);
		//ImGui::Text("pointer = %x", my_image_texture);
		//ImGui::Text("size = %d x %d", my_image_width, my_image_height);
		ImGui::Image((void*)(intptr_t)my_image_texture, ImVec2(drawsize.x, drawsize.y));
		ImGui::End();
#endif
	}

	void TextureGadget::PostRender(Leadwerks::Framebuffer* context)
	{
		context->SetColor(1, 1, 1, 1);
		if (texture)
		{
			// If no size is provided, set the size to the image.
			auto drawsize = size;
			if (drawsize.x == 0) size.x = texture->GetWidth();
			if (drawsize.y == 0) size.y = texture->GetHeight();

			context->DrawImage(texture, position.x, position.y, drawsize.x, drawsize.y);
		}
	}
}