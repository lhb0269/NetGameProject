#include "stdafx.h"
#include "UIManager.h"

void UIManager::Recv_UI(UI& rui)
{
	memcpy(&ui, &rui, sizeof(UI));
}
