#include "Class_main.h"

// функции объекта игрока
player::player(int x, int y)
{
	this->x = x;
	this->y = y;
}
void player::move(int offsetX, int offsetY) 
{
	x += offsetX;
	y += offsetY;
}
void player::show(HDC hdc, int key) 
{
	HPEN pen = CreatePen(PS_SOLID, weightLine * 2 + 1, RGB(0, 0, 0));
	SelectObject(hdc, pen);

	// "человечек"
	Ellipse(hdc, 0 + x + weightLine, 0 + y + weightLine, 36 + x + weightLine, 36 + y + weightLine);
	MoveToEx(hdc, 18 + x + weightLine, 0 + y + weightLine, NULL);
	LineTo(hdc, 46 + x + weightLine, 0 + y + weightLine);
	MoveToEx(hdc, 18 + x + weightLine, 36 + y + weightLine, NULL);
	LineTo(hdc, 46 + x + weightLine, 36 + y + weightLine);

	SelectObject(hdc, GetStockObject(BLACK_PEN));
	DeleteObject(pen);
}
void player::onKeyDown(WPARAM wParam) {
	int step = 6; // стандарт - 6
	switch (wParam) {
	case 'W':
		move(0, -step);
		break;
	case 'S':
		move(0, step);
		break;
	case 'A':
		move(-step, 0);
		break;
	case 'D':
		move(step, 0);
		break;
	}
}
int player::hit(int xl_2, int yup_2, int xr_2, int ydown_2)
{
	// инциализация координат
	int xl_1 = x, xr_1 = x + 46 + weightLine, yup_1 = y, ydown_1 = y + 36;
	// сравнение координат
	if (!(xr_1 < xl_2 || ydown_1 < yup_2 || xl_1 > xr_2 || yup_1 > ydown_2)) {
		return 1;
	}
	return 0;
}

// функции объекта "npc"
object::object(int x, int y)
{
	this->x = x;
	this->y = y;
}
void object::move(int offsetX, int offsetY) 
{
	x += offsetX;
	y += offsetY;
}
void object::show(HDC hdc, int key)
{
	HPEN pen1 = CreatePen(PS_SOLID, weightLine * 2 + 1, RGB(139, 69, 19));
	HPEN pen2 = CreatePen(PS_SOLID, weightLine * 2 + 1, RGB(255, 179, 0));
	SelectObject(hdc, pen1);

	// закрашенная часть коробки
	RECT r;
	r.left = 0 + x + weightLine; //левый верхний угол
	r.top = 20 + y + weightLine;
	r.right = 80 + x + weightLine; //правый нижний
	r.bottom = 50 + y + weightLine;
	FillRect(hdc, &r, (HBRUSH)CreateSolidBrush(RGB(250, 200, 100)));

	// корпус
	MoveToEx(hdc, 0 + x + weightLine, 0 + y + weightLine, NULL);
	LineTo(hdc, 0 + x + weightLine, 50 + y + weightLine);
	LineTo(hdc, 80 + x + weightLine, 50 + y + weightLine);
	LineTo(hdc, 80 + x + weightLine, 20 + y + weightLine);
	LineTo(hdc, 0 + x + weightLine, 20 + y + weightLine);
	MoveToEx(hdc, 0 + x + weightLine, 0 + y + weightLine, NULL);
	LineTo(hdc, 80 + x + weightLine, 0 + y + weightLine);
	LineTo(hdc, 80 + x + weightLine, 20 + y + weightLine);

	// замок
	SelectObject(hdc, pen2);
	MoveToEx(hdc, 39 + x + weightLine, 8 + y + weightLine, NULL);
	LineTo(hdc, 39 + x + weightLine, 11 + y + weightLine);

	SelectObject(hdc, GetStockObject(BLACK_PEN));
	DeleteObject(pen1);
	DeleteObject(pen2);
}
