#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <iostream>
#include <list>
#include <vector>

#ifndef UNICODE
typedef std::string String;
#else
typedef std::wstring String;
#endif

using namespace std;

LPCWSTR szWindowClass = L"Klasa Okienka";
LPCWSTR okienkoTekst = L"Pytanie czy wyzwanie";

MSG Komunikat;
HWND pytaniePrzycisk;
HWND wyzwaniePrzycisk;
HWND pytanieWyswietlacz;
HWND wyzwanieWyswietlacz;
HWND pytanieOdpowiedz;
HWND pytaniePrzyciskAkceptacji;
HWND pytaniaRezultatWyœwietlacz;

//Kolekcje pytañ i wyzwañ
vector<string> pytania = { "pytanie 1", "pytanie 2", "pytanie 3" };
vector<string> odpowiedziNaPytania = { "a", "b", "c" };
vector<string> wyzwania = { "wyzwanie 1", "wyzwanie 2", "wyzwanie 3" };

//Liczniki potrzebne do zarz¹dzania które pytanie wyswietliæ nastêpnym razem
int pytaniaLicznik = pytania.size() - 1;
int wyzwaniaLicznik = wyzwania.size() - 1;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

//
//https://stackoverflow.com/a/27296/8365453
wstring s2ws(const string& s)
{
  int len;
  int slength = (int)s.length() + 1;
  len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
  wchar_t* buf = new wchar_t[len];
  MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
  wstring r(buf);
  delete[] buf;
  return r;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
  // WYPE£NIANIE STRUKTURY
  WNDCLASSEX wcex;

  wcex.cbSize = sizeof(WNDCLASSEX);
  wcex.style = CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc = WndProc;
  wcex.cbClsExtra = 0;
  wcex.cbWndExtra = 0;
  wcex.hInstance = hInstance;
  wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
  wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
  wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
  wcex.lpszMenuName = NULL;
  wcex.lpszClassName = szWindowClass;
  wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

  // REJESTROWANIE KLASY OKNA
  if (!RegisterClassEx(&wcex))
  {
    MessageBox(NULL,
      _T("Call to RegisterClassEx failed!"),
      _T("Win32 Guided Tour"),
      NULL);

    return 1;
  }

  // TWORZENIE OKNA
  HWND hwnd;

  hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, szWindowClass, okienkoTekst, WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, CW_USEDEFAULT, 960, 720, NULL, NULL, hInstance, NULL);

  if (hwnd == NULL)
  {
    MessageBox(NULL, L"Problem przy tworzeniu okienka g³ównego!", L"Spróbuj ponownie", MB_ICONEXCLAMATION);
    return 1;
  }
  pytaniePrzycisk = CreateWindowEx(0, L"BUTTON", L"Pytanie", WS_CHILD | WS_VISIBLE,
    100, 100, 150, 30, hwnd, NULL, hInstance, NULL);

  pytanieWyswietlacz = CreateWindowEx(0, L"STATIC", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 50, 150, 250, 200,
    hwnd, NULL, hInstance, NULL);

  HWND pytanieOdpowiedzLabel = CreateWindowEx(0, L"STATIC", L"Wpisz odpowiedŸ:", WS_CHILD | WS_VISIBLE, 50, 390, 100, 40,
    hwnd, NULL, hInstance, NULL);

  pytanieOdpowiedz = CreateWindowEx(0, L"EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 150, 400, 150, 20,
    hwnd, NULL, hInstance, NULL);

  pytaniePrzyciskAkceptacji = CreateWindowEx(0, L"BUTTON", L"SprawdŸ!", WS_CHILD | WS_VISIBLE,
    100, 440, 150, 20, hwnd, NULL, hInstance, NULL);

  pytaniaRezultatWyœwietlacz = CreateWindowEx(0, L"STATIC", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 100, 480, 150, 30,
    hwnd, NULL, hInstance, NULL);

  wyzwaniePrzycisk = CreateWindowEx(0, L"BUTTON", L"Wyzwanie", WS_CHILD | WS_VISIBLE,
    400, 100, 150, 30, hwnd, NULL, hInstance, NULL);

  wyzwanieWyswietlacz = CreateWindowEx(0, L"STATIC", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 350, 150, 250, 200,
    hwnd, NULL, hInstance, NULL);

  //HWND hText = CreateWindowEx(0, L"Pole", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 50, 50, 150, 20,
  //  hwnd, NULL, hInstance, NULL);

  ShowWindow(hwnd, nCmdShow); // Poka¿ okienko...
  UpdateWindow(hwnd);

  // Pêtla komunikatów
  while (GetMessage(&Komunikat, NULL, 0, 0))
  {
    TranslateMessage(&Komunikat);
    DispatchMessage(&Komunikat);
  }
  return Komunikat.wParam;
}

// OBS£UGA ZDARZEÑ
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  switch (msg)
  {
  case WM_CLOSE:
    DestroyWindow(hwnd);
    break;

  case WM_DESTROY:
    PostQuitMessage(0);
    break;

  case WM_COMMAND:
  {
    //naciœniêcie przycisku pokazuj¹cego nastêpne pytanie
    if ((HWND)lParam == pytaniePrzycisk) {
      //konwersja z stringa do LPCWSTR
      //https://stackoverflow.com/a/4139684/8365453
      wstring stemp = wstring(pytania[pytaniaLicznik].begin(), pytania[pytaniaLicznik].end());
      LPCWSTR sw = stemp.c_str();

      SetWindowText(pytanieWyswietlacz, sw);
      SetWindowText(wyzwanieWyswietlacz, L"");
      SetWindowText(pytaniaRezultatWyœwietlacz, L"");
      SetWindowText(pytanieOdpowiedz, L"");

      pytaniaLicznik--;
      if (pytaniaLicznik < 0) {
        pytaniaLicznik = pytania.size() - 1;
      }
    }
    //naciœniêcie przycisku pokazuj¹cego nastêpne wyzwanie
    else  if ((HWND)lParam == wyzwaniePrzycisk) {
      wstring stemp = wstring(wyzwania[wyzwaniaLicznik].begin(), wyzwania[wyzwaniaLicznik].end());
      LPCWSTR sw = stemp.c_str();

      SetWindowText(pytanieWyswietlacz, L"");
      SetWindowText(pytaniaRezultatWyœwietlacz, L"");
      SetWindowText(pytanieOdpowiedz, L"");
      SetWindowText(wyzwanieWyswietlacz, sw);

      wyzwaniaLicznik--;
      if (wyzwaniaLicznik < 0) {
        wyzwaniaLicznik = wyzwania.size() - 1;
      }
    }
    //naciœniêcie przycisku sprawdzaj¹cego odpowiedŸ na pytanie
    else if ((HWND)lParam == pytaniePrzyciskAkceptacji) {
      int ktoraOdpowiedzSprawdzic = pytaniaLicznik + 1;
      wstring stemp = wstring(odpowiedziNaPytania[ktoraOdpowiedzSprawdzic].begin(), odpowiedziNaPytania[ktoraOdpowiedzSprawdzic].end());
      LPCWSTR sw = stemp.c_str();
      string cc = "sa";
      TCHAR  pobranaOdpowiedz[100]; //https://stackoverflow.com/a/5690378/8365453
      GetWindowText(pytanieOdpowiedz, pobranaOdpowiedz, 100);

      LPWSTR ptr = pobranaOdpowiedz;
      ////bool isTrue = odpowiedziNaPytania[ktoraOdpowiedzSprawdzic].compare(ptr)
      if (pobranaOdpowiedz == sw) {
        SetWindowText(pytaniaRezultatWyœwietlacz, L"OdpowiedŸ poprawna. +1 pkt");
      }
      else {
        SetWindowText(pytaniaRezultatWyœwietlacz, L"OdpowiedŸ niepoprawna.");
      }
    }
  }
  case WM_USER + 5:

  default:
    return DefWindowProc(hwnd, msg, wParam, lParam);
  }

  return 0;
}
