#pragma once

#include "UiModels.h"

#include <functional>
#include <memory>

class Application {
public:
    using PaintCallback = std::function<void(const PageViewState&)>;

    explicit Application(PaintCallback paintCallback);
    ~Application();

    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    // API publica que usaran los botones de la interfaz grafica definitiva.
    void onFiltersButtonClicked();
    void onCloseFiltersButtonClicked();
    void repaint();

    // Bucle interactivo usado solamente por el prototipo de consola.
    void run();

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};
