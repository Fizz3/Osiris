#include "../Config.h"
#include "../Memory.h"
#include "Visuals.h"

void Visuals::colorWorld() noexcept
{
    static auto red = interfaces.cvar->findVar("mat_ambient_light_r");
    static auto green = interfaces.cvar->findVar("mat_ambient_light_g");
    static auto blue = interfaces.cvar->findVar("mat_ambient_light_b");

    red->setValue(config.visuals.misc.worldColor[0]);
    green->setValue(config.visuals.misc.worldColor[1]);
    blue->setValue(config.visuals.misc.worldColor[2]);
}

void Visuals::reduceFlashEffect() noexcept
{
    auto localPlayer = interfaces.entityList->getEntity(interfaces.engine->getLocalPlayer());
    *reinterpret_cast<float*>(localPlayer + netvars["m_flFlashMaxAlpha"]) = 255.0f - config.visuals.misc.flashReduction * 2.55f;
}

void Visuals::modifySmoke() noexcept
{
    static constexpr const char* smokeMaterials[]{
        "particle/vistasmokev1/vistasmokev1_emods",
        "particle/vistasmokev1/vistasmokev1_emods_impactdust",
        "particle/vistasmokev1/vistasmokev1_fire",
        "particle/vistasmokev1/vistasmokev1_smokegrenade",
    };

    for (const auto mat : smokeMaterials) {
        auto material = interfaces.materialSystem->findMaterial(mat);
        material->setMaterialVarFlag(MaterialVar::NO_DRAW, config.visuals.misc.noSmoke);
        material->setMaterialVarFlag(MaterialVar::WIREFRAME, config.visuals.misc.wireframeSmoke);
    }

    if (config.visuals.misc.noSmoke || config.visuals.misc.wireframeSmoke)
        *memory.smokeCount = 0;
}

void Visuals::thirdperson() noexcept
{
    static bool isInThirdperson{ true };
    static float lastTime{ 0.0f };

    if (GetAsyncKeyState(config.visuals.misc.thirdpersonKey) && memory.globalVars->realtime - lastTime > 0.5f) {
        isInThirdperson = !isInThirdperson;
        lastTime = memory.globalVars->realtime;
    }

    if (config.visuals.misc.thirdperson)
        if (memory.input->isCameraInThirdPerson = (!config.visuals.misc.thirdpersonKey || isInThirdperson)
            && interfaces.entityList->getEntity(interfaces.engine->getLocalPlayer())->isAlive())
            memory.input->cameraOffset.z = static_cast<float>(config.visuals.misc.thirdpersonDistance);
}

void Visuals::removeVisualRecoil(FrameStage stage) noexcept
{
    if (config.visuals.misc.noVisualRecoil && stage == FrameStage::RENDER_START) {
        auto localPlayer = interfaces.entityList->getEntity(interfaces.engine->getLocalPlayer());
        localPlayer->setProperty("m_viewPunchAngle", Vector{ });
        if (!config.misc.recoilCrosshair)
            localPlayer->setProperty("m_aimPunchAngle", Vector{ });
    }
}

void Visuals::removeBlur() noexcept
{
    static auto blur = interfaces.materialSystem->findMaterial("dev/scope_bluroverlay");
    blur->setMaterialVarFlag(MaterialVar::NO_DRAW, config.visuals.misc.noBlur);
}

void Visuals::updateBrightness() noexcept
{
    static auto brightness = interfaces.cvar->findVar("mat_force_tonemap_scale");
    brightness->setValue(config.visuals.misc.brightness);
}

void Visuals::removeGrass() noexcept
{
    static auto grass = interfaces.materialSystem->findMaterial("detail/detailsprites_survival");
    static auto incrementOnce = grass->incrementReferenceCount();
    grass->setMaterialVarFlag(MaterialVar::NO_DRAW, config.visuals.misc.noGrass);
}
