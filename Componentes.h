#pragma once

#include "Graficos.h"

#include <string>

// Dibuja una version simplificada del logo de la Universidad Blas Pascal.
class LogoUBP : public ElementoVisual {
private:
    int x;
    int y;

public:
    LogoUBP(int px, int py);

    void dibujar(Lienzo& lienzo) override;
};

// Franja superior de la maqueta: logo y acceso de usuario.
class Header : public ElementoVisual {
private:
    LogoUBP logo;

public:
    Header();

    void dibujar(Lienzo& lienzo) override;
};

// Buscador visual del portal. En este prototipo no procesa texto real.
class Buscador : public ElementoVisual {
private:
    int x;
    int y;

public:
    Buscador(int px, int py);

    void dibujar(Lienzo& lienzo) override;
};

// Seccion principal que presenta el mensaje y contiene el buscador.
class Hero : public ElementoVisual {
private:
    Buscador buscador;

public:
    Hero();

    void dibujar(Lienzo& lienzo) override;
};

// Pequena etiqueta usada dentro de las tarjetas de ofertas.
class Etiqueta : public ElementoVisual {
private:
    wstring valor;
    int x;
    int y;
    int ancho;
    Color fondo;
    Color colorTexto;

public:
    Etiqueta(const wstring& pvalor, int px, int py, int pancho, Color pfondo, Color pcolorTexto);

    void dibujar(Lienzo& lienzo) override;
};

// Tarjeta que muestra una oferta laboral dentro del listado.
class TarjetaOferta : public ElementoVisual {
private:
    int x;
    int y;
    wstring logo;
    wstring titulo;
    wstring empresa;
    wstring publicado;

public:
    TarjetaOferta(int px, int py, const wstring& plogo, const wstring& ptitulo, const wstring& pempresa, const wstring& ppublicado);

    void dibujar(Lienzo& lienzo) override;
};

// Contenido principal de resultados: fondo y tarjetas de ofertas.
class Contenido : public ElementoVisual {
private:
    TarjetaOferta tarjetaIzquierda;
    TarjetaOferta tarjetaDerecha;

public:
    Contenido();

    void dibujar(Lienzo& lienzo) override;
};

// Boton real de Win32 que abre la ventana emergente de filtros.
// Es owner-draw para poder mantener el estilo visual de la maqueta.
class BotonFiltros {
private:
    HWND handle;
    int x;
    int y;
    int ancho;
    int alto;

public:
    BotonFiltros();

    void crear(HWND padre, HINSTANCE instancia);
    void posicionar(HWND padre);

    // Dibuja manualmente el boton cuando Win32 envia WM_DRAWITEM.
    void dibujar(const DRAWITEMSTRUCT* item);
};

// Composicion de la pagina completa.
class PaginaEmpleos : public ElementoVisual {
private:
    Header header;
    Hero hero;
    Contenido contenido;

public:
    void dibujar(Lienzo& lienzo) override;
};
