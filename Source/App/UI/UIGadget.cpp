#include "pch.h"
#include "../App.h"

namespace App
{
	using namespace Leadwerks;

	UIGadget::UIGadget()
	{
		position = iVec2(0, 0);
		size = iVec2(80, 20);
		//color = Vec4(1.0f);

		color[UIGADGET_COLOR_BASE] = Vec4(1.0f);
		color[UIGADGET_COLOR_BORDER] = Vec4(0.0f, 0.0f, 0.0f, 1.0f);
		color[UIGADGET_COLOR_HIGHLIGHT] = Vec4(0.75f, 0.75f, 0.75f, 1.0f);
		color[UIGADGET_COLOR_SUNKEN] = Vec4(0.25f, 0.25f, 0.25f, 1.0f);
		activecolor = color[UIGADGET_COLOR_BASE];
		texture = NULL;
		font = NULL;
		parent = NULL;
		children.clear();
		text.clear();
		keyvalues.clear();
		mousedown = false;
		hasmouse = false;
		dropshadow = false;
		nodraw = false;
		enabled = false;
		
		borderthickness = 0;
		fadetime = 0.05f;
		fademin = 1.0f;
		fademax = 0.0f;
		fadeout = false;
		enabled = false;
		

		Gadget::Gadget();
	}

	UIGadget::~UIGadget()
	{
		if (font)
		{
			font->Release();
			font = NULL;
		}

		if (texture)
		{
			texture->Release();
			texture = NULL;
		}

		text.clear();
		parent = NULL;
		children.clear();
		keyvalues.clear();
		mousedown = false;
		hasmouse = false;
		dropshadow = false;
		nodraw = false;
		fadeout = false;
	}

	bool UIGadget::IntersectsPoint(const iVec2& p, const int radius)
	{
		if (p.x < GetPosition().x - radius) { return false; }
		if (p.y < GetPosition().y - radius) { return false; }
		if (p.x > GetPosition().x + GetSize().x + radius) { return false; }
		if (p.y > GetPosition().y + GetSize().y + radius) { return false; }
		return true;
	}

	bool UIGadget::ProcessEvent(const Leadwerks::Event& e)
	{
		if (GetHidden()) return false;
		if (!enabled) return false;
		if (e.id == Event::MouseMove)
		{
			if (!hasmouse)
			{
				if (IntersectsPoint(e.position))
				{
					hasmouse = true;
					OnMouseEnter();
					EmitEvent(Event::MouseEnter, this);
				}
			}
			else 
			{
				if (hasmouse && !IntersectsPoint(e.position))
				{
					hasmouse = false;
					OnMouseLeave();
					EmitEvent(Event::MouseLeave, this);
				}
			}
		}

		if (hasmouse)
		{
			if (e.id == Event::MouseDown && e.data == Mouse::Left)
			{
				if (IntersectsPoint(e.position))
				{
					OnMouseDown();
				}

				if (!mousedown)
				{
					OnMouseClick();
					//EmitEvent(Event::WidgetAction, this, 1);
					mousedown = true;
				}
			}
			else if (e.id == Event::MouseUp && e.data == Mouse::Left && mousedown)
			{
				OnMouseRelease();
				EmitEvent(Event::WidgetAction, this, 0);
				mousedown = false;
			}
		}

		return true;
	}

	void UIGadget::EnableListening()
	{
		Listen(Event::MouseMove, NULL);
		Listen(Event::MouseDown, NULL);
		Listen(Event::MouseUp, NULL);
		enabled = true;
	}

	void UIGadget::SetParent(UIGadget* parent)
	{
		if (this->parent != NULL)
		{
			if (std::find(this->parent->children.begin(), this->parent->children.end(), this) != this->parent->children.end())
			{
				this->parent->children.erase(std::remove(this->parent->children.begin(), this->parent->children.end(), this), this->parent->children.end());
				this->parent = NULL;
			}
		}

		if (parent != NULL)
		{
			this->parent = parent;
			this->parent->children.push_back(this);
		}
	}

	int Fpn(const int in)
	{
		int ret = in;
		if (ret < 0) ret *= -1;
		return ret;
	}

	void UIGadget::SetPosition(const Leadwerks::iVec2& pos)
	{
		iVec2 parent_pos = iVec2(0, 0);
		if (parent)
		{
			parent_pos = parent->GetPosition();
		}

		auto oldpos = position;

		position.x = parent_pos.x + pos.x;
		position.y = parent_pos.x + pos.y;

		auto scale = static_cast<int>(OS::GetDisplayScale());
		this->position.x *= scale;
		this->position.y *= scale;

		for (const auto& child : children)
		{
			auto child_old_positon = child->position;
			iVec2 diff = iVec2(child_old_positon.x - oldpos.x, child_old_positon.y - oldpos.y);
			child->position.x = position.x + diff.x;
			child->position.y = position.y + diff.y;
		}
	}

	void UIGadget::Show()
	{
		shown = true;
		for (const auto& child : children)
		{
			child->Show();
		}
	}

	void UIGadget::Hide()
	{
		shown = false;
		for (const auto& child : children)
		{
			child->Hide();
		}
	}

	iVec2 UIGadget::GetPosition()
	{
		return position;
	}

	void UIGadget::SetSize(const Leadwerks::iVec2& size)
	{
		this->size = size;

		auto scale = static_cast<int>(OS::GetDisplayScale());
		this->size.x *= scale;
		this->size.y *= scale;
	}

	iVec2 UIGadget::GetSize()
	{
		return size;
	}

	void UIGadget::Highlight()
	{
		if (!hasmouse)
		{
			hasmouse = true;
			OnMouseEnter();
			EmitEvent(Event::MouseEnter, this);
		}
	}

	const bool UIGadget::Highlighted()
	{
		return hasmouse;
	}

	void UIGadget::OnMouseClick()
	{
		activecolor = color[UIGADGET_COLOR_SUNKEN];
		//DPrint("UIGadget::OnMouseClick()");
	}

	void UIGadget::OnMouseDown()
	{
		activecolor = color[UIGADGET_COLOR_SUNKEN];
		//DPrint("ButtonGadget::OnMouseDown()");
	}

	void UIGadget::OnMouseRelease()
	{
		activecolor = color[UIGADGET_COLOR_BASE];
		//DPrint("ButtonGadget::OnMouseRelease()");
	}

	void UIGadget::OnMouseEnter()
	{
		activecolor = color[UIGADGET_COLOR_HIGHLIGHT];
		//DPrint("ButtonGadget::OnMouseEnter()");
	}

	void UIGadget::OnMouseLeave()
	{
		activecolor = color[UIGADGET_COLOR_BASE];
		//DPrint("ButtonGadget::OnMouseLeave()");
	}

	void UIGadget::DrawUI() {}

	void UIGadget::SetImage(const std::string& path)
	{
		if (texture)
		{
			texture->Release();
			texture = NULL;
		}

		texture = AssetLoader::LoadTexture(path);
	}

	void UIGadget::SetImage(Leadwerks::Texture* texture)
	{
		if (this->texture)
		{
			this->texture->Release();
			this->texture = NULL;
		}

		this->texture = texture;
	}

	void UIGadget::SetFont(const std::string& path, const int size)
	{
		if (this->font)
		{
			this->font->Release();
			this->font = NULL;
		}

		font = AssetLoader::LoadFont(path, size);
	}

	void UIGadget::SetFont(Leadwerks::Font* font)
	{
		if (this->font)
		{
			this->font->Release();
			this->font = NULL;
		}

		this->font = font;
	}

	void UIGadget::SetText(const std::string& text)
	{
		this->text = Translator::TranslateToken(text);
	}

	void UIGadget::SetDropShadowMode(const bool mode)
	{
		dropshadow = mode;
	}

	void UIGadget::SetColor(const float r, const float b, const float g, const float a)
	{
		color[UIGADGET_COLOR_BASE] = Vec4(r, g, b, a);
		activecolor = color[UIGADGET_COLOR_BASE];
	}

	void UIGadget::FadeIn(const float target, const float speed)
	{
		if (GetHidden())
		{
			activecolor.a = 0.0f;
			Show();
		}

		if (fadeout)
		{
			//activecolor.a = 1.0f;
		}

		fademax = target;
		fadetime = speed;
		fadeout = false;

		for (const auto& child : children)
		{
			child->FadeIn(target, speed);
		}
	}

	void UIGadget::FadeOut(const float target, const float speed)
	{
		fademin = target;
		fadetime = speed;
		fadeout = true;

		for (const auto& child : children)
		{
			child->FadeOut(target, speed);
		}
	}

	void UIGadget::HandleFading()
	{
		float speed = Time::GetSpeed();
		if (PauseState()) speed = 0.125f;

		if (fadeout && activecolor.a != fademin)
		{
			activecolor.a = Math::Inc(fademin, activecolor.a, speed * fadetime);
			if (activecolor.a == 0.0f) Hide();
		}
		else if (!fadeout && activecolor.a < fademax)
		{
			activecolor.a = Math::Inc(fademax, activecolor.a, speed * fadetime);
		}	
	}

	void UIGadget::DrawUI(Leadwerks::Framebuffer* context)
	{
		// Default drawing code.
		if (activecolor.a != 0.0f)
		{
			iVec2 parent_pos = iVec2(0, 0);
			if (parent)
			{
				parent_pos = parent->GetPosition();
			}

			context->SetBlendMode(Blend::Alpha);
			context->SetColor(activecolor);

			// nodraw breaks fading on children.
			if (nodraw == false)
			{
				if (!text.empty())
				{
					if (font)
					{
						if (font != context->GetFont()) context->SetFont(font);
					}
					else
					{
						font = context->GetFont();
					}

					// Modify size to fit the text
					iVec2 fontsize;
					fontsize.x = font->GetTextWidth(text);
					fontsize.y = font->GetHeight() + 16;
					SetSize(fontsize);

					if (dropshadow)
					{
						context->SetColor(0.0f, 0.0f, 0.0f, activecolor.a);
						context->DrawText(text, position.x + 2, position.y + 2, size.x, size.y);
						context->SetColor(activecolor);
					}

					context->DrawText(text, position.x, position.y, size.x, size.y);
				}
				else
				{
					if (dropshadow)
					{
						context->SetColor(0.0f, 0.0f, 0.0f, activecolor.a);
						context->DrawRect(position.x + 2, position.y + 2, size.x, size.y);
						context->SetColor(activecolor);
					}

					if (!texture)
					{
						context->DrawRect(position.x, position.y, size.x, size.y);
					}
					else
					{
						context->DrawImage(texture, position.x, position.y, size.x, size.y);
					}
				}
			}

			if (borderthickness > 0)
			{
				for (int i = 0; i < borderthickness; i++)
				{
					context->SetColor(color[UIGADGET_COLOR_BORDER].r, color[UIGADGET_COLOR_BORDER].g, color[UIGADGET_COLOR_BORDER].b, activecolor.a);
					context->DrawRect(position.x + i, position.y + i, size.x, size.y, 1);
				}
			}
		}
	}

	void UIGadget::PostRender(Leadwerks::Framebuffer* context)
	{
		HandleFading();
		DrawUI(context);
	}
}