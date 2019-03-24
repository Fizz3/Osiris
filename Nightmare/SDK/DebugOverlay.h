#pragma once
#include "Utils.h"
#include "Vector.h"

class DebugOverlay
{
public:
    constexpr bool ScreenPosition(const Vector& vIn, Vector& vOut) {
        return callVirtualFunction<bool(__thiscall*)(void*, const Vector&, Vector&)>(this, 13)(this, vIn, vOut);
    }
};