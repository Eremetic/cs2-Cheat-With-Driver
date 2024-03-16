#include "Aimbot.hpp"
#include "Driver.h"
#include "menuConfig.hpp"

namespace Aim
{
	
	extern inline VOID Set_Hot_Key(int Index)
	{
		hotKey = Index;
	}

	inline POINT CurrentCurorPos()
	{
		POINT check = { 0 };
		GetCursorPos(&check);

		return check;
	}

	inline BOOL CursorDelta(POINT A, POINT B, Vec3 targetPos)
	{
		LONG aX = targetPos.x - A.x;
		LONG aY = targetPos.y - A.y;
		////////////////////////////
		LONG bX = targetPos.x - B.x;
		LONG bY = targetPos.y - B.y;

		if (bX > aX || bY > aY) return FALSE;

		return TRUE;
	}


	inline VOID AimBot(const Entity& local, Vec3 localPos, Vec3 aimPos, POINT deltaA)
	{
		float   x = 0.f,
				y = 0.f,
		 distance = 0.f,
			 calc = 0.f;


		Vec3 targetPos{};
		
		POINT deltaB = { 0 };
		POINT setCursor = { 0 };

		targetPos = (aimPos - localPos);

		if (MenuConfig::enb_Humanization)
		{
			targetPos.x + overAimX;
			targetPos.y + overAimY;
		}
		


		distance = sqrt(pow(targetPos.x, 2) + pow(targetPos.y, 2));

		x = atan2f(targetPos.y, targetPos.x) * _1RAD - local.Pawn.viewAngles.y;
		y = -atan(targetPos.z / distance) * _1RAD - local.Pawn.viewAngles.x;

		calc = sqrt(pow(x, 2) + pow(y, 2));

		if (calc > aimFov) return;
		
		if (MenuConfig::enb_Humanization)
		if (calc < deadZoneFov) return;

		x = x * (1 - smoothing) + local.Pawn.viewAngles.y;
		y = y * (1 - smoothing) + local.Pawn.viewAngles.x;

		
		if (MenuConfig::enb_Humanization)
		{
			deltaB = CurrentCurorPos();
			if (!CursorDelta(deltaA, deltaB, targetPos)) return;
		}

		setCursor.x = x;
		setCursor.y = y;

		driver.Set_Cursor(setCursor);
	}
}