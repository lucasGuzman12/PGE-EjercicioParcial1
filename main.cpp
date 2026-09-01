#include "Aplicacion.h"
#include "RenderizadorConsola.h"

#ifdef _WIN32
#include <windows.h>
#endif

int main() {
#ifdef _WIN32
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
