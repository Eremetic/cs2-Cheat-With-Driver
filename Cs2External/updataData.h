#pragma once
#include "offsets.hpp"
#ifndef BONE_H
#include "Bone.hpp"
#else
#include "structs.hpp"
#endif



class PlayerController
{
public:
	DWORD64 Address = 0,
			 TeamId = 0,
			 Health = 0,
			  Alive = 0,
			   Pawn = 0;

	std::string playerName;

	BOOL     Get_Team_Id(),
			  Get_Health(),
		Get_Alive_Status();
	
	DWORD64 Get_Pawn_Address();
	std::string Get_Player_Name();
		
};





class PlayerPawn
{
public:	
	DWORD64 Address = 0;
	   Vec3 position;
	  Vec2 ScreenPos;
	  Vec2 viewAngles;
	  Vec3 CameraPos;
	  CBone BoneData;
	DWORD64	 Health = 0,
			 TeamId = 0,
				Fov = 0,
		    Spotted = 0;


	BOOL	 Get_Spotted(),
		Get_Viewangles(),
		Get_Team_Id(),
		Get_Health(),
		Get_Position(),
		Get_Fov(),
		Get_CameraPos();
};

class Entity
{
public:
	PlayerController Controller;
	PlayerPawn Pawn;
	CBone boneArray;

	BOOL Update_Controller_Data(const DWORD64& PlayerControllerAddress),
		Update_Pawn_Data(const DWORD64& PlayerPawnAddress);

	CBone get_Bone() const;
	 BOOL is_Alive(),
		 on_Screen();
};


