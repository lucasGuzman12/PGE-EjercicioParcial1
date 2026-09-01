#pragma once

#include <cstddef>
#include <string>

enum class ConsoleCommand {
    OpenFilters,
    CloseFilters,
    Repaint,
    Quit,
    Invalid
};

// Funciones que convierten datos de entrada o del modelo a tipos utiles.
namespace Conversion {
std::string normalize(std::string text);
ConsoleCommand textToCommand(const std::string& text);
std::string popupStatusToText(bool isOpen);
std::string optionCountToText(std::size_t count);
}  // namespace Conversion
