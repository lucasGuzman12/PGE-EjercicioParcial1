#include "Graficos.h"

Color::Color() : r(0), g(0), b(0) {}

Color::Color(int rojo, int verde, int azul) : r(rojo), g(verde), b(azul) {}

COLORREF Color::valor() const {
    return RGB(r, g, b);
}

EscalaVentana::EscalaVentana() : ancho(DISENO_ANCHO), alto(DISENO_ALTO), margenX(0) {}

// Calcula el area escalada disponible dentro de la ventana actual.
EscalaVentana EscalaVentana::desdeVentana(HWND ventana) {
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
int EscalaVentana::x(int valor) const {
    return margenX + (valor * ancho) / DISENO_ANCHO;
}

int EscalaVentana::y(int valor) const {
    return (valor * alto) / DISENO_ALTO;
}

int EscalaVentana::w(int valor) const {
    return (valor * ancho) / DISENO_ANCHO;
}

int EscalaVentana::h(int valor) const {
    return (valor * alto) / DISENO_ALTO;
}

Fuente::Fuente(int tamano, int peso, const wchar_t* nombre) {
    handle = CreateFontW(
        tamano, 0, 0, 0, peso, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, nombre
    );
}

Fuente::~Fuente() {
    DeleteObject(handle);
}

HFONT Fuente::obtener() const {
    return handle;
}

Lienzo::Lienzo(HDC contexto) : hdc(contexto) {}

// Aplica el escalado calculado para dibujar con coordenadas del diseno base.
void Lienzo::configurarEscala(const EscalaVentana& escala) {
    SetMapMode(hdc, MM_ISOTROPIC);
    SetWindowExtEx(hdc, DISENO_ANCHO, DISENO_ALTO, NULL);
    SetViewportExtEx(hdc, escala.ancho, escala.alto, NULL);
    SetViewportOrgEx(hdc, escala.margenX, 0, NULL);
}

// Dibuja un rectangulo simple o redondeado.
void Lienzo::rectangulo(int x, int y, int ancho, int alto, Color relleno, Color borde, int radio) {
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
void Lienzo::texto(const wstring& valor, int x, int y, int ancho, int alto, const Fuente& fuente, Color color, UINT formato) {
    RECT rect = { x, y, x + ancho, y + alto };
    SelectObject(hdc, fuente.obtener());
    SetTextColor(hdc, color.valor());
    SetBkMode(hdc, TRANSPARENT);
    DrawTextW(hdc, valor.c_str(), -1, &rect, formato);
}

// Dibuja figuras libres, como la marca del logo.
void Lienzo::poligono(POINT puntos[], int cantidad, Color relleno, Color borde) {
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
void Lienzo::limpiarRect(const RECT& rect, Color fondo) {
    HBRUSH brush = CreateSolidBrush(fondo.valor());
    FillRect(hdc, &rect, brush);
    DeleteObject(brush);
}

ElementoVisual::~ElementoVisual() {}

RectanguloVisual::RectanguloVisual(int px, int py, int pancho, int palto, Color prelleno, Color pborde, int pradio)
    : x(px), y(py), ancho(pancho), alto(palto), radio(pradio), relleno(prelleno), borde(pborde) {}

void RectanguloVisual::dibujar(Lienzo& lienzo) {
    lienzo.rectangulo(x, y, ancho, alto, relleno, borde, radio);
}

TextoVisual::TextoVisual(const wstring& pvalor, int px, int py, int pancho, int palto, int ptamano, int ppeso, Color pcolor, UINT pformato)
    : valor(pvalor), x(px), y(py), ancho(pancho), alto(palto), tamano(ptamano), peso(ppeso), colorTexto(pcolor), formato(pformato) {}

void TextoVisual::dibujar(Lienzo& lienzo) {
    Fuente fuente(tamano, peso);
    lienzo.texto(valor, x, y, ancho, alto, fuente, colorTexto, formato);
}
