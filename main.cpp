#include "Application.h"
#include "ConsoleRenderer.h"

int main() {
    ConsoleRenderer renderer;

    // La lambda es el callback de pintado. Mas adelante se reemplaza por el
    // codigo que dibuje la interfaz grafica definitiva.
    Application application([&renderer](const PageViewState& state) {
        renderer.paint(state);
    });

    application.run();
    return 0;
}
