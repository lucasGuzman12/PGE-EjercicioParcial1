#include "Aplicacion.h"

#include "Conversion.h"

#include <functional>
#include <iostream>
#include <queue>
#include <string>
#include <utility>
#include <vector>

namespace {
enum class TipoEventoInterfaz {
    AperturaFiltrosSolicitada,
    CierreFiltrosSolicitado,
    PintadoSolicitado
};

struct EventoInterfaz {
    TipoEventoInterfaz tipo;
};

// Cola y bucle de eventos. También procesa eventos agregados por una función de retorno.
class BucleEventos {
public:
    using ManejadorEvento = std::function<void(const EventoInterfaz&)>;

    void publicar(EventoInterfaz evento) {
        eventosPendientes_.push(evento);
    }

    void procesarPendientes(const ManejadorEvento& manejador) {
        while (!eventosPendientes_.empty()) {
            EventoInterfaz evento = eventosPendientes_.front();
            eventosPendientes_.pop();
            manejador(evento);
        }
    }

private:
    std::queue<EventoInterfaz> eventosPendientes_;
};

// Botón reutilizable basado en una función de retorno std::function<void()>.
class Boton {
public:
    using FuncionClic = std::function<void()>;

    explicit Boton(std::string etiqueta) : etiqueta_(std::move(etiqueta)) {}

    void establecerAlHacerClic(FuncionClic funcion) {
        alHacerClic_ = std::move(funcion);
    }

    void hacerClic() const {
        if (alHacerClic_) {
            alHacerClic_();
        }
    }

    const std::string& etiqueta() const {
        return etiqueta_;
    }

private:
    std::string etiqueta_;
    FuncionClic alHacerClic_;
};

class ModeloVentanaFiltros {
public:
    ModeloVentanaFiltros()
        : grupos_{
              {"Categoría", {"Tecnología", "Hogar", "Indumentaria"}},
              {"Precio", {"Menos de $20.000", "$20.000 a $50.000", "Más de $50.000"}},
              {"Ordenar por", {"Más relevantes", "Menor precio", "Mayor precio"}},
              {"Disponibilidad", {"En existencias", "Envío inmediato"}}} {}

    void abrir() {
        estaAbierta_ = true;
    }

    void cerrar() {
        estaAbierta_ = false;
    }

    bool estaAbierta() const {
        return estaAbierta_;
    }

    const std::vector<GrupoFiltros>& grupos() const {
        return grupos_;
    }

private:
    bool estaAbierta_{};
    std::vector<GrupoFiltros> grupos_;
};
}  // namespace

struct Aplicacion::Implementacion {
    explicit Implementacion(FuncionPintado funcion)
        : botonFiltros("Filtros"),
          botonCerrar("Cerrar"),
          funcionPintado(std::move(funcion)) {
        // Función principal: el clic publica el evento que abre Filtros.
        botonFiltros.establecerAlHacerClic([this]() {
            bucleEventos.publicar({TipoEventoInterfaz::AperturaFiltrosSolicitada});
        });

        botonCerrar.establecerAlHacerClic([this]() {
            bucleEventos.publicar({TipoEventoInterfaz::CierreFiltrosSolicitado});
        });
    }

    EstadoVistaPagina crearEstadoVista() const {
        return {
            "Página de productos (prototipo)",
            botonFiltros.etiqueta(),
            ventanaFiltros.estaAbierta(),
            ventanaFiltros.grupos()};
    }

    void solicitarPintado() {
        bucleEventos.publicar({TipoEventoInterfaz::PintadoSolicitado});
    }

    void manejarEvento(const EventoInterfaz& evento) {
        switch (evento.tipo) {
        case TipoEventoInterfaz::AperturaFiltrosSolicitada:
            ventanaFiltros.abrir();
            solicitarPintado();
            break;
        case TipoEventoInterfaz::CierreFiltrosSolicitado:
            ventanaFiltros.cerrar();
            solicitarPintado();
            break;
        case TipoEventoInterfaz::PintadoSolicitado:
            // Evento de pintado: redibuja usando el estado actual de la lógica.
            if (funcionPintado) {
                funcionPintado(crearEstadoVista());
            }
            break;
        }
    }

    void procesarEventos() {
        bucleEventos.procesarPendientes([this](const EventoInterfaz& evento) {
            manejarEvento(evento);
        });
    }

    void procesarComando(ComandoConsola comando) {
        switch (comando) {
        case ComandoConsola::AbrirFiltros:
            botonFiltros.hacerClic();
            procesarEventos();
            break;
        case ComandoConsola::CerrarFiltros:
            botonCerrar.hacerClic();
            procesarEventos();
            break;
        case ComandoConsola::Repintar:
            solicitarPintado();
            procesarEventos();
            break;
        case ComandoConsola::Salir:
            enEjecucion = false;
            break;
        case ComandoConsola::Invalido:
            std::cout << "Comando no reconocido.\n";
            break;
        }
    }

    bool enEjecucion{true};
    BucleEventos bucleEventos;
    ModeloVentanaFiltros ventanaFiltros;
    Boton botonFiltros;
    Boton botonCerrar;
    FuncionPintado funcionPintado;
};

Aplicacion::Aplicacion(FuncionPintado funcionPintado)
    : implementacion_(std::make_unique<Implementacion>(std::move(funcionPintado))) {}

Aplicacion::~Aplicacion() = default;

void Aplicacion::alHacerClicEnBotonFiltros() {
    implementacion_->botonFiltros.hacerClic();
    implementacion_->procesarEventos();
}

void Aplicacion::alHacerClicEnBotonCerrarFiltros() {
    implementacion_->botonCerrar.hacerClic();
    implementacion_->procesarEventos();
}

void Aplicacion::repintar() {
    implementacion_->solicitarPintado();
    implementacion_->procesarEventos();
}

void Aplicacion::ejecutar() {
    repintar();

    // Bucle continuo de interacción del prototipo.
    while (implementacion_->enEjecucion) {
        std::cout << "\nComando (f=filtros, c=cerrar, r=repintar, s=salir): ";

        std::string entrada;
        if (!std::getline(std::cin, entrada)) {
            implementacion_->enEjecucion = false;
            break;
        }

        implementacion_->procesarComando(Conversion::textoAComando(entrada));
    }

    std::cout << "Prototipo finalizado.\n";
}
