#pragma once

#include <string>
#include <vector>

// Modelo de datos de una seccion del panel.
// Cada grupo representa una sección ordenada de la futura ventana emergente.
// Por ahora las opciones son solamente datos: no filtran productos.
struct GrupoFiltros {
    std::string titulo;
    std::vector<std::string> opciones;
};

// Estado completo que se le pasa al renderizador.
// Contrato entre la lógica y cualquier interfaz gráfica que se agregue.
struct EstadoVistaPagina {
    std::string tituloPagina;
    std::string etiquetaBotonFiltros;
    bool ventanaFiltrosAbierta{};
    std::vector<GrupoFiltros> gruposFiltros;
};
