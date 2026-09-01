#pragma once

#include "ModelosInterfaz.h"

#include <functional>
#include <memory>

// Clase principal del prototipo de consola.
// Separa la logica de eventos del modo concreto en que se pinta la interfaz.
class Aplicacion {
public:
    // Callback de pintado: cualquier renderizador puede recibir el estado actual.
    using FuncionPintado = std::function<void(const EstadoVistaPagina&)>;

    explicit Aplicacion(FuncionPintado funcionPintado);
    ~Aplicacion();

    Aplicacion(const Aplicacion&) = delete;
    Aplicacion& operator=(const Aplicacion&) = delete;

    // API pública que usarán los botones de la interfaz gráfica definitiva.
    void alHacerClicEnBotonFiltros();
    void alHacerClicEnBotonCerrarFiltros();
    void repintar();

    // Bucle interactivo usado solamente por el prototipo de consola.
    void ejecutar();

private:
    // Oculta los detalles internos para mantener una interfaz publica simple.
    struct Implementacion;
    std::unique_ptr<Implementacion> implementacion_;
};
