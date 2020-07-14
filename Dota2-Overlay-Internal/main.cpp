#pragma once
#include "pch.h"
#include "d3d9.h"
#include "hook.h"
#include "Draw.h"

//Prototype
std::vector<unsigned int> getOffsetFromText();
int InitMainHack();
unsigned int ReadVBE();

//Global Vars
DWORD ProcId;
uintptr_t modBase = 0;
std::vector<unsigned int> offsets;
uintptr_t dynamicAddr = 0;
HMODULE myModule;
unsigned int bVbE;
Draw draw;


bool init = false; 
//Our Endscence Function
long __stdcall D3D9::hkEndScene(LPDIRECT3DDEVICE9 pDevice)
{
	// Initialize
	if (!init)
	{
		draw.DrawInit(pDevice);
		init = true;
	}

	// Read Visible by enemy Tag 
	bVbE = ReadVBE();
	if (bVbE == 0)
	{
		draw.DrawTest("Not Visible");
	}
	else if (bVbE == 1)
	{
		draw.DrawTest("Visible");

	}
	else
	{
		draw.DrawTest("Waiting.");
	}

	if (GetAsyncKeyState(VK_END) & 1)
	{
		Hooks::Shutdown();
	}
	return oEndScene(pDevice); //Restore original EndScene function
}




BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hModule);
		CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)Hooks::Init, hModule, 0, nullptr));
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		Hooks::Shutdown();
		break;
	}
	return TRUE;
}

//Read offsets from config file.
std::vector<unsigned int> getOffsetFromText() {

	std::fstream file;
	std::string word;
	std::vector<std::string> offsets;
	std::vector<unsigned int> offsetsInt;

	// retrieving temp path
	CHAR czTempPath[MAX_PATH] = { 0 };
	GetTempPathA(MAX_PATH, czTempPath); 
	std::string sPath = czTempPath;
	sPath += "offs.conf";

	//Open text file
	file.open(sPath, std::ios::out | std::ios::in);
	if (file.fail()) {
		MessageBox(NULL, L"Config file not found", L"ERROR", NULL);
	}

	int i = 0;
	while (file >> word)
	{
		if (i >= 8) break;

		offsets.push_back(word);
		i++;
	}
	for (size_t i = 0; i < offsets.size(); i++)
	{
		std::istringstream buffer(offsets[i]);
		unsigned long long value;
		buffer >> std::hex >> value;
		offsetsInt.push_back(value);
	}
	return offsetsInt;
}

//Populate Vars
int InitMainHack()
{
	int i = 0;
	ProcId = Memory::In::GetCurrentProcessID();
	if (ProcId == 0)
	{
		MessageBox(NULL, L"Process not found!", L"ERROR", 0);
		return 1;
	}

	modBase = Memory::In::GetModuleAddress(L"engine2.dll");
	if (modBase == 0)
	{
		MessageBox(NULL, L"Module base not found!", L"ERROR", 0);
		return 2;
	}

	offsets = getOffsetFromText();
	if (offsets.size() < 1)
	{
		MessageBox(NULL, L"Config not found!", L"ERROR", 0);
		return 3;
	}

	dynamicAddr = modBase + offsets[0];

	return 0;
}

//Read Address of Visible by enemy tag in memory
unsigned int ReadVBE()
{
	uintptr_t vbEAddr = Memory::In::ReadMultiLevelPointer(dynamicAddr, offsets, true);
	if (vbEAddr == 0)
	{
		return 3;
	}
	int* vbEVal = (int*)vbEAddr;
	if (*vbEVal == 14) // Visible by enemy
	{
		return 1;
	}
	else if (*vbEVal >= 6 && *vbEVal <= 10) // Not visible by enemy
	{
		return 0;
	}
	return 3;
}