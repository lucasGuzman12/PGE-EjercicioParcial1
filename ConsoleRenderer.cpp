#include "ConsoleRenderer.h"

#include "Conversion.h"

#include <iostream>

void ConsoleRenderer::paint(const PageViewState& state) const {
    std::cout << "\n==================================================\n";
    std::cout << state.pageTitle << '\n';
    std::cout << "[ F ] " << state.filtersButtonLabel << '\n';
    std::cout << "Estado del panel: "
              << Conversion::popupStatusToText(state.isFilterPopupOpen) << '\n';
    std::cout << "==================================================\n";

    if (!state.isFilterPopupOpen) {
        std::cout << "Contenido simulado de la pagina...\n";
        return;
    }

    std::cout << "\n+-------------- VENTANA: FILTROS --------------+\n";
    for (const FilterGroup& group : state.filterGroups) {
        std::cout << "\n" << group.title << " ("
                  << Conversion::optionCountToText(group.options.size()) << ")\n";

        for (const std::string& option : group.options) {
            std::cout << "  [ ] " << option << '\n';
        }
    }
    std::cout << "\n[ C ] Cerrar\n";
    std::cout << "+------------------------------------------------+\n";
    std::cout << "Las opciones son visuales y todavia no aplican filtros.\n";
}
