#include "Hook.h"
namespace WaitMenuRedirected
{
	void OpenWaitMenuHook::OpenWaitMenu(bool isSleep)
	{
		return;
	}

	EventResult MenuControlsHook::ProcessEvent(RE::MenuControls* a_menuControls, RE::InputEvent* const* a_event, RE::BSTEventSource<RE::InputEvent*>* a_eventSource)
	{
		using EventType = RE::INPUT_EVENT_TYPE;
		using DeviceType = RE::INPUT_DEVICE;
		using Flag = RE::IMenu::Flag;
		using Context = RE::IMenu::Context;
		using INPUT_ID = RE::UserEvents::INPUT_CONTEXT_ID;

		const auto ui = RE::UI::GetSingleton();
		auto menuStringsHolder = RE::InterfaceStrings::GetSingleton();
		auto userEventHolder = RE::UserEvents::GetSingleton();
		auto tweenMenu = ui->GetMenu<RE::TweenMenu>(menuStringsHolder->tweenMenu);
		const auto controlMaps = RE::ControlMap::GetSingleton();

		auto IsTopMostInteractionMenu = [](const RE::UI* ui, const RE::GPtr<RE::IMenu>& iMenu) -> bool {
			for (auto iter = ui->menuStack.end() - 1; iter >= ui->menuStack.begin(); iter--) {
				auto menu = iter->get();
				if (menu && (menu->inputContext.get() < Context::kTotal || menu->PausesGame() || menu->UsesCursor())) {
					return menu == iMenu.get();
				}
			}

			return false;
		};

		if (!a_menuControls->remapMode && !a_menuControls->isProcessing && tweenMenu && tweenMenu->OnStack() && IsTopMostInteractionMenu(ui, tweenMenu) && !ui->IsMenuOpen(menuStringsHolder->inventoryMenu)) {
			for (auto event = *a_event; event; event = event->next) {
				if (event->eventType != EventType::kButton)
					continue;

				const auto button = event->AsButtonEvent();
				if (!button || !button->IsDown())
					continue;

				if (_strcmpi(button->QUserEvent().c_str(), userEventHolder->wait.c_str()) == 0) {
					RE::TweenMenu::CloseTweenMenu();
					RE::SleepWaitMenu::OpenSleepWaitMenu(false);
					return EventResult::kStop;
				}
			}
		}

		return _ProcessEvent(a_menuControls, a_event, a_eventSource);
	}
}