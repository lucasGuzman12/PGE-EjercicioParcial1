#pragma once

#include "UiModels.h"

// Renderer reemplazable: solo conoce el estado que tiene que dibujar.
class ConsoleRenderer {
public:
    void paint(const PageViewState& state) const;
};
