// Super Duper Options Base/Framework
// GPL3 License
// Coding: bads.tm, steakhal
// Injector: thelink2012,
// IniReader: ThirteenAG,

#include <windows.h>
#include <thread>
#include <chrono>
#include "..\includes\injector\injector.hpp"

#include <iostream>  // only for debugging

using namespace injector;

template <std::size_t Level>
static DWORD FindDynAddr(DWORD baseAddr, const DWORD (&offsets)[Level]) {
	for (int i = 0; i < Level; ++i)
		baseAddr = ReadMemory<DWORD>(baseAddr, false) + offsets[i];
	return baseAddr;
}

// RAII wrapper for opening and binding console
// and restroying in the destructor
class EnableConsoleGuard {
	FILE *fdOut, *fdIn;
public:
	EnableConsoleGuard() {
		if (!AllocConsole())
			MessageBox(NULL, "The console window was not created", NULL, MB_ICONEXCLAMATION);
		freopen_s(&fdOut, "CONOUT$", "w", stdout);
		freopen_s(&fdIn, "CONIN$", "r", stdin);
	}
	~EnableConsoleGuard() {
		std::cout << "console exits" << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		fclose(fdIn);
		fclose(fdOut);
		if (!FreeConsole())
			MessageBox(NULL, "Failed to free the console!", NULL, MB_ICONEXCLAMATION);
	}
};


/* From "S.W.I.N.E. Hacker v1.1.CT" file
<CheatEntry>
	<ID>438</ID>
	<Description>"Ingame money"</Description>
	<VariableType>4 Bytes</VariableType>
	<Address>0057B270</Address>
	<Offsets>
		<Offset>24</Offset>
	</Offsets>
</CheatEntry
*/
static void readMoney() {
	const DWORD moneyBaseAddr = 0x0057B270;
	const DWORD moneyOffsets[] = { 0x24 };

	const DWORD dynMoneyAddr = FindDynAddr(moneyBaseAddr, moneyOffsets);
	const int value = ReadMemory<int>(dynMoneyAddr, false);

	std::cout << std::hex << dynMoneyAddr  << " :: money: "
		<< std::dec << value << std::endl;
}

/* From "S.W.I.N.E. Hacker v1.1.CT" file
<CheatEntry>
	<ID>440</ID>
	<Description>"Max zoom in"</Description>
	<VariableType>Float</VariableType>
	<Address>0057B270</Address>
	<Offsets>
		<Offset>1C</Offset>
		<Offset>0</Offset>
		<Offset>3C</Offset>
		<Offset>404</Offset>
		<Offset>88</Offset>
	</Offsets>
</CheatEntry>
*/
static void setMaxZoomIn() {
	const DWORD base  = 0x0057B270;
	const DWORD off[] = { 0x88, 0x404, 0x3c, 0x0, 0x1c };

	const DWORD dyn = FindDynAddr(base, off);
	const float value = ReadMemory<float>(dyn, false);

	std::cout << std::hex << dyn << " :: max zoom in old: "
		<< std::dec << value << std::flush;

	WriteMemory<float>(dyn, 1.0, true /*unpotect*/);
	std::cout << ", new value: " << 1 << std::endl;
}

DWORD WINAPI backgroundThread(LPVOID) {
	EnableConsoleGuard console;

	bool is_1 = false;
	bool is_2 = false;
	bool is_3 = false;

	std::cout << "Press numpad 1 to close console." << std::endl;
	std::cout << "Press numpad 2 to read ingame money." << std::endl;
	std::cout << "Press numpad 3 to set max zoom in level to 1." << std::endl;
	
	while (!is_1) {
		std::cout << "." << std::flush;
		std::this_thread::sleep_for(std::chrono::seconds(1));

		if (GetAsyncKeyState(VK_NUMPAD1)) {
			is_1 = !is_1;
			std::cout << "\nState 1 switched" << std::endl;
		}
		if (GetAsyncKeyState(VK_NUMPAD2)) {
			is_2 = !is_2;
			std::cout << "\nState 2 switched" << std::endl;
		}
		if (GetAsyncKeyState(VK_NUMPAD3)) {
			is_3 = !is_3;
			std::cout << "\nState 3 switched" << std::endl;
		}

		if (is_2)
			readMoney();
		if (is_3)
			setMaxZoomIn();

	}	
	return TRUE;
}

BOOL WINAPI DllMain(HINSTANCE hInst, DWORD reason, LPVOID) {
	if (reason == DLL_PROCESS_ATTACH) {
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)&backgroundThread, NULL, 0, NULL);
	}
	return TRUE;
}
