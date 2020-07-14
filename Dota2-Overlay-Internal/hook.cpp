#pragma once
#include "pch.h"
#include "hook.h"

//Check if config exist in path
inline bool fileExist(const std::string& name) {
#pragma warning(suppress : 4996).
	if (FILE* file = fopen(name.c_str(), "r")) {
		fclose(file);
		return true;
	}
	else {
		return false;
	}
}

void Hooks::Init(HMODULE hModule)
{
	CHAR czTempPath[MAX_PATH] = { 0 };
	GetTempPathA(MAX_PATH, czTempPath); // retrieving temp path
	std::string sPath = czTempPath;
	sPath += "offs.conf";

	// check if offset file is in temp.
	if (fileExist(sPath))
	{
		if (!D3D9::CreateDummyDevice(D3D9::vTable, sizeof(D3D9::vTable))) return;
		D3D9::EndSceneAddress = (mem_t)D3D9::vTable[42];

		D3D9::oEndScene = (D3D9::EndScene_t)Memory::In::Hook::TrampolineHook((byte_t*)D3D9::EndSceneAddress, (byte_t*)D3D9::hkEndScene, ENDSCENE_HOOK_LENGTH);

	}
	else
	{
		MessageBox(NULL, L"Config file not found", L"ERROR", MB_OK);
	}


}

void Hooks::Shutdown()
{
	if (!D3D9::EndSceneAddress) return;
	Memory::In::Hook::Restore(D3D9::EndSceneAddress);
}

