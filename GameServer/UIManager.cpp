#include "UIManager.h"

UIManager::UIManager()
{
}

UIManager::~UIManager()
{
}

void UIManager::init()
{
	for (int i = 0; i < MAX_PLAYER; ++i) {
		ui[i].PlayerNum = NULL;
		ui[i].PlayerID = -1;
		ui[i].score = NULL;
		ui[i].Stage = NULL;
	}
}

UI* UIManager::HandOverInfo()
{
	return ui;
}

void UIManager::Recv_UI(SOCKET& clientsock)
{
	UI rui;
	int retval = recv(clientsock, (char*)&rui, sizeof(UI), MSG_WAITALL);
	if (retval == SOCKET_ERROR) err_display("recv()");
	for (int i = 0; i < MAX_PLAYER; ++i) {
		if (rui.PlayerID < 4 && rui.PlayerID>-1 && rui.Stage < 100) {
			ui[rui.PlayerID].PlayerID = rui.PlayerID;
			ui[rui.PlayerID].score = rui.score;
			ui[rui.PlayerID].Stage = rui.Stage;
		}
	}
}

void UIManager::Recv_UI(UI& rui)
{
	for (int i = 0; i < MAX_PLAYER; ++i) {
		if (rui.PlayerID < 4 && rui.PlayerID>-1 && rui.Stage < 100) {
			ui[rui.PlayerID].PlayerID = rui.PlayerID;
			ui[rui.PlayerID].score = rui.score;
			ui[rui.PlayerID].Stage = rui.Stage;
		}
	}
}

void UIManager::Send_Msg()
{

}
