#include "Aplicacion.h"

#include "Conversion.h"

#include <functional>
#include <iostream>
#include <queue>
#include <string>
#include <utility>
#include <vector>

namespace {
// Tipos de eventos que puede procesar el prototipo.
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

    // Agrega un evento para que sea atendido en orden de llegada.
    void publicar(EventoInterfaz evento) {
        eventosPendientes_.push(evento);
    }

    // Bucle interno que consume todos los eventos pendientes.
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
    // Callback que se ejecuta al simular el clic.
    FuncionClic alHacerClic_;
};

// Estado propio del panel de filtros usado por la aplicacion.
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

// Implementacion privada de Aplicacion: concentra estado, botones y eventos.
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

    // Arma el estado que se envia al renderizador.
    EstadoVistaPagina crearEstadoVista() const {
        return {
            "Página de productos (prototipo)",
            botonFiltros.etiqueta(),
            ventanaFiltros.estaAbierta(),
            ventanaFiltros.grupos()};
    }

    // El pintado tambien se maneja como evento para mantener el mismo flujo.
    void solicitarPintado() {
        bucleEventos.publicar({TipoEventoInterfaz::PintadoSolicitado});
    }

    // Resuelve cada evento y actualiza el estado correspondiente.
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

    // Ejecuta la cola de eventos usando la propia aplicacion como manejador.
    void procesarEventos() {
        bucleEventos.procesarPendientes([this](const EventoInterfaz& evento) {
            manejarEvento(evento);
        });
    }

    // Convierte un comando ya interpretado en acciones del prototipo.
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

// API que simula el clic del boton Filtros.
void Aplicacion::alHacerClicEnBotonFiltros() {
    implementacion_->botonFiltros.hacerClic();
    implementacion_->procesarEventos();
}

// API que simula el clic del boton Cerrar.
void Aplicacion::alHacerClicEnBotonCerrarFiltros() {
    implementacion_->botonCerrar.hacerClic();
    implementacion_->procesarEventos();
}

// Fuerza un repintado con el estado actual.
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
