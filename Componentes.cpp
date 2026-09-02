#include "Componentes.h"

LogoUBP::LogoUBP(int px, int py) : x(px), y(py) {}

void LogoUBP::dibujar(Lienzo& lienzo) {
    Color bordo(172, 28, 62);
    Color blanco(255, 255, 255);

    RectanguloVisual bloque(x + 26, y, 48, 48, bordo, bordo);
    bloque.dibujar(lienzo);

    POINT marca[] = {
        { x + 64, y + 7 },
        { x + 77, y + 14 },
        { x + 64, y + 21 }
    };
    lienzo.poligono(marca, 3, blanco, blanco);

    TextoVisual textoMarca(L"UBP", x + 31, y + 8, 40, 18, 15, FW_BOLD, blanco);
    TextoVisual universidad(L"UNIVERSIDAD", x, y + 60, 120, 18, 14, FW_NORMAL, bordo);
    TextoVisual nombre(L"Blas Pascal", x, y + 78, 140, 22, 18, FW_BOLD, bordo);

    textoMarca.dibujar(lienzo);
    universidad.dibujar(lienzo);
    nombre.dibujar(lienzo);
}

Header::Header() : logo(170, 17) {}

void Header::dibujar(Lienzo& lienzo) {
    Color blanco(255, 255, 255);
    Color rojo(239, 0, 0);
    Color sombra(230, 230, 235);

    RectanguloVisual fondo(0, 0, DISENO_ANCHO, 126, blanco, blanco);
    RectanguloVisual sombraUsuario(1509, 41, 244, 60, sombra, sombra, 28);
    RectanguloVisual botonUsuario(1506, 35, 244, 60, rojo, rojo, 28);
    TextoVisual textoUsuario(L"Clara  O  v", 1506, 35, 244, 60, 19, FW_BOLD, blanco, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    fondo.dibujar(lienzo);
    logo.dibujar(lienzo);
    sombraUsuario.dibujar(lienzo);
    botonUsuario.dibujar(lienzo);
    textoUsuario.dibujar(lienzo);
}

Buscador::Buscador(int px, int py) : x(px), y(py) {}

void Buscador::dibujar(Lienzo& lienzo) {
    Color blanco(255, 255, 255);
    Color rojo(239, 0, 0);
    Color textoOscuro(10, 20, 35);

    RectanguloVisual caja(x, y, 1138, 86, blanco, blanco, 42);
    RectanguloVisual boton(x + 972, y + 18, 142, 50, rojo, rojo, 25);
    TextoVisual palabraClave(L"universidad", x + 35, y + 2, 330, 82, 18, FW_NORMAL, textoOscuro);
    TextoVisual ubicacion(L"cordoba", x + 740, y + 2, 180, 82, 18, FW_NORMAL, textoOscuro);
    TextoVisual buscar(L"Buscar  O", x + 972, y + 18, 142, 50, 16, FW_BOLD, blanco, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    caja.dibujar(lienzo);
    palabraClave.dibujar(lienzo);
    ubicacion.dibujar(lienzo);
    boton.dibujar(lienzo);
    buscar.dibujar(lienzo);
}

Hero::Hero() : buscador(277, 343) {}

void Hero::dibujar(Lienzo& lienzo) {
    Color fondo(247, 247, 255);
    Color textoAzul(45, 67, 94);
    Color textoOscuro(0, 14, 28);

    RectanguloVisual contenedor(0, 126, DISENO_ANCHO, 377, fondo, fondo);
    TextoVisual titulo(L"Lanzate a por el trabajo que buscas", 277, 205, 760, 55, 40, FW_NORMAL, textoOscuro);
    TextoVisual subtitulo(L"Ofertas de trabajo para empezar tu carrera profesional o darle un giro de 180 grados.", 278, 276, 950, 34, 23, FW_NORMAL, textoAzul);

    contenedor.dibujar(lienzo);
    titulo.dibujar(lienzo);
    subtitulo.dibujar(lienzo);
    buscador.dibujar(lienzo);
}

Etiqueta::Etiqueta(const wstring& pvalor, int px, int py, int pancho, Color pfondo, Color pcolorTexto)
    : valor(pvalor), x(px), y(py), ancho(pancho), fondo(pfondo), colorTexto(pcolorTexto) {}

void Etiqueta::dibujar(Lienzo& lienzo) {
    RectanguloVisual contenedor(x, y, ancho, 28, fondo, fondo, 4);
    TextoVisual texto(valor, x, y, ancho, 28, 14, FW_NORMAL, colorTexto, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    contenedor.dibujar(lienzo);
    texto.dibujar(lienzo);
}

TarjetaOferta::TarjetaOferta(int px, int py, const wstring& plogo, const wstring& ptitulo, const wstring& pempresa, const wstring& ppublicado)
    : x(px), y(py), logo(plogo), titulo(ptitulo), empresa(pempresa), publicado(ppublicado) {}

void TarjetaOferta::dibujar(Lienzo& lienzo) {
    Color blanco(255, 255, 255);
    Color sombra(235, 235, 238);
    Color azulOscuro(48, 70, 94);
    Color crema(255, 250, 232);
    Color dorado(148, 110, 12);
    Color bordo(172, 28, 62);
    Color borde(238, 238, 238);

    RectanguloVisual sombraTarjeta(x + 2, y + 5, 658, 170, sombra, sombra, 4);
    RectanguloVisual contenedor(x, y, 658, 170, blanco, borde, 4);
    sombraTarjeta.dibujar(lienzo);
    contenedor.dibujar(lienzo);

    // Se dibuja una marca distinta segun la empresa de la oferta.
    if (logo == L"UBP") {
        RectanguloVisual marca(x + 35, y + 26, 25, 24, bordo, bordo);
        TextoVisual ubp(L"UBP", x + 37, y + 28, 24, 10, 9, FW_BOLD, blanco);
        TextoVisual universidad(L"UNIVERSIDAD", x + 20, y + 65, 70, 12, 9, FW_BOLD, bordo, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        TextoVisual nombre(L"Blas Pascal", x + 20, y + 77, 70, 12, 9, FW_BOLD, bordo, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

        marca.dibujar(lienzo);
        ubp.dibujar(lienzo);
        universidad.dibujar(lienzo);
        nombre.dibujar(lienzo);
    } else {
        TextoVisual marca(L"SPINLOCK", x + 20, y + 40, 75, 18, 9, FW_BOLD, Color(55, 105, 125));
        marca.dibujar(lienzo);
    }

    TextoVisual textoTitulo(titulo, x + 80, y + 22, 555, 32, 23, FW_BOLD, Color(0, 14, 28));
    TextoVisual textoEmpresa(empresa, x + 80, y + 55, 500, 28, 19, FW_NORMAL, azulOscuro);
    TextoVisual textoUbicacion(L"Cordoba (Argentina)", x + 20, y + 95, 300, 28, 19, FW_NORMAL, azulOscuro);
    TextoVisual textoPublicado(publicado + L" | 1 vacante", x + 20, y + 133, 300, 24, 16, FW_NORMAL, azulOscuro);
    Etiqueta presencial(L"Presencial", x + 458, y + 122, 90, azulOscuro, blanco);
    Etiqueta pasantia(L"Pasantia", x + 558, y + 122, 78, crema, dorado);

    textoTitulo.dibujar(lienzo);
    textoEmpresa.dibujar(lienzo);
    textoUbicacion.dibujar(lienzo);
    textoPublicado.dibujar(lienzo);
    presencial.dibujar(lienzo);
    pasantia.dibujar(lienzo);
}

Contenido::Contenido()
    : tarjetaIzquierda(276, 680, L"SPINLOCK", L"Pasantia Administrativa - Part Time (estudiantes...", L"Spinlock NMR", L"Publicado hace 3 dias"),
      tarjetaDerecha(968, 680, L"UBP", L"Pasante Facilitador Tecnologico", L"Universidad Blas Pascal", L"Publicado hace 7 dias") {}

void Contenido::dibujar(Lienzo& lienzo) {
    Color blanco(255, 255, 255);
    RectanguloVisual fondo(0, 503, DISENO_ANCHO, 520, blanco, blanco);

    fondo.dibujar(lienzo);
    tarjetaIzquierda.dibujar(lienzo);
    tarjetaDerecha.dibujar(lienzo);
}

BotonFiltros::BotonFiltros() : handle(NULL), x(238), y(575), ancho(190), alto(50) {}

void BotonFiltros::crear(HWND padre, HINSTANCE instancia) {
    handle = CreateWindowW(
        L"BUTTON",
        L"Filtros",
        WS_CHILD | WS_VISIBLE | BS_OWNERDRAW | WS_TABSTOP,
        0,
        0,
        ancho,
        alto,
        padre,
        reinterpret_cast<HMENU>(static_cast<INT_PTR>(ID_BOTON_FILTROS)),
        instancia,
        NULL
    );
    posicionar(padre);
}

void BotonFiltros::posicionar(HWND padre) {
    if (handle == NULL) {
        return;
    }

    // El boton tambien se escala para quedar alineado con el dibujo.
    EscalaVentana escala = EscalaVentana::desdeVentana(padre);
    MoveWindow(handle, escala.x(x), escala.y(y), escala.w(ancho), escala.h(alto), TRUE);
}

// Dibuja manualmente el boton cuando Win32 envia WM_DRAWITEM.
void BotonFiltros::dibujar(const DRAWITEMSTRUCT* item) {
    HDC hdc = item->hDC;
    RECT area = item->rcItem;
    int anchoActual = area.right - area.left;
    int altoActual = area.bottom - area.top;

    if (anchoActual <= 8 || altoActual <= 8) {
        return;
    }

    bool presionado = (item->itemState & ODS_SELECTED) != 0;
    bool enfocado = (item->itemState & ODS_FOCUS) != 0;
    int desplazamiento = presionado ? 2 : 0;
    Color fondoBoton = presionado ? Color(185, 0, 43) : Color(239, 0, 0);
    Color bordeBoton = presionado ? Color(145, 0, 34) : Color(205, 0, 30);
    Color blanco(255, 255, 255);

    Lienzo lienzo(hdc);
    lienzo.limpiarRect(area, blanco);

    // La sombra, el color intenso y el mayor tamano destacan la accion.
    RectanguloVisual sombra(
        3,
        4,
        anchoActual - 6,
        altoActual - 5,
        Color(205, 205, 215),
        Color(205, 205, 215),
        altoActual
    );
    RectanguloVisual fondo(
        1,
        1 + desplazamiento,
        anchoActual - 3,
        altoActual - 6,
        fondoBoton,
        bordeBoton,
        altoActual
    );

    sombra.dibujar(lienzo);
    fondo.dibujar(lienzo);

    // Icono de embudo dibujado con GDI para identificar visualmente Filtros.
    int tamanoIcono = altoActual / 3;
    int iconoX = altoActual / 3;
    int iconoY = (altoActual - tamanoIcono) / 2 + desplazamiento;
    int centroIcono = iconoX + tamanoIcono / 2;
    POINT embudo[] = {
        { iconoX, iconoY },
        { iconoX + tamanoIcono, iconoY },
        { centroIcono + 3, iconoY + tamanoIcono / 2 },
        { centroIcono + 3, iconoY + tamanoIcono },
        { centroIcono - 3, iconoY + tamanoIcono - 3 },
        { centroIcono - 3, iconoY + tamanoIcono / 2 }
    };
    lienzo.poligono(embudo, 6, blanco, blanco);

    int inicioTexto = iconoX + tamanoIcono + 8;
    int tamanoTexto = altoActual >= 40 ? 17 : 14;
    TextoVisual texto(
        L"FILTROS",
        inicioTexto,
        1 + desplazamiento,
        anchoActual - inicioTexto - 8,
        altoActual - 6,
        tamanoTexto,
        FW_BOLD,
        blanco,
        DT_CENTER | DT_VCENTER | DT_SINGLELINE
    );
    texto.dibujar(lienzo);

    // Conserva una indicacion visible para la navegacion con teclado.
    if (enfocado) {
        RECT areaFoco = { 7, 7, anchoActual - 7, altoActual - 10 };
        DrawFocusRect(hdc, &areaFoco);
    }
}

void PaginaEmpleos::dibujar(Lienzo& lienzo) {
    header.dibujar(lienzo);
    hero.dibujar(lienzo);
    contenido.dibujar(lienzo);
}
