// HIDConsoleApplication.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <windows.h>

#if defined(OS_LINUX) || defined(OS_MACOSX)
#include <sys/ioctl.h>
#include <termios.h>
#elif defined(OS_WINDOWS)
#include <conio.h>
#endif

#include "hid.h"


static char get_keystroke(void);
static void Paste();
static void typeString(wchar_t* text);

static const wchar_t* text[141] = {L"(⊙⌓ ☉)", L"(✖╭╮✖)", L"(̿▀̿ ̿Ĺ̯̿̿▀̿ ̿)̄", L"(¬､¬)", L"ɿ(｡･-･)ɾⓌⓐⓣ？", L"˚‧º·(˚ ˃̣̣̥⌓˂̣̣̥ )‧º·˚", //47 * 3
		L"(╥﹏╥)", L"•̀.̫•́✧", L"( •_•) ( •_•)>⌐■-■ (⌐■_■)", L"(∩❛ڡ❛∩)", L"(╯°□°）╯︵ ┻━┻", L"•̀⌓ •́", L"ヾ(⊙ε◎)シ", "(งಠ_ಠ)ง", 
		L"( ˘▽˘)っ♨", L"ﾏｯﾃｪ(ノ；Д；)ノ ｀、、ヽ｀☂", L"ヽ(。_°)ノ", L"༼ ⍢ ༽", L" ̿ ̿ ̿ ̿’̿’̵з=༼ ▀̿Ĺ̯▀̿ ̿ ༽", L"＼(º □ º l|l)/", 
		L"[¬º-°]¬", L"•ᴗ•", L"₍˄·͈༝·͈˄₎◞ ̑̑ෆ", L"▬▬ι═══════ﺤ", L"ᕕ( ᐛ )ᕗ", L"(∩≧ヮ≦∩)", L"(⁄ ⁄•⁄ω⁄•⁄ ⁄)", L"(ɔ ˘⌣˘)˘⌣˘ c)", 
		L"( ˘ ³˘)♡", L"٩(♡ε♡ )۶", L"( ͜。 ͡ʖ ͜。)", L"⚈ ̫ ⚈", L"ヽ(⌐■_■)ノ♪♬", L"♫♪.ılılıll|̲̅̅●̲̅̅|̲̅̅=̲̅̅|̲̅̅●̲̅̅|llılılı.♫♪", 
		L"(x(x_(x_x(O_o)x_x)_x)x)", L"(＾་།＾)", L"ᙙᙖ", L"™", L"ᶘ ᵒᴥᵒᶅ", L"solid ⚁⚂/⚀⚅", L"ø,¸¸,ø¤º°`°º¤ø", L"●︿●", 
		L"[̲̅$̲̅(̲̅1̲̅)̲̅$̲̅]", L"[̲̅$̲̅(̲̅2̲̅0̲̅)̲̅$̲̅]", L"●▬▬▬▬▬๑۩۩๑▬▬▬▬▬●", L"(◠‿◠✿)", L"ー=≡Σ( ε¦)", 
		/* ALT emoticons */
		L"｡oㅇo｡", L"(⋋▂⋌)", L"(⌐▨_▨)", L"( -_・)?", L"┐(￣ヘ￣;)┌", L"╥﹏╥", L".·´¯`(>▂<)´¯`·.", L"┬┴┬┴┤(･_├┬┴┬┴", 
		L"(._.) ( l: ) ( .-. ) ( :l ) (._.)", L"<・ )))><", L"┬──┬◡ﾉ(° -°ﾉ)", L"(｀ﾛ´)ᕗ", L"(º﹃º )", L"ಠ_ಠ", L"( -_-)旦~", 
		L"°。°。°。个o(-ω-｡)ﾟ。°。°。°", L"┗(｡_ ｡) ┐", L"༼ つ ◕_◕ ༽つ", L"( ´-ω･)︻┻┳══━一", L"¯\_(ツ)_/¯", L"(°∋°)", L"ಠ‿↼", 
		L"[^._.^]ﾉ彡", L"╾━╤デ╦︻", L"٩( ╹▿╹ )۶", L"╰(✧∇✧)╯", L"(,,Ծ‸Ծ,, )", L"(⊃｡•́‿•̀｡)⊃", L"(｡♥‿♥｡)", L"(˵ᄑ_ᄑ˵)", 
		L"( ͠° ͟ʖ ͡°)", L"(∩ ͡° ͜ʖ ͡°)⊃━☆ﾟ", L"(￣▽￣)/♫•*¨*•.¸¸♪", L"(+[__]∙:∙)", L"(ﾟ(ﾟω(ﾟωﾟ(☆ω☆)ﾟωﾟ)ωﾟ)ﾟ)", L"( '┏_┓' )", 
		L"★彡", L"®", L"| (•‿•) |", L"̵̄/͇̐\  eliminate!", L"•.¸¸.•*´¨`*•", L"( ̲̅:̲̅:̲̅:̲̅[̲̅ ̲̅]̲̅:̲̅:̲̅:̲̅ )", L"[̲̅$̲̅(̲̅5̲̅)̲̅$̲̅]", L"[̲̅$̲̅(̲̅5̲̅0̲̅)̲̅$̲̅]", 
		L"ס₪₪₪₪§|(Ξ≥≤≥≤≥≤ΞΞΞΞΞΞΞΞΞΞ>", L"(◕‿◕✿)", L"(:D)┼─┤",
		/* CTRL emoticons */
		L"⊙０⊙", L"＿|￣|○", L"눈_눈", L"( ・◇・)？", L"╮(￣ω￣;)╭", L"(oT-T)尸", L"(ᗒᗣᗕ)՞", L"(¦☐[▓▓]", 
		L"( •_•) ( •_•)>⌐■ (⌐■_•)", L"(ノಠ ∩ಠ)ノ彡( o°o)", L"ノ┬─┬ノ ︵ ( o°o)", L"ᕙ(⇀‸↼‶)ᕗ", L"๏¬๏", L"ಠ_ರೃ", L"(⊡.⊡)و✎", 
		L"ヽ(^o^)ρ┳┻┳°σ(^o^)ノ", L"༼  ͒ ̶ ͒  ༽", L"»-(º □ º)->", L"(҂`з´).っ︻デ═一", L"へ‿(ツ)‿ㄏ", L"ㄖεㄖ", L"(͠≖ ͜ʖ͠≖)👌", 
		L"ฅ^•ﻌ•^ฅ", L"¤=[]::::::>", L"(^～^)", L"＼（●⌒∇⌒●）／", L"(˵¯͒ བ¯͒˵)", L"(づ￣ ³￣)づ", L"♡⌓♡", L"(✿˵•́ ‸ •̀˵)", 
		L"( ͡° ͜ʖ ͡°)", L"(ﾉ´ヮ´)ﾉ*:･ﾟ✧", L"~(˘▽˘~)  ~(˘▽˘)~  (~˘▽˘)~", L"இڿڰۣ-ڰۣ~—", L"(-(-_(-_-)_-)-)", L"ಥಠ", L"c[_]", 
		L"©", L"(❍ᴥ❍ʋ)", L"(￣^￣)ゞ", L"☆.。.:*・°☆", L"シ", L"[̲̅$̲̅(̲̅1̲̅0̲̅)̲̅$̲̅]", L"[̲̅$̲̅(̲̅ ̲̅͡°̲̅ ̲̅͜ʖ ̲̅͡°̲̅)̲̅$̲̅]", L"__̴ı̴̴̡̡̡ ̡͌l̡̡̡ ̡͌l̡*̡̡ ̴̡ı̴̴̡ ̡̡͡|̲̲̲͡͡͡ ̲▫̲͡ ̲̲̲͡͡π̲̲͡͡ ̲̲͡▫̲̲͡͡ ̲|̡̡̡ ̡ ̴̡ı̴̡̡ ̡͌l̡̡̡̡.___", 
		L"(◡‿◡✿)", L"─=≡Σ((( つ＞＜)つ"};


int main()
{
	int i, r, num;
	char c, buf[64];

	// C-based example is 16C0:0480:FFAB:0200
	
	// Arduino-based example is 16C0:0486:FFAB:0200
	r = rawhid_open(1, 0x16C0, 0x0486, 0xFFAB, 0x0200);
	if (r <= 0) {
		printf("no rawhid device found\n");
		Sleep(1000);
		return -1;
	}
	printf("found rawhid device\n");

	while (1) {
		// check if any Raw HID packet has arrived
		num = rawhid_recv(0, buf, 64, 220);
		if (num < 0) {
			printf("\nerror reading, device went offline\n");
			rawhid_close(0);
			Sleep(1000);


			return 0;
		}
		if (num > 0) {
			printf("\nrecv %d bytes:\n", num);
			for (i = 0; i<num; i++) {
				printf("%02X ", buf[i] & 255);
				if (i % 16 == 15 && i < num - 1) printf("\n");
			}
			printf("%d\n",buf[0]);
			if (buf[0] > 0) {
				
				//printf("\ninvalid key: %d\n", buf[0]);

				typeString(text[buf[0]]);

			}
		}
		
		// check if any input on stdin
		/*while ((c = get_keystroke()) >= 32) {
			printf("\ngot key '%c', sending...\n", c);
			buf[0] = c;
			for (i = 1; i<64; i++) {
				buf[i] = 0;
			}
			rawhid_send(0, buf, 64, 100);
		}*/
	}
}

static void typeString(wchar_t* text) {
	int len = wcslen(text);

	HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, (len + 1) * sizeof(wchar_t));
	wchar_t* buffer = (wchar_t*)GlobalLock(hMem);
	wcscpy_s(buffer, len + 1, text);
	GlobalUnlock(hMem);

	OpenClipboard(NULL);
	EmptyClipboard();
	SetClipboardData(CF_UNICODETEXT, hMem);
	CloseClipboard();
	Paste();
	/*CGEventRef event1, event2;
	event1 = CGEventCreateKeyboardEvent(NULL, (CGKeyCode)9, true); // v
	event2 = CGEventCreateKeyboardEvent(NULL, (CGKeyCode)9, false); // v

	CGEventSetFlags(event1, (CGEventFlags)kCGEventFlagMaskCommand);
	CGEventSetFlags(event2, (CGEventFlags)kCGEventFlagMaskCommand);

	CGEventPost(kCGSessionEventTap, event1);
	CGEventPost(kCGSessionEventTap, event2);*/
}

static void Paste() {
	INPUT ip;
	ip.type = INPUT_KEYBOARD;
	ip.ki.wScan = 0;
	ip.ki.time = 0;
	ip.ki.dwExtraInfo = 0;
	{
		// Press the "Ctrl" key
		ip.ki.wVk = VK_CONTROL;
		ip.ki.dwFlags = 0; // 0 for key press
		SendInput(1, &ip, sizeof(INPUT));

		// Press the "V" key
		ip.ki.wVk = 'V';
		ip.ki.dwFlags = 0; // 0 for key press
		SendInput(1, &ip, sizeof(INPUT));

		// Release the "V" key
		ip.ki.wVk = 'V';
		ip.ki.dwFlags = KEYEVENTF_KEYUP;
		SendInput(1, &ip, sizeof(INPUT));

		// Release the "Ctrl" key
		ip.ki.wVk = VK_CONTROL;
		ip.ki.dwFlags = KEYEVENTF_KEYUP;
		SendInput(1, &ip, sizeof(INPUT));
	}
}

#if defined(OS_LINUX) || defined(OS_MACOSX)
// Linux (POSIX) implementation of _kbhit().
// Morgan McGuire, morgan@cs.brown.edu
static int _kbhit() {
	static const int STDIN = 0;
	static int initialized = 0;
	int bytesWaiting;

	if (!initialized) {
		// Use termios to turn off line buffering
		struct termios term;
		tcgetattr(STDIN, &term);
		term.c_lflag &= ~ICANON;
		tcsetattr(STDIN, TCSANOW, &term);
		setbuf(stdin, NULL);
		initialized = 1;
	}
	ioctl(STDIN, FIONREAD, &bytesWaiting);
	return bytesWaiting;
}
static char _getch(void) {
	char c;
	if (fread(&c, 1, 1, stdin) < 1) return 0;
	return c;
}
#endif


static char get_keystroke(void)
{
	if (_kbhit()) {
		char c = _getch();
		if (c == 97) {
			const wchar_t* text = L"ヽ(⌐■_■)ノ♪♬E";
			int len = wcslen(text);

			HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, (len + 1) * sizeof(wchar_t));
			wchar_t* buffer = (wchar_t*)GlobalLock(hMem);
			wcscpy_s(buffer, len + 1, text);
			GlobalUnlock(hMem);

			OpenClipboard(NULL);
			EmptyClipboard();
			SetClipboardData(CF_UNICODETEXT, hMem);
			CloseClipboard();
			//http://stackoverflow.com/questions/1264137/how-to-copy-string-to-clipboard-in-c
			/*const char* output = "ヽ(⌐■_■)ノ♪♬";
			const size_t len = strlen(output) + 1;

			u_int size = MultiByteToWideChar(CP_UTF8, NULL,output,-1,NULL,0);
			wchar_t* buffer = (wchar_t*)LocalAlloc(LMEM_ZEROINIT, sizeof(wchar_t) * (size));///
			int bufLen = size;
			size = MultiByteToWideChar(CP_UTF8, NULL, output, -1, buffer, bufLen);
				
			
			FILE *stream = NULL;
			errno_t err = fopen_s(&stream, "d:\\test.dat", "wb, ccs=UTF-8");
			if (err == 0)
			{
				fwrite(buffer, sizeof(WCHAR), bufLen, stream);
				fclose(stream);
			}
			else
				printf("Errno result: %d\n", err);

			//LPWSTR 
			HGLOBAL hMem =  GlobalAlloc(GMEM_MOVEABLE, 5);
			memcpy(GlobalLock(hMem), buffer, 5);
			GlobalUnlock(hMem);
			OpenClipboard(0);
			EmptyClipboard();
			SetClipboardData(CF_UNICODETEXT, hMem);
			CloseClipboard();*/
		}
		if (c >= 32) return c;
	}
	return 0;
}



