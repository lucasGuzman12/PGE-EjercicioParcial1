#pragma once

#include "ModelosInterfaz.h"

#include <functional>
#include <memory>

class Aplicacion {
public:
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
    struct Implementacion;
    std::unique_ptr<Implementacion> implementacion_;
};
