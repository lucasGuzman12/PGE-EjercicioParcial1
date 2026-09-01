#pragma once

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
