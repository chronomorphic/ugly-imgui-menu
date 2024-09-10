#include "Includes.cpp"

std::string tm_to_readable_time(tm ctx);
static std::time_t string_to_timet(std::string timestamp);
static std::tm timet_to_tm(time_t timestamp);


std::string name = XorStr("PHASE-ONE.xyz"); //Application name found in application settings
std::string ownerid = XorStr("l6MsKZLtnM"); //Owner ID Found in user settings
std::string secret = XorStr("9f57f7693879cfeed01b931911b74d07b813ccab120bb30a1670af3b364403ed"); //Application secret found in Application settings
std::string version = XorStr("1.0"); // Version can be changed but is not really important
std::string url = "https://keyauth.win/api/1.1/"; // change if you're self-hosting
std::string sslPin = "ssl pin key (optional)"; // don't change unless you intend to pin public certificate key. you can get here in the "Pin SHA256" field https://www.ssllabs.com/ssltest/analyze.html?d=ke
api KeyAuthApp(name, ownerid, secret, version, url, sslPin);

bool loader_active = true;


char PassWord[20] = "";
char Licence[50] = "";
char UserName[20] = "";
char RgPassWord[20] = "";
char RgUserName[20] = "";



HWND main_hwnd = nullptr;

LPDIRECT3DDEVICE9        g_pd3dDevice;
D3DPRESENT_PARAMETERS    g_d3dpp;
LPDIRECT3D9              g_pD3D;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//FUNCTIONS/////////////////////////////////////////////////////////

bool CreateDeviceD3D(HWND hWnd)
{
	if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
		return false;

	// Create the D3DDevice
	ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
	g_d3dpp.Windowed = TRUE;
	g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	g_d3dpp.EnableAutoDepthStencil = TRUE;
	g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;           // Present with vsync
	//g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;   // Present without vsync, maximum unthrottled framerate
	if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0)
		return false;
	return true;
}

void CleanupDeviceD3D()
{
	if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
	if (g_pD3D) { g_pD3D->Release(); g_pD3D = NULL; }
}

void ResetDevice()
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
	HRESULT hr = g_pd3dDevice->Reset(&g_d3dpp);
	if (hr == D3DERR_INVALIDCALL)
		IM_ASSERT(0);
	ImGui_ImplDX9_CreateDeviceObjects();
}


void InfLog()
{
	RECT screen_rect;
	GetWindowRect(GetDesktopWindow(), &screen_rect);
	auto x = float(screen_rect.right - width) / 2.f;
	auto y = float(screen_rect.bottom - height) / 2.f;

	ImGui::SetNextWindowPos(ImVec2(x, y), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(200, 100));
	ImGui::Begin("Information", &loader_active, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
	{
		ImGui::BeginChild("##childinfo", ImVec2(ImGui::GetContentRegionAvail().x / 1, ImGui::GetContentRegionAvail().y / 1), true, ImGuiWindowFlags_NoResize);

			ImGui::SetCursorPos({ 150, 5 });
			if (ImGui::Button("X", { 25, 25 }))
			{
				InfWindow = false;
			}
			/*ImGui::Text("UserName : %s", KeyAuthApp.data.username);*/

			/*ImGui::Text("Your IP: %s", KeyAuthApp.data.ip);*/

			/*ImGui::Text("Hardware-Id : %s", KeyAuthApp.data.hwid);

			ImGui::Text("Create date : %s", tm_to_readable_time(timet_to_tm(string_to_timet(KeyAuthApp.data.createdate))));

			ImGui::Text(" Last login : %s", tm_to_readable_time(timet_to_tm(string_to_timet(KeyAuthApp.data.lastlogin))));*/

			ImGui::EndChild();
	}
	ImGui::End();
}


void LoginSuccess()
{
	//MessageBox(NULL, TEXT("Login Success!"), TEXT("Login"), MB_OK);
	ofstream KeyConfig;
	KeyConfig.open("key.config");
	KeyConfig << Licence;
	KeyConfig.close();
	step = 2;
}

void TextCentered(std::string text) {
	auto windowWidth = ImGui::GetWindowSize().x;
	auto textWidth = ImGui::CalcTextSize(text.c_str()).x;

	ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
	ImGui::Text(text.c_str());
}


////////////////////////////////////////////////////////////////////


int APIENTRY WindownsMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL,"Loaders", NULL };
	RegisterClassEx(&wc);
	main_hwnd = CreateWindow(wc.lpszClassName, "PHASE-ONE", WS_POPUP, 0, 0, 5, 5, NULL, NULL, wc.hInstance, NULL);

	if (!CreateDeviceD3D(main_hwnd)) {
		CleanupDeviceD3D();
		UnregisterClass(wc.lpszClassName, wc.hInstance);
		return 1;
	}
	ShowWindow(main_hwnd, SW_HIDE);
	UpdateWindow(main_hwnd);

        HWND Stealth;
        AllocConsole();
        Stealth = FindWindowA("ConsoleWindowClass", NULL);
        ShowWindow(Stealth, 0);
	
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.IniFilename = nullptr;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	constexpr auto ColorFromBytes = [](uint8_t r, uint8_t g, uint8_t b)
	{
		return ImVec4((float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f, 1.0f);
	};




	io.Fonts->AddFontFromFileTTF("font.ttf", size_pixels);
	ImFont* font1 = io.Fonts->AddFontFromFileTTF("font.ttf", size_pixels);
	ImFont* font2 = io.Fonts->AddFontFromFileTTF("anotherfont.otf", size_pixels);
	auto& style = ImGui::GetStyle();
	ImVec4* Colors = style.Colors;

	const ImVec4 bgColor = ColorFromBytes(37, 37, 38);
	const ImVec4 lightBgColor = ColorFromBytes(82, 82, 85);
	const ImVec4 veryLightBgColor = ColorFromBytes(90, 90, 95);

	const ImVec4 panelColor = ColorFromBytes(51, 51, 55);
	const ImVec4 panelHoverColor = ColorFromBytes(29, 151, 236);
	const ImVec4 panelActiveColor = ColorFromBytes(0, 224, 255);

	const ImVec4 textColor = ColorFromBytes(255, 255, 255);
	const ImVec4 textDisabledColor = ColorFromBytes(151, 151, 151);
	const ImVec4 borderColor = ColorFromBytes(78, 78, 78);

	Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
	Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
	Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
	Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
	Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.10f, 1.00f);
	Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);

	style.WindowPadding = { 10.f, 10.f };
	style.PopupRounding = 0.f;
	style.FramePadding = { 8.f, 4.f };
	style.ItemSpacing = { 10.f, 8.f };
	style.ItemInnerSpacing = { 6.f, 6.f };
	style.TouchExtraPadding = { 0.f, 0.f };
	style.IndentSpacing = 21.f;
	style.ScrollbarSize = 15.f;
	style.GrabMinSize = 8.f;
	style.WindowBorderSize = 1.f;
	style.ChildBorderSize = 0.f;
	style.PopupBorderSize = 1.f;
	style.FrameBorderSize = 1.f;
	style.TabBorderSize = 0.f;
	style.WindowRounding = 0.f;
	style.ChildRounding = 0.f;
	style.FrameRounding = 0.f;
	style.ScrollbarRounding = 0.f;
	style.GrabRounding = 0.f;
	style.TabRounding = 0.f;
	style.WindowTitleAlign = { 0.5f, 0.5f };
	style.ButtonTextAlign = { 0.5f, 0.5f };
	style.DisplaySafeAreaPadding = { 3.f, 3.f };

	ImGui_ImplWin32_Init(main_hwnd);
	ImGui_ImplDX9_Init(g_pd3dDevice);

	DWORD window_flags = ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize;
	RECT screen_rect;
	GetWindowRect(GetDesktopWindow(), &screen_rect);
	auto x = float(screen_rect.right - width) / 2.f;
	auto y = float(screen_rect.bottom - height) / 2.f;

	KeyAuthApp.init();

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	while (msg.message != WM_QUIT && !LoginCheck)
	{

		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			continue;
		}

		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		{
			ImGui::SetNextWindowPos(ImVec2(x, y), ImGuiCond_Once);
			ImGui::SetNextWindowSize(ImVec2(width, height));
			ImGui::SetNextWindowBgAlpha(1.0f);

			ImGui::Begin("Vegas Bypass", &loader_active, window_flags);
			{

				if (Tick || Alpha >= 255)
				{
					Tick = true;
					if (!(Alpha <= 30))
						Alpha -= Speed;
					else if (Alpha <= 30)
						Tick ^= 1;
				}

				else   if (!Tick || Alpha >= 255)
				{
					Tick = false;
					if (!(Alpha >= 255))
						Alpha += Speed;
					else if (Alpha >= 255)
						Tick ^= 1;
				}

				if (step == 0)
				{



					//AUTO-UPDATE




					
					








					/////////////

					
					KeyConfig.open("key.config");
					KeyConfig >> Licence;
					KeyConfig.close();

					animate = true;

					if (animate)
					{
						progress += progress_dir * -0.5f * ImGui::GetIO().DeltaTime;
					}

					ImGui::SetCursorPos({ 200, 200 });
					ImGui::ProgressBar(progress, ImVec2(158.0f, 0.0f));


					if (progress >= 1.0f)
					{
						step = 1;
					}
					
				}
				if (step == 1)
				{
					
						if (filesystem::exists("key.config"))
						{

							KeyAuthApp.license(Licence);

							if (!KeyAuthApp.data.success)
							{
								MessageBox(NULL, TEXT("Key is invalid!"), TEXT("Login"), MB_ICONASTERISK);
								remove("key.config"); // delete file
								step = 0;
							}

							if (KeyAuthApp.data.success)
							{
								LoginSuccess();
							}
						}



					ImGui::BeginChild("##child1", ImVec2(ImGui::GetContentRegionAvail().x / 1, ImGui::GetContentRegionAvail().y / 1), true, ImGuiWindowFlags_NoResize);

					ImGui::SetCursorPos({ 140, 130 });
					TextCentered("KEY");

					ImGui::SetCursorPos({ 100, 155 });
					ImGui::SetNextItemWidth(325);
					ImGui::InputText("##Key", Licence, IM_ARRAYSIZE(Licence));

					ImGui::SetCursorPos({ 100, 190 });
					if (ImGui::Button("Login", ImVec2(155, 35)))
					{
						KeyAuthApp.license(Licence);

						if (!KeyAuthApp.data.success)
						{
							MessageBox(NULL, TEXT("Key is invalid!"), TEXT("Login"), MB_ICONASTERISK);
							remove("key.config"); // delete file
							step = 0;
						}

						if (KeyAuthApp.data.success)
						{
							LoginSuccess();
						}
					}

					ImGui::SetCursorPos({ 270, 190 });
					if (ImGui::Button("Get Key", ImVec2(155, 35)))
					{
						system("start ");
					}

					ImGui::EndChild();
				}




				if (step == 2)
				{

					ImGui::BeginChild("##child2", ImVec2(ImGui::GetContentRegionAvail().x / 1, ImGui::GetContentRegionAvail().y / 1), true, ImGuiWindowFlags_NoResize);

					

					//GAME-VERSION
					ImGui::SetCursorPos({ 220, 10 });
					ImGui::TextColored(ImColor(200, 50, 255, (int)Alpha), "GAME VERSION");

					ImGui::SetCursorPos({ 180, 35 });
					ImGui::RadioButton("##Global", &choices, 1);

					ImGui::SetCursorPos({ 170, 60 });
					ImGui::Text("GLOBAL");

					ImGui::SetCursorPos({ 230, 35 });
					ImGui::RadioButton("##Korea", &choices, 2);

					ImGui::SetCursorPos({ 222, 60 });
					ImGui::Text("KOREA");

					ImGui::SetCursorPos({ 280, 35 });
					ImGui::RadioButton("##Taiwan", &choices, 3);

					ImGui::SetCursorPos({ 270, 60 });
					ImGui::Text("TAIWAN");

					ImGui::SetCursorPos({ 330, 35 });
					ImGui::RadioButton("##Vietnam", &choices, 4);

					ImGui::SetCursorPos({ 320, 60 });
					ImGui::Text("VIETNAM");


					//EMULATOR


					ImGui::SetCursorPos({ 230, 90 });
					ImGui::TextColored(ImColor(200, 50, 255, (int)Alpha), "EMULATOR");

					ImGui::SetCursorPos({ 200, 105 });
					ImGui::RadioButton("##GAMPLOOP 7.1", &choice, 1);

					ImGui::SetCursorPos({ 170, 130 });
					//ImGui::PushFont(font1);
					ImGui::Text("GAMPLOOP 7.1");

					ImGui::SetCursorPos({ 298, 105 });
					ImGui::RadioButton("##SMARTGAGA", &choice, 2);

					ImGui::SetCursorPos({ 280, 130 });
					ImGui::Text("SMARTGAGA");

					

					ImGui::SetCursorPos({ 90, 165 });
					if (ImGui::Button("Start Game", ImVec2(170, 30)))
					{
						//Your code
					}

					ImGui::SetCursorPos({ 270, 165 });
					if (ImGui::Button("Safe Exit", ImVec2(170, 30)))
					{
						//Your code
					}

					ImGui::SetCursorPos({ 245, 210 });
					ImGui::TextColored(ImColor(200, 50, 255, (int)Alpha), "MISC");

					ImGui::Spacing();
					ImGui::Separator();

					ImGui::SetCursorPos({ 100, 250 });
					ImGui::Checkbox("90 FPS", &fps);

					ImGui::SameLine();

					ImGui::Checkbox("Less Recoil", &lessRecoil);

					ImGui::SameLine();

					ImGui::Checkbox("Ultra HD", &ultraHD);

					ImGui::SetCursorPos({ 100, 280 });
					ImGui::Checkbox("No Head", &noHead);

					ImGui::SetCursorPos({ 179, 280 });
					ImGui::Checkbox("IPAD VIEW", &ipadView);

					if (ipadView == true)
					{
						ImGui::SetCursorPos({ 70, 310 });
						ImGui::SliderFloat("iPad View", &iPad, 0.0f, 100.0f);
					}
					

					if (Game == 2)
					{
						ImGui::SetCursorPos(ImVec2(0, 50));
						TextCentered("Korea");
					}

					if (Game == 3)
					{
						ImGui::SetCursorPos(ImVec2(0, 50));
						TextCentered("Vietnam");
					}

					if (Game == 4)
					{
						ImGui::SetCursorPos(ImVec2(0, 50));
						TextCentered("Taiwan");
					}

					ImGui::SetCursorPos(ImVec2(5, 338));
					ImGui::Separator();


					ImGui::SetCursorPos(ImVec2(218, 342));
					ImGui::TextColored(ImColor(200, 200, 255, 122), "Status: ");

					ImGui::SetCursorPos(ImVec2(5, 342));
					ImGui::TextColored(ImColor(200, 50, 255, 122), "Version 1.0");

					ImGui::EndChild();


				}

				if (InfWindow == true)
				{
					InfLog();
				}

			}
			ImGui::End();
		}
		ImGui::EndFrame();

		g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
		if (g_pd3dDevice->BeginScene() >= 0)
		{
			ImGui::Render();
			ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
			g_pd3dDevice->EndScene();
		}
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
		HRESULT result = g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
		if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET) {
			ResetDevice();
		}
		if (!loader_active) {
			msg.message = WM_QUIT;
		}
	}

	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	CleanupDeviceD3D();
	//	DestroyWindow(main_hwnd);
	UnregisterClass(wc.lpszClassName, wc.hInstance);
	//	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;
	switch (msg)
	{
	case WM_SIZE:
		if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
		{
			g_d3dpp.BackBufferWidth = LOWORD(lParam);
			g_d3dpp.BackBufferHeight = HIWORD(lParam);
			ResetDevice();
		}
		return 0;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU)
			return 0;
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
	WindownsMain(0, 0, 0, 0);
}

std::string tm_to_readable_time(tm ctx) {
	char buffer[80];

	strftime(buffer, sizeof(buffer), "%a %m/%d/%y %H:%M:%S %Z", &ctx);

	return std::string(buffer);
}

static std::time_t string_to_timet(std::string timestamp) {
	auto cv = strtol(timestamp.c_str(), NULL, 10); // long

	return (time_t)cv;
}

static std::tm timet_to_tm(time_t timestamp) {
	std::tm context;

	localtime_s(&context, &timestamp);

	return context;
}