#pragma once
#include "Server.h"
#include "global.h"

DWORD WINAPI ServerUpdate(LPVOID arg);
DWORD WINAPI ProcessClient(LPVOID arg);

int SERVER::Init()
{
#ifdef	TEST__DEBUG_TIMER_SETTING
	SetTimer();
#endif

	enemyManager->init();
	int retval = 0;

	enemyManager->playerMng = playerMng;

	playerMng->InitPlayer(ClientCount);
	UIMng->init();
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	ServerEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	hThread = CreateThread(NULL, 0, ServerUpdate,
		(LPVOID)this, 0, NULL);
	if (hThread == NULL) { closesocket(client_sock); }
	else { CloseHandle(hThread); }
}

DWORD WINAPI ServerUpdate(LPVOID arg)
{
	int retval;
	SERVER* server = (SERVER*)arg;
	while (1)
	{
		int nCount = server->GetThreadUpdateEvent().size();
		const HANDLE* lpHandles = NULL;
		if(nCount) lpHandles = (const HANDLE*)&server->GetThreadUpdateEvent()[0];

		WaitForMultipleObjects(nCount, lpHandles, TRUE, INFINITE);
		//std::cout << "server" << std::endl;
		server->UpdateInitVariable();
		if (server->GetTime() > 0.005f)
		{
			QueryPerformanceFrequency(&server->timer);
			QueryPerformanceCounter(&server->start);

			server->UpdateFrequent();
		}
		server->UpdateMovement();
		server->UpdateImmediately();
		SetEvent(server->GetServerEvent());

		WaitForMultipleObjects(nCount, lpHandles, TRUE, INFINITE);

		ResetEvent(server->GetServerEvent());
	}

	return 0;
}

DWORD WINAPI ProcessClient(LPVOID arg)
{
	int retval;
	SERVER* server = (SERVER*)arg;
	SOCKET client_sock = server->GetClinetSock();
	HANDLE ThreadEvent;
	UINT nThreadEvent;

	server->ClientLogin(client_sock, nThreadEvent, ThreadEvent);

	while (1) {
		server->Processing(client_sock, ThreadEvent);
	}

	// 소켓 닫기
	return 0;
}

void SERVER::Recv_Packet(SOCKET& clientsock)
{
	EnterCriticalSection(&csRecv);
	int retval;
	//패킷 type 이랑 크기 받고 type에 따라 처리해야함
	PREPARE_INFO pre_info;
	retval = recv(clientsock, (char*)&pre_info, sizeof(PREPARE_INFO), MSG_WAITALL);
	//PACKET_TYPE type;
	//retval = recv(clientsock, (char*)&type, sizeof(type), MSG_WAITALL);
	if (retval == SOCKET_ERROR) err_display("recv()");

	switch (pre_info.packet_type)
	{
	case CLIENTINFO:
	{

		if (ClientCount != playerMng->GetPlayerNum())
			playerMng->SetPlayerNum(ClientCount);

		retval = recv(clientsock, (char*)&Clientinfo, sizeof(ClientInfo), MSG_WAITALL);
		if (pre_info.collide_ememy_num)
		{
			Collideinfo.resize(pre_info.collide_ememy_num);
			retval = recv(clientsock, (char*)&Collideinfo[0], sizeof(CollideInfo) * Collideinfo.size(), MSG_WAITALL);
#ifdef TEST__SWORD_TO_ENEMY_PRT
			std::cout << "Collideinfo Size: " << Collideinfo.size() << std::endl;
			for (int i = 0; i < Collideinfo.size(); ++i)
			{
				std::cout << "index: " << Collideinfo[i].index << std::endl;
				std::cout << "type: " << (int)Collideinfo[i].collide_type << std::endl << std::endl;
			}
#endif
		}
		if (pre_info.collide_player_num)
		{
			PlayerCollideinfo.resize(pre_info.collide_player_num);
			retval = recv(clientsock, (char*)&PlayerCollideinfo[0], sizeof(CollideInfo) * PlayerCollideinfo.size(), MSG_WAITALL);
		}

		playerMng->RecvPlayer(Clientinfo.Pinfo);
		UIMng->Recv_UI(Clientinfo.Ui);
		
		break;
	}
	case LOBBYPACKET:
		break;
	case ALLPACKET:
		break;
	}
	if (retval == SOCKET_ERROR) err_display("recv()");
#ifdef TEST__PRT_PLAYER_INFO__PINFO_POS
	PlayerInfo* pInfo = playerMng->HandOverInfo();
	for (int i = 0; i < MAX_PLAYER; ++i)
	{
		std::cout << "id: " << pInfo[i].id << ", x: " <<
			pInfo[i].pos.x << ", y: " << pInfo[i].pos.y << endl;
	}
	std::cout << std::endl;
#endif

	LeaveCriticalSection(&csRecv);
}

void SERVER::Send_AllPacket()
{
	EnterCriticalSection(&csSend);
	ALL_PACKET packet;
	memcpy(packet.P_info, playerMng->HandOverInfo(), sizeof(PlayerInfo) * MAX_PLAYER);
	for (int i = 0; i < enemyManager->bulletMng->getBulletNum(); ++i)
	{
		if (!enemyManager->bulletMng->getBulletPtr(i)) break;
		memcpy(&packet.bulletList[i], enemyManager->bulletMng->getBulletPtr(i), sizeof(Bullet));
	}

	for (int i = 0; i < enemyManager->getEnemyNumber(); ++i)
	{
		if (!enemyManager->HandOverInfo(i)) break;
		memcpy(&packet.enemyList[i], enemyManager->HandOverInfo(i), sizeof(Enemy));
	}
	packet.mob_num = enemyManager->getEnemyNumber();
	//std::cout << "bulletNum: " << enemyManager->bulletMng->getBulletNum() << std::endl;
	packet.bullet_num = enemyManager->bulletMng->getBulletNum();
	memcpy(packet.Ui, UIMng->HandOverInfo(), sizeof(UI) * MAX_PLAYER);

	memcpy(packet.effectEnemy, &enemyManager->effectInfo, sizeof(EFFECT_INFO) * MAX_MOB);
	memcpy(packet.effectBullet, &enemyManager->effectBulletInfo, sizeof(EFFECT_INFO) * MAX_BULLET);
	packet.effectEnemyNum = enemyManager->effectNum;
	packet.effectBulletNum = enemyManager->effectBulletNum;
	for (int i = 0; i < enemyManager->effectNum; ++i)
	{
		std::cout << "index: " << enemyManager->effectInfo[i].state << ", pos: " << enemyManager->effectInfo[i].previous_pos.x << ", " << enemyManager->effectInfo[i].previous_pos.y << std::endl;
	}
#ifdef TEST__SEND_ALLPACKET__PINFO_POS
	for (int i = 0; i < ClientCount; ++i)
	{
		cout << packet.P_info->id << ": " <<
			packet.P_info->pos.x << ", " << packet.P_info->pos.y << endl;
	}
#endif

#if defined(TEST__PRT_ENEMY_INFO) && defined(TEST__DEBUG_TIMER_SETTING)
	QueryPerformanceCounter(&end);
	if (GetTime() > 2.5f)
	{
		QueryPerformanceFrequency(&timer);
		QueryPerformanceCounter(&start);

		for (int i = 0; i < MAX_MOB; ++i)
		{
			cout << "enemyList[" << i << "]'s SpawnState: " << packet.enemyList[i].isSpawned() <<
				", enemyPos: " << packet.enemyList[i].getPos().x << ", " << packet.enemyList[i].getPos().y << endl;
		}
		cout << endl;
	}
#endif

	for (auto& cl : v_clients)
		send(cl, (char*)&packet, sizeof(packet), 0);
	LeaveCriticalSection(&csSend);
}

void SERVER::ClientLogin(SOCKET& clientsock, UINT& nThreadEvent, HANDLE& ThreadEvent)
{
	int retval;
	v_clients.push_back(clientsock);
	retval = send(clientsock, (char*)&ClientCount, sizeof(int), 0);
	if (retval == SOCKET_ERROR) err_display("send()");

	ThreadEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	nThreadEvent = Thread_UpdateEvent.size();
	Thread_UpdateEvent.push_back(ThreadEvent);

	ClientCount++;
	playerMng->SetPlayerNum(ClientCount);
}

void SERVER::UpdateInitVariable()
{
	for (int i = 0; i < MAX_MOB; ++i)
		enemyManager->effectInfo->state = none;
	enemyManager->effectNum = 0;
	enemyManager->UpdateState();

	for (int i = 0; i < MAX_BULLET; ++i)
		enemyManager->effectBulletInfo->state = none;
	enemyManager->effectBulletNum = 0;


	for (int i = 0; i < MAX_PLAYER; ++i)
		playerMng->effectInfo->state = none;
	playerMng->effectNum = 0;
}

void SERVER::UpdateMovement()
{
	enemyManager->move(playerMng->HandOverInfo());
}

void SERVER::UpdateFrequent()
{
	if (playerMng->getReady()) {
		waveMng->update();
		Spawn();
	}
	//플레이어 받아오면 player->getcore() 넘겨준다.
}

void SERVER::UpdateMergeInfo()
{
	EnterCriticalSection(&csMerge);

	Mergeinfo.clear();
	Mergeinfo.reserve(Collideinfo.size() + PlayerCollideinfo.size());
	for (int i = 0; i < Collideinfo.size(); ++i)
		Mergeinfo.push_back(Collideinfo[i]);
	for (int i = 0; i < PlayerCollideinfo.size(); ++i)
		Mergeinfo.push_back(PlayerCollideinfo[i]);


	sort(Mergeinfo.begin(), Mergeinfo.end(), [](const CollideInfo& lhs, const CollideInfo& rhs) { return lhs.index < rhs.index; });
	if (Collideinfo.size() + PlayerCollideinfo.size())
	{
		std::cout << "merge size: " << Mergeinfo.size() << "== sum: " << Collideinfo.size() + PlayerCollideinfo.size() << std::endl;
		for (int i = 0; i < Collideinfo.size(); ++i)
			std::cout << "colid inex: " << Collideinfo[i].index << ", type: " << (int)Collideinfo[i].collide_type << std::endl;
		for (int i = 0; i < PlayerCollideinfo.size(); ++i)
			std::cout << "Player inex: " << PlayerCollideinfo[i].index << ", type: " << (int)PlayerCollideinfo[i].collide_type << std::endl;
		for (int i = 0; i < Mergeinfo.size(); ++i)
		{
			std::cout << "merge inex: " << Mergeinfo[i].index << ", type: " << (int)Mergeinfo[i].collide_type << std::endl;
		}
	}
	Collideinfo.clear();
	PlayerCollideinfo.clear();
	LeaveCriticalSection(&csMerge);
}

void SERVER::UpdateImmediately()
{
	PlayerInfo* pInfo = playerMng->HandOverInfo();

	for (int i = 0; i < Mergeinfo.size(); ++i)
	{
		//std::cout << "check" << std::endl;
		switch (Mergeinfo.back().collide_type)
		{
		case COLLIDE_TYPE::SWORD_TO_PLAYER:
		{
			int id = Mergeinfo[i].index;
			COLLIDE_TYPE type = Mergeinfo[i].collide_type;
			int count = std::count_if(Mergeinfo.begin(), Mergeinfo.end(), [id, type](const CollideInfo& cinfo) { return (cinfo.index == id && cinfo.collide_type == type); });
			if (count > 1) {
				std::cout << "\n\n\n\n------delete error------\n\n\n\n" << std::endl;
				break;
			}
			if (pInfo[id].numOfShell == 0)
				pInfo[id].hp.Add_damage(SWORD_DAMAGE);
			else if (pInfo[id].numOfShell >= 1)
				pInfo[id].numOfShell--;
		}
		break;
		case COLLIDE_TYPE::SWORD_TO_ENEMY:
		{
			int index = Mergeinfo.back().index;
			COLLIDE_TYPE type = Mergeinfo[i].collide_type;
			int count = std::count_if(Mergeinfo.begin(), Mergeinfo.end(), [index, type](const CollideInfo& cinfo) { return (cinfo.index == index && cinfo.collide_type == type); });
			if (count > 1) {
				std::cout << "\n\n\n\n------delete error------\n\n\n\n" << std::endl;
				break;
			}
			if (index >= enemyManager->getEnemyNumber()) {
				std::cout << "bug:sword_to_enemy[invalide index]" << std::endl;
				break;
			}
			enemyManager->destroy(index);
		}
		break;
		case COLLIDE_TYPE::SWORD_TO_ENEMYS_BULLET:
		{
			int index = Mergeinfo.back().index;
			COLLIDE_TYPE type = Mergeinfo[i].collide_type;
			int count = std::count_if(Mergeinfo.begin(), Mergeinfo.end(), [index, type](const CollideInfo& cinfo) { return (cinfo.index == index && cinfo.collide_type == type); });
			if (count > 1) {
				std::cout << "\n\n\n\n------delete error------\n\n\n\n" << std::endl;
				break;
			}
			if (index >= enemyManager->bulletMng->getBulletNum()) {
				std::cout << "bug:sword_to_enemys_bullet[invalide index]" << std::endl;
				break;
			}
			enemyManager->SetBulletInfo(index);
			enemyManager->bulletMng->destroy(index);
			}
		break;
		case COLLIDE_TYPE::BULLET_TO_ENEMY:
		{
			int index = Mergeinfo.back().index;
			COLLIDE_TYPE type = Mergeinfo[i].collide_type;
			int count = std::count_if(Mergeinfo.begin(), Mergeinfo.end(), [index, type](const CollideInfo& cinfo) { return (cinfo.index == index && cinfo.collide_type == type); });
			if (count > 1) {
				std::cout << "\n\n\n\n------delete error------\n\n\n\n" << std::endl;
				break;
			}
			if (index >= enemyManager->getEnemyNumber()) {
				std::cout << "bug:bullet_to_enemy[invalide index]" << std::endl;
				break;
			}
			enemyManager->destroy(index);
		}
		break;
		case COLLIDE_TYPE::BULLET_TO_ENEMYS_BULLET:
		{
			int index = Mergeinfo.back().index;
			COLLIDE_TYPE type = Mergeinfo[i].collide_type;
			int count = std::count_if(Mergeinfo.begin(), Mergeinfo.end(), [index, type](const CollideInfo& cinfo) { return (cinfo.index == index && cinfo.collide_type == type); });
			if (count > 1) {
				std::cout << "\n\n\n\n------delete error------\n\n\n\n" << std::endl;
				break;
			}
			if (index >= enemyManager->bulletMng->getBulletNum()) {
				std::cout << "bug:bullet_to_enemys_bullet[invalide index]" << std::endl;
				break;
			}
			enemyManager->SetBulletInfo(index);
			enemyManager->bulletMng->destroy(index);
		}
		break;
		case COLLIDE_TYPE::ENEMYS_BULLET_TO_PLAYER:

			break;
		case COLLIDE_TYPE::ENEMYS_BOMB_TO_PLAYER:
			break;
		default:
			break;
		}
		Mergeinfo.pop_back();
	}
	//if (ClientCount > 1) WaitForSingleObject(UpdateEvent, INFINITE);
	//playerMng->UpdateCollide(PlayerCollideinfo);
	//enemyManager->UpdateCollide(Collideinfo);
	//if (ClientCount > 1) SetEvent(UpdateEvent);
}

EnemyManager* SERVER::getList()
{
	return enemyManager;
}

void SERVER::Spawn()
{
	int mobNum = enemyManager->getEnemyNumber();

	if (waveMng->isSpawn()) {
		Monster* now = waveMng->getMob();
		if (now != nullptr) {
			enemyManager->spawn(now->start, now->type, now->protect);
		}
	}

	static int shootTerm = 50;
	static int shootStock = 0;
	shootStock++;
	if (shootStock > shootTerm) {
		shootStock = 0;
		std::cout << std::endl << "Mob Number: " << enemyManager->getEnemyNumber() << std::endl << std::endl;
		enemyManager->shoot();
	}
}

void SERVER::Processing(SOCKET& client_sock, HANDLE& ThreadEvent)
{
	QueryPerformanceCounter(&end);
	Recv_Packet(client_sock);
	//std::cout << "thread - " << (int)ThreadEvent << std::endl;
	// if all thread's recved packet, start server update
	UpdateMergeInfo();
	SetEvent(ThreadEvent);
	WaitForSingleObject(GetServerEvent(), INFINITE);
	//
	//std::cout << "take off" << std::endl;
	//UpdateImmediately();
	SetEvent(ThreadEvent);
	// if the server has been updated, start sending packets
	Send_AllPacket();
}

int SERVER::Update()
{
	InitializeCriticalSection(&csMerge);
	InitializeCriticalSection(&csRecv);
	InitializeCriticalSection(&csSend);

	while (1) {
		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (struct sockaddr*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}

		char addr[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
		printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
			addr, ntohs(clientaddr.sin_port));

		// 스레드 생성
		hThread = CreateThread(NULL, 0, ProcessClient,
			(LPVOID)this, 0, NULL);
		if (hThread == NULL) { closesocket(client_sock); }
		else { CloseHandle(hThread); }

	}
	DeleteCriticalSection(&csMerge);
	DeleteCriticalSection(&csRecv);
	DeleteCriticalSection(&csSend);

	// 소켓 닫기
	closesocket(listen_sock);

	// 윈속 종료
	WSACleanup();
	return 0;
}

#ifdef TEST__DEBUG_TIMER_SETTING
float SERVER::GetTime()
{
	return (end.QuadPart - start.QuadPart) / (float)timer.QuadPart;
}

void SERVER::SetTimer()
{
	QueryPerformanceFrequency(&timer);
	QueryPerformanceCounter(&start);
}
#endif