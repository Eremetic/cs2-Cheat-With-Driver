#pragma once
#include "Aimbot.hpp"
#include <map>
#include "menuConfig.hpp"
#include <mutex>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dcompiler")



class Overlay 
{
private:
	
	 ID3D11Device* d3d_device;
	 ID3D11DeviceContext* device_context;
	 IDXGISwapChain* swap_chain;
	 ID3D11RenderTargetView* render_target_view;
	 ImDrawList* draw_list;
	

	struct s_font 
	{
		ImFont* im_font;
		FLOAT font_size;
	};
private:
	template <typename T>
	inline VOID safe_release(T*& p);

	BOOL init_device();
	VOID dest_device();

	VOID init_imgui();
	VOID dest_imgui();

	VOID init_render_target();
	VOID dest_render_target();
	const VOID render(FLOAT width, FLOAT height);

	VOID input_handler();
	VOID GameCheck();

public:
	Overlay();
	~Overlay();
	BOOL msg_loop();
	

	 VOID init_draw_list();	
	inline VOID crosshair(const FLOAT aSize, ImU32 color);
	static inline VOID Line(Vec2 From, Vec2 To, ImColor Color, float Thickness);
	static inline VOID Rectangle(Vec2 Pos, Vec2 Size, ImColor Color, float Thickness, float Rounding = 0);
	static inline VOID Circle(Vec2 Center, float Radius, ImColor Color, float Thickness, int Num = 50);
	inline VOID ConnectPoints(std::vector<Vec2> Points, ImColor Color, float Thickness);
	static inline VOID StrokeText(std::string Text, Vec2 Pos, ImColor Color, float FontSize = 15, bool KeepCenter = false);
	static inline VOID Text(std::string Text, Vec2 Pos, ImColor Color, float FontSize = 15, bool KeepCenter = false);
	
public:
	FLOAT window_width, window_height;
	inline static bool exit = FALSE, ready = FALSE, show_menu = TRUE, CheatsCheck = FALSE;
	s_font* font = NULL;
	static inline Vec2 Centre;
	static inline boolean checkForGame = TRUE;
	SHORT  vkKey;

public:
	///esp functions
	static inline VOID DrawFovCircle(const Entity& LocalEntity);
	static inline VOID DrawDistance(const Entity& LocalEntity, Entity& Entity, ImVec4 Rect);
	static inline ImVec4 Get2DBox(const Entity& Entity);
	static inline VOID DrawBone(const Entity& Entity, ImColor Color, float Thickness);
	static inline ImVec4 Get2DBoneRect(const Entity& Entity);



	class Cheats
	{
	public:
		static inline int exitStatus{};

		typedef struct _AimBotInput
		{
			Entity localEntity;
			Vec3 AimPosition;
			int playerDistance;
			float distanceToSight;
		}AB, * PAB;

		static inline Entity espInput[2] = { 0 };
		static inline AB aimbotInput = { 0 };
		static inline AB distance = { 0 };
		static inline AB crosshair = { 0 };
		static inline POINT crosshairDelta{};


		VOID MainLoop();
		VOID ESP() const;
		VOID AimBot();
		BOOL Run();

	};
};

inline Overlay::Cheats cheats;




