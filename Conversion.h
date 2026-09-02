#pragma once

// ============================================================================
// ARCHIVO HISTORICO - NO FORMA PARTE DEL PROYECTO ACTUAL
// Este archivo queda como evidencia de una version inicial del prototipo que se
// probo en consola. La entrega actual utiliza la interfaz grafica Win32/GDI.
// ============================================================================

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
