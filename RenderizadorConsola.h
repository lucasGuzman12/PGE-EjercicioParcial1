#pragma once

#include "ModelosInterfaz.h"

// Renderizador reemplazable: solo conoce el estado que tiene que dibujar.
class RenderizadorConsola {
public:
    void pintar(const EstadoVistaPagina& estado) const;
};
