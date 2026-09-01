#include "Aplicacion.h"

// Punto de entrada usado cuando el proyecto se compila como aplicacion Windows.
int WINAPI WinMain(HINSTANCE instancia, HINSTANCE, LPSTR, int mostrar) {
    Aplicacion app(instancia);
    return app.ejecutar(mostrar);
}

// Punto de entrada alternativo para compilar el mismo prototipo como consola.
int main() {
    Aplicacion app(GetModuleHandleW(NULL));
    return app.ejecutar(SW_SHOWDEFAULT);
}
