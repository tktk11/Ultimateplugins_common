#pragma once

#include "skse64/GameMenus.h"

namespace UPCommon
{
	class DummyMenuCreator
	{
	public:
		static IMenu* Create();
		static void SetSwfPath(const char* path);

	private:
		DummyMenuCreator();

		static std::string swfPath_;
	};

	class DummyMenu : public IMenu
	{
	public:
		DummyMenu(const char* swfPath);
		void Accept(CallbackProcessor* processor) override;
		void Render() override;
	};
}
