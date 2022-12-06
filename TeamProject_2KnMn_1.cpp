// TeamProject_2KnMn_1.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "TeamProject_2KnMn_1.h"
#include "Clinet.h"
#include<mmsystem.h>
#pragma comment(lib,"winmm.lib")
#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 여기에 코드를 입력합니다.

	// 전역 문자열을 초기화합니다.
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_TEAMPROJECT2KNMN1, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 응용 프로그램 초기화를 수행합니다:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TEAMPROJECT2KNMN1));

	MSG msg;

	// 기본 메시지 루프입니다:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TEAMPROJECT2KNMN1));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;// MAKEINTRESOURCEW(IDC_TEAMPROJECT2KNMN1);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, 1024, 768, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}




//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
#define TERM 100

std::random_device sid;
std::uniform_int_distribution<int> uid_int(TERM, WHOLE_MAP - TERM);
std::mt19937_64 mte(uid_int(sid));


static Player player;
Player OtherPlayers[3];

PlayerBulletManager OtherPlayerBulletMng;
EnemyManager* enemyMng = new EnemyManager;
MapManager mapMng({ WHOLE_MAP, WHOLE_MAP });
WaveManager* waveMng = new WaveManager;
UIManager* UIMng = new UIManager;
extern EffectManager effectMng;
RECT win;

BOOL playerKeyInput[6] = { NULL };

POINT mPos;

int collideNum;
int collideTime;

CLIENT Client;
//bool bStart = false;
char str[] = "Slash And Shoot";
void menu(HDC hdc)
{
	HFONT hfont = CreateFont(80, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 3, 2, 1, VARIABLE_PITCH | FF_ROMAN, L"Benguiat");
	HFONT oldfont = (HFONT)SelectObject(hdc, hfont);
	SetBkMode(hdc, TRANSPARENT);
	TextOut(hdc, mapMng.getCameraPoint().x + 350, mapMng.getCameraPoint().y + 200, L"Slash And Shoot", 15);
	SelectObject(hdc, oldfont);
	DeleteObject(hfont);
	hfont = CreateFont(40, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 3, 2, 1, VARIABLE_PITCH | FF_ROMAN, L"Benguiat");
	oldfont = (HFONT)SelectObject(hdc, hfont);
	TextOut(hdc, mapMng.getCameraPoint().x + 525, mapMng.getCameraPoint().y + 500, L"Press Any Key", 13);
	SelectObject(hdc, oldfont);
	DeleteObject(hfont);
}
void CALLBACK timeProc(HWND, UINT, UINT, DWORD);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int score = 0;
	static POINT client;
	static POINT screen;
	static HBITMAP hbm;

	switch (message)
	{
	case WM_CREATE:
		Client.Init(&player, enemyMng, OtherPlayers, UIMng, &OtherPlayerBulletMng); // 서버와 연결
			//PlaySound(MAKEINTRESOURCE(IDR_WAVE1), hInst, SND_RESOURCE | SND_ASYNC | SND_LOOP);
		screen = { GetSystemMetrics(SM_CXFULLSCREEN), GetSystemMetrics(SM_CYFULLSCREEN) };
		MoveWindow(hWnd, 0, 0, mapMng.getCameraSize().x, mapMng.getCameraSize().y, FALSE);
		GetClientRect(hWnd, &win);
		SetTimer(hWnd, 0x1000, 1000 / 60, (TIMERPROC)timeProc);
		{
			POINT temp = mapMng.getWholeMapSize();
			temp.x /= 2; temp.y /= 2;
			player.start(temp);

			for (int i = 0; i < 3; ++i)
				OtherPlayers[i].start(temp);

			Client.SetMapSize(mapMng.getMapRect());

			HDC hdc = GetDC(hWnd);
			hbm = CreateCompatibleBitmap(hdc, mapMng.getWholeMapSize().x, mapMng.getWholeMapSize().y);
			ReleaseDC(hWnd, hdc);
		}
		break;
	case WM_GETMINMAXINFO:
		((MINMAXINFO*)lParam)->ptMaxTrackSize = mapMng.getCameraSize();//screen.x;
		((MINMAXINFO*)lParam)->ptMinTrackSize = mapMng.getCameraSize();
		break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// 메뉴 선택을 구문 분석합니다:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_KEYDOWN:
		bStart = true;
		switch (wParam) {
		case 'A':
			if (playerKeyInput[0] == FALSE) {
				playerKeyInput[0] = TRUE;
			}
			break;
		case 'D':
			if (playerKeyInput[1] == FALSE) {
				playerKeyInput[1] = TRUE;
			}
			break;
		case 'S':
			if (playerKeyInput[2] == FALSE) {
				playerKeyInput[2] = TRUE;
			}
			break;
		case 'W':
			if (playerKeyInput[3] == FALSE) {
				playerKeyInput[3] = TRUE;
			}
			break;
		case 'R':
		{
			POINT temp = mapMng.getWholeMapSize();
			temp.x /= 2; temp.y /= 2;
			player.start(temp);
			delete enemyMng;
			EnemyManager* enemyMng = new EnemyManager;
			waveMng->setLevel();
			break;
		}
		case VK_LEFT:
			playerKeyInput[4] = TRUE;
			break;
		case VK_RIGHT:
			playerKeyInput[5] = TRUE;
			break;
		case VK_ESCAPE:
			bStart = false;
			break;
		case VK_CAPITAL:
		{
			static bool bCursorShow = false;
			ShowCursor(bCursorShow);
			bCursorShow = !bCursorShow;
		}
		break;
		case VK_TAB:
			break;
		}
		break;
	case WM_KEYUP:
		switch (wParam) {
		case 'A':
			playerKeyInput[0] = FALSE;
			break;
		case 'D':
			playerKeyInput[1] = FALSE;
			break;
		case 'S':
			playerKeyInput[2] = FALSE;
			break;
		case 'W':
			playerKeyInput[3] = FALSE;
			break;
		case VK_LEFT:
			playerKeyInput[4] = FALSE;
			break;
		case VK_RIGHT:
			playerKeyInput[5] = FALSE;
			break;
		}
		break;
	case WM_LBUTTONDOWN:
		if (bStart)
			playerKeyInput[5] = TRUE;
		break;
	case WM_LBUTTONUP:
		if (bStart)
			playerKeyInput[5] = FALSE;
		break;
	case WM_RBUTTONDOWN:
		if (bStart)
			playerKeyInput[4] = TRUE;
		break;
	case WM_RBUTTONUP:
		if (bStart)
			playerKeyInput[4] = FALSE;
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		HDC memDC = CreateCompatibleDC(hdc);
		SelectObject(memDC, hbm);

		static double magnitude = 10;
		POINT qPos = { (LONG)(20 * (float)sin(magnitude) * (float)pow(0.5, magnitude) + 0.5f) , 0 };
		magnitude += 0.2;

		draw(memDC);
		if (!bStart)
			menu(memDC);

		POINT camera = mapMng.getCameraPoint();
		POINT win = mapMng.getCameraSize();
		BitBlt(hdc, qPos.x, qPos.y, win.x, win.y, memDC, camera.x, camera.y, SRCCOPY);

		DeleteDC(memDC);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		KillTimer(hWnd, 0x1000);
		DeleteObject(hbm);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return NULL;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

void spawn() {
	static int shootTerm = 100;
	static int shootStock = shootTerm;
	shootStock++;
	if (shootStock > shootTerm) {
		shootStock = 0;
		enemyMng->shoot(player.getPos());
	}
}


void collide() {
	//Player -> Enemy
	LKM::Shape sword(6);
	player.getSwordCollider(&sword);

	int addLength = 0;
	if (enemyMng->isAttacked(&sword)) { // Sword to Enemy
		waveMng->addScore(11);
		addLength++;
	}
	if (enemyMng->bulletMng->isCollideToSword(&sword)) { // Sword to Enemy's Bullet
		waveMng->addScore(1);
		addLength++;
	}
	//player.sword.addLength(addLength);
	//Player's Bullet -> Enemy
	for (int i = 0; i < player.pbManager.getNum(); ++i) {
		LKM::Shape* bullet = player.pbManager.getBulletShape(i);
		bool result = false;
		if (enemyMng->isAttacked(bullet)) {
			waveMng->addScore(6);
		}
		if (enemyMng->bulletMng->isCollideToBullet(bullet)) {
			waveMng->addScore(2);
		}
		if (result) {
			player.pbManager.destroy(i);
		}
	}

	//Enemy -> Player
	for (int i = 0; i < enemyMng->bulletMng->getBulletNum(); ++i) {
		Bullet* const bullet = enemyMng->bulletMng->getBulletPtr(i);
		if (player.isDamaged() && LKM::collideP2P(player.getPos(), bullet->getPos(), (FLOAT)(player.getCoreRay() + bullet->getSize()))) {
			enemyMng->bulletMng->destroy(i);
			player.beAttacked();
			collideTime++;
		}
	}
	for (int i = 0; i < enemyMng->getEnemyNumber(); ++i) {
		Bomber* bomb = enemyMng->getBomb(i);
		if (bomb != nullptr)
			if (player.isDamaged() && LKM::collideP2P(player.getPos(), bomb->getPos(), (FLOAT)(player.getCoreRay() + bomb->getBombRange()))) {
				player.beAttacked();
				collideTime++;
				effectMng.add(bomb->getPos(), BTOOM);
				enemyMng->destroy(i);
			}
	}
}

void update(HWND hWnd, BOOL buffer[])
{
	mapMng.update(hWnd, player.getPos());

	if (player.gameovercheck())
		return;

	enemyMng->move(player.getCore());
	RECT map = mapMng.getMapRect();
	RECT whole = mapMng.getWholeMapRect();
	player.move(&map, &whole, NULL);

	Client.Send_Packet(CLIENTINFO);

	//Client.UpdateOtherPlayers(); //다른 플레이어들의 정보를 갱신
	//Client.UpdateOtherPlayerBullets(&whole); //다른 플레이어들이 쏜 총알 갱신
	//Client.UpdateEnemy();
	
	Client.UpdateUIInfo(waveMng->getLevel(), 10);
}


void moniter(HDC hdc) {
	TCHAR word[50];
	POINT point = mapMng.getCameraPoint();
	SetBkMode(hdc, TRANSPARENT);
	/*
	wsprintf(word, L"killenemy : %d", collidenum);
	TextOut(hdc, point.x + 300, point.y, word, _tcslen(word));
	*/
	wsprintf(word, L"Level : %d", waveMng->getLevel());
	TextOut(hdc, point.x + 600, point.y, word, _tcslen(word));
	Client.printUI(point,hdc);
}


void draw(HDC hdc)
{
	mapMng.draw(hdc);
	player.draw(hdc);
	for (int i = 0; i < 3; ++i)
		OtherPlayers[i].draw(hdc);

	OtherPlayerBulletMng.draw(hdc);
	enemyMng->draw(hdc);
	moniter(hdc);
}

void CALLBACK timeProc(HWND hWnd, UINT uMsg, UINT id, DWORD dwTime) {
	GetCursorPos(&mPos);
	ScreenToClient(hWnd, &mPos);
	mapMng.ScreenToGame(mPos);
	player.mouseInput(playerKeyInput[5], playerKeyInput[4], mPos);
	player.input(playerKeyInput);
	update(hWnd, playerKeyInput);

	InvalidateRect(hWnd, NULL, FALSE);
}
