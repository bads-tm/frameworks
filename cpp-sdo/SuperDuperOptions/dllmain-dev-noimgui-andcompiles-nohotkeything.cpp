// Super Duper Options Base/Framework
// ("Experimental") Console window-less
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

	/*
	std::cout << std::hex << dynMoneyAddr  << " :: money: "
		<< std::dec << value << std::endl;
	*/ //maybe use https://github.com/ocornut/imgui , also for bad exe thing??
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

	WriteMemory<float>(dyn, 1.0, true /*unpotect*/);
}

