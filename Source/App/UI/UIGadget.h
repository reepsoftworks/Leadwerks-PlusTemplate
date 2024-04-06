#pragma once
#include "../App.h"

namespace App
{
	enum
	{
		UIGADGET_COLOR_BASE = 0,
		UIGADGET_COLOR_BORDER,
		UIGADGET_COLOR_HIGHLIGHT,
		UIGADGET_COLOR_SUNKEN,
	};

	class UIGadget : public Gadget
	{
		bool mousedown;
		bool hasmouse;
		bool IntersectsPoint(const Leadwerks::iVec2& p, const int radius = 0);

		UIGadget* parent;
		std::vector<UIGadget*> children;
		Leadwerks::Vec4 activecolor;
		bool fadeout;
		float fadetime;
		float fademin;
		float fademax;
		void HandleFading();

	protected:
		Leadwerks::iVec2 position;
		Leadwerks::iVec2 size;
		std::array<Leadwerks::Vec4, 4> color;
		
		Leadwerks::Font* font;
		Leadwerks::Texture* texture;
		std::string text;
		int borderthickness;
		

		virtual void SetDropShadowMode(const bool mode);
		virtual bool ProcessEvent(const Leadwerks::Event& e);
		virtual void DrawUI(Leadwerks::Framebuffer* context);
		const bool Highlighted();
	public:
		std::map<std::string, std::string> keyvalues;
		bool dropshadow;
		bool nodraw;
		bool enabled;

		UIGadget();
		~UIGadget();

		virtual void Show();
		virtual void Hide();

		void SetParent(UIGadget* parent);
		virtual void SetPosition(const Leadwerks::iVec2& pos);
		virtual void SetSize(const Leadwerks::iVec2& size);
		virtual void SetImage(const std::string& path);
		virtual void SetImage(Leadwerks::Texture* texture);
		virtual void SetFont(const std::string& path, const int size);
		virtual void SetFont(Leadwerks::Font* font);
		virtual void SetText(const std::string& text);

		virtual Leadwerks::iVec2 GetPosition();
		virtual Leadwerks::iVec2 GetSize();

		virtual void OnMouseClick();
		virtual void OnMouseDown();
		virtual void OnMouseRelease();
		virtual void OnMouseEnter();
		virtual void OnMouseLeave();
		virtual void Highlight();

		void SetColor(const float r, const float b, const float g, const float a);

		void FadeIn(const float target = 1.0f, const float speed = 0.05f);
		void FadeOut(const float target = 0.0f, const float speed = 0.05f);

		virtual void UIGadget::DrawUI();
		virtual void PostRender(Leadwerks::Framebuffer* context);
		virtual void EnableListening();
	};
}