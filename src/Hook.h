#pragma once

namespace WaitMenuRedirected
{
	class OpenWaitMenuHook
	{
	public:
		static void Hook()
		{
			REL::Relocation<uintptr_t> hook{ REL::ID(51400) };  //1408AA140
			auto& trampoline = SKSE::GetTrampoline();
			SKSE::AllocTrampoline(1 << 4);
			_OpenWaitMenu = trampoline.write_call<5>(hook.address() + 0x394, OpenWaitMenu);
		};

	private:
		static void OpenWaitMenu(bool isSleep);

		static inline REL::Relocation<decltype(OpenWaitMenu)> _OpenWaitMenu;
	};

	using EventResult = RE::BSEventNotifyControl;

	class MenuControlsHook
	{
	public:
		static void InstallHook()
		{
			REL::Relocation<std::uintptr_t> MenuControlsVtbl{ RE::VTABLE_MenuControls[0] };
			_ProcessEvent = MenuControlsVtbl.write_vfunc(0x1, ProcessEvent);
		}

	private:
		static EventResult ProcessEvent(RE::MenuControls* a_menuControls, RE::InputEvent* const* a_event, RE::BSTEventSource<RE::InputEvent*>* a_eventSource);

		static inline REL::Relocation<decltype(ProcessEvent)> _ProcessEvent;
	};
}