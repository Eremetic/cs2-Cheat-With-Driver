#include"ConfigSaver.hpp"
#include "menuConfig.hpp"
#include "Aimbot.hpp"
#include <sstream>

namespace ConfigSaver
{
    void SaveConfig(const std::string& filename)
    {
        std::ofstream configFile(MenuConfig::path + '/' + filename);
    

        configFile << "ShowBoneESP " << MenuConfig::show_Boneesp << std::endl; 
        configFile << "BoneThickness " << MenuConfig::boneThickness << std::endl;
        configFile << "ShowBoxESP " << MenuConfig::show_Boxesp << std::endl;
        configFile << "BoxThickness " << MenuConfig::boxThickness << std::endl;
        configFile << "AimFov " << Aim::aimFov << std::endl;
        configFile << "FovLineSize " << MenuConfig::fovLineSize << std::endl;
        configFile << "AimBotHotKey " << MenuConfig::aimKey << std::endl;       
        configFile << "ShowDistance " << MenuConfig::show_Distance << std::endl;
        configFile << "Smooth " << Aim::smoothing << std::endl;
        configFile << "ShowFovLine " << MenuConfig::show_Fov << std::endl;      
        configFile << "ShowPlayerName " << MenuConfig::show_PlayerName << std::endl;
        configFile << "AimBot " << MenuConfig::enb_Aimbot << std::endl;
        configFile << "AimBotDeadZone " << Aim::deadZoneFov << std::endl;
        configFile << "AimBotOverAimX " << Aim::overAimX << std::endl;
        configFile << "AimBotOverAimY " << Aim::overAimY << std::endl;
        configFile << "AimCurrentBone " << MenuConfig::aimLock_select << std::endl;
        configFile << "AimBoneSelect " << MenuConfig::bone_Select << std::endl;      
        configFile << "BoneColorVisible " << MenuConfig::BoneColorVis.Value.x << " " << MenuConfig::BoneColorVis.Value.y << " " << MenuConfig::BoneColorVis.Value.z << " " << MenuConfig::BoneColorVis.Value.w << std::endl;
        configFile << "BoneColorInvisible " << MenuConfig::BoneColorInVis.Value.x << " " << MenuConfig::BoneColorInVis.Value.y << " " << MenuConfig::BoneColorInVis.Value.z << " " << MenuConfig::BoneColorInVis.Value.w << std::endl;
        configFile << "FovLineColor " << MenuConfig::FovLineColor.Value.x << " " << MenuConfig::FovLineColor.Value.y << " " << MenuConfig::FovLineColor.Value.z << " " << MenuConfig::FovLineColor.Value.w << std::endl;     
        configFile << "BoxColor " << MenuConfig::BoxColorVis.Value.x << " " << MenuConfig::BoxColorVis.Value.y << " " << MenuConfig::BoxColorVis.Value.z << " " << MenuConfig::BoxColorVis.Value.w << std::endl;
        configFile << "BoxColor " << MenuConfig::BoxColorInVis.Value.x << " " << MenuConfig::BoxColorInVis.Value.y << " " << MenuConfig::BoxColorInVis.Value.z << " " << MenuConfig::BoxColorInVis.Value.w << std::endl;
        configFile << "DeadZoneColor " << MenuConfig::DeadZoneColor.Value.x << " " << MenuConfig::DeadZoneColor.Value.y << " " << MenuConfig::DeadZoneColor.Value.z << " " << MenuConfig::DeadZoneColor.Value.w << std::endl;
        configFile << "TeamCheck " << MenuConfig::enb_Teamcheck << std::endl;
        configFile << "VisibleCheck " << MenuConfig::enb_Vischeck << std::endl;       
        configFile << "CrossHairColor " << MenuConfig::CrossHairColor.Value.x << " " << MenuConfig::CrossHairColor.Value.y << " " << MenuConfig::CrossHairColor.Value.z << " " << MenuConfig::CrossHairColor.Value.w << std::endl;
        configFile << "CrossHairSize " << MenuConfig::CrossHairSize << std::endl;
        configFile << "ShowAimFovRange " << MenuConfig::show_Fov << std::endl;
        configFile << "targetSelection " <<  MenuConfig::aimLock_select <<std::endl;
        configFile << "AimFovRangeColor " << MenuConfig::FovLineColor.Value.x << " " << MenuConfig::FovLineColor.Value.y << " " << MenuConfig::FovLineColor.Value.z << " " << MenuConfig::FovLineColor.Value.w << std::endl;
        configFile.close();
        std::cout << "Configuration saved to " << MenuConfig::path + '/' + filename << std::endl;
    }
    
    


    void LoadConfig(const std::string& filename)
    {
        std::ifstream configFile(MenuConfig::path + '/' + filename);
       
        std::string line;
        while (std::getline(configFile, line))
        {
            std::stringstream iss(line);
            std::string key;
            if (iss >> key)
            {
                if (key == "ShowBoneESP ") iss >> MenuConfig::show_Boneesp;
                else if (key == "BoneThickness ") iss >> MenuConfig::boneThickness;
                else if (key == "ShowBoxESP ") iss >> MenuConfig::show_Boxesp;
                else if (key == "BoxThickness ") iss >> MenuConfig::boxThickness;
                else if (key == "AimFov ") iss >> Aim::aimFov;
                else if (key == "FovLineSize ") iss >> MenuConfig::fovLineSize;
                else if (key == "AimBotHotKey ") iss >> MenuConfig::aimKey;
                else if (key == "ShowDistance ") iss >> MenuConfig::show_Distance;
                else if (key == "Smooth ") iss >> Aim::smoothing;
                else if (key == "ShowFovLine ") iss >> MenuConfig::show_Fov;
                else if (key == "ShowPlayerName ") iss >> MenuConfig::show_PlayerName;
                else if (key == "AimBot ") iss >> MenuConfig::enb_Aimbot;
                else if (key == "AimBotDeadZone ") iss >> Aim::deadZoneFov;
                else if (key == "AimBotOverAimX ") iss >> Aim::overAimX;
                else if (key == "AimBotOverAimY ") iss >> Aim::overAimY;
                else if (key == "AimCurrentBone ") iss >> MenuConfig::aimLock_select;
                else if (key == "AimBoneSelect ") iss >> MenuConfig::bone_Select;
                else if (key == "BoneColorVisible ") iss >> MenuConfig::BoneColorVis.Value.x >> MenuConfig::BoneColorVis.Value.y >> MenuConfig::BoneColorVis.Value.z >> MenuConfig::BoneColorVis.Value.w;
                else if (key == "BoneColorInvisible ") iss >> MenuConfig::BoneColorInVis.Value.x >> MenuConfig::BoneColorInVis.Value.y >> MenuConfig::BoneColorInVis.Value.z >> MenuConfig::BoneColorInVis.Value.w;
                else if (key == "FovLineColor ") iss >> MenuConfig::FovLineColor.Value.x >> MenuConfig::FovLineColor.Value.y >> MenuConfig::FovLineColor.Value.z >> MenuConfig::FovLineColor.Value.w;
                else if (key == "BoxColor ") iss >> MenuConfig::BoxColorVis.Value.x >> MenuConfig::BoxColorVis.Value.y >> MenuConfig::BoxColorVis.Value.z >> MenuConfig::BoxColorVis.Value.w;
                else if (key == "BoxColor ") iss >> MenuConfig::BoxColorInVis.Value.x >> MenuConfig::BoxColorInVis.Value.y >> MenuConfig::BoxColorInVis.Value.z >> MenuConfig::BoxColorInVis.Value.w;
                else if (key == "DeadZoneColor ") iss >> MenuConfig::DeadZoneColor.Value.x >> MenuConfig::DeadZoneColor.Value.y >> MenuConfig::DeadZoneColor.Value.z >> MenuConfig::DeadZoneColor.Value.w;
                else if (key == "TeamCheck ") iss >> MenuConfig::enb_Teamcheck;
                else if (key == "VisibleCheck ") iss >> MenuConfig::enb_Vischeck;
                else if (key == "CrossHairColor ") iss >> MenuConfig::CrossHairColor.Value.x >> MenuConfig::CrossHairColor.Value.y >> MenuConfig::CrossHairColor.Value.z >> MenuConfig::CrossHairColor.Value.w;
                else if (key == "CrossHairSize ") iss >> MenuConfig::CrossHairSize;
                else if (key == "ShowAimFovRange ") iss >> MenuConfig::show_Fov;
                else if (key == "targetSelection ") iss >> MenuConfig::aimLock_select;
                else if (key == "AimFovRangeColor ") iss >> MenuConfig::FovLineColor.Value.x >> MenuConfig::FovLineColor.Value.y >> MenuConfig::FovLineColor.Value.z >> MenuConfig::FovLineColor.Value.w;
            }
        }
    }

    VOID resetValues()
    {

        /// ESP
        MenuConfig::show_PlayerName = false;
        MenuConfig::show_Boxesp = false;
        MenuConfig::minBoneThickness = 1.0f;
        MenuConfig::boneThickness = 1.0f;
        MenuConfig::maxBoneThickness = 5.0f;
        MenuConfig::minBoxThickness = 1.0f;
        MenuConfig::boxThickness = 1.0f;
        MenuConfig::maxBoxThickness = 5.0f;
        MenuConfig::show_Boneesp = false;
        MenuConfig::show_Distance = false;
        MenuConfig::espDistance = 10000.0f;
        MenuConfig::maxEspDistance = 10000.0f;
        MenuConfig::show_Fps = false;
        MenuConfig::show_Fov = false;
        MenuConfig::show_deadZone = false;
        MenuConfig::fovLineSize = 2.f;
        MenuConfig::deadZoneLineSize = 2.f;
        MenuConfig::show_Crosshair = false;
        MenuConfig::CrossHairSize = 10.f;
        MenuConfig::CrosshairThickness = 2;
        /////////////////////////////
        ///AimBot
        MenuConfig::enb_Aimbot = false;
        MenuConfig::enb_Teamcheck = true;
        MenuConfig::aimKey = VK_LBUTTON;
        MenuConfig::aimDistance = 250.0f;
        MenuConfig::maxAimDistance = 1000.0f;
        MenuConfig::mindeadZone = 0.0f;
        MenuConfig::maxdeadZone = 15.0;
        MenuConfig::fovRadius = 0.0f;
        MenuConfig::minFov = 0.1f;
        MenuConfig::maxFov = 360.0f;
        MenuConfig::smoothing = 1.0f;
        MenuConfig::minSmooth = 0.0f;
        MenuConfig::maxSmooth = 90.0f;
        MenuConfig::minOveraim = 0.0f;
        MenuConfig::maxOveraim = 5.0f;
        MenuConfig::enb_Vischeck = false;
        MenuConfig::aim_Select = 0;
        MenuConfig::bone_Select = BONEINDEX::head;
        MenuConfig::aimLock_select = 0;
        ////////////////////////////////
        ///Colors
        MenuConfig::BoneColorVis = ImColor(255, 0, 0, 255);
        MenuConfig::BoneColorInVis = ImColor(255, 255, 255, 255);
        MenuConfig::BoxColorVis = ImColor(255, 0, 0, 255);
        MenuConfig::BoxColorInVis = ImColor(255, 255, 255, 255);
        MenuConfig::FovLineColor = ImColor(255, 165, 0, 220);
        MenuConfig::CrossHairColor = ImColor(255, 165, 0, 255);
        MenuConfig::DeadZoneColor = ImColor(255, 165, 0, 255);
        ///////////////////////////////
        ///Aimbot
        Aim::aimFov = 10;
        Aim::deadZoneFov = 0;
        Aim::overAimX = 0;
        Aim::overAimY = 0;
    }
}
