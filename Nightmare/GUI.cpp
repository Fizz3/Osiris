#include <Windows.h>

#include "imgui/imgui.h"
#include "imgui/imgui_markdown.h"
#include "GUI.h"
#include "Config.h"
#include "Hacks/Misc.h"
#include "Hacks/Visuals.h"

constexpr auto windowFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize
| ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

void GUI::render() noexcept
{
    renderMenuBar();
    renderAimbotWindow();
    renderTriggerbotWindow();
    renderVisualsWindow();
    renderKnifeChangerWindow();
    renderMiscWindow();
    renderConfigWindow();
    renderCreditsWindow();
}

void GUI::hotkey(int& key) noexcept
{
    ImGui::Text("[ %s ]",  key ? std::to_string(key).c_str() : "key");
    if (ImGui::IsItemHovered()) {
        ImGuiIO& io = ImGui::GetIO();
        for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++)
            if (ImGui::IsKeyPressed(i) && i != VK_INSERT)
                key = i != VK_ESCAPE ? i : 0;

        for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++)
            if (ImGui::IsMouseDown(i))
                key = i + (i > 1 ? 2 : 1);
    }
}

void GUI::renderMenuBar() noexcept
{
    std::vector<std::string> windows{ "Aimbot", "Triggerbot", "Visuals", "Knife Changer", "Misc", "Config", "Credits" };

    ImGuiContext& g = *GImGui;
    ImGuiStyle& style = g.Style;

    //ImGui::SetNextWindowSize(ImVec2(g.IO.DisplaySize.x - 40, (40 + (style.WindowPadding.y * 2))));
    ImGui::SetNextWindowSize(ImVec2(0, (40 + (style.WindowPadding.y * 2))));
    ImGui::SetNextWindowPos(ImVec2(20, 20));
    ImGui::Begin("Nightmare", (bool*)true, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

    for (int i = 0; i < static_cast<int>(windows.size()); i++) {
        if ((window.aimbot && i == 0) ||
            (window.triggerbot && i == 1) ||
            (window.visuals && i == 2) ||
            (window.knifeChanger && i == 3) ||
            (window.misc && i == 4) ||
            (window.config && i == 5) ||
            (window.credits && i == 6))
        {
            ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_ButtonActive]);
        }
        else {
            ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_Button]);
        }

        if (ImGui::Button(windows[i].c_str(), ImVec2(120, 40))) {
            switch (i)
            {
                case 0:
                    window.aimbot = !window.aimbot;
                    break;
                case 1:
                    window.triggerbot = !window.triggerbot;
                    break;
                case 2:
                    window.visuals = !window.visuals;
                    break;
                case 3:
                    window.knifeChanger = !window.knifeChanger;
                    break;
                case 4:
                    window.misc = !window.misc;
                    break;
                case 5:
                    window.config = !window.config;
                    break;
                case 6:
                    window.credits = !window.credits;
                    break;
            }
        }
        ImGui::PopStyleColor();
        
        if (i < static_cast<int>(windows.size()) - 1)
            ImGui::SameLine();

        if (i == static_cast<int>(windows.size()) - 3)
            ImGui::VerticalSeparator(); ImGui::SameLine();
    }

}

void GUI::renderAimbotWindow() noexcept
{
    if (window.aimbot) {
        ImGui::SetNextWindowSize({ 260.0f, 0.0f });
        ImGui::Begin("Aimbot", &window.aimbot, windowFlags);
        static int currentCategory{ 0 };
        ImGui::PushItemWidth(110.0f);
        ImGui::PushID(0);
        ImGui::Combo("", &currentCategory, "All\0Pistols\0Heavy\0SMG\0Rifles");
        ImGui::SameLine();
        static int currentWeapon{ 0 };

        switch (currentCategory) {
        case 0:
            currentWeapon = 0;
            ImGui::NewLine();
            break;
        case 1: {
            static int currentPistol{ 0 };
            ImGui::PushID(1);
            ImGui::Combo("", &currentPistol, "Glock-18\0P2000\0USP-S\0Dual Berettas\0P250\0Tec-9\0Five-Seven\0CZ-75\0Desert Eagle\0Revolver");
            currentWeapon = currentPistol + 1;
            break;
        }
        case 2: {
            static int currentHeavy{ 0 };
            ImGui::PushID(2);
            ImGui::Combo("", &currentHeavy, "Nova\0XM1014\0Sawed-off\0MAG-7\0M249\0Negev");
            currentWeapon = currentHeavy + 11;
            break;
        }
        case 3: {
            static int currentSmg{ 0 };
            ImGui::PushID(3);
            ImGui::Combo("", &currentSmg, "Mac-10\0MP9\0MP7\0MP5-SD\0UMP-45\0P90\0PP-Bizon");
            currentWeapon = currentSmg + 17;
            break;
        }
        case 4: {
            static int currentRifle{ 0 };
            ImGui::PushID(4);
            ImGui::Combo("", &currentRifle, "Galil AR\0Famas\0AK-47\0M4A4\0M4A1-S\0SSG-08\0SG-553\0AUG\0AWP\0G3SG1\0SCAR-20");
            currentWeapon = currentRifle + 24;
            break;
        }
        }
        ImGui::Checkbox("Enabled", &config.aimbot.weapons[currentWeapon].enabled);
        ImGui::Checkbox("Silent", &config.aimbot.weapons[currentWeapon].silent);
        ImGui::Checkbox("Friendly fire", &config.aimbot.weapons[currentWeapon].friendlyFire);
        ImGui::Checkbox("Visible only", &config.aimbot.weapons[currentWeapon].visibleOnly);
        ImGui::Checkbox("Scoped only", &config.aimbot.weapons[currentWeapon].scopedOnly);
        ImGui::Checkbox("Auto shot", &config.aimbot.weapons[currentWeapon].autoShot);
        ImGui::Combo("Bone", &config.aimbot.weapons[currentWeapon].bone, "Head\0Neck\0Sternum\0Chest\0Stomach\0Pelvis");

        if (ImGui::Button("-", ImVec2(20, 0)))
            config.aimbot.weapons[currentWeapon].fov -= 0.05f;

        ImGui::SameLine();

        ImGui::PushID(5);
        ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth() - 20 - ImGui::GetStyle().ItemSpacing.x);
        ImGui::SliderFloat("", &config.aimbot.weapons[currentWeapon].fov, 0.0f, 255.0f, "Fov: %.2f");
        ImGui::PopItemWidth();

        ImGui::SameLine();

        if (ImGui::Button("+", ImVec2(20, 0)))
            config.aimbot.weapons[currentWeapon].fov += 0.05f;

        ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth());

        ImGui::PushID(6);
        ImGui::SliderFloat("", &config.aimbot.weapons[currentWeapon].smooth, 1.0f, 100.0f, "Smooth: %.2f");
        ImGui::PushID(7);
        ImGui::SliderFloat("", &config.aimbot.weapons[currentWeapon].recoilControlX, 0.0f, 1.0f, "Recoil control x: %.2f");
        ImGui::PushID(9);
        ImGui::SliderFloat("", &config.aimbot.weapons[currentWeapon].recoilControlY, 0.0f, 1.0f, "Recoil control y: %.2f");
        ImGui::End();
    }
}

void GUI::renderTriggerbotWindow() noexcept
{
    if (window.triggerbot) {
        ImGui::SetNextWindowSize({ 180.0f, 115.0f });
        ImGui::Begin("Triggerbot", &window.triggerbot, windowFlags);
        ImGui::Checkbox("Enabled", &config.triggerbot.enabled);
        ImGui::Checkbox("On key", &config.triggerbot.onKey);
        ImGui::SameLine();
        hotkey(config.triggerbot.key);
        ImGui::PushItemWidth(160.0f);
        ImGui::PushID(0);
        ImGui::SliderInt("", &config.triggerbot.shotDelay, 0, 250, "Shot delay: %d ms");
        ImGui::End();
    }
}

void GUI::renderVisualsWindow() noexcept
{
    if (window.visuals) {
        ImGui::SetNextWindowSize({ 460.0f, 280.0f });
        ImGui::Begin("Visuals", &window.visuals, windowFlags);

        ImGui::Columns(2, nullptr, false);
        ImGui::SetColumnOffset(1, 120 + ImGui::GetStyle().WindowPadding.y);

        static int tab = 0;
        static std::vector<std::string> tabs{ "Main", "Glow", "Chams", "Misc" };

        for (int i = 0; i < static_cast<int>(tabs.size()); i++) {
            if (tab == i)
                ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_ButtonActive]);
            else
                ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_Button]);

            if (ImGui::Button(tabs[i].c_str(), ImVec2(120, 40)))
                tab = i;

            ImGui::PopStyleColor();
        }

        ImGui::NextColumn();
        switch (tab)
        {
        case 0:
            ImGui::BeginChild("Main", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
            renderVisualsMainTab();
            ImGui::EndChild();
            break;
        case 1:
            ImGui::BeginChild("Glow", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
            renderVisualsGlowTab();
            ImGui::EndChild();
            break;
        case 2:
            ImGui::BeginChild("Chams", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
            renderVisualsChamsTab();
            ImGui::EndChild();
            break;
        case 3:
            ImGui::BeginChild("Misc", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_AlwaysVerticalScrollbar);
            renderVisualsMiscTab();
            ImGui::EndChild();
            break;
        }

        ImGui::End();
    }
}

void GUI::renderVisualsMainTab() noexcept
{
    ImGui::Checkbox("Enabled", &config.visuals.main.enabled);
    ImGui::Checkbox("Box2D", &config.visuals.main.box);

    ImGui::Checkbox("Enemies", &config.visuals.main.enemies);
    ImGui::SameLine(ImGui::GetContentRegionAvailWidth() - 180);
    bool openPopupEnemies = ImGui::ColorButton("Color", ImColor{ config.visuals.main.enemyColor[0], config.visuals.main.enemyColor[1], config.visuals.main.enemyColor[2] }, ImGuiColorEditFlags_NoTooltip);
    if (openPopupEnemies)
        ImGui::OpenPopup("##1");
    if (ImGui::BeginPopup("##1")) {
        ImGui::ColorPicker3("##2", config.visuals.main.enemyColor, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoSidePreview);
        ImGui::EndPopup();
    }

    ImGui::Checkbox("Teammates", &config.visuals.main.allies);
    ImGui::SameLine(ImGui::GetContentRegionAvailWidth() - 180);
    bool openPopupTeam = ImGui::ColorButton("Color2", ImColor{ config.visuals.main.allyColor[0], config.visuals.main.allyColor[1], config.visuals.main.allyColor[2] }, ImGuiColorEditFlags_NoTooltip);
    if (openPopupTeam)
        ImGui::OpenPopup("##3");
    if (ImGui::BeginPopup("##3")) {
        ImGui::ColorPicker3("##4", config.visuals.main.allyColor, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoSidePreview);
        ImGui::EndPopup();
    }

    ImGui::Checkbox("Name", &config.visuals.main.name);
    ImGui::Checkbox("Health", &config.visuals.main.health);
}

void GUI::renderVisualsGlowTab() noexcept
{
    static int currentCategory{ 0 };
    ImGui::PushItemWidth((ImGui::GetContentRegionAvailWidth() / 2) - (ImGui::GetStyle().ItemSpacing.x));
    ImGui::PushID(0);
    ImGui::Combo("", &currentCategory, "Allies\0Enemies\0Planting\0Defusing\0Local player\0Weapons\0C4\0Planted C4\0Chickens\0");
    static int currentItem{ 0 };
    if (currentCategory <= 3) {
        ImGui::SameLine();
        static int currentType{ 0 };
        ImGui::PushID(1);
        ImGui::Combo("", &currentType, "Visible\0Occluded");
        currentItem = currentCategory * 2 + currentType;
    }
    else {
        currentItem = currentCategory + 4;
    }

    ImGui::Checkbox("Enabled", &config.visuals.glow[currentItem].enabled);
    bool openPopup = ImGui::ColorButton("Color", ImColor{ config.visuals.glow[currentItem].color[0], config.visuals.glow[currentItem].color[1], config.visuals.glow[currentItem].color[2] }, ImGuiColorEditFlags_NoTooltip);
    ImGui::SameLine(0.0f, 5.0f);
    ImGui::Text("Color");
    ImGui::PushID(2);
    if (openPopup)
        ImGui::OpenPopup("");
    if (ImGui::BeginPopup("")) {
        ImGui::PushID(3);
        ImGui::ColorPicker3("", config.visuals.glow[currentItem].color, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoSidePreview);
        ImGui::EndPopup();
    }
    ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth());
    ImGui::PushID(4);
    ImGui::SliderFloat("", &config.visuals.glow[currentItem].thickness, 0.0f, 1.0f, "Thickness: %.2f");
    ImGui::PushID(5);
    ImGui::SliderFloat("", &config.visuals.glow[currentItem].alpha, 0.0f, 1.0f, "Alpha: %.2f");
    ImGui::PushID(6);
    ImGui::SliderInt("", &config.visuals.glow[currentItem].style, 0, 3, "Style: %d");
}

void GUI::renderVisualsChamsTab() noexcept
{
    static int currentCategory{ 0 };
    ImGui::PushItemWidth((ImGui::GetContentRegionAvailWidth() / 2) - (ImGui::GetStyle().ItemSpacing.x));
    ImGui::PushID(0);
    ImGui::Combo("", &currentCategory, "Allies\0Enemies\0Planting\0Defusing\0Local player\0Weapons\0Hands\0");
    static int currentItem{ 0 };

    if (currentCategory <= 3) {
        ImGui::SameLine();
        static int currentType{ 0 };
        ImGui::PushID(1);
        ImGui::Combo("", &currentType, "Visible\0Occluded");
        currentItem = currentCategory * 2 + currentType;
    }
    else {
        currentItem = currentCategory + 4;
    }

    ImGui::Checkbox("Enabled", &config.visuals.chams[currentItem].enabled);
    ImGui::Checkbox("Flat", &config.visuals.chams[currentItem].flat);
    ImGui::Checkbox("Wireframe", &config.visuals.chams[currentItem].wireframe);

    bool openPopup = ImGui::ColorButton("Color", ImColor{ config.visuals.chams[currentItem].color[0], config.visuals.chams[currentItem].color[1], config.visuals.chams[currentItem].color[2] }, ImGuiColorEditFlags_NoTooltip);
    ImGui::SameLine(0.0f, 5.0f);
    ImGui::Text("Color");
    ImGui::PushID(2);
    if (openPopup)
        ImGui::OpenPopup("");
    if (ImGui::BeginPopup("")) {
        ImGui::PushID(3);
        ImGui::ColorPicker3("", config.visuals.chams[currentItem].color, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoSidePreview);
        ImGui::EndPopup();
    }
    ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth());
    ImGui::PushID(4);
    ImGui::SliderFloat("", &config.visuals.chams[currentItem].alpha, 0.0f, 1.0f, "Alpha: %.2f");
}

void GUI::renderVisualsMiscTab() noexcept
{
    ImGui::Checkbox("Disable post-processing", &config.visuals.misc.disablePostProcessing);
    ImGui::Checkbox("Inverse ragdoll gravity", &config.visuals.misc.inverseRagdollGravity);
    ImGui::Checkbox("No visual recoil", &config.visuals.misc.noVisualRecoil);
    ImGui::Checkbox("No hands", &config.visuals.misc.noHands);
    ImGui::Checkbox("No sleeves", &config.visuals.misc.noSleeves);
    ImGui::Checkbox("No weapons", &config.visuals.misc.noWeapons);
    ImGui::Checkbox("No smoke", &config.visuals.misc.noSmoke);
    ImGui::Checkbox("No blur", &config.visuals.misc.noBlur);
    ImGui::Checkbox("No scope overlay", &config.visuals.misc.noScopeOverlay);
    ImGui::Checkbox("No grass", &config.visuals.misc.noGrass);
    ImGui::Checkbox("Wireframe smoke", &config.visuals.misc.wireframeSmoke);
    ImGui::Checkbox("Thirdperson", &config.visuals.misc.thirdperson);
    ImGui::SameLine();
    hotkey(config.visuals.misc.thirdpersonKey);
    ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth() - ImGui::GetStyle().WindowPadding.x);
    ImGui::PushID(0);
    ImGui::SliderInt("", &config.visuals.misc.thirdpersonDistance, 0, 1000, "Thirdperson distance: %d");
    ImGui::PushID(1);
    ImGui::SliderInt("", &config.visuals.misc.viewmodelFov, -60, 60, "Viewmodel FOV: %d");
    ImGui::PushID(2);
    ImGui::SliderInt("", &config.visuals.misc.flashReduction, 0, 100, "Flash reduction: %d%%");
    ImGui::PushID(3);
    ImGui::SliderFloat("", &config.visuals.misc.brightness, 0.0f, 1.0f, "Brightness: %.2f");
    ImGui::PopItemWidth();
    ImGui::Combo("Skybox", &config.visuals.misc.skybox, "Default\0cs_baggage_skybox_\0cs_tibet\0embassy\0italy\0jungle\0nukeblank\0office\0sky_cs15_daylight01_hdr\0sky_cs15_daylight02_hdr\0sky_cs15_daylight03_hdr\0sky_cs15_daylight04_hdr\0sky_csgo_cloudy01\0sky_csgo_night_flat\0sky_csgo_night02\0sky_day02_05_hdr\0sky_day02_05\0sky_dust\0sky_l4d_rural02_ldr\0sky_venice\0vertigo_hdr\0vertigo\0vertigoblue_hdr\0vietnam");
    ImGui::ColorEdit3("World color", config.visuals.misc.worldColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoTooltip);
}

void GUI::renderKnifeChangerWindow() noexcept
{
    if (window.knifeChanger) {
        ImGui::SetNextWindowSize({ 180.0f, 0.0f });
        ImGui::Begin("Knife changer", &window.knifeChanger, windowFlags);
        ImGui::PushItemWidth(130.0f);
        ImGui::Checkbox("Enabled", &config.knifeChanger.enabled);
        ImGui::Combo("Knife", &config.knifeChanger.knife, "Default\0Bayonet\0Bowie\0Butterfly\0Falchion\0Flip\0Gut\0Huntsman\0Karambit\0M9 Bayonet\0Shadow Daggers\0Navaja\0Stiletto\0Talon\0Ursus\0Golden");
        ImGui::End();
    }
}

void GUI::renderMiscWindow() noexcept
{
    if (window.misc) {
        ImGui::SetNextWindowSize({ 220.0f, 0.0f });
        ImGui::Begin("Misc", &window.misc, windowFlags);
        ImGui::Checkbox("Bunny hop", &config.misc.bunnyHop);
        static char buffer[16];
        ImGui::PushItemWidth(120.0f);
        ImGui::PushID(0);
        ImGui::InputText("", buffer, IM_ARRAYSIZE(buffer));
        ImGui::SameLine();
        if (ImGui::Button("Set clantag")) {
            Misc::setClanTag(buffer);
            Misc::animateClanTag(buffer);
        }
        ImGui::Checkbox("Animated clan tag", &config.misc.animatedClanTag);
        ImGui::Checkbox("Fast duck", &config.misc.fastDuck);
        ImGui::Checkbox("Sniper crosshair", &config.misc.sniperCrosshair);
        ImGui::Checkbox("Recoil crosshair", &config.misc.recoilCrosshair);
        ImGui::Checkbox("FOV crosshair", &config.misc.fovCrosshair);
        ImGui::Checkbox("Auto pistol", &config.misc.autoPistol);
        ImGui::Checkbox("Auto accept", &config.misc.autoAccept);
        ImGui::Checkbox("Radar hack", &config.misc.radarHack);
        ImGui::Checkbox("Spectator list", &config.misc.spectatorList);
        ImGui::Checkbox("Watermark", &config.misc.watermark);

        if (ImGui::Button("Reveal ranks"))
            Misc::revealRanks();
        ImGui::End();
    }
}

void GUI::renderConfigWindow() noexcept
{
    if (window.config) {
        ImGui::SetNextWindowSize({ 290.0f, 190.0f });
        ImGui::Begin("Config", &window.config, windowFlags);

        std::string currentConfigString("Current Config: " + config.currentConfig);
        ImGui::Text(currentConfigString.c_str());

        ImGui::Columns(2, nullptr, false);
        ImGui::SetColumnOffset(1, 170.0f);

        ImGui::PushItemWidth(160.0f);

        std::vector<std::string> configItems = config.getConfigs();
        static int configCurrent = -1;

        ImGui::ListBox("", &configCurrent, configItems, 5);

        if (configCurrent < static_cast<int>(configItems.size())) {
            if (configCurrent != -1) {
                char buffer[16];
                strcpy_s(buffer, configItems[configCurrent].c_str());

                if (ImGui::InputText("##1", buffer, IM_ARRAYSIZE(buffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
                    char* nameBuffer = buffer;
                    char* str_end = nameBuffer + strlen(nameBuffer);
                    while (str_end > nameBuffer && str_end[-1] == ' ')
                        str_end--; *str_end = 0;

                    std::string name(nameBuffer);
                    config.rename(configItems[configCurrent], name);
                }
            }
            else {
                ImGui::InputTextDisabled("Default");
            }
        }
        else {
            ImGui::InputTextDisabled("No Config Selected");
        }

        ImGui::PopItemWidth();
        ImGui::NextColumn();

        ImGui::PushItemWidth(100.0f);

        if (configCurrent >= static_cast<int>(configItems.size())) {
            ImGui::ButtonDisabled("Load Selected");
            ImGui::ButtonDisabled("Save Selected");
            if (ImGui::Button("New Config", { 100.0f, 25.0f })) {
                config.add(GetNewConfigName());
            }
            ImGui::ButtonDisabled("Delete Config");
        }
        else {
            if (ImGui::Button("Load Selected", { 100.0f, 25.0f })) {
                config.load(configItems[configCurrent].c_str());
            }

            if (configCurrent != 0) {
                if (ImGui::Button("Save Selected", { 100.0f, 25.0f })) {
                    config.save(configItems[configCurrent]);
                }
            }
            else {
                ImGui::ButtonDisabled("Save Selected");
            }

            if (ImGui::Button("New Config", { 100.0f, 25.0f })) {
                config.add(GetNewConfigName());
            }

            if (configCurrent != 0)
            {
                if (ImGui::Button("Delete Config", { 100.0f, 25.0f })) {
                    config.remove(configItems[configCurrent]);
                }
            }
            else {
                ImGui::ButtonDisabled("Delete Config");
            }
        }

        ImGui::PopItemWidth();
        ImGui::End();
    }
}

void GUI::renderCreditsWindow() noexcept
{
    if (window.credits) {
        ImGui::SetNextWindowSize({ 300.0f, 350.0f });
        ImGui::Begin("Credits", &window.credits, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollWithMouse);

        const std::string markdown = u8R"([@Fizz3](https://github.com/Fizz3)
I put together this cheat for my own personal use and as a way to learn C++. Barely any of it is coded from scratch, almost all of the credit goes to the following people.

[@danielkrupinski](https://github.com/danielkrupinski)
Created the project that Nightmare is forked from. I wouldn't have made this if I didn't stumble across his source which reminded my of when AimTux first came out.

[@luk1337](https://github.com/luk1337), [@McSwaggens](https://github.com/McSwaggens) and [Contributors](https://github.com/AimTuxOfficial/AimTux/graphs/contributors)
Creators of the AimTux Linux cheat, which I often use as a reference.

[unknowncheats.me](https://unknowncheats.me) Community
Lots of useful resources and helpful people, I would have a lot less features without some of the stuff I've found here.
)";
        ImGui::MarkdownConfig mdConfig{ NULL, NULL, NULL, { { NULL, true }, { NULL, true }, { NULL, false } } };
        ImGui::Markdown(markdown.c_str(), markdown.length(), mdConfig);

        ImGui::End();
    }
}

std::string GUI::GetNewConfigName(std::string name, int i) noexcept {
    std::vector<std::string> configItems = config.getConfigs();

    auto cName = std::find(configItems.begin(), configItems.end(), name + (i == 0 ? "" : " " + std::to_string(i)));
    if (cName == configItems.end())
        if (i == 0)
            return name;
        else
            return name + " " + std::to_string(i);
    else
        return GetNewConfigName(name, i + 1);
}