#pragma once

// ============================================================================
// ARCHIVO HISTORICO - NO FORMA PARTE DEL PROYECTO ACTUAL
// Este archivo queda como evidencia de una version inicial del prototipo que se
// probo en consola. La entrega actual utiliza la interfaz grafica Win32/GDI.
// ============================================================================

#include "ModelosInterfaz.h"

// Renderizador reemplazable: solo conoce el estado que tiene que dibujar.
class RenderizadorConsola {
public:
    void pintar(const EstadoVistaPagina& estado) const;
};
