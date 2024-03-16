#pragma once
#include "framework.h"


namespace Offset
{
    constexpr std::ptrdiff_t dwBuildNumber = 0x513574;
    constexpr std::ptrdiff_t dwViewAngles = 0x19309B0;
    constexpr std::ptrdiff_t dwViewMatrix = 0x19241A0;
    constexpr std::ptrdiff_t dwEntityList = 0x18C2D58;
    constexpr std::ptrdiff_t dwLocalPlayerController = 0x1912578;
    constexpr std::ptrdiff_t dwLocalPlayerPawn = 0x17371A8;
    
    enum Entity : const int
    {
         Health = 0x7F0,
         TeamID = 0x728,
         Alive = 0x7EC,
         PlayerPawn = 0x7E4,
         PlayerName = 0x748,
    };


    enum Pawn : const int
    {
        
         GameSceneNode = 0x318,
         Pos = (GameSceneNode + 0xC8),
         MaxHealth = 0x330,
         CurrentHealth = 0x334,
         LastClipCameraPos = 0x12EC,
         CameraServices = 0x1138,
         iFovStart = (CameraServices + 0x214),
         m_CBodyComponent = 0x38,
         m_skeletonInstance = (m_CBodyComponent + 0x50),
         m_modelState = 0x160,
         BoneArray = (GameSceneNode + m_skeletonInstance + m_modelState + 0x80),
         EyeAngles = 0x1578,
         aimPunchAngle = 0x177C,
         iTeamNum = 0x3CB,
         m_entitySpottedState = 0x1698,
         SpottedByMask = m_entitySpottedState + 0xC,
    };
}


