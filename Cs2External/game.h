#pragma once
#include "framework.h"
#include "offsets.hpp"
#include "windowCreate.hpp"
#include "structs.hpp"


class CView
{
public:
	float Matrix[4][4]{};
	float Width = 0, Height = 0;
	

	bool WorldToScreen(const Vec3 &Pos, Vec2 &ToPos)
	{
		float View = 0.f;
	
		Width = (float)(WC.cs2Rect.right - WC.cs2Rect.left);
		Height = (float)(WC.cs2Rect.bottom - WC.cs2Rect.top);

		float SightX = Width / 2, SightY = Height / 2;

		View = Matrix[3][0] * Pos.x + Matrix[3][1] * Pos.y + Matrix[3][2] * Pos.z + Matrix[3][3];

		if (View <= 0.01) return FALSE;

		ToPos.x = SightX + (Matrix[0][0] * Pos.x + Matrix[0][1] * Pos.y + Matrix[0][2] * Pos.z + Matrix[0][3]) / View * SightX;
		ToPos.y = SightY - (Matrix[1][0] * Pos.x + Matrix[1][1] * Pos.y + Matrix[1][2] * Pos.z + Matrix[1][3]) / View * SightY;

		return TRUE;
	}
};



class Game
{
public:
	
	DWORD64 clientBase = 0, engineBase = 0;
	struct _Address
	{
		std::ptrdiff_t 
		   BuildNumber,
			EntityList,
			    Matrix,
			ViewAngles,
	   EntityListEntry,
	   LocalController,
			 LocalPawn;
	}Address;
	
	std::ptrdiff_t currentBuild = 0x36AD;

	CView View;

	BOOL CheckBuild();
	BOOL InitGameData();
	
	std::ptrdiff_t
		Get_Local_Controller(),
		Get_Local_Pawn();

	BOOL Update_Matrix();

	BOOL Update_Entity_Entry();
	BOOL fetch_base_Address();
};

inline Game pGame;



