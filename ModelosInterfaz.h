#pragma once

// ============================================================================
// ARCHIVO HISTORICO - NO FORMA PARTE DEL PROYECTO ACTUAL
// Este archivo queda como evidencia de una version inicial del prototipo que se
// probo en consola. La entrega actual utiliza la interfaz grafica Win32/GDI.
// ============================================================================

#include <string>
#include <vector>

// Cada grupo representa una sección ordenada de la futura ventana emergente.
// Por ahora las opciones son solamente datos: no filtran productos.
struct GrupoFiltros {
    std::string titulo;
    std::vector<std::string> opciones;
};

// Contrato entre la lógica y cualquier interfaz gráfica que se agregue.
struct EstadoVistaPagina {
    std::string tituloPagina;
    std::string etiquetaBotonFiltros;
    bool ventanaFiltrosAbierta{};
    std::vector<GrupoFiltros> gruposFiltros;
};
