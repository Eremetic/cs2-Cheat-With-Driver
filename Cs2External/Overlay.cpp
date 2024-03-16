#include "Overlay.hpp"
#include "windowCreate.hpp"
#include "utils.hpp"
#include <sstream>
#include "Driver.h"
#include "ConfigSaver.hpp"
#include <filesystem>
#define _USE_MATH_DEFINES


template <typename T>
inline VOID Overlay::safe_release(T*& p) 
{
	if (nullptr != p) {
		p->Release();
		p = nullptr;
	}
}



BOOL Overlay::init_device() 
{
   
	DXGI_SWAP_CHAIN_DESC vSwapChainDesc;
	ZeroMemory(&vSwapChainDesc, sizeof(vSwapChainDesc));
	vSwapChainDesc.BufferCount = 2;
	vSwapChainDesc.BufferDesc.Width = 0;
	vSwapChainDesc.BufferDesc.Height = 0;
	vSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	vSwapChainDesc.BufferDesc.RefreshRate.Numerator = (WC.refreshRate + 10);
	vSwapChainDesc.BufferDesc.RefreshRate.Denominator = (WC.refreshRate - 10);
	vSwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	vSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	vSwapChainDesc.OutputWindow = WC.overlayhWnd;
	vSwapChainDesc.SampleDesc.Count = 1;
	vSwapChainDesc.SampleDesc.Quality = 0;
	vSwapChainDesc.Windowed = TRUE;
	vSwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	D3D_FEATURE_LEVEL vFeatureLevel;
	D3D_FEATURE_LEVEL vFeatureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };

    HRESULT result = D3D11CreateDeviceAndSwapChain(nullptr, 
        D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, 
        vFeatureLevelArray, 2, D3D11_SDK_VERSION, 
        &vSwapChainDesc, &swap_chain, &d3d_device, 
        &vFeatureLevel, &device_context);
    if (result != S_OK)
    {
        std::ostringstream errorLog;
        errorLog << "Failed To Create Dx Device With Error : " << result << "\n";
        Logger.Logger(errorLog.str());
        
        return false;
    }
		

	init_render_target();

	ShowWindow(WC.overlayhWnd, SW_SHOWNORMAL);
	UpdateWindow(WC.overlayhWnd);
	return true;
}


VOID Overlay::dest_device() 
{
	dest_render_target();
	safe_release(swap_chain);
	safe_release(device_context);
	safe_release(d3d_device);
}


VOID Overlay::init_imgui() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiStyle* style = &ImGui::GetStyle();

	style->WindowPadding = ImVec2(15, 15);
	style->WindowRounding = 5.0f;
	style->FramePadding = ImVec2(5, 5);
	style->FrameRounding = 4.0f;
	style->ItemSpacing = ImVec2(12, 8);
	style->ItemInnerSpacing = ImVec2(8, 6);
	style->IndentSpacing = 25.0f;
	style->ScrollbarSize = 15.0f;
	style->ScrollbarRounding = 9.0f;
	style->GrabMinSize = 5.0f;
	style->GrabRounding = 3.0f;

    style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
    style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style->Colors[ImGuiCol_ChildBg] = ImVec4(0.132f, 0.169f, 0.32f, 1.00f);
    style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
    style->Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
    style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
    style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
    style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
    style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.04f, 0.01f, 0.09f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
    style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
    style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
    style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style->Colors[ImGuiCol_Button] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
    style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.1f, 0.09f, 0.1f, 1.00f);
    style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style->Colors[ImGuiCol_Header] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
    style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
    style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
    style->Colors[ImGuiCol_Button] = ImVec4(0.40f, 0.39f, 0.38f, 0.16f);
    style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.40f, 0.39f, 0.38f, 0.39f);
    style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.40f, 0.39f, 0.38f, 1.00f);
    style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
    style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
    style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
    style->Colors[ImGuiCol_TextSelectedBg] = ImColor(0.25f, 1.00f, 0.00f, 0.43f);
    style->Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);


	auto& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Ruda-Bold.ttf", 14);
    io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Ruda-Bold.ttf", 12);
    io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Ruda-Bold.ttf", 16);
    io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Ruda-Bold.ttf", 20);

	ImGui_ImplWin32_Init(WC.overlayhWnd);
	ImGui_ImplDX11_Init(d3d_device, device_context);
}


VOID Overlay::dest_imgui() 
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}



VOID Overlay::init_render_target() 
{
	ID3D11Texture2D* back_buffer = NULL;
	swap_chain->GetBuffer(0, IID_PPV_ARGS(&back_buffer));
	if (back_buffer)
		d3d_device->CreateRenderTargetView(back_buffer, NULL, &render_target_view);
	// todo: error here if it can't get the backbuffer of the render target
	back_buffer->Release();
}



VOID Overlay::dest_render_target() 
{
	if (!render_target_view)
		return;

	render_target_view->Release();
	render_target_view = NULL;
}


Overlay::Overlay() :
	window_width(WC.cs2Rect.right - WC.cs2Rect.left),
	window_height(WC.cs2Rect.bottom - WC.cs2Rect.top),
	font(NULL),
	d3d_device(NULL),
	device_context(NULL),
	swap_chain(NULL),
	render_target_view(NULL),
	draw_list(NULL),
	vkKey(NULL)
{
	font = new s_font();

	if (!init_device()) return; // error creating device

	init_imgui();

	std::thread(&Overlay::input_handler, this).detach();	
	if (cheats.Run()) this->CheatsCheck == TRUE;
	if (!this->CheatsCheck) Logger.Logger("Failed To Initialize Cheats Closing App");
}


Overlay::~Overlay() 
{
	dest_imgui();
	dest_device();

	delete font;
}



const void Overlay::render(FLOAT width, FLOAT height) 
{
	if (exit) 
    {
		ready = true;
		return;
	}
	this->Centre.x = window_width / 2;
	this->Centre.y = window_height / 2;
	ImGuiStyle& style = ImGui::GetStyle();

	////////////////////////////////////
	init_draw_list(); 

	
	ImGui::Begin("IceCoaleds Cs2 - External", 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);
	{
		ImGui::BeginTabBar("");

		if (ImGui::BeginTabItem("ESP & Visuals Menu", &MenuConfig::espTab, NULL))
		{
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Checkbox("Enable Bone ESP\n", &MenuConfig::show_Boneesp);
			ImGui::SliderFloat("Select Bone Thickness\n", &MenuConfig::boneThickness, MenuConfig::minBoneThickness, MenuConfig::maxBoneThickness);
			ImGui::Spacing();
			ImGui::Checkbox("Enable Box ESP\n", &MenuConfig::show_Boxesp);
			ImGui::SliderFloat("Select Box Thickness\n", &MenuConfig::boxThickness, MenuConfig::minBoxThickness, MenuConfig::maxBoxThickness);
			ImGui::Spacing();
			ImGui::Checkbox("Enable Distance\n", &MenuConfig::show_Distance);
			ImGui::SliderFloat("Select Max ESP Distance\n", &MenuConfig::espDistance, MenuConfig::minSmooth, MenuConfig::maxEspDistance);
			ImGui::Spacing();
			ImGui::Checkbox("Enable Player Name\n", &MenuConfig::show_PlayerName);
			ImGui::Spacing();
			ImGui::Checkbox("Enable Crosshair\n", &MenuConfig::show_Crosshair);
			ImGui::SliderFloat("Select Crosshair Size\n", &MenuConfig::CrossHairSize, 2.0f, 50.0f);
			ImGui::SliderFloat("Select Crosshair Line Thickness\n", &MenuConfig::CrosshairThickness, 2.0f, 50.0f);
			if (MenuConfig::show_Crosshair) crosshair(MenuConfig::CrossHairSize, MenuConfig::CrossHairColor);
			ImGui::Spacing();
			ImGui::Checkbox("Enable FOV\n", &MenuConfig::show_Fov);
			ImGui::SliderFloat("Select FOV Line Thickness\n", &MenuConfig::fovLineSize, 2.0f, 50.0f);
			ImGui::Spacing();
			ImGui::Checkbox("Enable DeadZone\n", &MenuConfig::show_deadZone);
			ImGui::SliderFloat("Select DeadZone Line Thickness\n", &MenuConfig::deadZoneLineSize, 2.0f, 50.0f);
			if (MenuConfig::show_deadZone)
				Overlay::Circle(Overlay::Centre, Aim::deadZoneFov, MenuConfig::DeadZoneColor, MenuConfig::deadZoneLineSize);

			ImGui::EndTabItem();
		}


		if (ImGui::BeginTabItem("AimBot", &MenuConfig::aimbotTab, NULL))
		{
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Checkbox("Enable Aimbot\n", &MenuConfig::enb_Aimbot);
			ImGui::Spacing();
			if (ImGui::Combo("Aim Key", &MenuConfig::aimKey, "LBUTTON\0MENU\0RBUTTON\0XBUTTON1\0XBUTTON2\0CAPITAL\0SHIFT\0CONTROL"))
			{
				Aim::Set_Hot_Key(MenuConfig::aimKey);
			}
			ImGui::SliderFloat("Select FOV\n", &Aim::aimFov, MenuConfig::minFov, MenuConfig::maxFov);
			ImGui::Spacing();
			ImGui::SliderFloat("Select Max AimBot Distance\n", &MenuConfig::aimDistance, MenuConfig::minSmooth, MenuConfig::maxAimDistance);
			ImGui::SliderFloat("Select Smoothing\n", &MenuConfig::smoothing, MenuConfig::minSmooth, MenuConfig::maxSmooth);
			ImGui::Spacing();
			ImGui::Combo("Target Selection", &MenuConfig::aimLock_select, "Closest to Crosshair\0Closest Target");
			ImGui::Checkbox("Enable VisCheck\n", &MenuConfig::enb_Vischeck);
			ImGui::Checkbox("Enable Team Check\n", &MenuConfig::enb_Teamcheck);
			ImGui::Spacing();
			if (ImGui::Combo("Aim Postion", &MenuConfig::aim_Select, "Head\0Neck\0Spine"))
			{
				switch (MenuConfig::aim_Select)
				{
				case 0:
					MenuConfig::bone_Select = BONEINDEX::head;
					break;
				case 1:
					MenuConfig::bone_Select = BONEINDEX::neck_0;
					break;
				case 2:
					MenuConfig::bone_Select = BONEINDEX::spine_1;
					break;
				default:
					break;
				}
			}
			ImGui::Separator();
			if(ImGui::BeginChild("Humanization", ImVec2(500, 200)))
			{
				ImGui::Checkbox("Enable Humanization\n", &MenuConfig::enb_Humanization);
				ImGui::Spacing();
				ImGui::SliderFloat("Select OverAim X axis\n", &Aim::overAimX, MenuConfig::minOveraim, MenuConfig::maxOveraim);
				ImGui::Spacing();
				ImGui::SliderFloat("Select OverAim Y axis\n", &Aim::overAimY, MenuConfig::minOveraim, MenuConfig::maxOveraim);
				ImGui::Spacing();
				ImGui::SliderFloat("Select DeadZone\n", &Aim::deadZoneFov, MenuConfig::mindeadZone, MenuConfig::maxdeadZone);
				
			}
			ImGui::EndChild();
			ImGui::EndTabItem();
		}

	
		
		if (ImGui::BeginTabItem("Configs", &MenuConfig::configTab, ImGuiConfigFlags_NavEnableKeyboard))
		{
			const std::string configDir = MenuConfig::path;
			static std::vector<std::string> configFiles;
			char configNameBuffer[MAX_PATH] = { 0 };
			static int selectedConfig = -1,
						  configNumber = 0;
			MenuConfig::path = Logger.docsPath;
			
	
			
			if (ImGui::Button("Create Config"))
			{
				std::ostringstream configFileName; 
				configFileName << "config" << configNumber << ".config";
				ConfigSaver::SaveConfig(configFileName.str());
			}
			ImGui::Separator();

			configFiles.clear();
			for (const auto& entry : std::filesystem::directory_iterator(configDir))
			{
				if (entry.is_regular_file() && entry.path().extension() == ".config")
				{
					configFiles.push_back(entry.path().filename().string());
				}
			}

			configNumber = configFiles.size();

			for (int i = 0; i < configFiles.size(); ++i)
			{
				if (ImGui::Selectable(configFiles[i].c_str(), selectedConfig == i))
				{
					selectedConfig = i;
				}
			}

			if (selectedConfig != -1)
			{
				ImGui::Text("Selected Config: %s", configFiles[selectedConfig].c_str());
			}

			if (ImGui::Button("Load Selected") && selectedConfig >= 0 && selectedConfig < configFiles.size())
			{
				std::string selectedConfigFile = configFiles[selectedConfig];
				ConfigSaver::LoadConfig(selectedConfigFile);
			}

			if (ImGui::Button("Save Selected") && selectedConfig >= 0 && selectedConfig < configFiles.size())
			{
				std::string selectedConfigFile = configFiles[selectedConfig];
				ConfigSaver::SaveConfig(selectedConfigFile);
			}

			ImGui::Separator();

			if (ImGui::Button("Delete Selected") && selectedConfig >= 0 && selectedConfig < configFiles.size())
			{
				std::string selectedConfigFile = configFiles[selectedConfig];
				std::string fullPath = configDir + "/" + selectedConfigFile;
				if (std::remove(fullPath.c_str()) == 0)
				{
					configFiles.erase(configFiles.begin() + selectedConfig);
					selectedConfig = -1;
				}
				else
				{
				}
			}

			if (ImGui::Button("Reset to Default"))
			{
				ConfigSaver::resetValues();
			}

			ImGui::EndTabItem();
		}
	


		if (ImGui::BeginTabItem("Color Selection", &MenuConfig::colorTab, NULL))
		{

			ImGui::ColorEdit4("##InvisibleBoneColor", reinterpret_cast<float*>(&MenuConfig::BoneColorInVis), ImGuiColorEditFlags_NoInputs);
			ImGui::SameLine();
			ImGui::ColorEdit4("Bone Color", reinterpret_cast<float*>(&MenuConfig::BoneColorVis), ImGuiColorEditFlags_NoInputs);
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::ColorEdit4("##InvisibleBoxColor", reinterpret_cast<float*>(&MenuConfig::BoxColorInVis), ImGuiColorEditFlags_NoInputs);
			ImGui::SameLine();
			ImGui::ColorEdit4("Box Color", reinterpret_cast<float*>(&MenuConfig::BoxColorVis), ImGuiColorEditFlags_NoInputs);
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::ColorEdit4("FOV Color", reinterpret_cast<float*>(&MenuConfig::FovLineColor), ImGuiColorEditFlags_NoInputs);
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::ColorEdit4("Crosshair Color", reinterpret_cast<float*>(&MenuConfig::CrossHairColor), ImGuiColorEditFlags_NoInputs);
			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::ColorEdit4("DeadZone Color", reinterpret_cast<float*>(&MenuConfig::DeadZoneColor), ImGuiColorEditFlags_NoInputs);


			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
		
	}ImGui::End();
}
	





BOOL Overlay::msg_loop() 
{
	if (WC.overlayhWnd) 
	{
		ImVec4 clear_clr = { 0,0,0,0 };
		MSG Msg = { 0 };

		if (::PeekMessage(&Msg, NULL, 0U, 0U, PM_REMOVE))
		{		
			::TranslateMessage(&Msg);
			::DispatchMessage(&Msg);
		}
		
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		if(show_menu)render(window_width, window_height);

		ImGui::Render();
		device_context->OMSetRenderTargets(1, &render_target_view, NULL);
		device_context->ClearRenderTargetView(render_target_view, (float*)&clear_clr); // might break, if so then put cheat color of type ImVec4 { 0,0,0,0 }
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		swap_chain->Present(1, 0);

		return Msg.message != WM_QUIT;
	}
	return false;
}

VOID Overlay::input_handler() 
{
	for (; !exit; Sleep(1)) 
	{
		//checking if game is still active
		Overlay::GameCheck();
		// Z order loop
		WC.ResetWindZ();	
		// panic key
		exit = GetAsyncKeyState(VK_END) & 0x8000;
		//showing menu
		if (GetAsyncKeyState(VK_INSERT) & 0x0001)
		{
			show_menu = !show_menu;
		}


		// for our imgui menu interaction
		ImGuiIO& io = ImGui::GetIO();
		if (MenuConfig::enb_Keyboard)
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		

		POINT p{};
		GetCursorPos(&p);
		io.MousePos = ImVec2((float)p.x, (float)p.y);

		io.MouseDown[0] = GetAsyncKeyState(VK_LBUTTON) & 0x8000;
		io.MouseDown[1] = GetAsyncKeyState(VK_RBUTTON) & 0x8000;
	}
}

VOID Overlay::GameCheck()
{
	for (; !exit; Sleep(1))
	{
		exit = GetAsyncKeyState(VK_END) & 0x8000;
		if (!WC.SingleInjection())
		{
			while (this->checkForGame = TRUE)
			{
				if (WC.SingleInjection()) this->checkForGame = TRUE;
			}
		}
	}
}

VOID Overlay::init_draw_list() 
{
	Overlay::draw_list = ImGui::GetBackgroundDrawList();
}

VOID Overlay::crosshair(const FLOAT aSize, ImU32 color) {
	draw_list->AddLine({ this->window_width / 2, this->window_height / 2 - (aSize + 1) }, { this->window_width / 2 , this->window_height / 2 + (aSize + 1) }, color, MenuConfig::CrosshairThickness);
	draw_list->AddLine({ this->window_width / 2 - (aSize + 1), this->window_height / 2 }, { this->window_width / 2 + (aSize + 1), this->window_height / 2 }, color, MenuConfig::CrosshairThickness);
}


void Overlay::ConnectPoints(std::vector<Vec2> Points, ImColor Color, float Thickness)
{
	if (Points.size() <= 0)
		return;
	for (int i = 0; i < Points.size() - 1; i++)
	{
		Line(Points[i], Points[i + 1], Color, Thickness);
		if (i == Points.size() - 2)
			Line(Points[i + 1], Points[0], Color, Thickness);
	}
}

void Overlay::Line(Vec2 From, Vec2 To, ImColor Color, float Thickness)
{
	ImGui::GetBackgroundDrawList()->AddLine(From.ToImVec2(), To.ToImVec2(), Color, Thickness);
}

void Overlay::Rectangle(Vec2 Pos, Vec2 Size, ImColor Color, float Thickness, float Rounding)
{
	ImGui::GetBackgroundDrawList()->AddRect(Pos.ToImVec2(), { Pos.x + Size.x,Pos.y + Size.y }, Color, Rounding, 0, Thickness);
}


void Overlay::Circle(Vec2 Center, float Radius, ImColor Color, float Thickness, int Num)
{
	ImGui::GetBackgroundDrawList()->AddCircle(Center.ToImVec2(), Radius, Color, Num, Thickness);
}

void Overlay::StrokeText(std::string Text, Vec2 Pos, ImColor Color, float FontSize, bool KeepCenter)
{
	Overlay::Text(Text, Vec2(Pos.x - 1, Pos.y + 1), ImColor(0, 0, 0), FontSize, KeepCenter);
	Overlay::Text(Text, Vec2(Pos.x - 1, Pos.y - 1), ImColor(0, 0, 0), FontSize, KeepCenter);
	Overlay::Text(Text, Vec2(Pos.x + 1, Pos.y + 1), ImColor(0, 0, 0), FontSize, KeepCenter);
	Overlay::Text(Text, Vec2(Pos.x + 1, Pos.y - 1), ImColor(0, 0, 0), FontSize, KeepCenter);
	Overlay::Text(Text, Pos, Color, FontSize, KeepCenter);
}

VOID Overlay::Text(std::string Text, Vec2 Pos, ImColor Color, float FontSize, bool KeepCenter)
{
	if (!KeepCenter)
	{
		ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), FontSize, Pos.ToImVec2(), Color, Text.c_str());
	}
	else
	{
		float TextWidth = ImGui::GetFont()->CalcTextSizeA(FontSize, FLT_MAX, 0.f, Text.c_str()).x;
		ImVec2 Pos_ = { Pos.x - TextWidth / 2,Pos.y };
		ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), FontSize, Pos_, Color, Text.c_str());
	}
}



///esp functions
void Overlay::DrawFovCircle(const Entity& LocalEntity)
{

	float Radius = tan(Aim::aimFov / 180.0f * M_PI / 2.0f) / tan(LocalEntity.Pawn.Fov / 180.0f * M_PI / 2.0f) * (Overlay::Centre.x * 2.0f);
	Overlay::Circle(Overlay::Centre, Radius, MenuConfig::FovLineColor, MenuConfig::fovLineSize);
}


void Overlay::DrawDistance(const Entity& LocalEntity, Entity& Entity, ImVec4 Rect)
{
	int distance = static_cast<int>(Entity.Pawn.ScreenPos.DistanceTo(LocalEntity.Pawn.ScreenPos) / 100);
	std::string dis_str = Format("%im", distance);
	Overlay::StrokeText(dis_str, { Rect.x + Rect.z + 4, Rect.y }, ImColor(255, 255, 255, 255), 14, false);
}

ImVec4 Overlay::Get2DBox(const Entity& Entity)
{
	BoneJointPos Head = Entity.get_Bone().BonePosList[BONEINDEX::head];

	Vec2 Size, Pos;
	Size.y = (Entity.Pawn.ScreenPos.y - Head.ScreenPos.y) * 1.09f;
	Size.x = Size.y * 0.6f;

	Pos = ImVec2(Entity.Pawn.ScreenPos.x - Size.x / 2, Head.ScreenPos.y - Size.y * 0.08f);

	return ImVec4{ Pos.x,Pos.y,Size.x,Size.y };
}

void Overlay::DrawBone(const Entity& Entity, ImColor Color, float Thickness)
{
	BoneJointPos Previous, Current;
	for (auto i : BoneJointList::List)
	{
		Previous.Pos = Vec3(0, 0, 0);
		for (auto Index : i)
		{
			Current = Entity.get_Bone().BonePosList[Index];
			if (Previous.Pos == Vec3(0, 0, 0))
			{
				Previous = Current;
				//pGame->View->Gui->Text(Current.Name.c_str(), Current.ScreenPos, ImColor(255, 255, 0, 255));
				continue;
			}
			if (Previous.IsVisible && Current.IsVisible)
			{
				Overlay::Line(Previous.ScreenPos, Current.ScreenPos, Color, Thickness);
				//pGame->View->Gui->Text(Current.Name.c_str(), Current.ScreenPos, ImColor(255, 255, 0, 255));
			}
			Previous = Current;
		}
	}
}

ImVec4 Overlay::Get2DBoneRect(const Entity& Entity)
{
	Vec2 Min, Max, Size;
	Min = Max = Entity.get_Bone().BonePosList[0].ScreenPos;

	for (auto& BoneJoint : Entity.get_Bone().BonePosList)
	{
		if (!BoneJoint.IsVisible)
			continue;
		Min.x = min(BoneJoint.ScreenPos.x, Min.x);
		Min.y = min(BoneJoint.ScreenPos.y, Min.y);
		Max.x = max(BoneJoint.ScreenPos.x, Max.x);
		Max.y = max(BoneJoint.ScreenPos.y, Max.y);
	}
	Size.x = Max.x - Min.x;
	Size.y = Max.y - Min.y;

	return ImVec4(Min.x, Min.y, Size.x, Size.y);
}
 

////////////////////////////////////////////////


BOOL Overlay::Cheats::Run()
{
	
	if (!pGame.InitGameData())
	{
		Logger.Logger("Main Thread Closed Due To Game Data Failure\n");
		return FALSE;
	}
	if (!pGame.CheckBuild())
	{
		Logger.Logger("Closing New Game Build, Please Update Offsets");
		return FALSE;
	}
	std::thread(&Cheats::MainLoop, this).detach();
	std::thread(&Cheats::ESP, this).detach();
	std::thread(&Cheats::AimBot, this).detach();

	return TRUE;
}


VOID Overlay::Cheats::MainLoop()
{
	std::mutex mutex;
	std::unique_lock lock{ mutex, std::defer_lock };
	
	while (Overlay::exit)
	{
		if (!WC.gameCheck)
		{
			lock.lock();
			while (WC.gameCheck == FALSE)
			{
				std::this_thread::sleep_for(5000ms);
			}
			lock.unlock();
		}

		int errorCount{};
		if (!pGame.Update_Matrix())
		{
			Logger.Logger("Failed To Update Matrix");
			errorCount++;

			if (errorCount == 10)
			{
				Logger.Logger("Closing Cheats From To Many Errors");
				this->exitStatus = 2;
			}

		}

		if (!pGame.Update_Entity_Entry())
		{
			Logger.Logger("Failed To Update Entity");

			errorCount++;

			if (errorCount == 10)
			{
				Logger.Logger("Closing Cheats From To Many Errors");
				this->exitStatus = 2;
			}
		}


		std::ptrdiff_t localController = pGame.Get_Local_Controller();
		std::ptrdiff_t localPawn = pGame.Get_Local_Pawn();
		if (localController == 0 || localPawn == 0)
		{
			Logger.Logger("Failed To Get Local Pawn, Or Local Controller");
			errorCount++;

			if (errorCount == 10)
			{
				Logger.Logger("Closing Cheats From To Many Errors");
				this->exitStatus = 2;
			}
		}

		Entity localEntity = { 0 };
		static int localPlayerIndex = 1;

		if (!localEntity.Update_Controller_Data(localController))
		{
			Logger.Logger("Failed To Update Local Controller");
			errorCount++;

			if (errorCount == 10)
			{
				Logger.Logger("Closing Cheats From To Many Errors");
				this->exitStatus = 2;
			}
		}

		if (!localEntity.Update_Pawn_Data(localPawn))
		{
			Logger.Logger("Failed To Update Local Pawn");
			errorCount++;

			if (errorCount == 10)
			{
				Logger.Logger("Closing Cheats From To Many Errors");
				this->exitStatus = 2;
			}
		}

		int playerIndex = 0;
		int playerDistance = 0;
		float distanceToSight = 0.f;
		Vec3 HeadPos = { 0,0,0 };
		Vec3 AimPos = { 0,0,0 };

		while (playerIndex < _MAX_PLAYERS)
		{
			while (playerIndex < _MAX_PLAYERS)
			{
				Entity Entity = { 0 };
				DWORD64 EntityAddress = driver.fetch_Data<DWORD64>(pGame.Update_Entity_Entry(), ((static_cast<DWORD64>(playerIndex) + 1) * 0x78));
				if (EntityAddress == 0)  continue;

				if (EntityAddress == localEntity.Controller.Address)
				{
					localPlayerIndex = playerIndex;
					continue;
				}

				if (!Entity.Update_Controller_Data(EntityAddress))  continue;

				if (!Entity.Update_Pawn_Data(Entity.Pawn.Address))  continue;

				if (MenuConfig::enb_Teamcheck && Entity.Controller.TeamId == localEntity.Controller.TeamId)  continue;

				if (!Entity.is_Alive())  continue;

				if (!Entity.on_Screen()) continue;

				if (MenuConfig::enb_Humanization) cheats.crosshairDelta = Aim::CurrentCurorPos();

				distanceToSight = Entity.get_Bone().BonePosList[BONEINDEX::head].ScreenPos.DistanceTo(Overlay::Centre);

				playerDistance = static_cast<int>(Entity.Pawn.ScreenPos.DistanceTo(localEntity.Pawn.ScreenPos) / 100);

				
				//For ESP Thread
				if (playerDistance < MenuConfig::espDistance)
				{
					lock.lock();
					this->espInput[0] = Entity;
					this->espInput[1] = localEntity;
					lock.unlock();
				}


				//For Aimbot Thread
				if (playerDistance < MenuConfig::aimDistance)
				{
					if (!MenuConfig::enb_Vischeck
						&& Entity.Pawn.Spotted && Entity.Pawn.Spotted > 0)
					{
						AimPos = Entity.get_Bone().BonePosList[MenuConfig::bone_Select].Pos;
						if (MenuConfig::bone_Select == BONEINDEX::head)AimPos.z -= 1.f;
					}

					if (this->crosshair.distanceToSight == 0 || this->distance.playerDistance == 0)
					{
						this->crosshair.distanceToSight = distanceToSight;
						this->crosshair.AimPosition = AimPos;
						this->crosshair.localEntity = localEntity;

						this->distance.playerDistance = distanceToSight;
						this->distance.AimPosition = AimPos;
						this->distance.localEntity = localEntity;
					}


					if (distanceToSight < crosshair.distanceToSight)
					{
						this->crosshair.playerDistance = distanceToSight;
						this->crosshair.AimPosition = AimPos;
						this->crosshair.localEntity = localEntity;
					}
					else if (playerDistance < distance.playerDistance)
					{
						this->distance.playerDistance = distanceToSight;
						this->distance.AimPosition = AimPos;
						this->distance.localEntity = localEntity;
					}
				}

				lock.lock();
				if (MenuConfig::aimLock_select == 0)
				{
					this->aimbotInput = crosshair;
				}
				else
					this->aimbotInput = distance;

				lock.unlock();
			}

			playerIndex++;
		}
	}
}

VOID Overlay::Cheats::ESP() const
{
	ImVec4 espBox{};
	std::mutex mutex;
	std::unique_lock lock{ mutex, std::defer_lock };

	while (Overlay::exit)
	{

		if (!WC.gameCheck)
		{
			lock.lock();
			while (WC.gameCheck == FALSE)
			{
				std::this_thread::sleep_for(25000ms);
			}
			lock.unlock();
		}

		lock.lock();
		
		espBox = Overlay::Get2DBox(this->espInput[0]);

		if (MenuConfig::show_Boneesp && espInput[0].Pawn.Spotted > 0)
		{
			Overlay::DrawBone(espInput[0], MenuConfig::BoneColorVis, MenuConfig::boneThickness);
		}
		else if (MenuConfig::show_Boneesp)
		{
			Overlay::DrawBone(espInput[0], MenuConfig::BoneColorInVis, MenuConfig::boneThickness);
		}

		if (MenuConfig::show_Boxesp && espInput[0].Pawn.Spotted > 0)
		{
			Overlay::Rectangle({ espBox.x,espBox.y }, { espBox.z,espBox.w }, MenuConfig::BoxColorVis, 1.3);
		}
		else if (MenuConfig::show_Boxesp)
		{
			Overlay::Rectangle({ espBox.x,espBox.y }, { espBox.z,espBox.w }, MenuConfig::BoxColorInVis, 1.3);
		}


		if (MenuConfig::show_Fov)
		{
			Overlay::DrawFovCircle(espInput[1]);
		}

		if (MenuConfig::show_PlayerName)
			Overlay::StrokeText(espInput[0].Controller.playerName, { espBox.x + espBox.z / 2,espBox.y - 13 - 14 }, ImColor(255, 255, 255, 255), 14, true);

		if (MenuConfig::show_Distance) Overlay::DrawDistance(espInput[1], espInput[0], espBox);

		lock.unlock();
	}

}



VOID Overlay::Cheats::AimBot()
{
	std::mutex mutex;
	std::unique_lock lock{ mutex, std::defer_lock };

	while (Overlay::exit)
	{
		
		if (!WC.gameCheck)
		{
			lock.lock();
			while (WC.gameCheck == FALSE)
			{
				std::this_thread::sleep_for(25000ms);	
			}
			lock.unlock();
		}
		
		
		
		if (MenuConfig::enb_Aimbot && GetAsyncKeyState(Aim::hotKey))
		{
			lock.lock();
			if (Cheats::aimbotInput.AimPosition != Vec3(0, 0, 0))
				Aim::AimBot(aimbotInput.localEntity, aimbotInput.localEntity.Pawn.CameraPos, aimbotInput.AimPosition, this->crosshairDelta);
			lock.unlock();
		}	
	}	
}


