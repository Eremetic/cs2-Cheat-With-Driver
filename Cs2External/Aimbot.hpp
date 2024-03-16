#pragma once
#include "updataData.h"

namespace Aim
{
	///space holders
	static inline int hotKey = VK_LBUTTON; 
	static inline float	   aimFov = 10.0f,
					   deadZoneFov = 0.0f,
						 smoothing = 0.7f,
						  overAimX = 0.0f,
						  overAimY = 0.0f;
	
	extern inline VOID Set_Hot_Key(int Index);
	inline POINT CurrentCurorPos();
	inline BOOL CursorDelta(POINT A, POINT B, Vec3 targetPos);
	extern inline VOID AimBot(const Entity& local, Vec3 localPos, Vec3 aimPos, POINT crosshairCheck);
};
