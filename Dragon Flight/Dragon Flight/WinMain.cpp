#include "Object_BG1.h"
#include "Object_BG2.h"
#include "Object_BG3.h"
#include "Object_Monster.h"
#include "Object_midMonster.h"
#include "Object_Player.h"
#include "Object_missile.h"
#include "Object_Boss.h"
#include "Object_Explosion.h"
#include "Object_missile2.h"
#include "Object_missile3.h"
#include "Object_Meteo.h"
#include "Object_Stone.h"
#include "Scene.h"
#include "resource.h"
#include <MMSystem.h>
#pragma comment(lib, "winmm.lib" )

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// LRESULT == long , �޽��� ó�� �Լ�
BOOL CALLBACK DIALOG1Proc(HWND, UINT, WPARAM, LPARAM);
enum enemy {NORMAL, MID, BOSS, SCORE};

Scene* scene = new Scene;
HINSTANCE hIns, g_hInst;
clock_t frame_time;
clock_t current_time;
RECT start[5], rect;
BG1* bg1 = new BG1;
BG2* bg2 = new BG2;
BG3* bg3 = new BG3;
Player* player = new Player;
Monster* normal_monster[10];
MidMonster* mid_monster[10];
Explosion* explosion_show = new Explosion;
Boss* boss = new Boss;
Meteo* meteo = new Meteo;
Stone* stones[5];
Missile* missiles[20];
Missile2* missiles2[20];
Missile3* missiles3[20];

RECT pRect, mon1Rect[MAX_MONSTER], mon2Rect, missilesRect[MAX_MISSILE], pointt, scoreRC, error;

double tempY[5];
static int point = 0, score = 0, hp = 5, missileLev = 1, missilecnt = 1;
bool shop = false;
int count = 0;
static int item[4];
static float ms = 5000.f;
static float meteoTime = 1500.f;
static float stoneTime = 0.f;
bool bossInit = false;
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR strCmdLine, int nShowCmd)
{
	WNDCLASSEX wcex;	//	�Ӽ� ����
	memset(&wcex, 0, sizeof(wcex));

	g_hInst = hInstance;
	wcex.cbSize = sizeof(wcex);
	wcex.style = CS_HREDRAW | CS_VREDRAW;	// ������ ��Ÿ�� ����
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	// ��� ����
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);	// Ŀ��
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);	// ������
	wcex.hInstance = hInstance;	// ���α׷� ��ȣ
	hIns = hInstance;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hIconSm = NULL;	// ���� ������
	wcex.lpszMenuName = NULL;	// �޴� �̸�
	wcex.lpfnWndProc = WndProc;	// �޽��� ó�� �Լ�
	wcex.lpszClassName = "MyWindow";	// Ŭ���� �̸�
	RegisterClassEx(&wcex);		// Ŭ���� ���

	HWND hWnd = CreateWindow("MyWIndow", "Dragon Flight", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 500, 800,
		NULL, NULL, hInstance, NULL);	// ������ Ŭ���� �̸�, ������ ĸ��, ������ ũ��, ��ġ ����, ���α׷� ��ȣ

	if (hWnd == NULL)	// �����츦 �������� ������ ��
	{
		MessageBox(NULL, "������ ���� ����", "����", MB_ICONERROR | MB_OK);
		return -1;
	}

	ShowWindow(hWnd, SW_SHOW);	// ������ �����ֱ�

	MSG msg;
	memset(&msg, 0, sizeof(msg));

	//--------------------------------

	current_time = clock();
	while(1) {
		if (!shop)
		{
			frame_time = clock() - current_time;
			current_time += frame_time;
			scene->run((double)frame_time / 1000);
		}
		//	WM_TIMER ��� ���� ���� �ð��Լ� - double ������ �ѱ�
		InvalidateRect(hWnd, NULL, false);

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)	// �޽��� ó�� �Լ�
{
	static HDC hdc, backDC, memDC;
	PAINTSTRUCT ps;
	HBRUSH hbrush, oldbrush;
	static HBITMAP hBit, backBit, bgBit, fontBit, img1Bit, img2Bit, img3Bit,
		coin, coinmask, wing, wingmask, shleid, shleidmask, super, supermask, gameoverbit, clearBit,
		stone, stone_mask;
	static bool isAttack, supereff;
	static int back, gamestart, select[3], shleideff, supery, gameover, clear;
	static int mousex, mousey;
	static HINSTANCE hInstance;
	static char pointtext[10], scoreText[10], errortext[100];
	int randNum;
	srand(time(NULL));

	if (gamestart)
	{
		count += 2;
		if (count > 700)
		{
			count = 0;
			if (score < 10000)
				score += 1;
		}

		//	���� ���� ����
		for (int i = 0; i < MAX_MONSTER; ++i)
		{
			if (normal_monster[i] == NULL)	continue;
			if (normal_monster[i]->getIsDead())
				normal_monster[i] = NULL;
			if (mid_monster[i] == NULL)	continue;
			if (mid_monster[i]->getIsDead())
				mid_monster[i] = NULL;
		}

		//	���� �������� �߰�
		ms += 1;
		meteoTime += 1;
		stoneTime += 1;
		if (score >= 0 && score < 100)	//	�⺻ ��
		{
			if (ms > 5000)		//	5�ʸ���
			{
				ms = 0;
				for (int i = 0; i < MAX_MONSTER; ++i)
				{
					normal_monster[i] = new Monster;
					normal_monster[i]->init(memDC, backDC, hIns);
					normal_monster[i]->setX(i * 100);
					scene->addObject(normal_monster[i]);
				}
			}

			if (stoneTime > 5500)
			{
				stoneTime = 0;
				randNum = rand() % 4;
				for (int i = 0; i < 5; ++i)
				{
					if (randNum == i)
					{
						stones[i] = new Stone;
						stones[i]->init(memDC, backDC, hIns);
						stones[i]->setX(i * 100);
						scene->addObject(stones[i]);

					}
				}
			}
		}

		if (score >= 100 && score < 200)		//	�������
		{
			if (ms > 4000)
			{
				ms = 0;
				for (int i = 0; i < MAX_MONSTER; ++i)
				{
					randNum = rand() % 2 + 1;
					if (randNum == 1)
					{
						normal_monster[i] = new Monster;
						normal_monster[i]->init(memDC, backDC, hIns);
						normal_monster[i]->setX(i * 100);
						scene->addObject(normal_monster[i]);

					}
					else		//	�������
					{
						mid_monster[i] = new MidMonster;
						mid_monster[i]->init(memDC, backDC, hIns);
						mid_monster[i]->setX(i * 100);
						scene->addObject(mid_monster[i]);
					}
				}
				if (stoneTime > 4500)
				{
					stoneTime = 0;
					randNum = rand() % 4;
					for (int i = 0; i < 5; ++i)
					{
						if (randNum == i)
						{
							stones[i] = new Stone;
							stones[i]->init(memDC, backDC, hIns);
							stones[i]->setX(i * 100);
							scene->addObject(stones[i]);
						}
					}
				}
			}
		}

		if (score >= 200)	//	��������
		{
			if (ms > 4000)
			{
				ms = 0;
				for (int i = 0; i < MAX_MONSTER; ++i)
				{
					randNum = rand() % 2 + 1;
					if (randNum == 1)
					{
						normal_monster[i] = new Monster;
						normal_monster[i]->init(memDC, backDC, hIns);
						normal_monster[i]->setX(i * 100);
						scene->addObject(normal_monster[i]);

					}
					else		//	�������
					{
						mid_monster[i] = new MidMonster;
						mid_monster[i]->init(memDC, backDC, hIns);
						mid_monster[i]->setX(i * 100);
						scene->addObject(mid_monster[i]);
					}
				}
			}

			if (bossInit == false)
			{
				bossInit = true;
				PlaySound("Sound/������������������.wav", NULL, SND_ASYNC);
				boss->init(memDC, backDC, hIns);
				scene->addObject(boss);
			}

			//	���׿�
			if (meteoTime > 4500)
			{
				meteoTime = 0.f;
				randNum = rand() % 4;
				for (int i = 0; i < MAX_MONSTER; ++i)
				{
					if (randNum == i)
					{
						meteo->init(memDC, backDC, hIns);
						meteo->setX(i * 100);
						scene->addObject(meteo);
					}
				}
			}

		}

		//	�̻��� - �� �浹üũ
		if (isAttack)
		{
			if (missileLev == 1)
			{
				for (int i = 0; i < MAX_MISSILE; ++i)
				{
					if (missiles[i] == NULL)	continue;
					if (missiles[i]->getIsDead())
					{
						missiles[i] = NULL;
						continue;
					}
					POINT missileP = { (LONG)(missiles[i]->getW() - 15), (LONG)(missiles[i]->getY()) };

					for (int j = 0; j < MAX_MONSTER; ++j)
					{
						if (normal_monster[j] == NULL)
							continue;
						if (normal_monster[j]->getIsDead())
						{
							normal_monster[j] = NULL;
							continue;
						}
						if (PtInRect(&normal_monster[j]->getCollRect(), missileP))	//	�⺻�� �浹
						{
							explosion_show->init(memDC, backDC, hIns);
							explosion_show->setX(normal_monster[j]->getX());
							explosion_show->setY(normal_monster[j]->getY());
							explosion_show->reUse();
							scene->addObject(explosion_show);
							normal_monster[j]->hp -= missiles[i]->attack;
							normal_monster[j]->setX(-100);
							normal_monster[j]->setY(-100);
							normal_monster[j]->setDead(true);
							normal_monster[j] = NULL;
							missiles[i]->setX(-1);
							missiles[i]->setY(-1);
							missiles[i]->setDead(true);
							missiles[i] = NULL;
							point += 10;
						}
					}

					for (int j = 0; j < MAX_MONSTER; ++j)
					{
						if (mid_monster[j] == NULL)
							continue;
						if (mid_monster[j]->getIsDead())
						{
							mid_monster[j] = NULL;
							continue;
						}
						if (PtInRect(&mid_monster[j]->getCollRect(), missileP))	//	�߰��� �浹
						{
							explosion_show->init(memDC, backDC, hIns);
							explosion_show->setX(mid_monster[j]->getX());
							explosion_show->setY(mid_monster[j]->getY());
							explosion_show->reUse();
							scene->addObject(explosion_show);
							mid_monster[j]->hp -= missiles[i]->attack;
							if (mid_monster[j]->hp < 0)
							{
								mid_monster[j]->setDead(true);
								mid_monster[j] = NULL;
							}
							//mid_monster[j]->setX(-100);
							//mid_monster[j]->setY(-100);
							//mid_monster[j]->setDead(true);
							
							missiles[i]->setX(-1);
							missiles[i]->setY(-1);
							missiles[i]->setDead(true);
							missiles[i] = NULL;
							point += 30;
						}
					}
					if (PtInRect(&boss->getCollRect(), missileP))	//	������ �浹
					{
						if (missiles[i] == NULL)	continue;
						explosion_show->init(memDC, backDC, hIns);
						explosion_show->setX(boss->getX());
						explosion_show->setY(boss->getY());
						explosion_show->reUse();
						scene->addObject(explosion_show);
						boss->hp -= missiles[i]->attack;
						if (boss->hp < 0)
						{
							boss->setDead(true);
						}
						missiles[i]->setX(-1);
						missiles[i]->setY(-1);
						missiles[i]->setDead(true);
						missiles[i] = NULL;
						point += 30;
					}

				}
			}

			if (missileLev == 2)
			{
				for (int i = 0; i < MAX_MISSILE; ++i)
				{
					if (missiles2[i] == NULL)	continue;
					else if (missiles2[i]->getIsDead())
					{
						missiles2[i] = NULL;
						continue;
					}
					POINT missileP = { (LONG)(missiles2[i]->getW() - 15), (LONG)(missiles2[i]->getY()) };

					for (int j = 0; j < MAX_MONSTER; ++j)
					{
						if (normal_monster[j] == NULL)
							continue;
						if (normal_monster[j]->getIsDead())
						{
							normal_monster[j] = NULL;
							continue;
						}
						if (PtInRect(&normal_monster[j]->getCollRect(), missileP))	//	�⺻�� �浹
						{
							explosion_show->init(memDC, backDC, hIns);
							explosion_show->setX(normal_monster[j]->getX());
							explosion_show->setY(normal_monster[j]->getY());
							explosion_show->reUse();
							scene->addObject(explosion_show);
							normal_monster[j]->hp -= missiles2[i]->attack;
							normal_monster[j]->setX(-100);
							normal_monster[j]->setY(-100);
							normal_monster[j]->setDead(true);
							normal_monster[j] = NULL;
							missiles2[i]->setX(-1);
							missiles2[i]->setY(-1);
							missiles2[i]->setDead(true);
							missiles2[i] = NULL;
							point += 10;
						}
					}

					for (int j = 0; j < MAX_MONSTER; ++j)
					{
						if (mid_monster[j] == NULL)
							continue;
						if (mid_monster[j]->getIsDead())
						{
							mid_monster[j] = NULL;
							continue;
						}
						if (PtInRect(&mid_monster[j]->getCollRect(), missileP))	//	�߰��� �浹
						{
							explosion_show->init(memDC, backDC, hIns);
							explosion_show->setX(mid_monster[j]->getX());
							explosion_show->setY(mid_monster[j]->getY());
							explosion_show->reUse();
							scene->addObject(explosion_show);
							mid_monster[j]->hp -= missiles2[i]->attack;
							//mid_monster[j]->setX(-100);
							//mid_monster[j]->setY(-100);
							//mid_monster[j]->setDead(true);
							if (mid_monster[j]->hp < 0)
							{
								mid_monster[j]->setDead(true);
								mid_monster[j] = NULL;
							}
							missiles2[i]->setX(-1);
							missiles2[i]->setY(-1);
							missiles2[i]->setDead(true);
							missiles2[i] = NULL;
							point += 30;
						}
					}

					if (PtInRect(&boss->getCollRect(), missileP))	//	������ �浹
					{
						explosion_show->init(memDC, backDC, hIns);
						explosion_show->setX(boss->getX());
						explosion_show->setY(boss->getY());
						explosion_show->reUse();
						scene->addObject(explosion_show);
						boss->hp -= missiles2[i]->attack;
						if (boss->hp < 0)
						{
							boss->setDead(true);
						}
						missiles2[i]->setX(-1);
						missiles2[i]->setY(-1);
						missiles2[i]->setDead(true);
						missiles2[i] = NULL;
					}
				}
			}
			if (missileLev == 3)
			{
				for (int i = 0; i < MAX_MISSILE; ++i)
				{
					if (missiles3[i] == NULL)	continue;
					else if (missiles3[i]->getIsDead())
					{
						missiles3[i] = NULL;
						continue;
					}
					POINT missileP = { (LONG)(missiles3[i]->getW() - 15), (LONG)(missiles3[i]->getY()) };

					for (int j = 0; j < MAX_MONSTER; ++j)
					{
						if (normal_monster[j] == NULL)
							continue;
						if (normal_monster[j]->getIsDead())
						{
							normal_monster[j] = NULL;
							continue;
						}
						if (PtInRect(&normal_monster[j]->getCollRect(), missileP))	//	�⺻�� �浹
						{
							explosion_show->init(memDC, backDC, hIns);
							explosion_show->setX(normal_monster[j]->getX());
							explosion_show->setY(normal_monster[j]->getY());
							explosion_show->reUse();
							scene->addObject(explosion_show);
							normal_monster[j]->hp -= missiles3[i]->attack;
							normal_monster[j]->setX(-100);
							normal_monster[j]->setY(-100);
							normal_monster[j]->setDead(true);
							normal_monster[j] = NULL;
							missiles3[i]->setX(-1);
							missiles3[i]->setY(-1);
							missiles3[i]->setDead(true);
							missiles3[i] = NULL;
							point += 10;
						}
					}

					for (int j = 0; j < MAX_MONSTER; ++j)
					{
						if (mid_monster[j] == NULL)
							continue;
						if (mid_monster[j]->getIsDead())
						{
							mid_monster[j] = NULL;
							continue;
						}
						if (PtInRect(&mid_monster[j]->getCollRect(), missileP))	//	�߰��� �浹
						{
							explosion_show->init(memDC, backDC, hIns);
							explosion_show->setX(mid_monster[j]->getX());
							explosion_show->setY(mid_monster[j]->getY());
							explosion_show->reUse();
							scene->addObject(explosion_show);
							mid_monster[j]->hp -= missiles3[i]->attack;
							//mid_monster[j]->setX(-100);
							//mid_monster[j]->setY(-100);
							//mid_monster[j]->setDead(true);
							if (mid_monster[j]->hp < 0)
							{
								mid_monster[j]->setDead(true);
								mid_monster[j] = NULL;
							}
							mid_monster[j] = NULL;
							missiles3[i]->setX(-1);
							missiles3[i]->setY(-1);
							missiles3[i]->setDead(true);
							missiles3[i] = NULL;
							point += 30;
						}
					}
					if (PtInRect(&boss->getCollRect(), missileP))	//	������ �浹
					{
						explosion_show->init(memDC, backDC, hIns);
						explosion_show->setX(boss->getX());
						explosion_show->setY(boss->getY());
						explosion_show->reUse();
						scene->addObject(explosion_show);
						boss->hp -= missiles3[i]->attack;
						if (boss->hp < 0)
						{
							boss->setDead(true);
						}
						missiles3[i]->setX(-1);
						missiles3[i]->setY(-1);
						missiles3[i]->setDead(true);
						missiles3[i] = NULL;
					}
				}
			}
		}
		//	�� - �÷��̾� �浹üũ
		for (int i = 0; i < MAX_MONSTER; ++i)
		{
			POINT playerP = { (LONG)(player->getW() - 15), (LONG)(player->getY()) };

			if (normal_monster[i] == NULL)
				continue;
			if (normal_monster[i]->getIsDead())
			{
				normal_monster[i] = NULL;
				continue;
			}

			if (PtInRect(&normal_monster[i]->getCollRect(), playerP))	//	�⺻�� �浹
			{
				PlaySound("Sound/�÷��̾ �¾�����.wav", NULL, SND_ASYNC);
				explosion_show->init(memDC, backDC, hIns);
				explosion_show->setX(player->getX());
				explosion_show->setY(player->getY());
				explosion_show->reUse();
				scene->addObject(explosion_show);

				normal_monster[i]->setX(-100);
				normal_monster[i]->setY(-100);
				normal_monster[i]->setDead(true);
				normal_monster[i] = NULL;
				if (shleideff == 0)
					hp -= 1;
			}
		}

		for (int i = 0; i < MAX_MONSTER; ++i)
		{
			POINT playerP = { (LONG)(player->getW() - 15), (LONG)(player->getY()) };

			if (mid_monster[i] == NULL)
				continue;
			if (mid_monster[i]->getIsDead())
			{
				mid_monster[i] = NULL;
				continue;
			}

			if (PtInRect(&mid_monster[i]->getCollRect(), playerP))	//	�߰��� �浹
			{
				PlaySound("Sound/�÷��̾ �¾�����.wav", NULL, SND_ASYNC);
				explosion_show->init(memDC, backDC, hIns);
				explosion_show->setX(player->getX());
				explosion_show->setY(player->getY());
				explosion_show->reUse();
				scene->addObject(explosion_show);

				mid_monster[i]->setX(-100);
				mid_monster[i]->setY(-100);
				mid_monster[i]->setDead(true);
				mid_monster[i] = NULL;
				if (shleideff == 0)
					hp -= 1;
			}
		}
		//	���̻��� - �÷��̾� �浹üũ
		POINT playerP = { (LONG)(player->getW() - 15), (LONG)(player->getY()) };
		if (PtInRect(&meteo->getCollRect(), playerP))	
		{
			PlaySound("Sound/�÷��̾ �¾�����.wav", NULL, SND_ASYNC);
			explosion_show->init(memDC, backDC, hIns);
			explosion_show->setX(player->getX());
			explosion_show->setY(player->getY());
			explosion_show->reUse();
			scene->addObject(explosion_show);

			meteo->init(memDC, backDC, hIns);
			meteo->setDead(true);
			if (shleideff == 0)
				hp -= 1;
		}

		//	�� �÷��̾� �浹
		for (int i = 0; i < 5; ++i)
		{
			if (stones[i] == NULL)	continue;
			if (PtInRect(&stones[i]->getCollRect(), playerP))	
			{
				PlaySound("Sound/�÷��̾ �¾�����.wav", NULL, SND_ASYNC);
				explosion_show->init(memDC, backDC, hIns);
				explosion_show->setX(player->getX());
				explosion_show->setY(player->getY());
				explosion_show->reUse();
				scene->addObject(explosion_show);

				stones[i]->init(memDC, backDC, hIns);
				stones[i]->setDead(true);
				if (shleideff == 0)
					hp -= 1;
			}

		}
		
		//���ӿ���
		if (hp == 0)
		{
			gameover = 1;
			gamestart = 0;
		}
		if (boss->hp == 0)
		{
			clear = 1;
			gamestart = 0;
			PlaySound("Sound/���������׾�����.wav", NULL, SND_ASYNC);
		}
	}
	switch (uMsg)
	{
	case WM_CREATE:
		hInstance = g_hInst;
		GetClientRect(hWnd, &rect);

		//��Ʈ��
		hdc = GetDC(hWnd);
		bgBit = LoadBitmap(hIns, MAKEINTRESOURCE(IDB_BITMAP8));
		fontBit = LoadBitmap(hIns, MAKEINTRESOURCE(IDB_BITMAP9));
		img1Bit = LoadBitmap(hIns, MAKEINTRESOURCE(IDB_BITMAP1));
		img2Bit = LoadBitmap(hIns, MAKEINTRESOURCE(IDB_BITMAP6));
		img3Bit = LoadBitmap(hIns, MAKEINTRESOURCE(IDB_BITMAP7));
		coin = LoadBitmap(hIns, MAKEINTRESOURCE(IDB_BITMAP14));
		coinmask = LoadBitmap(hIns, MAKEINTRESOURCE(IDB_BITMAP22));
		wing = LoadBitmap(hIns, MAKEINTRESOURCE(IDB_BITMAP28));
		wingmask = LoadBitmap(hIns, MAKEINTRESOURCE(IDB_BITMAP29));
		shleid = LoadBitmap(hIns, MAKEINTRESOURCE(IDB_BITMAP30));
		shleidmask = LoadBitmap(hIns, MAKEINTRESOURCE(IDB_BITMAP31));
		super = LoadBitmap(hIns, MAKEINTRESOURCE(IDB_BITMAP35));
		supermask = LoadBitmap(hIns, MAKEINTRESOURCE(IDB_BITMAP36));
		gameoverbit = LoadBitmap(hIns, MAKEINTRESOURCE(IDB_BITMAP41));
		clearBit = LoadBitmap(hIns, MAKEINTRESOURCE(IDB_BITMAP44));
		backDC = CreateCompatibleDC(hdc);
		memDC = CreateCompatibleDC(hdc);
		backBit = CreateCompatibleBitmap(hdc, SCREEN_WIDTH, SCREEN_HEIGHT);
		hBit = CreateCompatibleBitmap(hdc, SCREEN_WIDTH, SCREEN_HEIGHT);
		SelectObject(hdc, hBit);
		SelectObject(backDC, backBit);
		ReleaseDC(hWnd, hdc);

		//�ʱ�ȭ
		isAttack = false;
		supereff = false;
		shleideff = 0;
		gamestart = 0;
		gameover = 0;
		clear = 0;
		scene->shop = false;
		memset(normal_monster, NULL, sizeof(normal_monster));
		memset(mid_monster, NULL, sizeof(mid_monster));
		memset(missiles, NULL, sizeof(missiles));
		memset(missiles2, NULL, sizeof(missiles2));
		memset(missiles3, NULL, sizeof(missiles3));
		memset(scoreText, 0, sizeof(scoreText));
		memset(stones, NULL, sizeof(stones));
		start[3] = { 150,470,350,570 };
		start[4] = { 150,615,350,715 };
		player->init(memDC, backDC, hIns);
		explosion_show->init(memDC, backDC, hIns);
		boss->init(memDC, backDC, hIns);
		for (int a = 0; a < 4; a++)
			item[a] = 0;
		break;

	case WM_LBUTTONDOWN:
	{
		mousex = LOWORD(lParam);
		mousey = HIWORD(lParam);

		POINT mouse = { mousex, mousey };
		if (!gamestart)
		{
			if (PtInRect(&start[0], mouse))
			{
				back = 1;
				select[0] = 7;
				select[1] = 0;
				select[2] = 0;
			}
			else if (PtInRect(&start[1], mouse))
			{
				back = 2;
				select[0] = 0;
				select[1] = 7;
				select[2] = 0;
			}
			else if (PtInRect(&start[2], mouse))
			{
				back = 3;
				select[0] = 0;
				select[1] = 0;
				select[2] = 7;
			}
			else if (PtInRect(&start[3], mouse))
			{
				if (back != 0)
				{
					gamestart = 1;
					PlaySound("Sound/�����Ҷ�.wav", NULL, SND_ASYNC);
					SetTimer(hWnd, SCORE, 1000, NULL);
				}
				else
					MessageBox(hWnd, "����� �������ּ���!", "����� �������ּ���!", MB_OK);
			}
			else if (PtInRect(&start[4], mouse))
			{
				PostQuitMessage(0);
			}
		}

		if (back == 1 && gamestart)		//	���ӽ��� ��ư ���� ��
		{
			bg1->init(memDC, backDC, hIns);	//	��� �ʱ�ȭ
			scene->addObject(bg1);
			scene->addObject(player);
		}
		if (back == 2 && gamestart)
		{
			bg2->init(memDC, backDC, hIns);
			scene->addObject(bg2);
			scene->addObject(player);
		}
		if (back == 3 && gamestart)
		{
			bg3->init(memDC, backDC, hIns);
			scene->addObject(bg3);
			scene->addObject(player);
		}
		InvalidateRect(hWnd, NULL, false);
		break;
	}

	case WM_KEYDOWN:
		if (GetAsyncKeyState('z') || GetAsyncKeyState('Z') && !isAttack)
		{
			isAttack = true;
			if (isAttack)
			{
				for (int i = 0; i < MAX_MISSILE; ++i)
				{
					//	�̻��� ���� 1
					if (missileLev == 1)
					{
						//	������� ������
						if (missiles[i] != NULL)		continue;

						missiles[i] = new Missile;
						missiles[i]->init(memDC, backDC, hIns);
						missiles[i]->setActive(true);
						missiles[i]->setX(player->getW() - 70);
						missiles[i]->setY(player->getY());
						scene->addObject(missiles[i]);
						break;
					}

					else if (missileLev == 2)
					{
						//	������� ������
						if (missiles2[i] != NULL)		continue;

						missiles2[i] = new Missile2;
						missiles2[i]->init(memDC, backDC, hIns);
						missiles2[i]->setActive(true);
						missiles2[i]->setX(player->getW() - 70);
						missiles2[i]->setY(player->getY());
						scene->addObject(missiles2[i]);
						break;

					}

					else if (missileLev == 3)
					{
						//	������� ������
						if (missiles3[i] != NULL)		continue;

						missiles3[i] = new Missile3;
						missiles3[i]->init(memDC, backDC, hIns);
						missiles3[i]->setActive(true);
						missiles3[i]->setX(player->getW() - 70);
						missiles3[i]->setY(player->getY());
						scene->addObject(missiles3[i]);
						break;

					}
				}
			}
			PlaySound("Sound/�����Ҷ�.wav", NULL, SND_ASYNC);
		}
		else if (wParam == VK_CONTROL && gamestart)
		{
			if (shop == false)
			{
				shop = true;
				scene->shop = true;

				PlaySound("Sound/��������.wav", NULL, SND_ASYNC);
				DialogBox(g_hInst, MAKEINTRESOURCE(IDD_shop), hWnd, DIALOG1Proc);
				missilecnt = item[0] + missileLev;
			}
		}
		else if (GetAsyncKeyState('A') || GetAsyncKeyState('a') && gamestart)//���ݷ�����
		{
			if (item[0] > 0)
			{
				item[0]--;
				if (missileLev < 3)	//	�̻��� ������ 3�̻� ���ö󰡰�
					missileLev++;
			}
		}
		else if (GetAsyncKeyState('S') || GetAsyncKeyState('s') && gamestart)//����
		{
			if (item[1] > 0)
			{
				item[1]--;
				shleideff = 1;

			}
		}
		else if (GetAsyncKeyState('D') || GetAsyncKeyState('D') && gamestart)//ü��
		{
			if (item[2] > 0)
			{
				item[2]--;
				hp++;
			}
		}
		else if (GetAsyncKeyState('F') || GetAsyncKeyState('f') && gamestart)//�ʻ��
		{
			if (item[3] > 0)
			{
				PlaySound("Sound/�ʻ��.wav", NULL, SND_ASYNC);
				item[3]--;
				supereff = true;
				supery = 800;
			}
		}
		else if (GetAsyncKeyState('Q') || GetAsyncKeyState('q') && gameover==1 || clear == 1)//�ʻ��
		{
			PostQuitMessage(0);
		}
		InvalidateRect(hWnd, NULL, false);
		break;

	case WM_KEYUP:
		isAttack = false;
		InvalidateRect(hWnd, NULL, false);
		break;

	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);

		if (!gamestart)
		{
			if (gameover == 1)
			{
				SelectObject(memDC, gameoverbit);
				BitBlt(backDC, 0, 0, rect.right, rect.bottom, memDC, 0, 0, SRCCOPY);
			}

			else if (clear == 1)
			{
				SelectObject(memDC, clearBit);
				BitBlt(backDC, 0, 0, rect.right, rect.bottom, memDC, 0, 0, SRCCOPY);

			}
			else
			{
				SetStretchBltMode(backDC, COLORONCOLOR);

				SelectObject(memDC, bgBit);
				BitBlt(backDC, 0, 0, rect.right, rect.bottom, memDC, 0, 0, SRCCOPY);

				SelectObject(memDC, fontBit);
				BitBlt(backDC, 150, 470, 200, 100, memDC, 0, 0, SRCAND);
				BitBlt(backDC, 150, 615, 200, 100, memDC, 0, 100, SRCAND);

				start[0] = { 90 - select[0],220 - select[0],190 + select[0],380 + select[0] };
				start[1] = { 200 - select[1],220 - select[1],300 + select[1],380 + select[1] };
				start[2] = { 310 - select[2],220 - select[2],410 + select[2],380 + select[2] };

				SelectObject(memDC, img1Bit);
				StretchBlt(backDC, 90 - select[0], 220 - select[0], 100 + select[0] * 2, 160 + select[0] * 2, memDC, 0, 0, 500, 800, SRCCOPY);
				SelectObject(memDC, img2Bit);
				StretchBlt(backDC, 200 - select[1], 220 - select[1], 100 + select[1] * 2, 160 + select[1] * 2, memDC, 0, 0, 500, 800, SRCCOPY);
				SelectObject(memDC, img3Bit);
				StretchBlt(backDC, 310 - select[2], 220 - select[2], 100 + select[2] * 2, 160 + select[2] * 2, memDC, 0, 0, 500, 800, SRCCOPY);
			}
		}
		else
		{
			SelectObject(memDC, coinmask);
			StretchBlt(backDC, 0, 0, 50, 50, memDC, 0, 0, 50, 50, SRCPAINT);
			SelectObject(memDC, coin);
			StretchBlt(backDC, 0, 0, 50, 50, memDC, 0, 0, 50, 50, SRCAND);

			wsprintf(pointtext, "%d ����Ʈ", point);
			pointt = { 50,15,150,50 };
			SetBkMode(backDC, TRANSPARENT);
			DrawText(backDC, pointtext, strlen(pointtext), &pointt, DT_CENTER);

			wsprintf(scoreText, "%d M", score);
			scoreRC = { 370, 20, 470, 40 };
			SetBkMode(backDC, TRANSPARENT);
			DrawText(backDC, scoreText, strlen(scoreText), &scoreRC, DT_CENTER);

			if (shleideff > 0)
			{
				shleideff++;
				SelectObject(memDC, shleidmask);
				StretchBlt(backDC, player->getX(), player->getY(), 100, 100, memDC, 0, 0, 150, 150, SRCPAINT);
				SelectObject(memDC, shleid);
				StretchBlt(backDC, player->getX(), player->getY(), 100, 100, memDC, 0, 0, 150, 150, SRCAND);
				if (shleideff == 1000)
					shleideff = 0;
			}

			if (supereff)
			{
				SelectObject(memDC, supermask);
				StretchBlt(backDC, 100, supery--, 250, 200, memDC, 0, 0, 200, 200, SRCPAINT);
				SelectObject(memDC, super);
				StretchBlt(backDC, 100, supery--, 250, 200, memDC, 0, 0, 200, 200, SRCAND);

				if (supery <= -300)
					supereff = false;
				else if (supery == 400 || supery == 401)
				{

					for (int j = 0; j < MAX_MONSTER; ++j)
					{
						if (normal_monster[j] == NULL)
							continue;
						if (normal_monster[j]->getIsDead())
						{
							normal_monster[j] = NULL;
							continue;
						}
						explosion_show->init(memDC, backDC, hIns);
						explosion_show->setX(normal_monster[j]->getX());
						explosion_show->setY(normal_monster[j]->getY());
						explosion_show->reUse();
						scene->addObject(explosion_show);
						normal_monster[j]->setX(-100);
						normal_monster[j]->setY(-100);
						normal_monster[j]->setDead(true);
						normal_monster[j] = NULL;
						point += 10;
					}

					for (int j = 0; j < MAX_MONSTER; ++j)
					{
						if (mid_monster[j] == NULL)
							continue;
						if (mid_monster[j]->getIsDead())
						{
							mid_monster[j] = NULL;
							continue;
						}
						explosion_show->init(memDC, backDC, hIns);
						explosion_show->setX(mid_monster[j]->getX());
						explosion_show->setY(mid_monster[j]->getY());
						explosion_show->reUse();
						scene->addObject(explosion_show);
						mid_monster[j]->setX(-100);
						mid_monster[j]->setY(-100);
						mid_monster[j]->setDead(true);
						mid_monster[j] = NULL;
						point += 30;
					}
				}
			}

			for (int a = 0; a < hp; a++)
			{
				SelectObject(memDC, wingmask);
				StretchBlt(backDC, a * 50, 700, 35, 35, memDC, 0, 0, 115, 115, SRCPAINT);
				SelectObject(memDC, wing);
				StretchBlt(backDC, a * 50, 700, 35, 35, memDC, 0, 0, 115, 115, SRCAND);
			}
		}
		BitBlt(hdc, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, backDC, 0, 0, SRCCOPY);
		EndPaint(hWnd, &ps);
	}
	break;

	case WM_GETMINMAXINFO:
		((MINMAXINFO *)lParam)->ptMaxTrackSize.x = 0;
		((MINMAXINFO *)lParam)->ptMaxTrackSize.y = 0;
		((MINMAXINFO *)lParam)->ptMinTrackSize.x = 500;
		((MINMAXINFO *)lParam)->ptMinTrackSize.y = 800;
		return FALSE;

	case WM_DESTROY:	//	�����츦 �� ��
	{
		PostQuitMessage(0);	//	WM_QUIT�� ȣ��
		DeleteObject(hBit);
		DeleteDC(memDC);
		DeleteDC(backDC);
		delete player;
		delete bg1;
		delete bg2;
		delete bg3;
		for (int i = 0; i < 100; ++i)
		{
			delete normal_monster[i];
			delete missiles[i];
		}
		for (int i = 0; i < 30; ++i)
			delete missiles[i];
		delete[] normal_monster;
		delete[] mid_monster;
		delete[] missiles;
		break;
	}
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);	//	������ �����
}

BOOL CALLBACK DIALOG1Proc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hList;
	HDC d_hdc;
	PAINTSTRUCT d_ps;
	HBITMAP item1, item2, item3, item4, item5, itemselect;
	HWND hButton;

	static char itemlist[6][40], buytext[30], pointtext[10], buypointtext[10];
	static int saveitem[4];
	static int cho = 0, chocount = 0, pointbuy;
	int answer;

	switch (uMsg)
	{

	case WM_INITDIALOG:
		hList = GetDlgItem(hDlg, IDC_buylist);

		item1 = LoadBitmap(hIns, MAKEINTRESOURCE(IDB_BITMAP13));
		item2 = LoadBitmap(hIns, MAKEINTRESOURCE(IDB_BITMAP14));
		item3 = LoadBitmap(hIns, MAKEINTRESOURCE(IDB_BITMAP15));
		item4 = LoadBitmap(hIns, MAKEINTRESOURCE(IDB_BITMAP28));
		item5 = LoadBitmap(hIns, MAKEINTRESOURCE(IDB_BITMAP17));

		hButton = GetDlgItem(hDlg, IDC_attack);
		SendMessage(hButton, BM_SETIMAGE, 0, (LPARAM)item1);
		hButton = GetDlgItem(hDlg, IDC_imagep);
		SendMessage(hButton, BM_SETIMAGE, 0, (LPARAM)item2);
		hButton = GetDlgItem(hDlg, IDC_shield);
		SendMessage(hButton, BM_SETIMAGE, 0, (LPARAM)item3);
		hButton = GetDlgItem(hDlg, IDC_hpup);
		SendMessage(hButton, BM_SETIMAGE, 0, (LPARAM)item4);
		hButton = GetDlgItem(hDlg, IDC_super);
		SendMessage(hButton, BM_SETIMAGE, 0, (LPARAM)item5);


		wsprintf(pointtext, "%d", point);
		SetDlgItemText(hDlg, IDC_point, pointtext);
		return true;

	case WM_CREATE:
		pointbuy = 0;
		for (int a = 0; a < 4; a++)
			saveitem[a] = '\0';
		break;


	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_attack:
			itemselect = LoadBitmap(hIns, MAKEINTRESOURCE(IDB_BITMAP13));
			hButton = GetDlgItem(hDlg, IDC_select);
			SendMessage(hButton, BM_SETIMAGE, 0, (LPARAM)itemselect);
			SetDlgItemText(hDlg, IDC_explan, "�� �������� ���ݷ��� �����ϴ� �������Դϴ�.");
			SetDlgItemText(hDlg, IDC_itembuypoint, "�Ҹ�Ǵ� ����Ʈ : 150");
			cho = 1;
			break;

		case IDC_shield:
			itemselect = LoadBitmap(hIns, MAKEINTRESOURCE(IDB_BITMAP15));
			hButton = GetDlgItem(hDlg, IDC_select);
			SendMessage(hButton, BM_SETIMAGE, 0, (LPARAM)itemselect);
			SetDlgItemText(hDlg, IDC_explan, "�� �������� �������� �� �Ǵ� �������Դϴ�.");
			SetDlgItemText(hDlg, IDC_itembuypoint, "�Ҹ�Ǵ� ����Ʈ : 80");
			cho = 2;
			break;

		case IDC_hpup:
			itemselect = LoadBitmap(hIns, MAKEINTRESOURCE(IDB_BITMAP28));
			hButton = GetDlgItem(hDlg, IDC_select);
			SendMessage(hButton, BM_SETIMAGE, 0, (LPARAM)itemselect);
			SetDlgItemText(hDlg, IDC_explan, "�� �������� ������ ä��� �������Դϴ�.");
			SetDlgItemText(hDlg, IDC_itembuypoint, "�Ҹ�Ǵ� ����Ʈ : 200");
			cho = 3;
			break;

		case IDC_super:
			itemselect = LoadBitmap(hIns, MAKEINTRESOURCE(IDB_BITMAP17));
			hButton = GetDlgItem(hDlg, IDC_select);
			SendMessage(hButton, BM_SETIMAGE, 0, (LPARAM)itemselect);
			SetDlgItemText(hDlg, IDC_explan, "�� �������� �ʻ�� �����Դϴ�.");
			SetDlgItemText(hDlg, IDC_itembuypoint, "�Ҹ�Ǵ� ����Ʈ : 300");
			cho = 4;
			break;

		case IDC_select:
			PlaySound("Sound/�����۴�����.wav", NULL, SND_ASYNC);
			if (cho == 0)
			{
				MessageBox(hDlg, "���õ� �������� �����ϴ�.", "���źҰ�!   ", MB_OK);
				break;
			}
			chocount++;
			if (chocount <= 6)
			{
				if (cho == 1)
				{
					if (missilecnt >= 3)
					{
						MessageBox(hDlg, "���ݷ� ���� �������� 3�ܰ� ���׷��̵� ���� �����մϴ�", "���� �Ұ�!", MB_OK);
						chocount--;
					}
					else
					{
						strcpy_s(itemlist[chocount], "���ݷ� ����! : 150����Ʈ");
						SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)itemlist[chocount]);
						pointbuy += 150;
						saveitem[0] += 1;
						wsprintf(buypointtext, "%d", pointbuy);
						SetDlgItemText(hDlg, IDC_buypoint, buypointtext);
						missilecnt++;
					}
				}
				else if (cho == 2)
				{
					strcpy_s(itemlist[chocount], "����! : 80����Ʈ");
					SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)itemlist[chocount]);
					pointbuy += 80;
					saveitem[1] += 1;
					wsprintf(buypointtext, "%d", pointbuy);
					SetDlgItemText(hDlg, IDC_buypoint, buypointtext);
				}
				else if (cho == 3)
				{
					strcpy_s(itemlist[chocount], "����! : 200����Ʈ");
					SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)itemlist[chocount]);
					pointbuy += 200;
					saveitem[2] += 1;
					wsprintf(buypointtext, "%d", pointbuy);
					SetDlgItemText(hDlg, IDC_buypoint, buypointtext);
				}
				else if (cho == 4)
				{
					strcpy_s(itemlist[chocount], "�ʻ��! : 300����Ʈ");
					SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)itemlist[chocount]);
					pointbuy += 300;
					saveitem[3] += 1;
					wsprintf(buypointtext, "%d", pointbuy);
					SetDlgItemText(hDlg, IDC_buypoint, buypointtext);
				}
			}
			else
				MessageBox(hDlg, "�ִ� 6������ ���ð��� �մϴ�. �����Ͻ� �� �̿��� �ּ���", "������ �߰� �Ұ�", MB_OK);
			break;

		case IDC_buy:
			if (pointbuy == 0)
			{
				MessageBox(hDlg, "���� �� �������� �����ϴ�.", "���źҰ�!   ", MB_OK);
				break;
			}
			wsprintf(buytext, "%d ����Ʈ�� �Ҹ� �˴ϴ�.", pointbuy);
			answer = MessageBox(hDlg, buytext, "���� �Ͻðڽ��ϱ�?", MB_OKCANCEL);
			if (answer == IDOK)
			{
				if (point >= pointbuy)
				{
					PlaySound("Sound/�����ۻ�����.wav", NULL, SND_ASYNC);
					MessageBox(hDlg, "���ſϷ�!", "���ſϷ�!   ", MB_OK);
					point -= pointbuy;
					for (int a = 0; a < 4; a++)
						item[a] += saveitem[a];
					wsprintf(pointtext, "%d", point);
					SetDlgItemText(hDlg, IDC_point, pointtext);
					pointbuy = 0;
					wsprintf(buypointtext, "%d", pointbuy);
					SetDlgItemText(hDlg, IDC_buypoint, buypointtext);
					for (int i = 0; i < chocount; i++)
						SendMessage(hList, LB_DELETESTRING, 0, (LPARAM)itemlist[i]);
					for (int a = 0; a < 6; a++)
						for (int b = 0; b < 40; b++)
						{
							itemlist[a][b] = '\0';
						}
					for (int a = 0; a < 4; a++)
						saveitem[a] = '\0';
					chocount = 0;
				}
				else
				{
					MessageBox(hDlg, "���� �� �� �����ϴ�.\n���Ÿ���� ���µ˴ϴ�.", "����Ʈ ����   ", MB_OK);
					pointbuy = 0;

					wsprintf(buypointtext, "%d", pointbuy);
					SetDlgItemText(hDlg, IDC_buypoint, buypointtext);
					for (int i = 0; i < chocount; i++)
						SendMessage(hList, LB_DELETESTRING, 0, (LPARAM)itemlist[i]);

					for (int a = 0; a < 6; a++)
						for (int b = 0; b < 40; b++)
						{
							itemlist[a][b] = '\0';
						}
					for (int a = 0; a < 4; a++)
						saveitem[a] = '\0';
					chocount = 0;
				}
			}
			else
			{
				MessageBox(hDlg, "���Ÿ���� ���µ˴ϴ�.", "���� ���   ", MB_OK);
				pointbuy = 0;
				wsprintf(buypointtext, "%d", pointbuy);
				SetDlgItemText(hDlg, IDC_buypoint, buypointtext);
				for (int i = 0; i < chocount; i++)
					SendMessage(hList, LB_DELETESTRING, 0, (LPARAM)itemlist[i]);

				for (int a = 0; a < 6; a++)
					for (int b = 0; b < 40; b++)
					{
						itemlist[a][b] = '\0';
					}
				for (int a = 0; a < 4; a++)
					saveitem[a] = '\0';
				chocount = 0;
			}
			break;

		case IDC_exit:
			EndDialog(hDlg, 0);
			pointbuy = 0;
			shop = false;
			scene->shop = false;
			wsprintf(buypointtext, "%d", pointbuy);
			SetDlgItemText(hDlg, IDC_buypoint, buypointtext);
			for (int i = 0; i < chocount; i++)
				SendMessage(hList, LB_DELETESTRING, 0, (LPARAM)itemlist[i]);

			for (int a = 0; a < 6; a++)
				for (int b = 0; b < 40; b++)
				{
					itemlist[a][b] = '\0';
				}
			chocount = 0;
			cho = 0;
			break;
		}
		InvalidateRect(hDlg, &rect, true);
		break;

	case WM_PAINT:
		d_hdc = BeginPaint(hDlg, &d_ps);

		EndPaint(hDlg, &d_ps);
		break;
	}
	return false;
}