#pragma once

#include "ModelosInterfaz.h"

// Renderizador reemplazable: solo conoce el estado que tiene que dibujar.
class RenderizadorConsola {
public:
    // Pinta en consola una representacion simple del estado de la pagina.
    void pintar(const EstadoVistaPagina& estado) const;
};
