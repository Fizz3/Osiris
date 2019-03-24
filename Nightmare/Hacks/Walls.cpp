#include "../Config.h"
#include "Walls.h"

#include "../Interfaces.h"
#include "../SDK/Entity.h"
#include "../SDK/Vector.h"
#include "../SDK/WeaponId.h"

static bool getBox(Entity* entity, int& x, int& y, int& w, int& h) noexcept {
    Vector vOrigin, min, max;
    Vector flb, brt, blb, frt, frb, brb, blt, flt;
    float left, top, right, bottom;

    vOrigin = entity->getProperty<Vector>("m_vecOrigin");
    min = entity->getCollideableMins() + vOrigin;
    max = entity->getCollideableMaxs() + vOrigin;

    Vector points[] = {
        Vector{ min.x, min.y, min.z },
        Vector{ min.x, max.y, min.z },
        Vector{ max.x, max.y, min.z },
        Vector{ max.x, min.y, min.z },
        Vector{ max.x, max.y, max.z },
        Vector{ min.x, max.y, max.z },
        Vector{ min.x, min.y, max.z },
        Vector{ max.x, min.y, max.z }
    };

    if (interfaces.debugOverlay->ScreenPosition(points[3], flb) || interfaces.debugOverlay->ScreenPosition(points[5], brt)
        || interfaces.debugOverlay->ScreenPosition(points[0], blb) || interfaces.debugOverlay->ScreenPosition(points[4], frt)
        || interfaces.debugOverlay->ScreenPosition(points[2], frb) || interfaces.debugOverlay->ScreenPosition(points[1], brb)
        || interfaces.debugOverlay->ScreenPosition(points[6], blt) || interfaces.debugOverlay->ScreenPosition(points[7], flt)) {
        return false;
    }

    Vector arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };
    left = flb.x;
    top = flb.y;
    right = flb.x;
    bottom = flb.y;

    for (int i = 1; i < 8; i++) {
        if (left > arr[i].x)
            left = arr[i].x;
        if (bottom < arr[i].y)
            bottom = arr[i].y;
        if (right < arr[i].x)
            right = arr[i].x;
        if (top > arr[i].y)
            top = arr[i].y;
    }

    x = (int)left;
    y = (int)top;
    w = (int)(right - left);
    h = (int)(bottom - top);

    return true;
}

static void drawPlayerText(Entity* player, int x, int y, int w, int h) noexcept
{
    int boxSpacing = config.visuals.main.box ? 3 : 0;
    int lineNum = 0;
    int lineHeight = 10;

    static PlayerInfo playerInfo;
    interfaces.engine->getPlayerInfo(player->getIndex(), playerInfo);
    interfaces.surface->setTextColor(255, 255, 255, 255);

    if (config.visuals.main.name) {
        interfaces.surface->setTextPosition(x, y + h + boxSpacing + (lineHeight * lineNum));
        interfaces.surface->printText(playerInfo.name);
        lineNum++;
    }

    if (config.visuals.main.health) {
        interfaces.surface->setTextPosition(x, y + h + boxSpacing + (lineHeight * lineNum));
        interfaces.surface->printText(std::string(std::to_string(player->getProperty<int>("m_iHealth")) + " HP"));
        lineNum++;
    }
}

static void drawPlayer(Entity* player) noexcept
{
    auto localplayer = interfaces.entityList->getEntity(interfaces.engine->getLocalPlayer());
    if (!localplayer)
        return;

    if (player->isEnemy() && !config.visuals.main.enemies)
        return;

    if (player->isInLocalTeam() && !config.visuals.main.allies)
        return;

    int x, y, w, h;
    if (!getBox(player, x, y, w, h))
        return;

    if (config.visuals.main.box) {
        float* color;
        if (player->isEnemy())
            color = config.visuals.main.enemyColor;
        else
            color = config.visuals.main.allyColor;

        interfaces.surface->setDrawColor(color[0], color[1], color[2]);

        interfaces.surface->drawOutlinedRect(x, y, x + w, y + h);
    }

    drawPlayerText(player, x, y, w, h);
}

void Walls::draw() noexcept
{
    if (!config.visuals.main.enabled)
        return;

    if (!interfaces.engine->isInGame())
        return;

    auto localplayer = interfaces.entityList->getEntity(interfaces.engine->getLocalPlayer());
    if (!localplayer)
        return;

    for (int i = 1; i < interfaces.entityList->getHighestEntityIndex(); i++) {
        auto entity = interfaces.entityList->getEntity(i);
        if (!entity)
            continue;

        auto client = entity->getClientClass();

        if (client->classId == ClassId::CSPlayer && (config.visuals.main.enemies || config.visuals.main.allies)) {
            auto player = entity;

            if (player->isDormant() || !player->isAlive())
                continue;

            drawPlayer(player);
        }
    }
}