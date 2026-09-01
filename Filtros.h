#pragma once

#include "Graficos.h"

#include <string>
#include <vector>

// Datos de una seccion del panel de filtros.
struct GrupoFiltros {
    wstring titulo;
    vector<wstring> opciones;
};

// Modelo simple de estado: guarda si la ventana esta abierta y que filtros estan marcados.
class ModeloVentanaFiltros {
private:
    bool abierta;
    vector<GrupoFiltros> gruposFiltro;
    vector<vector<bool>> opcionesMarcadas;

public:
    ModeloVentanaFiltros()
        : abierta(false),
          gruposFiltro{
              { L"Area", { L"Tecnologia", L"Administracion", L"Marketing", L"Ventas" } },
              { L"Modalidad", { L"Presencial", L"Remoto", L"Hibrido" } },
              { L"Tipo de empleo", { L"Tiempo completo", L"Medio tiempo", L"Pasantia" } },
              { L"Ordenar por", { L"Mas relevantes", L"Mas recientes" } }
          } {
        for (size_t i = 0; i < gruposFiltro.size(); ++i) {
            opcionesMarcadas.push_back(vector<bool>(gruposFiltro[i].opciones.size(), false));
        }
    }

    void abrir() {
        abierta = true;
    }

    void cerrar() {
        abierta = false;
    }

    bool estaAbierta() const {
        return abierta;
    }

    const vector<GrupoFiltros>& grupos() const {
        return gruposFiltro;
    }

    bool estaMarcada(size_t grupo, size_t opcion) const {
        return grupo < opcionesMarcadas.size() && opcion < opcionesMarcadas[grupo].size() && opcionesMarcadas[grupo][opcion];
    }

    void alternarOpcion(size_t grupo, size_t opcion) {
        if (grupo < opcionesMarcadas.size() && opcion < opcionesMarcadas[grupo].size()) {
            opcionesMarcadas[grupo][opcion] = !opcionesMarcadas[grupo][opcion];
        }
    }
};

// Ventana emergente de filtros. Se dibuja solo cuando el modelo esta abierto.
class VentanaFiltros : public ElementoVisual {
private:
    ModeloVentanaFiltros modelo;
    int x;
    int y;
    int ancho;
    int alto;

public:
    VentanaFiltros() : x(1040), y(166), ancho(650), alto(560) {}

    void abrir() {
        modelo.abrir();
    }

    void cerrar() {
        modelo.cerrar();
    }

    bool estaAbierta() const {
        return modelo.estaAbierta();
    }

    bool manejarClick(int mouseX, int mouseY) {
        if (!estaAbierta()) {
            return false;
        }

        // Cierra el panel al hacer clic fuera, en la cruz o en el boton Cerrar.
        if (!contiene(mouseX, mouseY) || clickEnBotonCerrar(mouseX, mouseY) || clickEnCruzCerrar(mouseX, mouseY)) {
            cerrar();
            return true;
        }

        if (clickEnOpcion(mouseX, mouseY)) {
            return true;
        }

        return true;
    }

    void dibujar(Lienzo& lienzo) {
        if (!estaAbierta()) {
            return;
        }

        dibujarContenedor(lienzo);
        dibujarEncabezado(lienzo);
        dibujarGrupos(lienzo);
        dibujarBotonCerrar(lienzo);
    }

private:
    bool contiene(int px, int py) const {
        return px >= x && px <= x + ancho && py >= y && py <= y + alto;
    }

    bool clickEnCruzCerrar(int px, int py) const {
        int cerrarX = x + ancho - 58;
        int cerrarY = y + 24;
        return px >= cerrarX && px <= cerrarX + 34 && py >= cerrarY && py <= cerrarY + 34;
    }

    bool clickEnBotonCerrar(int px, int py) const {
        int botonX = x + 32;
        int botonY = y + alto - 68;
        int botonAncho = ancho - 64;
        int botonAlto = 42;
        return px >= botonX && px <= botonX + botonAncho && py >= botonY && py <= botonY + botonAlto;
    }

    bool clickEnOpcion(int px, int py) {
        const vector<GrupoFiltros>& grupos = modelo.grupos();

        for (size_t i = 0; i < grupos.size(); ++i) {
            int grupoX = columnaX(i);
            int opcionY = grupoInicioY(i) + 36;

            for (size_t j = 0; j < grupos[i].opciones.size(); ++j) {
                if (px >= grupoX - 8 && px <= grupoX + anchoColumna() && py >= opcionY - 1 && py <= opcionY + 28) {
                    modelo.alternarOpcion(i, j);
                    return true;
                }

                opcionY += 30;
            }
        }

        return false;
    }

    int anchoColumna() const {
        return (ancho - 92) / 2;
    }

    int columnaX(size_t indiceGrupo) const {
        int separacion = 28;
        int columna = static_cast<int>(indiceGrupo % 2);
        return x + 32 + columna * (anchoColumna() + separacion);
    }

    int grupoInicioY(size_t indiceGrupo) const {
        int fila = static_cast<int>(indiceGrupo / 2);
        return y + 114 + fila * 178;
    }

    void dibujarContenedor(Lienzo& lienzo) {
        Color sombra(222, 225, 232);
        Color blanco(255, 255, 255);
        Color borde(225, 229, 238);

        RectanguloVisual sombraPanel(x + 8, y + 10, ancho, alto, sombra, sombra, 8);
        RectanguloVisual panel(x, y, ancho, alto, blanco, borde, 8);

        sombraPanel.dibujar(lienzo);
        panel.dibujar(lienzo);
    }

    void dibujarEncabezado(Lienzo& lienzo) {
        Color textoOscuro(0, 14, 28);
        Color textoSuave(75, 91, 112);
        Color fondoCerrar(249, 249, 255);
        Color bordeCerrar(225, 229, 238);

        TextoVisual titulo(L"Filtros", x + 32, y + 24, 250, 36, 28, FW_BOLD, textoOscuro);
        TextoVisual subtitulo(L"Opciones disponibles", x + 32, y + 58, 250, 28, 16, FW_NORMAL, textoSuave);
        RectanguloVisual botonCerrar(x + ancho - 58, y + 24, 34, 34, fondoCerrar, bordeCerrar, 17);
        TextoVisual cruz(L"X", x + ancho - 58, y + 24, 34, 34, 16, FW_BOLD, textoOscuro, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

        titulo.dibujar(lienzo);
        subtitulo.dibujar(lienzo);
        botonCerrar.dibujar(lienzo);
        cruz.dibujar(lienzo);
    }

    void dibujarGrupos(Lienzo& lienzo) {
        const vector<GrupoFiltros>& grupos = modelo.grupos();

        for (size_t i = 0; i < grupos.size(); ++i) {
            dibujarGrupo(lienzo, grupos[i], i, columnaX(i), grupoInicioY(i));
        }
    }

    void dibujarGrupo(Lienzo& lienzo, const GrupoFiltros& grupo, size_t indiceGrupo, int grupoX, int inicioY) {
        Color textoOscuro(0, 14, 28);
        Color textoMedio(45, 67, 94);
        Color borde(214, 220, 231);
        Color blanco(255, 255, 255);
        Color rojo(239, 0, 0);
        Color fondoOpcion(249, 249, 255);

        TextoVisual titulo(grupo.titulo, grupoX, inicioY, anchoColumna(), 26, 18, FW_BOLD, textoOscuro);
        titulo.dibujar(lienzo);

        int opcionY = inicioY + 36;
        for (size_t i = 0; i < grupo.opciones.size(); ++i) {
            RectanguloVisual fondoFila(grupoX - 8, opcionY - 1, anchoColumna() + 8, 28, fondoOpcion, fondoOpcion, 5);
            RectanguloVisual caja(grupoX, opcionY + 5, 18, 18, blanco, borde, 3);
            TextoVisual texto(grupo.opciones[i], grupoX + 30, opcionY, anchoColumna() - 30, 28, 15, FW_NORMAL, textoMedio);

            fondoFila.dibujar(lienzo);
            caja.dibujar(lienzo);
            if (modelo.estaMarcada(indiceGrupo, i)) {
                RectanguloVisual marca(grupoX + 4, opcionY + 9, 10, 10, rojo, rojo, 2);
                marca.dibujar(lienzo);
            }
            texto.dibujar(lienzo);
            opcionY += 30;
        }
    }

    void dibujarBotonCerrar(Lienzo& lienzo) {
        Color rojo(239, 0, 0);
        Color blanco(255, 255, 255);

        RectanguloVisual boton(x + 32, y + alto - 68, ancho - 64, 42, rojo, rojo, 21);
        TextoVisual texto(L"Cerrar", x + 32, y + alto - 68, ancho - 64, 42, 17, FW_BOLD, blanco, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

        boton.dibujar(lienzo);
        texto.dibujar(lienzo);
    }
};
