#pragma once

#include "Componentes.h"
#include "Filtros.h"

#include <windows.h>

// Coordina la ventana de Win32, los eventos del usuario y el repintado.
class Aplicacion {
private:
    HINSTANCE instancia;
    HWND ventana;
    PaginaEmpleos pagina;
    BotonFiltros botonFiltros;
    VentanaFiltros ventanaFiltros;

public:
    explicit Aplicacion(HINSTANCE pinstancia);

    // Crea la ventana y ejecuta el bucle principal de mensajes.
    int ejecutar(int mostrar);

    // Atiende los mensajes que Win32 envia a la ventana.
    LRESULT manejarMensaje(HWND hwnd, UINT mensaje, WPARAM wParam, LPARAM lParam);

private:
    // Registra la clase de ventana y conecta el callback principal.
    void registrarClase();

    // Crea la ventana principal que contiene la maqueta.
    void crearVentana();

    // Dibuja toda la pantalla dentro de BeginPaint/EndPaint.
    void dibujar(HWND hwnd);

    int convertirX(LPARAM lParam, HWND hwnd) const;
    int convertirY(LPARAM lParam, HWND hwnd) const;

    static LRESULT CALLBACK procedimientoVentana(HWND hwnd, UINT mensaje, WPARAM wParam, LPARAM lParam);
};
