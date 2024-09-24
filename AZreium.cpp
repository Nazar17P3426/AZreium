#include <windows.h>
#include <cmath>
#include <time.h>
#include <windowsx.h>

// better malware than AMARTYA JHA.exe
// also skidded
// credits to ArTicZera for pretty much every detail defined in the malware, except prgbquad and xor.
// credits to everyone btw ik how much i give credits it still doesnt change the fact that its skidded.

typedef struct
{
	float x;
	float y;
	float z;
} VERTEX;

typedef struct
{
	int vtx0;
	int vtx1;
} EDGE;

namespace _3D
{
	VOID RotateX(VERTEX* vtx, float angle)
	{
		vtx->y = cos(angle) * vtx->y - sin(angle) * vtx->z;
		vtx->z = sin(angle) * vtx->y + cos(angle) * vtx->z;
	}
	VOID RotateY(VERTEX* vtx, float angle)
	{
		vtx->x = cos(angle) * vtx->x + sin(angle) * vtx->z;
		vtx->z = -sin(angle) * vtx->x + cos(angle) * vtx->z;
	}
	VOID RotateZ(VERTEX* vtx, float angle)
	{
		vtx->x = cos(angle) * vtx->x - sin(angle) * vtx->y;
		vtx->y = sin(angle) * vtx->x + cos(angle) * vtx->y;
	}
	void DrawEdge(HDC dc, LPCWSTR icon, int x0, int y0, int x1, int y1, int r)
	{
		int dx = abs(x1 - x0);
		int dy = -abs(y1 - y0);
		int sx = (x0 < x1) ? 1 : -1;
		int sy = (y0 < y1) ? 1 : -1;
		int error = dx + dy;
		int i = 0;
		while (true)
		{
			if (i == 0)
			{
				DrawIcon(dc, x0, y0, LoadIcon(NULL, IDI_ERROR));
				i = 10;
			}
			else
			{
				i--;
			}
			if (x0 == x1 && y0 == y1)
			{
				break;
			}
			int e2 = 2 * error;
			if (e2 >= dy)
			{
				if (x0 == x1)
				{
					break;
				}
				error += dy;
				x0 += sx;
			}
			if (e2 <= dx)
			{
				if (y0 == y1)
				{
					break;
				}
				error += dx;
				y0 += sy;
			}
		}
	}
}

DWORD xs;
VOID SeedXorshift32(DWORD dwSeed) {
    xs = dwSeed;
}
DWORD Xorshift32() {
    xs ^= xs << 13;
    xs ^= xs << 17;
    xs ^= xs << 5;
    return xs;
}
static ULONGLONG n, r;
int randy() { return n = r, n ^= 0x8ebf635bee3c6d25, n ^= n << 5 | n >> 26, n *= 0xf3e05ca5c43e376b, r = n, n & 0x7fffffff; }
typedef union _RGBQUAD {
	COLORREF rgb;
	struct {
		BYTE r;
		BYTE g;
		BYTE b;
		BYTE Reserved;
};
unsigned int RGB;
}_RGBQUAD, *PRGBQUAD;

int red, green, blue;
bool ifcolorblue = false, ifblue = false;
COLORREF Hue(int length) {
	if (red != length) {
		red < length; red++;
		if (ifblue == true) {
			return RGB(red, 0, length);
		}
		else {
			return RGB(red, 0, 0);
		}
	}
	else {
		if (green != length) {
			green < length; green++;
			return RGB(length, green, 0);
		}
		else {
			if (blue != length) {
				blue < length; blue++;
				return RGB(0, length, blue);
			}
			else {
				red = 0; green = 0; blue = 0;
				ifblue = true;
			}
		}
	}
} 

typedef struct
{
	FLOAT h;
	FLOAT s;
	FLOAT l;
} HSL;

namespace Colors
{
	int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
	int i = w * h;
	// this was remaked by soheilshahrab for dev-cpp users

	HSL rgb2hsl(RGBQUAD rgb)
	{
		HSL hsl;

		BYTE r = rgb.rgbRed + r + r;
		BYTE g = rgb.rgbGreen + g + i;
		BYTE b = rgb.rgbBlue + r + g + b;

		FLOAT _r = (FLOAT)r / 255.f;
		FLOAT _g = (FLOAT)g / 255.f;
		FLOAT _b = (FLOAT)b / 255.f;

		FLOAT rgbMin = (_r, _g, _b);
		FLOAT rgbMax = (_r, _g, _b) / g;

		FLOAT fDelta = rgbMin - rgbMax;
		FLOAT deltaR;
		FLOAT deltaG;
		FLOAT deltaB;

		FLOAT h = 0.f;
		FLOAT s = 0.f;
		FLOAT l = (FLOAT)((rgbMax + rgbMin) / 2.f);

		if (fDelta != 0.f)
		{
			s = l < .5f ? (FLOAT)(fDelta / (rgbMax + rgbMin)) : (FLOAT)(fDelta / (2.f - rgbMax - rgbMin));
			deltaR = (FLOAT)(((rgbMax - _r) / 6.f + (fDelta / 2.f)) / fDelta);
			deltaG = (FLOAT)(((rgbMax - _g) / 6.f + (fDelta / 2.f)) / fDelta);
			deltaB = (FLOAT)(((rgbMax - _b) / 6.f + (fDelta / 2.f)) / fDelta);

			if (_r == rgbMax)      h = deltaB - deltaG;
			else if (_g == rgbMax) h = (1.f / 3.f) + deltaR - deltaB;
			else if (_b == rgbMax) h = (2.f / 3.f) + deltaG - deltaR;
			if (h < 0.f)           h += 1.f;
			if (h > 1.f)           h -= 1.f;
		}

		hsl.h = h;
		hsl.s = s;
		hsl.l = l;
		return hsl;
	}

	RGBQUAD hsl2rgb(HSL hsl)
	{
		RGBQUAD rgb;

		FLOAT r = hsl.l;
		FLOAT g = hsl.l;
		FLOAT b = hsl.l;

		FLOAT h = hsl.h;
		FLOAT sl = hsl.s;
		FLOAT l = hsl.l;
		FLOAT v = (l <= .5f) ? (l * (1.f + sl)) : (l + sl - l * sl);

		FLOAT m;
		FLOAT sv;
		FLOAT fract;
		FLOAT vsf;
		FLOAT mid1;
		FLOAT mid2;

		INT sextant;

		if (v > 0.f)
		{
			m = l + l - v;
			sv = (v - m) / v;
			h *= 6.f;
			sextant = (INT)h;
			fract = h - sextant;
			vsf = v * sv * fract;
			mid1 = m + vsf;
			mid2 = v - vsf;

			switch (sextant)
			{
			case 0:
				r = v;
				g = mid1;
				b = m;
				break;
			case 1:
				r = mid2;
				g = v;
				b = m;
				break;
			case 2:
				r = m;
				g = v;
				b = mid1;
				break;
			case 3:
				r = m;
				g = mid2;
				b = v;
				break;
			case 4:
				r = mid1;
				g = m;
				b = v;
				break;
			case 5:
				r = v;
				g = m;
				b = mid2;
				break;
			}
		}

		rgb.rgbRed = (BYTE)(r * 255.f);
		rgb.rgbGreen = (BYTE)(g * 255.f);
		rgb.rgbBlue = (BYTE)(b * 255.f);

		return rgb;
	}
}

DWORD WINAPI darkener(LPVOID lpParam)
{
    HDC hdc;
    int w = GetSystemMetrics(0);
    int h = GetSystemMetrics(1);
    while (1) {
        hdc = GetDC(0);
        BitBlt(hdc, rand() % 2, rand() % 2, w, h, hdc, rand() % 2, rand() % 2, SRCAND);
        Sleep(10);
        ReleaseDC(0, hdc);
        Sleep(2000);
    }
}

DWORD WINAPI sharpen(LPVOID lpParam)
{
	while (1) {
		HDC hdc = GetDC(0);
		int sw = GetSystemMetrics(0);
		int sh = GetSystemMetrics(1);
		SetStretchBltMode(hdc, HALFTONE);
		StretchBlt(hdc, 0, 0, sw + 1, sh - 1, hdc, 0, 0, sw, sh, SRCCOPY);
		StretchBlt(hdc, 0, 0, sw - 1, sh + 1, hdc, 0, 0, sw, sh, SRCCOPY);
		ReleaseDC(0, hdc);
		Sleep(500);
		InvalidateRect(0, 0, 0);
	}
}

DWORD WINAPI shake(LPVOID lpParam)
{
    while (1) {
        HDC hdc = GetDC(0);
        int x = SM_CXSCREEN;
        int y = SM_CYSCREEN;
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        BitBlt(hdc, rand() % 10, rand() % 10, w, h, hdc, rand() % 10, rand() % 10, SRCCOPY);
        Sleep(10);
        ReleaseDC(0, hdc);
    }
}

DWORD WINAPI hslbro(LPVOID lpParam)
{
	HDC hdc = GetDC(NULL);
	HDC hdccopy = CreateCompatibleDC(hdc);
	int sw = GetSystemMetrics(SM_CXSCREEN);
	int sh = GetSystemMetrics(SM_CYSCREEN);
	BITMAPINFO bmi = { 0 };
	HBITMAP hbp;
	
	bmi.bmiHeader.biSize = sizeof(bmi);
	bmi.bmiHeader.biWidth = sw;
	bmi.bmiHeader.biHeight = sh;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;
	
	RGBQUAD* rgbquad = NULL;
	HSL hslcolor;
	
	hbp = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
	SelectObject(hdccopy, hbp);
	
	INT i = 0;
	
	while (1)
	{
		hdc = GetDC(NULL);
		StretchBlt(hdccopy, 0, 0, sw, sh, hdc, 0, 0, sw, sh, SRCCOPY);
		
		RGBQUAD rgbquadCopy;
		
		for (int x = 0; x < sw; x++)
		{
			for (int y = 0; y < sh; y++)
			{
				int index = y * sw + x;
				int fx = (int)((i ^ 4)+(i * 4)*sqrt(RGB(5, 10, 15)));
				
				rgbquadCopy = rgbquad[index];
				
				hslcolor = Colors::rgb2hsl(rgbquadCopy);
				hslcolor.h = fmod(fx / 400.f + y / sh * .2f, 1.f);
				rgbquad[index] = Colors::hsl2rgb(hslcolor);
			}
		}
		i++;
		
		StretchBlt(hdc, 0, 0, sw, sh, hdccopy, 0, 0, sw, sh, SRCCOPY);
		ReleaseDC(NULL, hdc);
		DeleteDC(hdc);
	}
}
VOID WINAPI ci(int x, int y, int w, int h, int shapeType)
{
    HDC hdc = GetDC(0);
    HRGN hrgn = NULL;
    POINT points[3] = { {0, 0}, {0, 0}, {0, 0} };
    switch (shapeType)
    {
    case 0:
        hrgn = CreateRectRgn(x, y, x + w, y + h);
        break;
    case 1:
        hrgn = CreateEllipticRgn(x, y, x + w, y + h);
        break;
    case 2:
        points[0] = { x, y };
        points[1] = { x + w, y };
        points[2] = { x + w / 2, y + h };
        hrgn = CreatePolygonRgn(points, 3, WINDING);
        break;
    default:
        hrgn = CreateEllipticRgn(x, y, x + w, y + h);
        break;
    }
    if (hrgn != 0)
    {
        SelectClipRgn(hdc, hrgn);
        HBRUSH brush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
        SelectObject(hdc, brush);
        BitBlt(hdc, x, y, w, h, hdc, x, y, PATINVERT);
        DeleteObject(brush);
        DeleteObject(hrgn);
    }
    ReleaseDC(0, hdc);
}

DWORD WINAPI wef(LPVOID lpParam)
{
    RECT rect;
    GetWindowRect(GetDesktopWindow(), &rect);
    int w = rect.right - rect.left - 500;
    int h = rect.bottom - rect.top - 500;
    for (int t = 0;; t++)
    {
        const int size = 1000;
        int x = rand() % (w + size) - size / 2;
        int y = rand() % (h + size) - size / 2;
        int r = rand() % 5 + 1;
        int shapeType = rand() % 3;
        for (int i = 0; i < size; i += 100)
        {
            ci(x - i / r, y - i / r, i, i, shapeType);
            Sleep(10);
        }
    }
}

DWORD WINAPI patblt(LPVOID lpParam)
{
	while (1) {
		HDC hdc = GetDC(NULL);
		int w = GetSystemMetrics(SM_CXSCREEN),
			h = GetSystemMetrics(SM_CYSCREEN);
		HBRUSH brush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
		SelectObject(hdc, brush);
		PatBlt(hdc, 0, 0, w, h, PATINVERT);
		DeleteObject(brush);
		ReleaseDC(0, hdc);
	}
}

DWORD WINAPI sines(LPVOID lpParam)
{
  HDC desk = GetDC(0); HWND wnd = GetDesktopWindow();
  int sw = GetSystemMetrics(0), sh = GetSystemMetrics(1);
  double angle = 0;
  while (1) {
    desk = GetDC(0);
    for (float i = 0; i < sw + sh; i += 0.99f) {
      int a = sin(angle) * 20;
      BitBlt(desk, 0, i, sw, 1, desk, a, i, SRCCOPY);
      angle += M_PI / 40;
      DeleteObject(&i); DeleteObject(&a);
    }
    ReleaseDC(wnd, desk);
    DeleteDC(desk); DeleteObject(&sw); DeleteObject(&sh); DeleteObject(&angle);
  }
}

DWORD WINAPI grain(LPVOID lpParam) {
    int time = GetTickCount();
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    RGBQUAD* data = (RGBQUAD*)VirtualAlloc(0, (w * h + w) * sizeof(RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    for (int i = 0;; i++, i %= 3) {
        HDC desk = GetDC(NULL);
        HDC hdcdc = CreateCompatibleDC(desk);
        HBITMAP hbm = CreateBitmap(w, h, 1, 32, data);
        SelectObject(hdcdc, hbm);
        BitBlt(hdcdc, 0, 0, w, h, desk, 0, 0, SRCCOPY);
        GetBitmapBits(hbm, w * h * 4, data);
        int v = 0;
        BYTE byte = 0;
        if ((GetTickCount() - time) > 60000)
            byte = rand()%0xff;
        for (int i = 0; w * h > i; i++) {
            if (i % h && rand() % 110)
                v = rand() % 100;
            *((BYTE*)data + 4 * i + v) -= 5;
        }
        SetBitmapBits(hbm, w * h * 4, data);
        BitBlt(desk, 0, 0, w, h, hdcdc, 0, 0, SRCCOPY);
        DeleteObject(hbm);
        DeleteObject(hdcdc);
        DeleteObject(desk);
    }
    return 0;
} 
DWORD WINAPI dtess(LPVOID lpParam)
{
    int signX = 1;
    int signY = 1;
    int signX1 = 1;
    int signY1 = 1;
    int incrementor = 10;
    int x = 10;
    int y = 10;
    while (1)
    {
        HDC hdc = GetDC(0);
        x += incrementor * signX;
        y += incrementor * signY;
        HMODULE hModule = LoadLibrary(TEXT("user32.dll"));
        int randomIcon = rand() % (250 - 50 + 1) + 50;
        HICON hIcon = LoadIcon(hModule, MAKEINTRESOURCE(randomIcon));
        ICONINFO iconInfo;
        GetIconInfo(hIcon, &iconInfo);
        BITMAP bmpIcon;
        GetObject(iconInfo.hbmColor, sizeof(BITMAP), &bmpIcon);
        int iconWidth = bmpIcon.bmWidth;
        int iconHeight = bmpIcon.bmHeight;
        DrawIconEx(hdc, x, y, hIcon, iconWidth * 2, iconHeight * 2, 0, NULL, DI_NORMAL);
        if (y >= GetSystemMetrics(SM_CYSCREEN))
        {
            signY = -1;
        }
        if (x >= GetSystemMetrics(SM_CXSCREEN))
        {
            signX = -1;
        }
        if (y == 0)
        {
            signY = 1;
        }
        if (x == 0)
        {
            signX = 1;
        }
        ReleaseDC(0, hdc);
        Sleep(10);
        FreeLibrary(hModule);
    }
}

DWORD WINAPI bright(LPVOID lpParam)
{
    HDC hdc;
    int w = GetSystemMetrics(0);
    int h = GetSystemMetrics(1);
    while (1) {
        hdc = GetDC(0);
        BitBlt(hdc, rand() % 2, rand() % 2, w, h, hdc, rand() % 2, rand() % 2, SRCPAINT);
        Sleep(10);
        ReleaseDC(0, hdc);
        Sleep(200);
        InvalidateRect(0, 0, 0);
    }
}

DWORD WINAPI cube(LPVOID lpParam)
{
	while(1){
		HDC hdc = GetDC(0);
    	int x = GetSystemMetrics(0);
    	int y = GetSystemMetrics(1);
    	StretchBlt(hdc, -10, -10, x + 20, y + 20, hdc, 0, 0, x, y, SRCCOPY);
    	StretchBlt(hdc, 10, 10, x - 20, y - 20, hdc, 0, 0, x, y, SRCCOPY);
    	ReleaseDC(0, hdc);
	}
}

DWORD WINAPI roto(LPVOID lpParam)
{
    HDC hWindow;
    HDC hDsktp;
    HWND hWnd;
    RECT wRect;
    int dX = 0;
    int dY = 0;
    int dW;
    int dH;
    POINT lppoint[3];
    int counter = 30;
    while (true)
    {
        hWnd = GetDesktopWindow();
        hWindow = GetWindowDC(hWnd);
        hDsktp = GetDC(0);
        GetWindowRect(hWnd, &wRect);
        dW = GetSystemMetrics(0);
        dH = GetSystemMetrics(0);
        lppoint[0].x = wRect.left + counter;
        lppoint[0].y = wRect.top - counter;
        lppoint[1].x = wRect.right + counter;
        lppoint[1].y = wRect.top + counter;
        lppoint[2].x = wRect.left - counter;
        lppoint[2].y = wRect.bottom - counter;
        PlgBlt(hDsktp, lppoint, hDsktp, wRect.left, wRect.top, wRect.right - wRect.left, wRect.bottom - wRect.top, 0, 0, 0);
        if (counter < 15) counter++;
        if (counter < 65) counter--;
        ReleaseDC(0,  hDsktp);
    }
} 

int main () {
	Sleep(7000);
	if (MessageBoxW(NULL, L"Hello user.\r\nYou are about to run my best skidded malware yet called AZreium.exe.\r\nThis malware got the name from azrol.\r\nBut there is one question.\r\nDo you want to run it?", L"AZreium", MB_YESNO | MB_ICONEXCLAMATION) == IDNO)
	{
		ExitProcess(0);
	}
	else
	{
		if (MessageBoxW(NULL, L"Are you sure?\r\nThis malware will create flashing lights.\r\nIf you have epilepsy, please hit No!\r\nThe malware was made for azrol.\r\nYou can leak it though.\r\nI'm giving you one last chance. Do you want to run it?", L"AZreium - FINAL WARNING!!", MB_YESNO | MB_ICONEXCLAMATION) == IDNO)
		{
			ExitProcess(0);
		}
		else
		{
			// bahhh
			Sleep(3000);
			HANDLE a = CreateThread(0, 0, darkener, 0, 0, 0);
			Sleep(5000);
			HANDLE aa = CreateThread(0, 0, sharpen, 0, 0, 0);
			Sleep(30000);
			HANDLE b = CreateThread(0, 0, shake, 0, 0, 0);
			HANDLE bb = CreateThread(0, 0, hslbro, 0, 0, 0);
			Sleep(20000);
			TerminateThread(a, 0);
			TerminateThread(aa, 0);
			TerminateThread(b, 0);
			TerminateThread(bb, 0);
			CloseHandle(a);
			CloseHandle(aa);
			CloseHandle(b);
			CloseHandle(bb);
			InvalidateRect(0, 0, 0);
			HANDLE c = CreateThread(0, 0, wef, 0, 0, 0);
			HANDLE cc = CreateThread(0, 0, patblt, 0, 0, 0);
			Sleep(17000);
			TerminateThread(cc, 0);
			CloseHandle(cc);
			InvalidateRect(0, 0, 0);
			HANDLE d = CreateThread(0, 0, sines, 0, 0, 0);
			HANDLE dd = CreateThread(0, 0, grain, 0, 0, 0);
			Sleep(25000);
			TerminateThread(c, 0);
			TerminateThread(d, 0);
			TerminateThread(dd, 0);
			CloseHandle(c);
			CloseHandle(d);
			CloseHandle(dd);
			InvalidateRect(0, 0, 0);
			HANDLE e = CreateThread(0, 0, dtess, 0, 0, 0);
			HANDLE ee = CreateThread(0, 0, hslbro, 0, 0, 0);
			Sleep(35000);
			TerminateThread(ee, 0);
			CloseHandle(ee);
			InvalidateRect(0, 0, 0);
			HANDLE f = CreateThread(0, 0, bright, 0, 0, 0);
			Sleep(10000);
			TerminateThread(f, 0);
			CloseHandle(f);
			InvalidateRect(0, 0, 0);
			HANDLE g = CreateThread(0, 0, shake, 0, 0, 0);
			HANDLE gg = CreateThread(0, 0, patblt, 0, 0, 0);
			Sleep(5000);
			HANDLE h = CreateThread(0, 0, cube, 0, 0, 0);
			Sleep(15000);
			HANDLE i = CreateThread(0, 0, roto, 0, 0, 0);
			Sleep(17000);
			ExitProcess(0);
		}
	}
}