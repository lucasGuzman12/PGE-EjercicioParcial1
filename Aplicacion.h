#pragma once

#include "Componentes.h"
#include "Filtros.h"

#include <windowsx.h>

// Coordina la ventana de Win32, los eventos del usuario y el repintado.
class Aplicacion {
private:
    HINSTANCE instancia;
    HWND ventana;
    PaginaEmpleos pagina;
    BotonFiltros botonFiltros;
    VentanaFiltros ventanaFiltros;

public:
    Aplicacion(HINSTANCE pinstancia) : instancia(pinstancia), ventana(NULL) {}

    // Crea la ventana y ejecuta el bucle principal de mensajes.
    int ejecutar(int mostrar) {
        registrarClase();
        crearVentana();

        if (ventana == NULL) {
            return 0;
        }

        ShowWindow(ventana, mostrar);
        UpdateWindow(ventana);

        MSG mensaje;
        ZeroMemory(&mensaje, sizeof(mensaje));

        // =====================================================================
        // PUNTO DE LA CONSIGNA: BUCLE DESPACHADOR DE EVENTOS
        // GetMessage obtiene cada evento; TranslateMessage lo interpreta y
        // DispatchMessage lo envia al callback procedimientoVentana.
        // El bucle mantiene activa la aplicacion hasta recibir WM_QUIT.
        // =====================================================================
        while (GetMessage(&mensaje, NULL, 0, 0)) {
            TranslateMessage(&mensaje);
            DispatchMessage(&mensaje);
        }

        return 0;
    }

    // Atiende los mensajes que Win32 envia a la ventana.
    LRESULT manejarMensaje(HWND hwnd, UINT mensaje, WPARAM wParam, LPARAM lParam) {
        switch (mensaje) {
            case WM_CREATE:
                botonFiltros.crear(hwnd, instancia);
                return 0;

            case WM_SIZE:
                botonFiltros.posicionar(hwnd);
                InvalidateRect(hwnd, NULL, TRUE);
                return 0;

            // =================================================================
            // PUNTO DE LA CONSIGNA: EVENTOS PAINT
            // WM_PAINT solicita repintar la ventana. dibujar() realiza el
            // proceso dentro de BeginPaint/EndPaint usando el estado actual.
            // =================================================================
            case WM_PAINT:
                dibujar(hwnd);
                return 0;

            case WM_DRAWITEM:
                // Evento Paint adicional para dibujar manualmente el boton.
                if (wParam == ID_BOTON_FILTROS) {
                    botonFiltros.dibujar((const DRAWITEMSTRUCT*)lParam);
                    return TRUE;
                }
                break;

            case WM_COMMAND:
                if (LOWORD(wParam) == ID_BOTON_FILTROS) {
                    ventanaFiltros.abrir();
                    InvalidateRect(hwnd, NULL, TRUE);
                    return 0;
                }
                break;

            case WM_LBUTTONDOWN:
                // Convierte el clic real de pantalla a coordenadas del diseno base.
                if (ventanaFiltros.manejarClick(convertirX(lParam, hwnd), convertirY(lParam, hwnd))) {
                    InvalidateRect(hwnd, NULL, TRUE);
                    return 0;
                }
                break;

            case WM_DESTROY:
                PostQuitMessage(0);
                return 0;
        }

        return DefWindowProc(hwnd, mensaje, wParam, lParam);
    }

private:
    // Registra la clase de ventana y conecta el callback principal.
    void registrarClase() {
        WNDCLASSW wc;
        ZeroMemory(&wc, sizeof(wc));
        wc.lpfnWndProc = Aplicacion::procedimientoVentana;
        wc.hInstance = instancia;
        wc.lpszClassName = NOMBRE_CLASE;
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

        RegisterClassW(&wc);
    }

    // Crea la ventana principal que contiene la maqueta.
    void crearVentana() {
        ventana = CreateWindowExW(
            0,
            NOMBRE_CLASE,
            L"Maqueta visual - Portal de empleos UBP",
            WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            1400,
            780,
            NULL,
            NULL,
            instancia,
            this
        );
    }

    // Dibuja toda la pantalla dentro de BeginPaint/EndPaint.
    void dibujar(HWND hwnd) {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        RECT cliente;
        GetClientRect(hwnd, &cliente);

        Lienzo lienzo(hdc);
        lienzo.rectangulo(
            0,
            0,
            cliente.right - cliente.left,
            cliente.bottom - cliente.top,
            Color(255, 255, 255),
            Color(255, 255, 255)
        );

        lienzo.configurarEscala(EscalaVentana::desdeVentana(hwnd));
        pagina.dibujar(lienzo);
        ventanaFiltros.dibujar(lienzo);

        EndPaint(hwnd, &ps);
    }

    // ========================================================================
    // PUNTO DE LA CONSIGNA: FUNCIONES DE CONVERSION
    // Estas funciones convierten las coordenadas reales recibidas desde Win32
    // al sistema de coordenadas fijo del diseno. Asi, los clics coinciden con
    // los elementos visuales aunque cambie el tamano de la ventana.
    // ========================================================================
    int convertirX(LPARAM lParam, HWND hwnd) const {
        EscalaVentana escala = EscalaVentana::desdeVentana(hwnd);
        if (escala.ancho == 0) {
            return 0;
        }

        int mouseX = GET_X_LPARAM(lParam) - escala.margenX;
        return (mouseX * DISENO_ANCHO) / escala.ancho;
    }

    int convertirY(LPARAM lParam, HWND hwnd) const {
        EscalaVentana escala = EscalaVentana::desdeVentana(hwnd);
        if (escala.alto == 0) {
            return 0;
        }

        int mouseY = GET_Y_LPARAM(lParam);
        return (mouseY * DISENO_ALTO) / escala.alto;
    }

    // ========================================================================
    // PUNTO DE LA CONSIGNA: CALLBACK
    // Win32 invoca automaticamente esta funcion cada vez que ocurre un evento.
    // El callback recupera la instancia de Aplicacion y delega el mensaje en
    // manejarMensaje(), donde se atienden clics, Paint y cierre de la ventana.
    // Se registra anteriormente mediante wc.lpfnWndProc.
    // ========================================================================
    static LRESULT CALLBACK procedimientoVentana(HWND hwnd, UINT mensaje, WPARAM wParam, LPARAM lParam) {
        Aplicacion* app = NULL;

        if (mensaje == WM_NCCREATE) {
            CREATESTRUCT* datosCreacion = (CREATESTRUCT*)lParam;
            app = (Aplicacion*)datosCreacion->lpCreateParams;
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)app);
        } else {
            app = (Aplicacion*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        }

        if (app != NULL) {
            return app->manejarMensaje(hwnd, mensaje, wParam, lParam);
        }

        return DefWindowProc(hwnd, mensaje, wParam, lParam);
    }
};
