#pragma once
#include <vector>
#include <iostream>
#include <Windows.h>
#include <tlhelp32.h>
#include <sstream>
#include <psapi.h>
#include <cstring>
#include <regex>


#pragma comment(lib, "ntdll.lib")



typedef struct _MEMORY_REGION {
	DWORD_PTR dwBaseAddr;
	DWORD_PTR dwMemorySize;
}MEMORY_REGION;

namespace memory {

	inline DWORD proc_id = 0;
	inline HANDLE proc_handle = nullptr;
	inline std::string proc_name;
	inline DWORD64 base_addr = NULL;
	inline DWORD64 hModule = NULL;
	inline DWORD64 modBaseSize = NULL;
	inline MODULEENTRY32 ModuleEntry;

	
	inline char* to_char(const wchar_t* wideString) {
		size_t convertedChars = 0;
		size_t bufferSize = wcslen(wideString) + 1; 
		char* mbString = static_cast<char*>(std::malloc(bufferSize));

		if (mbString) {
			wcstombs_s(&convertedChars, mbString, bufferSize, wideString, _TRUNCATE);
		}

		return mbString;
	}

	inline DWORD get_proc_id(const char* proc_name)
	{
		HANDLE hsnap;
		PROCESSENTRY32 pt;
		DWORD PiD;
		hsnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		pt.dwSize = sizeof(PROCESSENTRY32);
		do {
			if (!strcmp(to_char(pt.szExeFile), proc_name)) {
				CloseHandle(hsnap);
				PiD = pt.th32ProcessID;
				return PiD;
				if (PiD != NULL) {
					return 0;
				}
			}
		} while (Process32Next(hsnap, &pt));
		return 1;
	}

	inline DWORD64 get_mod_base_addr(const char* module_name) {
		auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, proc_id);
		if (snapshot == INVALID_HANDLE_VALUE) {
			char str[0xff];
			return 0;
		}

		MODULEENTRY32 module_entry;
		module_entry.dwSize = sizeof(MODULEENTRY32);
		if (Module32First(snapshot, &module_entry)) {
			do {
				if (strcmp(to_char(module_entry.szModule), module_name) == 0) {
					base_addr = (DWORD64)module_entry.modBaseAddr;
					hModule = (DWORD64)module_entry.hModule;
					modBaseSize = module_entry.modBaseSize;
					ModuleEntry = module_entry;
					break;
				}
			} while (Module32Next(snapshot, &module_entry));
		}
		CloseHandle(snapshot);
		return (DWORD64)module_entry.modBaseAddr;
	}

	inline bool open_proc(const char* procc_name) {

		proc_id = get_proc_id(procc_name);

		if (proc_id == 0) { return false; }

		proc_handle = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, proc_id);

		proc_name = procc_name;

		base_addr = get_mod_base_addr("client.dll");

		if (proc_handle == nullptr) { return false; }

		return true;
	}
	

	template <typename T>
	inline auto read(const std::uintptr_t address) noexcept -> T
	{
		T value = { };
		ReadProcessMemory(proc_handle, reinterpret_cast<void*>(address), &value, sizeof(value), NULL);
		return value;
	}

	template <typename T>
	inline auto write(const std::uintptr_t address, const T& value) noexcept -> bool
	{
		bool write = WriteProcessMemory(proc_handle, reinterpret_cast<void*>(address), &value, sizeof(value), NULL);
		return write;
	}

	inline std::string ReadString(uintptr_t address) {
		const int bufferSize = 256;
		char buffer[bufferSize];
		int bytesRead = 0;
		bool success = true;

		while (bytesRead < bufferSize) {
			char character;
			ReadProcessMemory(memory::proc_handle, (LPVOID)(address + bytesRead), &character, sizeof(char), NULL);
			buffer[bytesRead] = character;

			if (character == '\0') {
				break;
			}

			bytesRead++;
		}

		if (bytesRead == bufferSize) {
			success = false;
		}

		if (!success) {
			return "";
		}

		return std::string(buffer);
	}

	


}