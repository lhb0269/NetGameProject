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
		ui[i].PlayerNum = 0;
		ui[i].score = 0;
	}
}

UI* UIManager::HandOverInfo()
{
	return ui;
}

void UIManager::Recv_UI(SOCKET& clientsock)
{
	UI UIInfo;
	int retval = recv(clientsock, (char*)&UIInfo, sizeof(UI), MSG_WAITALL);
	if (retval == SOCKET_ERROR) err_display("recv()");
	ui[UIInfo.PlayerID].score = UIInfo.score;
	ui[UIInfo.PlayerID].Stage = UIInfo.Stage;

}

void UIManager::Send_Msg()
{

}
