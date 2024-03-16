#include "game.h"
#include "Driver.h"
#include <sstream>
#include "utils.hpp"


BOOL Game::InitGameData()
{
	std::ostringstream errorLog;
	if (!this->fetch_base_Address())
	{
		Logger.Logger("Failed To Update Dll Base Addresses\n");
		errorLog << "Client dll Base : " << Game::clientBase << "\n";
		Logger.Logger(errorLog.str());
		errorLog.clear();

		errorLog << "Engine dll Base : " << Game::engineBase << "\n";
		Logger.Logger(errorLog.str());
		errorLog.clear();
		return FALSE;
	}

	this->Get_Local_Controller();
	this->Get_Local_Pawn();

	if (Game::Address.LocalController == 0 || Game::Address.LocalPawn == 0)
	{		
		Logger.Logger("Failed To Update Local Player Addresses\n");
		errorLog << "Local Controller : " << Game::Address.LocalController << "\n";		
		Logger.Logger(errorLog.str());
		errorLog.clear();

		errorLog << "Local Pawn : " << Game::Address.LocalPawn << "\n";
		Logger.Logger(errorLog.str());
		errorLog.clear();

		return FALSE;
	}

	return TRUE;
}



BOOL Game::Update_Entity_Entry()
{
	DWORD64 Entry = 0;

	Entry = driver.fetch_Data<DWORD64>(clientBase, Offset::dwEntityList);

	Entry = driver.fetch_Data<DWORD64>(Entry, 0x10);

	if (!Entry)
	{
		Logger.Logger("Failed To Update Current Entity\n");
		return FALSE;
	}

	this->Address.EntityListEntry = Entry;
	
	return TRUE;
}


BOOL Game::fetch_base_Address()
{

	DWORD64 clientBuffer = 0;
	DWORD64 engineBuffer = 0;

	driver.Get_Proc_Info(&clientBuffer, &engineBuffer);

	
	pGame.clientBase = clientBuffer;
	pGame.engineBase = engineBuffer;
	
	if (clientBuffer == 0 || engineBuffer == 0)
	{
		return FALSE;
	}
	

	return TRUE;
}

std::ptrdiff_t Game::Get_Local_Controller()
{
	return driver.fetch_Data<std::ptrdiff_t>(Game::clientBase, Offset::dwLocalPlayerController);
}

std::ptrdiff_t Game::Get_Local_Pawn()
{
	return driver.fetch_Data<std::ptrdiff_t>(Game::clientBase, Offset::dwLocalPlayerPawn);
}

BOOL Game::CheckBuild()
{
	std::ptrdiff_t  buildNum = driver.fetch_Data<std::ptrdiff_t>(Game::clientBase, Offset::dwBuildNumber);
	
	if (buildNum != Game::currentBuild) return FALSE;

	return TRUE;
}


BOOL Game::Update_Matrix()
{
	driver.Read_To_Buffer((Game::clientBase + Offset::dwViewMatrix), _MATRIX, U_PTR(&Game::View.Matrix));

	return TRUE;
}