#include "RenderizadorConsola.h"

// ============================================================================
// ARCHIVO HISTORICO - NO FORMA PARTE DEL PROYECTO ACTUAL
// Este archivo queda como evidencia de una version inicial del prototipo que se
// probo en consola. La entrega actual utiliza la interfaz grafica Win32/GDI.
// ============================================================================

#include "Conversion.h"

#include <iostream>

void RenderizadorConsola::pintar(const EstadoVistaPagina& estado) const {
    std::cout << "\n==================================================\n";
    std::cout << estado.tituloPagina << '\n';
    std::cout << "[ F ] " << estado.etiquetaBotonFiltros << '\n';
    std::cout << "Estado del panel: "
              << Conversion::estadoVentanaATexto(estado.ventanaFiltrosAbierta) << '\n';
    std::cout << "==================================================\n";

    if (!estado.ventanaFiltrosAbierta) {
        std::cout << "Contenido simulado de la página...\n";
        return;
    }

    std::cout << "\n+-------------- VENTANA: FILTROS --------------+\n";
    for (const GrupoFiltros& grupo : estado.gruposFiltros) {
        std::cout << "\n" << grupo.titulo << " ("
                  << Conversion::cantidadOpcionesATexto(grupo.opciones.size()) << ")\n";

        for (const std::string& opcion : grupo.opciones) {
            std::cout << "  [ ] " << opcion << '\n';
        }
    }
    std::cout << "\n[ C ] Cerrar\n";
    std::cout << "+------------------------------------------------+\n";
    std::cout << "Las opciones son visuales y todavía no aplican filtros.\n";
}
