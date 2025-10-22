#ifndef LR_OOP2_FIGURES
#define LR_OOP2_FIGURES

#include <windows.h>

class player;
class object;

class player 
{
protected:
	int weightLine = 2;
public:
	int x;
	int y;
	player(int x, int y);
	void move(int offsetX, int offsetY);
	void show(HDC hdc, int key);
	void onKeyDown(WPARAM wParam);
	int hit(int xl_2, int yup_2, int xr_2, int ydown_2);
};

class object 
{
protected:
	int weightLine = 2;
public:
	int x;
	int y;
	object(int x, int y);
	void show(HDC hdc, int key);
	void move(int offsetX, int offsetY);
};

#endif
