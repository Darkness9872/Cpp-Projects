#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <windows.h>
#include <ctime>

#include "Class_main.h"

// скорость объекта, который убегает
int speed = 2; // стандарт - 2

// создание объектов-участников и глобальных переменных
int key = 0;
player player_1(0, 0);
object object_1(500, 500); // max - (1400,700) с полным экраном

// для движения объекта, который надо догонять
DWORD WINAPI MoveObjectThread(LPVOID lpParameter)
{
	int dx, dy;
	srand(time(0));
	// случайное начальное направление
	dx = rand() % (2 * speed + 1) - speed;
	dy = rand() % (2 * speed + 1) - speed;
	while (key == 0) {
		object_1.move(dx, dy);

		// смена направления при столкновении со стеной
		if (object_1.x < 0) {
			object_1.x = 0;
			dx = rand() % (speed + 1);
			dy = rand() % (2 * speed + 1) - speed;
		}
		if (object_1.x > 1400) {
			object_1.x = 1400;
			dx = rand() % (0 + speed + 1) - speed;
			dy = rand() % (2 * speed + 1) - speed;
		}
		if (object_1.y < 0) {
			object_1.y = 0;
			dy = rand() % (speed + 1);
			dx = rand() % (2 * speed + 1) - speed;
		}
		if (object_1.y > 700) {
			object_1.y = 700;
			dy = rand() % (0 + speed + 1) - speed;
			dx = rand() % (2 * speed + 1) - speed;
		}
		// чтобы не останавливался когда скорость по случайности станет = 0
		if (dx == 0) {
			dx = rand() % (2 * speed + 1) - speed;
		}
		if (dy == 0) {
			dy = rand() % (2 * speed + 1) - speed;
		}
		// регулирует раз в какое время объект производит движение
		Sleep(10);
	}
	return 0;
}

// таймер, в котором отрисовываются объекты и проверяется наличие столкновения
VOID CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	// проверяем наличие столкновения
	int weightLine = 2;
	key = player_1.hit(0 + object_1.x + weightLine, 0 + object_1.y + weightLine, 80 + object_1.x + weightLine, 50 + object_1.y + weightLine);

	// Получаем контекст устройства
	HDC hdc = GetDC(hwnd);

	// чистим экран
	RECT clientRect;
	GetClientRect(hwnd, &clientRect);

	HBRUSH brush = CreateSolidBrush(RGB(100, 170, 0));
	FillRect(hdc, &clientRect, brush);
	DeleteObject(brush);

	// рисуем  в соответствии с необходимым
	if (key == 0) {
		// Отрисовываем объекты
		object_1.show(hdc, key);
		player_1.show(hdc, key);
	}
	else {
		TextOutA(hdc, 300, 400, "Вы выиграли!", 12);
	}

	// Освобождаем контекст устройства
	ReleaseDC(hwnd, hdc);

}

static TCHAR szWindowClass[] = _T("DesktopApp");

static TCHAR szTitle[] = _T("Преследование блуждающего объекта на биллиардном столе");

HINSTANCE hInst;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(wcex.hInstance, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = CreateSolidBrush(RGB( 100, 170, 0));
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

	if (!RegisterClassEx(&wcex)) {
		MessageBox(NULL, _T("Call to RegisterClassEx failed!"), szTitle, NULL);
		return 1;
	}

	hInst = hInstance;

	HWND hWnd = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, szWindowClass, szTitle,
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		800, 600, NULL, NULL, hInstance, NULL);

	if (!hWnd) {
		MessageBox(NULL, _T("Call to CreateWindow failed!"), szTitle, NULL);
		return 1;
	}

	ShowWindow(hWnd, nCmdShow);

	// Запускаем таймер
	SetTimer(hWnd, 1, 1000 / 60, (TIMERPROC)TimerProc); // Здесь ставим интервал в 1/60 секунды
	// Создаем вторичный поток для перемещения объекта
	HANDLE hMoveThread = CreateThread(NULL, 0, MoveObjectThread, NULL, 0, NULL);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR greeting[] = _T("Hello, Windows desktop!");

	switch (message) {
	case WM_KEYDOWN:
	{
		// движение объекта-игрока по нажатию клавиш
		if (key == 0) {
		player_1.onKeyDown(wParam);
		}
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return 0;
}
