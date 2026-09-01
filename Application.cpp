#include "Application.h"

#include "Conversion.h"

#include <functional>
#include <iostream>
#include <queue>
#include <string>
#include <utility>
#include <vector>

namespace {
enum class UiEventType {
    OpenFiltersRequested,
    CloseFiltersRequested,
    PaintRequested
};

struct UiEvent {
    UiEventType type;
};

// Cola y bucle de eventos. Tambien procesa eventos agregados por un callback.
class EventLoop {
public:
    using EventHandler = std::function<void(const UiEvent&)>;

    void post(UiEvent event) {
        pendingEvents_.push(event);
    }

    void processPending(const EventHandler& handler) {
        while (!pendingEvents_.empty()) {
            UiEvent event = pendingEvents_.front();
            pendingEvents_.pop();
            handler(event);
        }
    }

private:
    std::queue<UiEvent> pendingEvents_;
};

// Boton reutilizable basado en un callback std::function<void()>.
class Button {
public:
    using ClickCallback = std::function<void()>;

    explicit Button(std::string label) : label_(std::move(label)) {}

    void setOnClick(ClickCallback callback) {
        onClick_ = std::move(callback);
    }

    void click() const {
        if (onClick_) {
            onClick_();
        }
    }

    const std::string& label() const {
        return label_;
    }

private:
    std::string label_;
    ClickCallback onClick_;
};

class FiltersPopupModel {
public:
    FiltersPopupModel()
        : groups_{
              {"Categoria", {"Tecnologia", "Hogar", "Indumentaria"}},
              {"Precio", {"Menos de $20.000", "$20.000 a $50.000", "Mas de $50.000"}},
              {"Ordenar por", {"Mas relevantes", "Menor precio", "Mayor precio"}},
              {"Disponibilidad", {"En stock", "Envio inmediato"}}} {}

    void open() {
        isOpen_ = true;
    }

    void close() {
        isOpen_ = false;
    }

    bool isOpen() const {
        return isOpen_;
    }

    const std::vector<FilterGroup>& groups() const {
        return groups_;
    }

private:
    bool isOpen_{};
    std::vector<FilterGroup> groups_;
};
}  // namespace

struct Application::Impl {
    explicit Impl(PaintCallback callback)
        : filtersButton("Filtros"),
          closeButton("Cerrar"),
          paintCallback(std::move(callback)) {
        // Callback principal: el click publica el evento que abre Filtros.
        filtersButton.setOnClick([this]() {
            eventLoop.post({UiEventType::OpenFiltersRequested});
        });

        closeButton.setOnClick([this]() {
            eventLoop.post({UiEventType::CloseFiltersRequested});
        });
    }

    PageViewState createViewState() const {
        return {
            "Pagina de productos (prototipo)",
            filtersButton.label(),
            filtersPopup.isOpen(),
            filtersPopup.groups()};
    }

    void requestPaint() {
        eventLoop.post({UiEventType::PaintRequested});
    }

    void handleEvent(const UiEvent& event) {
        switch (event.type) {
        case UiEventType::OpenFiltersRequested:
            filtersPopup.open();
            requestPaint();
            break;
        case UiEventType::CloseFiltersRequested:
            filtersPopup.close();
            requestPaint();
            break;
        case UiEventType::PaintRequested:
            // Evento Paint: redibuja usando el estado actual del backend.
            if (paintCallback) {
                paintCallback(createViewState());
            }
            break;
        }
    }

    void processEvents() {
        eventLoop.processPending([this](const UiEvent& event) {
            handleEvent(event);
        });
    }

    void dispatchCommand(ConsoleCommand command) {
        switch (command) {
        case ConsoleCommand::OpenFilters:
            filtersButton.click();
            processEvents();
            break;
        case ConsoleCommand::CloseFilters:
            closeButton.click();
            processEvents();
            break;
        case ConsoleCommand::Repaint:
            requestPaint();
            processEvents();
            break;
        case ConsoleCommand::Quit:
            isRunning = false;
            break;
        case ConsoleCommand::Invalid:
            std::cout << "Comando no reconocido.\n";
            break;
        }
    }

    bool isRunning{true};
    EventLoop eventLoop;
    FiltersPopupModel filtersPopup;
    Button filtersButton;
    Button closeButton;
    PaintCallback paintCallback;
};

Application::Application(PaintCallback paintCallback)
    : impl_(std::make_unique<Impl>(std::move(paintCallback))) {}

Application::~Application() = default;

void Application::onFiltersButtonClicked() {
    impl_->filtersButton.click();
    impl_->processEvents();
}

void Application::onCloseFiltersButtonClicked() {
    impl_->closeButton.click();
    impl_->processEvents();
}

void Application::repaint() {
    impl_->requestPaint();
    impl_->processEvents();
}

void Application::run() {
    repaint();

    // Bucle continuo de interaccion del prototipo.
    while (impl_->isRunning) {
        std::cout << "\nComando (f=filtros, c=cerrar, r=repintar, q=salir): ";

        std::string input;
        if (!std::getline(std::cin, input)) {
            impl_->isRunning = false;
            break;
        }

        impl_->dispatchCommand(Conversion::textToCommand(input));
    }

    std::cout << "Prototipo finalizado.\n";
}
