#include "updataData.h"
#include "Driver.h"
#include "utils.hpp"
#include "game.h"


BOOL Entity::Update_Controller_Data(const DWORD64& PlayerPawnAddress)
{
	std::string errorLog;
	boolean check = TRUE;

	this->Controller.Address = PlayerPawnAddress;

	if (!this->Controller.Get_Health())
	{
		check = FALSE;
		Logger.Logger("Failed To Update Controller Health\n");
	}

	if (!this->Controller.Get_Alive_Status()) 
	{
		check = FALSE;
		Logger.Logger("Failed To Update Controller Alive Status\n");
	}

	if (!this->Controller.Get_Team_Id())
	{
		check = FALSE;
		Logger.Logger("Failed To Update Controller Team Id\n");
	}

	this->Controller.Get_Player_Name();
	if (this->Controller.playerName.empty()) 
	{
		check = FALSE;
		Logger.Logger("Failed To Update Controller Player Name\n");
	}
	
	this->Controller.Pawn = this->Controller.Get_Pawn_Address();
	if (!this->Controller.Pawn) 
	{
		check = FALSE;
		Logger.Logger("Failed To Update Controller Pawn Address\n");
	}

	if (!check) return FALSE;
	

	return TRUE;
}

BOOL Entity::Update_Pawn_Data(const DWORD64& PlayerPawnAddress)
{
	std::string errorLog;
	boolean check = TRUE;
	
	this->Pawn.Address = PlayerPawnAddress;

	if (!this->Pawn.Get_Health())
	{
		check = FALSE;
		errorLog = "Failed To Update Pawn Health\n";
	}
	
	if (!this->Pawn.Get_Spotted())
	{
		check = FALSE;
		Logger.Logger("Failed To Update Pawn Spotted Status\n");
	}

	if (!this->Pawn.Get_Position()) 
	{
		check = FALSE;
		Logger.Logger("Failed To Update Pawn Position\n");
	}

	if (!this->Pawn.Get_CameraPos()) 
	{
		check = FALSE;
		Logger.Logger("Failed To Update Pawn Camera Position\n");
	}

	if (!this->Pawn.Get_Team_Id())
	{
		check = FALSE;
		Logger.Logger("Failed To Update Pawn Team Id\n");
	}

	if (!check) return FALSE;
	
	return TRUE;
}



BOOL PlayerController::Get_Team_Id()
{
	DWORD64 temp = driver.fetch_Data<DWORD64>(Address, Offset::Entity::TeamID);
	if (temp == 0) return FALSE;
	PlayerController::TeamId = temp;
	return TRUE;
}

BOOL PlayerController::Get_Health()
{
	DWORD64 temp = driver.fetch_Data<DWORD64>(Address, Offset::Entity::Health);
	if (temp == 0) return FALSE;
	PlayerController::Health = temp;
	return TRUE;
}

BOOL PlayerController::Get_Alive_Status()
{
	DWORD64 temp = driver.fetch_Data<DWORD64>(Address, Offset::Entity::Alive);
	if (temp == 0) return FALSE;
	PlayerController::Alive = temp;
	return TRUE;
}

std::string PlayerController::Get_Player_Name()
{
	std::string buffer;
	buffer.reserve(MAX_PATH);
	
	driver.Read_To_Buffer((Address + Offset::Entity::PlayerName), MAX_PATH, U_PTR(&buffer));

	return buffer;
}



BOOL PlayerPawn::Get_Position()
{
	Vec3 temp{};
	
	temp.x = driver.fetch_Data<float>(Address, Offset::Pawn::Pos);
	temp.y = driver.fetch_Data<float>(Address, (Offset::Pawn::Pos + sizeof(float)));
	temp.z = driver.fetch_Data<float>(Address, (Offset::Pawn::Pos + (sizeof(float) * 2)));

	if (temp.x == 0 || temp.y == 0 || temp.z == 0) return FALSE;
	PlayerPawn::position = temp;
	return TRUE;
}

BOOL PlayerPawn::Get_Fov()
{
	DWORD64 cameraServices = driver.fetch_Data<DWORD64>(Address, Offset::Pawn::CameraServices);
	DWORD64 temp = driver.fetch_Data<DWORD64>(cameraServices, Offset::Pawn::iFovStart);
	if (temp == 0) return FALSE;
	PlayerPawn::Fov = temp;
	return TRUE;
}

BOOL PlayerPawn::Get_Viewangles()
{
	Vec2 temp{};
	
	temp.x = driver.fetch_Data<float>(Address, Offset::Pawn::EyeAngles);
	temp.y = driver.fetch_Data<float>(Address, (Offset::Pawn::EyeAngles + sizeof(float)));

	if (temp.x == 0 || temp.y == 0) return FALSE;
	PlayerPawn::viewAngles = temp;
	return TRUE;
}

BOOL PlayerPawn::Get_CameraPos()
{
	Vec3 temp{};

	temp.x = driver.fetch_Data<float>(Address, Offset::Pawn::LastClipCameraPos);
	temp.y = driver.fetch_Data<float>(Address, (Offset::Pawn::LastClipCameraPos + sizeof(float)));
	temp.z = driver.fetch_Data<float>(Address, (Offset::Pawn::LastClipCameraPos + (sizeof(float) * 2)));

	if (temp.x == 0 || temp.y == 0 || temp.z == 0) return FALSE;
	PlayerPawn::CameraPos = temp;
	return TRUE;
}

BOOL PlayerPawn::Get_Health()
{
	DWORD64 temp = driver.fetch_Data<DWORD64>(Address, Offset::Pawn::CurrentHealth);
	if (temp == 0) return FALSE;
	PlayerPawn::Health = temp;
	return TRUE;
}

BOOL PlayerPawn::Get_Team_Id()
{
	DWORD64 temp = driver.fetch_Data<DWORD64>(Address, Offset::Pawn::iTeamNum);
	if (temp == 0) return FALSE;
	PlayerPawn::TeamId = temp;
	return TRUE;
}

BOOL PlayerPawn::Get_Spotted()
{
	DWORD64 temp = driver.fetch_Data<DWORD64>(Address, Offset::Pawn::SpottedByMask);
	if (temp == 0) return FALSE;
	PlayerPawn::Spotted = temp;
	return TRUE;
}



DWORD64 PlayerController::Get_Pawn_Address()
{
	DWORD64 pawnEntry = 0, pawnAddress = 0;

	pawnAddress = driver.fetch_Data<DWORD64>(Address, Offset::Entity::PlayerPawn);

	pawnEntry = driver.fetch_Data<DWORD64>(pGame.clientBase, Offset::dwEntityList);

	pawnEntry = driver.fetch_Data<DWORD64>(pawnEntry, (0x10 + 8 * ((Pawn & 0x7FFF) >> 9)));

	pawnAddress = driver.fetch_Data<DWORD64>(pawnEntry, (0x78 * (Pawn & 0x1FF)));

	return pawnAddress;
}


BOOL Entity::is_Alive()
{
	if (this->Controller.Alive == 1 && this->Pawn.Health > 0) return TRUE;

	return FALSE;
}

CBone Entity::get_Bone() const
{
	if (this->Pawn.Address == 0) return CBone{};

	return this->Pawn.BoneData;
}


BOOL Entity::on_Screen()
{
	return pGame.View.WorldToScreen(this->Pawn.position, this->Pawn.ScreenPos);
}