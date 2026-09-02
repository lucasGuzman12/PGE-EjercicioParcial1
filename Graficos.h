#pragma once

#include "Constantes.h"

#include <windows.h>
#include <string>

using namespace std;

// APOYO A LAS FUNCIONES DE CONVERSION:
// transforma un color RGB del prototipo al formato COLORREF que usa Win32.
class Color {
public:
    int r;
    int g;
    int b;

    Color();
    Color(int rojo, int verde, int azul);

    COLORREF valor() const;
};

// APOYO A LAS FUNCIONES DE CONVERSION:
// convierte coordenadas del diseno base a coordenadas reales de la ventana y
// mantiene la proporcion del prototipo para que la interfaz no se deforme.
class EscalaVentana {
public:
    int ancho;
    int alto;
    int margenX;

    EscalaVentana();

    // Calcula el area escalada disponible dentro de la ventana actual.
    static EscalaVentana desdeVentana(HWND ventana);

    // Conversiones de posicion y tamano desde la maqueta fija al viewport real.
    int x(int valor) const;
    int y(int valor) const;
    int w(int valor) const;
    int h(int valor) const;
};

// Maneja la fuente GDI y libera el recurso automaticamente al salir de scope.
class Fuente {
private:
    HFONT handle;

public:
    Fuente(int tamano, int peso = FW_NORMAL, const wchar_t* nombre = L"Segoe UI");
    ~Fuente();

    HFONT obtener() const;
};

// Envoltorio del HDC de Win32. Centraliza las operaciones basicas de dibujo.
class Lienzo {
private:
    HDC hdc;

public:
    Lienzo(HDC contexto);

    // Aplica el escalado calculado para dibujar con coordenadas del diseno base.
    void configurarEscala(const EscalaVentana& escala);

    // Dibuja un rectangulo simple o redondeado.
    void rectangulo(int x, int y, int ancho, int alto, Color relleno, Color borde, int radio = 0);

    // Dibuja texto usando la fuente y el formato indicados.
    void texto(const wstring& valor, int x, int y, int ancho, int alto, const Fuente& fuente, Color color, UINT formato = DT_LEFT | DT_VCENTER | DT_SINGLELINE);

    // Dibuja figuras libres, como la marca del logo.
    void poligono(POINT puntos[], int cantidad, Color relleno, Color borde);

    // Limpia una zona puntual, por ejemplo el fondo de un control owner-draw.
    void limpiarRect(const RECT& rect, Color fondo);
};

// Contrato comun para todo elemento que puede dibujarse sobre el lienzo.
class ElementoVisual {
public:
    virtual ~ElementoVisual();
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
    RectanguloVisual(int px, int py, int pancho, int palto, Color prelleno, Color pborde, int pradio = 0);

    void dibujar(Lienzo& lienzo) override;
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
    TextoVisual(const wstring& pvalor, int px, int py, int pancho, int palto, int ptamano, int ppeso, Color pcolor, UINT pformato = DT_LEFT | DT_VCENTER | DT_SINGLELINE);

    void dibujar(Lienzo& lienzo) override;
};
