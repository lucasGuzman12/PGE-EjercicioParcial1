#include "Conversion.h"

#include <algorithm>
#include <cctype>

namespace Conversion {
// Quita espacios iniciales/finales y pasa el texto a minusculas.
std::string normalizar(std::string texto) {
    const auto primero = std::find_if_not(texto.begin(), texto.end(), [](unsigned char caracter) {
        return std::isspace(caracter) != 0;
    });
    const auto ultimo = std::find_if_not(texto.rbegin(), texto.rend(), [](unsigned char caracter) {
                            return std::isspace(caracter) != 0;
                        }).base();

    if (primero >= ultimo) {
        return {};
    }

    std::string normalizado(primero, ultimo);
    std::transform(normalizado.begin(), normalizado.end(), normalizado.begin(), [](unsigned char caracter) {
        return static_cast<char>(std::tolower(caracter));
    });
    return normalizado;
}

// Traduce lo escrito por el usuario a un comando interno del programa.
ComandoConsola textoAComando(const std::string& texto) {
    const std::string comando = normalizar(texto);

    if (comando == "f" || comando == "filtros") {
        return ComandoConsola::AbrirFiltros;
    }
    if (comando == "c" || comando == "cerrar") {
        return ComandoConsola::CerrarFiltros;
    }
    if (comando == "r" || comando == "repintar") {
        return ComandoConsola::Repintar;
    }
    if (comando == "s" || comando == "salir") {
        return ComandoConsola::Salir;
    }
    return ComandoConsola::Invalido;
}

// Convierte el estado booleano del panel en texto para mostrarlo.
std::string estadoVentanaATexto(bool estaAbierta) {
    return estaAbierta ? "abierta" : "cerrada";
}

// Convierte una cantidad numerica en una frase legible.
std::string cantidadOpcionesATexto(std::size_t cantidad) {
    return std::to_string(cantidad) + (cantidad == 1 ? " opción" : " opciones");
}
}  // namespace Conversion
