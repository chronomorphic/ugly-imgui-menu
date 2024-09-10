#include <Windows.h>
#include <vector>
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <winbase.h>
#include <tchar.h>
#include "auth.hpp"
#include <CommCtrl.h>
#include <urlmon.h>
#include <stdio.h>
#include <TlHelp32.h>
#include <cstdint>
#include "xorstr.hpp"
#include <thread>
#include <random>
#include <Psapi.h>
#include <chrono>
#include <future>
#include <sys/stat.h>
#include <filesystem>
#include <direct.h>
#include "MMSystem.h"
#include "Variables.cpp"
#pragma warning(disable : 4996)
#pragma comment(lib, "psapi.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "urlmon.lib")
#include <d3d9.h>
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib, "D3dx9")
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx9.h"
#include "ImGui/imgui_impl_win32.h"
using namespace KeyAuth;
using namespace std;
IDirect3DTexture9* masterlogo;
ifstream inFile;
ifstream KeyConfig;
ifstream Version;


