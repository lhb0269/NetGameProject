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
	case PLAYERINFO:
	{
		UpdatePlayerInfo();
		retval = send(sock, (char*)&type, sizeof(PACKET_TYPE), 0);
		retval = send(sock, (char*)&pInfo, sizeof(PlayerInfo), 0);
		break;
	}
	case UIPACKET:
		uiinfo = UIMng.HandOverInfo();
		printf("%d\n", uiinfo.PlayerID);
		printf("%d\n", uiinfo.score);
		printf("%d\n", uiinfo.Stage);
		retval = send(sock, (char*)&type, sizeof(PACKET_TYPE), 0);
		retval = send(sock, (char*)&uiinfo, sizeof(UI), 0);
		break;
	case LOBBYPACKET:
		break;
	case COLLIDEENEMY:
		break;
	case ALLPACKET:
		break;
	}
	if (retval == SOCKET_ERROR) err_display("send()");
}

void CLIENT::UpdatePlayerInfo()
{
	pInfo.id = player->GetId();
	pInfo.pos = player->getPos();
	pInfo.sword = player->getSword();
	pInfo.numOfShell = player->GetNumOfShell();
	pInfo.orbitRay = player->GetorbitRay();
	pInfo.shellStack = player->GetshellStack();
	pInfo.isTouched = player->GetisTouched();
	pInfo.isdamaged = player->Getisdamaged();
	pInfo.bangMotion = player->GetbangMotion();
	pInfo.Bangpos = player->GetBangpos();
	pInfo.velocity = player->GetVelocity();
}

void CLIENT::UpdateUIInfo(int level,int score)
{
	UIMng.UpdateLevel(player, level);
	UIMng.UpdateScore(player, score);
	UIMng.Print();
}

void CLIENT::UpdateOtherPlayers()
{
	for (int i = 0; i < 4; ++i)
	{
		int pid = All_packet.P_info[i].id;
		//아직 접속하지 않는 플레이어의 아이디 이거나 자기 자신의 id값이 아닐 경우
		if (pid == player->GetId() || pid == -1) continue;

		Otherplayers[i].SetId(All_packet.P_info[i].id);
		Otherplayers[i].setPos(All_packet.P_info[i].pos);
		Otherplayers[i].SetSword(All_packet.P_info[i].sword);
		Otherplayers[i].setSwordShape();
		Otherplayers[i].SetNumOfShell(All_packet.P_info[i].numOfShell);
		Otherplayers[i].SetisTouched(All_packet.P_info[i].isTouched);
		Otherplayers[i].SetorbitRay(All_packet.P_info[i].orbitRay);
		Otherplayers[i].SetshellStack(All_packet.P_info[i].shellStack);
		Otherplayers[i].Setisdamaged(All_packet.P_info[i].isdamaged);
		Otherplayers[i].SetbangMotion(All_packet.P_info[i].bangMotion);
		Otherplayers[i].SetBangpos(All_packet.P_info[i].Bangpos);
		Otherplayers[i].SetVelocity(All_packet.P_info[i].velocity);
		
		if (Otherplayers[i].GetbangMotion() == 10) //총을 발사 했을때
		{
			POINT bulletpos = Otherplayers[i].GetBangpos();
			POINTFLOAT bulletVelocity = Otherplayers[i].GetVelocity();
			OtherPlayerBullets->add(bulletpos, bulletVelocity);
		}
	}
}

void CLIENT::UpdateOtherPlayerBullets(RECT* map)
{
	OtherPlayerBullets->move(map);
}

void CLIENT::Recv_Packet(SOCKET& sock)
{
	int retval;
	
	WaitForSingleObject(SendEvent, INFINITE);
	retval = recv(sock, (char*)&All_packet, sizeof(ALL_PACKET), MSG_WAITALL);
	if (retval == SOCKET_ERROR) err_display("send()");
	
	SetEvent(ReadEvent);
	/*for (int i = 0; i < 4; ++i) {
		printf("%d Player ID = %d \n",i, All_packet.Ui[i].PlayerID);
		printf("%d Score : %d \n",i, All_packet.Ui[i].score);
		printf("%d Stage : %d \n",i, All_packet.Ui[i].Stage);
	}*/
}

int CLIENT::Init(Player* p, EnemyManager* e,Player* o,UIManager* u, PlayerBulletManager* b)
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

	// connect()
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	inet_pton(AF_INET, SERVERIP, &serveraddr.sin_addr);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("connect()");

	SendEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
	ReadEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	hThread = CreateThread(NULL, 0, RecvThread,
		(LPVOID)this, 0, NULL);
	if (hThread != NULL) (CloseHandle(hThread));
}