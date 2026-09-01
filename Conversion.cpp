#include "Conversion.h"

#include <algorithm>
#include <cctype>

namespace Conversion {
std::string normalize(std::string text) {
    const auto first = std::find_if_not(text.begin(), text.end(), [](unsigned char character) {
        return std::isspace(character) != 0;
    });
    const auto last = std::find_if_not(text.rbegin(), text.rend(), [](unsigned char character) {
                          return std::isspace(character) != 0;
                      }).base();

    if (first >= last) {
        return {};
    }

    std::string normalized(first, last);
    std::transform(normalized.begin(), normalized.end(), normalized.begin(), [](unsigned char character) {
        return static_cast<char>(std::tolower(character));
    });
    return normalized;
}

ConsoleCommand textToCommand(const std::string& text) {
    const std::string command = normalize(text);

    if (command == "f" || command == "filtros") {
        return ConsoleCommand::OpenFilters;
    }
    if (command == "c" || command == "cerrar") {
        return ConsoleCommand::CloseFilters;
    }
    if (command == "r" || command == "repintar") {
        return ConsoleCommand::Repaint;
    }
    if (command == "q" || command == "salir") {
        return ConsoleCommand::Quit;
    }
    return ConsoleCommand::Invalid;
}

std::string popupStatusToText(bool isOpen) {
    return isOpen ? "abierta" : "cerrada";
}

std::string optionCountToText(std::size_t count) {
    return std::to_string(count) + (count == 1 ? " opcion" : " opciones");
}
}  // namespace Conversion
