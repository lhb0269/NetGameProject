#include "stdafx.h"
#include "Clinet.h"

void CLIENT::err_quit(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER
		| FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(char*)&lpMsgBuf, 0, NULL);
	MessageBoxA(NULL, (const char*)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}
void CLIENT::err_display(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER
		| FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(char*)&lpMsgBuf, 0, NULL);
	printf("[%s] %s\n", msg, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

DWORD WINAPI RecvThread(LPVOID arg)
{
	int retval = 0;
	CLIENT* pClient = (CLIENT*)arg;
	SOCKET sock = pClient->GetSock();
	char buf[BUFSIZE];

	pClient->Login();
	while (true)
	{
		pClient->Recv_Packet(sock);
		pClient->UpdateOtherPlayers();
		pClient->UpdateEnemy();
		pClient->UpdateOtherPlayerBullets(&pClient->GetMapSize());
		pClient->DistroyEnemy();
		//Client.UpdateOtherPlayers(); //다른 플레이어들의 정보를 갱신
		//Client.UpdateOtherPlayerBullets(&whole); //다른 플레이어들이 쏜 총알 갱신
		//Client.UpdateEnemy();
	}
}

void CLIENT::Login()
{
	int retval = 0;
	int id = 0;
	retval = recv(sock, (char*)&id, sizeof(int), MSG_WAITALL);
	if (retval == SOCKET_ERROR) err_display("send()");

	player->SetId(id);
}

void CLIENT::Send_Packet(void* pakcet, int size, PACKET_TYPE type)
{
	int retval;
	retval = send(sock, (char*)type, sizeof(type), 0);
	if (retval == SOCKET_ERROR) err_display("send()");

	retval = send(sock, (char*)pakcet, size, 0);
	if (retval == SOCKET_ERROR) err_display("send()");
}

void CLIENT::Send_Packet(PACKET_TYPE type)
{
	int retval = 0;
	switch (type)
	{
	case CLIENTINFO:
	{
		UpdateClientPacketData();
		retval = send(sock, (char*)&type, sizeof(PACKET_TYPE), 0);
		retval = send(sock, (char*)&Clientinfo, sizeof(ClientInfo), 0);
		break;
	}
	case LOBBYPACKET:
		break;
	case COLLIDEENEMY:
		break;
	case ALLPACKET:
		break;
	}
	if (retval == SOCKET_ERROR) err_display("send()");
	Clientinfo.ce.Enemyid = -1;
}

void CLIENT::UpdateClientPacketData()
{
	UpdateClientUiInfo();
	UpdatePlayerInfo();
}

void CLIENT::UpdatePlayerInfo()
{
	Clientinfo.Pinfo.id = player->GetId();
	Clientinfo.Pinfo.pos = player->getPos();
	Clientinfo.Pinfo.sword = player->getSword();
	Clientinfo.Pinfo.numOfShell = player->GetNumOfShell();
	Clientinfo.Pinfo.orbitRay = player->GetorbitRay();
	Clientinfo.Pinfo.shellStack = player->GetshellStack();
	Clientinfo.Pinfo.isTouched = player->GetisTouched();
	Clientinfo.Pinfo.isdamaged = player->Getisdamaged();
	Clientinfo.Pinfo.bangMotion = player->GetbangMotion();
	Clientinfo.Pinfo.Bangpos = player->GetBangpos();
	Clientinfo.Pinfo.velocity = player->GetVelocity();
}

void CLIENT::UpdateClientUiInfo()
{
	Clientinfo.Ui = UIMng.HandOverInfo();
}

void CLIENT::UpdateUIInfo(int level)
{
	UIMng.UpdateLevel(player, level);
	UIMng.Print();
}
void CLIENT::UpdateScore(int score)
{
	UIMng.UpdateScore(player, score);
}
void CLIENT::UpdateOtherPlayers()
{
	int count = 0;
	for (int i = 0; i < MAX_PLAYER; ++i)
	{
		int pid = All_packet.P_info[i].id;
		//아직 접속하지 않는 플레이어의 아이디 이거나 자기 자신의 id값이 아닐 경우
		if (pid == player->GetId() || pid == -1) continue;

		Otherplayers[count].SetId(All_packet.P_info[i].id);
		Otherplayers[count].setPos(All_packet.P_info[i].pos);
		Otherplayers[count].SetSword(All_packet.P_info[i].sword);
		Otherplayers[count].setSwordShape();
		Otherplayers[count].SetNumOfShell(All_packet.P_info[i].numOfShell);
		Otherplayers[count].SetisTouched(All_packet.P_info[i].isTouched);
		Otherplayers[count].SetorbitRay(All_packet.P_info[i].orbitRay);
		Otherplayers[count].SetshellStack(All_packet.P_info[i].shellStack);
		Otherplayers[count].Setisdamaged(All_packet.P_info[i].isdamaged);
		Otherplayers[count].SetbangMotion(All_packet.P_info[i].bangMotion);
		Otherplayers[count].SetBangpos(All_packet.P_info[i].Bangpos);
		Otherplayers[count].SetVelocity(All_packet.P_info[i].velocity);

		if (Otherplayers[count].GetbangMotion() == 10) //총을 발사 했을때

		{
			POINT bulletpos = Otherplayers[count].GetBangpos();
			POINTFLOAT bulletVelocity = Otherplayers[count].GetVelocity();
			OtherPlayerBullets->add(bulletpos, bulletVelocity);
		}
		count++;
	}
}

void CLIENT::UpdateOtherPlayerBullets(RECT* map)
{
	OtherPlayerBullets->move(map);
}
void CLIENT::setCollideEnemy(int in)
{
	Clientinfo.ce.Enemyid = in;
}
void CLIENT::setReady()
{
	Clientinfo.Pinfo.ready = true;
}
bool CLIENT::getReady()
{
	return Clientinfo.Pinfo.ready;
}
void CLIENT::printUI(POINT& point, HDC hdc)
{
	TCHAR playerID[MAX_PLAYER][25];
	TCHAR score[MAX_PLAYER][15];
	WCHAR playerColor[MAX_PLAYER][20] = { { L"Black" },{ L"Red" },{ L"Green" },{ L"Blue" } };
	for (int i = 0; i < MAX_PLAYER; ++i) {
		if (All_packet.Ui[i].PlayerID == -1)
			wsprintf(playerID[i], L"Disconnected");
		else
			wsprintf(playerID[i], L"Player %d", All_packet.Ui[i].PlayerID+1);
		TextOut(hdc, point.x + 970, point.y + i * 10, playerID[i], _tcslen(playerID[i]));
		TextOut(hdc, point.x + 1100, point.y + i * 10, playerColor[i], _tcslen(playerID[i]));
		wsprintf(score[i], L"score : %d", All_packet.Ui[i].score);
		TextOut(hdc, point.x + 1150, point.y + i * 10, score[i], _tcslen(score[i]));
	}
}
void CLIENT::UpdateEnemy()
{
	enemyMng->EnemyInfoUpdate(All_packet.enemyList);
}
void CLIENT::DistroyEnemy()
{
	if (All_packet.ce.Enemyid != -1)
		enemyMng->destroy(All_packet.ce.Enemyid);
}
void CLIENT::Recv_Packet(SOCKET& sock)
{
	int retval;

	retval = recv(sock, (char*)&All_packet, sizeof(ALL_PACKET), MSG_WAITALL);
	if (retval == SOCKET_ERROR) err_display("send()");
}

int CLIENT::Init(Player* p, EnemyManager* e, Player* o, UIManager* u, PlayerBulletManager* b)
{
	player = p;
	enemyMng = e;
	Otherplayers = o;
	UIMng.init(player);
	OtherPlayerBullets = b;
	int retval;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// ���� ����
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");


	DWORD optval = 1;
	setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (const char*)&optval, sizeof(optval));

	// connect()
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	inet_pton(AF_INET, SERVERIP, &serveraddr.sin_addr);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("connect()");

	hThread = CreateThread(NULL, 0, RecvThread,
		(LPVOID)this, 0, NULL);
	if (hThread != NULL) (CloseHandle(hThread));
}