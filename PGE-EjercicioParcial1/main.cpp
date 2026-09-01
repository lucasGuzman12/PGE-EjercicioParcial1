#include "Aplicacion.h"
#include "RenderizadorConsola.h"

#ifdef _WIN32
#include <windows.h>
#endif

// Punto de entrada del prototipo de consola.
int main() {
#ifdef _WIN32
    // Configura la consola de Windows para mostrar texto UTF-8 correctamente.
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
#endif

    RenderizadorConsola renderizador;

    // La función lambda se encarga del pintado. Más adelante se reemplaza por el
    // código que dibuje la interfaz gráfica definitiva.
    Aplicacion aplicacion([&renderizador](const EstadoVistaPagina& estado) {
        renderizador.pintar(estado);
    });

    aplicacion.ejecutar();
    return 0;
}
