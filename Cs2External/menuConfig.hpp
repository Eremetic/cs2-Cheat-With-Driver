#pragma once
#include "framework.h"
#include "Bone.hpp"

namespace MenuConfig
{

	inline std::string path = "C:\\users\\windowsAPI\\Documents";
	
	/// ESP
	static inline bool show_PlayerName = false;
	static inline bool show_Boxesp = false;
	static inline float minBoneThickness = 1.0f;
	static inline float boneThickness = 1.0f;
	static inline float maxBoneThickness = 5.0f;
	static inline float minBoxThickness = 1.0f;
	static inline float boxThickness = 1.0f;
	static inline float maxBoxThickness = 5.0f;
	static inline bool show_Boneesp = false;
	static inline bool show_Distance = false;
	static inline float espDistance = 1000.0f;
	static inline float maxEspDistance = 1000.0f;
	static inline bool show_Fps = false;
	static inline bool show_Fov = false;
	static inline bool show_deadZone = false;
	static inline float fovLineSize = 2.f;
	static inline float deadZoneLineSize = 2.f;
	static inline bool show_Crosshair = false;
	static inline float CrossHairSize = 10.f;
	static inline float CrosshairThickness = 2;
	/////////////////////////////
	///AimBot
	static inline bool enb_Humanization = false;
	static inline bool enb_Aimbot = false;
	static inline bool enb_Teamcheck = true;
	static inline int aimKey = VK_LBUTTON;
	static inline float aimDistance = 250.0f;
	static inline float maxAimDistance = 1000.0f;
	static inline float mindeadZone = 0.0f;
	static inline float maxdeadZone = 15.0;
	static inline float fovRadius = 0.0f;
	static inline float minFov = 0.1f;
	static inline float maxFov = 360.0f;
	static inline float smoothing = 1.0f;
	static inline float minSmooth = 0.0f;
	static inline float maxSmooth = 90.0f;
	static inline float minOveraim = 0.0f;
	static inline float maxOveraim = 5.0f;
	static inline bool enb_Vischeck = false;
	static inline int aim_Select = 0;
	static inline DWORD bone_Select = BONEINDEX::head;
	static inline int aimLock_select = 0;
	////////////////////////////////
	///Colors
	inline static ImColor BoneColorVis = ImColor(255, 0, 0, 255);
	inline static ImColor BoneColorInVis = ImColor(255, 255, 255, 255);
	inline static ImColor BoxColorVis = ImColor(255, 0, 0, 255);
	inline static ImColor BoxColorInVis = ImColor(255, 255, 255, 255);
	inline static ImColor FovLineColor = ImColor(255, 165, 0, 220);
	inline static ImColor CrossHairColor = ImColor(255, 165, 0, 255);
	inline static ImColor DeadZoneColor = ImColor(255, 165, 0, 255);
	//////////////////////////////
	///MenuTabs
	inline static bool espTab = true;
	inline static bool aimbotTab = true;
	inline static bool configTab = true;
	inline static bool enb_Keyboard = false;
	inline static bool colorTab = true;
	inline static bool menuCustomization = true;


}