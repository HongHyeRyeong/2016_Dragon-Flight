#include "Object_Player.h"
#include "Object_missile.h"

void Player::init(HDC memDC, HDC backDC, HINSTANCE hIns)
{
	resource = LoadBitmap(hIns, MAKEINTRESOURCE(IDB_BITMAP5));
	mask = LoadBitmap(hIns, MAKEINTRESOURCE(IDB_BITMAP12));
	this->memDC = memDC;
	this->backDC = backDC;
	GetObject(resource, sizeof(BITMAP), &bmp);
	x = SCREEN_WIDTH / 2 - 50;
	y = 600;
	w = bmp.bmWidth;
	h = bmp.bmHeight;
	frame = 0;
	state = FLY;
	speed = 300.f;
	frame_t = 0.f;
	isDead = false;
	collisionRC = { (LONG)x, (LONG)y, (LONG)(x + (w / 4)), (LONG)(y + h) };

}

void Player::update(double time)
{
	frame_t += time;

	if (frame_t > 0.15f)
	{
		frame_t = 0;
		frame++;
		frame %= 4;
	}
	collisionRC = { (LONG)x, (LONG)y, (LONG)(x + (w / 4)), (LONG)(y + h) };

	if (GetAsyncKeyState(VK_RIGHT) && x + (w / 4) < SCREEN_WIDTH)
		x += speed * time;
	if (GetAsyncKeyState(VK_LEFT) && x > 0)
		x -= speed * time;
	if (GetAsyncKeyState(VK_UP) && y > 270)
		y -= speed * time;
	if (GetAsyncKeyState(VK_DOWN) && y + h < SCREEN_HEIGHT - 20)
		y += speed * time;

	if (GetAsyncKeyState('Z') && state != ATTACK)
	{
		state = ATTACK;

	}
	else if (!GetAsyncKeyState('Z'))
		state = FLY;
}

void Player::draw()
{
	SelectObject(memDC, mask);
	BitBlt(backDC, x, y, w / 4, h, memDC, w / 4 * frame, 0, SRCAND);
	SelectObject(memDC, resource);
	BitBlt(backDC, x, y, w / 4, h, memDC, w / 4 * frame, 0, SRCPAINT);
}
