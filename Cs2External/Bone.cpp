#include "Bone.hpp"
#include "Driver.h"
#include "offsets.hpp"
#include <array>


BOOL CBone::Update_Bone_Data(const DWORD64& PlayerPawnAddress)
{
	if (PlayerPawnAddress == 0) return FALSE;

	this->EntityPawnAddress = PlayerPawnAddress;

	
	DWORD64 gameSceneNode = 0;
	DWORD64 boneArrayAddress = 0;
	std::array<JointData, TotalBones> boneArray;


	gameSceneNode = driver.fetch_Data<DWORD64>(this->EntityPawnAddress, Offset::Pawn::GameSceneNode);

	boneArrayAddress = driver.fetch_Data<DWORD64>(gameSceneNode, Offset::Pawn::BoneArray);


	driver.Read_To_Buffer(boneArrayAddress, (TotalBones * sizeof(JointData)), U_PTR(&boneArray));
	

	for (auto it = 0; it < TotalBones; it++)
	{
		Vec2 ScreenPos{};
		bool IsVisible = FALSE;

		if (pGame.View.WorldToScreen(boneArray[it].Pos, ScreenPos)) IsVisible = TRUE;

		this->BonePosList.push_back({ boneArray[it].Pos, ScreenPos, IsVisible });
	}

	return TRUE;
}
