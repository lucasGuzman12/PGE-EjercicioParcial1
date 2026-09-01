#pragma once

#include <string>
#include <vector>

// Cada grupo representa una seccion ordenada de la futura ventana emergente.
// Por ahora las opciones son solamente datos: no filtran productos.
struct FilterGroup {
    std::string title;
    std::vector<std::string> options;
};

// Contrato entre el backend y cualquier interfaz grafica que se agregue.
struct PageViewState {
    std::string pageTitle;
    std::string filtersButtonLabel;
    bool isFilterPopupOpen{};
    std::vector<FilterGroup> filterGroups;
};
