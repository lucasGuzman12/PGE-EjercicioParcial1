#pragma once

#include <cstddef>
#include <string>

enum class ComandoConsola {
    AbrirFiltros,
    CerrarFiltros,
    Repintar,
    Salir,
    Invalido
};

// Funciones que convierten datos de entrada o del modelo a tipos útiles.
namespace Conversion {
std::string normalizar(std::string texto);
ComandoConsola textoAComando(const std::string& texto);
std::string estadoVentanaATexto(bool estaAbierta);
std::string cantidadOpcionesATexto(std::size_t cantidad);
}  // namespace Conversion
