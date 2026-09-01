#pragma once

#include "Constantes.h"

#include <windows.h>
#include <string>

using namespace std;

// Representa un color RGB y lo convierte al formato COLORREF que usa Win32.
class Color {
public:
    int r;
    int g;
    int b;

    Color() : r(0), g(0), b(0) {}
    Color(int rojo, int verde, int azul) : r(rojo), g(verde), b(azul) {}

    COLORREF valor() const {
        return RGB(r, g, b);
    }
};

// Convierte coordenadas del diseno base a las coordenadas reales de la ventana.
// Mantiene la proporcion del prototipo para que la interfaz no se deforme.
class EscalaVentana {
public:
    int ancho;
    int alto;
    int margenX;

    EscalaVentana() : ancho(DISENO_ANCHO), alto(DISENO_ALTO), margenX(0) {}

    // Calcula el area escalada disponible dentro de la ventana actual.
    static EscalaVentana desdeVentana(HWND ventana) {
        RECT cliente;
        GetClientRect(ventana, &cliente);

        int anchoCliente = cliente.right - cliente.left;
        int altoCliente = cliente.bottom - cliente.top;
        int escalaX = anchoCliente;
        int escalaY = altoCliente;

        if (escalaX * DISENO_ALTO > escalaY * DISENO_ANCHO) {
            escalaX = (escalaY * DISENO_ANCHO) / DISENO_ALTO;
        } else {
            escalaY = (escalaX * DISENO_ALTO) / DISENO_ANCHO;
        }

        EscalaVentana escala;
        escala.ancho = escalaX;
        escala.alto = escalaY;
        escala.margenX = (anchoCliente - escalaX) / 2;
        return escala;
    }

    // Conversiones de posicion y tamano desde la maqueta fija al viewport real.
    int x(int valor) const {
        return margenX + (valor * ancho) / DISENO_ANCHO;
    }

    int y(int valor) const {
        return (valor * alto) / DISENO_ALTO;
    }

    int w(int valor) const {
        return (valor * ancho) / DISENO_ANCHO;
    }

    int h(int valor) const {
        return (valor * alto) / DISENO_ALTO;
    }
};

// Maneja la fuente GDI y libera el recurso automaticamente al salir de scope.
class Fuente {
private:
    HFONT handle;

public:
    Fuente(int tamano, int peso = FW_NORMAL, const wchar_t* nombre = L"Segoe UI") {
        handle = CreateFontW(
            tamano, 0, 0, 0, peso, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, nombre
        );
    }

    ~Fuente() {
        DeleteObject(handle);
    }

    HFONT obtener() const {
        return handle;
    }
};

// Envoltorio del HDC de Win32. Centraliza las operaciones basicas de dibujo.
class Lienzo {
private:
    HDC hdc;

public:
    Lienzo(HDC contexto) : hdc(contexto) {}

    // Aplica el escalado calculado para dibujar con coordenadas del diseno base.
    void configurarEscala(const EscalaVentana& escala) {
        SetMapMode(hdc, MM_ISOTROPIC);
        SetWindowExtEx(hdc, DISENO_ANCHO, DISENO_ALTO, NULL);
        SetViewportExtEx(hdc, escala.ancho, escala.alto, NULL);
        SetViewportOrgEx(hdc, escala.margenX, 0, NULL);
    }

    // Dibuja un rectangulo simple o redondeado.
    void rectangulo(int x, int y, int ancho, int alto, Color relleno, Color borde, int radio = 0) {
        HBRUSH brush = CreateSolidBrush(relleno.valor());
        HPEN pen = CreatePen(PS_SOLID, 1, borde.valor());
        HGDIOBJ viejoBrush = SelectObject(hdc, brush);
        HGDIOBJ viejoPen = SelectObject(hdc, pen);

        if (radio > 0) {
            RoundRect(hdc, x, y, x + ancho, y + alto, radio, radio);
        } else {
            Rectangle(hdc, x, y, x + ancho, y + alto);
        }

        SelectObject(hdc, viejoBrush);
        SelectObject(hdc, viejoPen);
        DeleteObject(brush);
        DeleteObject(pen);
    }

    // Dibuja texto usando la fuente y el formato indicados.
    void texto(const wstring& valor, int x, int y, int ancho, int alto, const Fuente& fuente, Color color, UINT formato = DT_LEFT | DT_VCENTER | DT_SINGLELINE) {
        RECT rect = { x, y, x + ancho, y + alto };
        SelectObject(hdc, fuente.obtener());
        SetTextColor(hdc, color.valor());
        SetBkMode(hdc, TRANSPARENT);
        DrawTextW(hdc, valor.c_str(), -1, &rect, formato);
    }

    // Dibuja figuras libres, como la marca del logo.
    void poligono(POINT puntos[], int cantidad, Color relleno, Color borde) {
        HBRUSH brush = CreateSolidBrush(relleno.valor());
        HPEN pen = CreatePen(PS_SOLID, 1, borde.valor());
        HGDIOBJ viejoBrush = SelectObject(hdc, brush);
        HGDIOBJ viejoPen = SelectObject(hdc, pen);

        Polygon(hdc, puntos, cantidad);

        SelectObject(hdc, viejoBrush);
        SelectObject(hdc, viejoPen);
        DeleteObject(brush);
        DeleteObject(pen);
    }

    // Limpia una zona puntual, por ejemplo el fondo de un control owner-draw.
    void limpiarRect(const RECT& rect, Color fondo) {
        HBRUSH brush = CreateSolidBrush(fondo.valor());
        FillRect(hdc, &rect, brush);
        DeleteObject(brush);
    }
};

// Contrato comun para todo elemento que puede dibujarse sobre el lienzo.
class ElementoVisual {
public:
    virtual ~ElementoVisual() {}
    virtual void dibujar(Lienzo& lienzo) = 0;
};

// Elemento visual reutilizable para rectangulos.
class RectanguloVisual : public ElementoVisual {
private:
    int x;
    int y;
    int ancho;
    int alto;
    int radio;
    Color relleno;
    Color borde;

public:
    RectanguloVisual(int px, int py, int pancho, int palto, Color prelleno, Color pborde, int pradio = 0)
        : x(px), y(py), ancho(pancho), alto(palto), radio(pradio), relleno(prelleno), borde(pborde) {}

    void dibujar(Lienzo& lienzo) {
        lienzo.rectangulo(x, y, ancho, alto, relleno, borde, radio);
    }
};

// Elemento visual reutilizable para textos.
class TextoVisual : public ElementoVisual {
private:
    wstring valor;
    int x;
    int y;
    int ancho;
    int alto;
    int tamano;
    int peso;
    Color colorTexto;
    UINT formato;

public:
    TextoVisual(const wstring& pvalor, int px, int py, int pancho, int palto, int ptamano, int ppeso, Color pcolor, UINT pformato = DT_LEFT | DT_VCENTER | DT_SINGLELINE)
        : valor(pvalor), x(px), y(py), ancho(pancho), alto(palto), tamano(ptamano), peso(ppeso), colorTexto(pcolor), formato(pformato) {}

    void dibujar(Lienzo& lienzo) {
        Fuente fuente(tamano, peso);
        lienzo.texto(valor, x, y, ancho, alto, fuente, colorTexto, formato);
    }
};
