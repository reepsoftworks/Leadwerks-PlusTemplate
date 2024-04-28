#include "pch.h"
#include "InputDevice.h"

namespace App
{
	namespace InputSystem
	{
		class GamePadDevice : public InputDevice
		{
			Leadwerks::Timer* rumbletimer;
			bool connected;

			bool IsConnected();

			virtual void Initialize();
			virtual void Update();

		protected:
			virtual bool Hit(const std::string& actionname);
			virtual bool Down(const std::string& actionname);
			virtual Leadwerks::Vec2 Axis(const std::string& actionname);
			virtual void Rumble(const float left, const float right, int time);

			virtual bool ProcessEvent(const Leadwerks::Event& e);
		public:
			GamePadDevice();

			static GamePadDevice* Create(const int controllerid = 0);
		};
	}
}