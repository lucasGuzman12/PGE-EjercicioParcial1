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
    ModeloVentanaFiltros();

    void abrir();
    void cerrar();
    bool estaAbierta() const;

    const vector<GrupoFiltros>& grupos() const;
    bool estaMarcada(size_t grupo, size_t opcion) const;
    void alternarOpcion(size_t grupo, size_t opcion);
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
    VentanaFiltros();

    void abrir();
    void cerrar();
    bool estaAbierta() const;
    bool manejarClick(int mouseX, int mouseY);

    void dibujar(Lienzo& lienzo) override;

private:
    bool contiene(int px, int py) const;
    bool clickEnCruzCerrar(int px, int py) const;
    bool clickEnBotonCerrar(int px, int py) const;
    bool clickEnOpcion(int px, int py);

    int anchoColumna() const;
    int columnaX(size_t indiceGrupo) const;
    int grupoInicioY(size_t indiceGrupo) const;

    void dibujarContenedor(Lienzo& lienzo);
    void dibujarEncabezado(Lienzo& lienzo);
    void dibujarGrupos(Lienzo& lienzo);
    void dibujarGrupo(Lienzo& lienzo, const GrupoFiltros& grupo, size_t indiceGrupo, int grupoX, int inicioY);
    void dibujarBotonCerrar(Lienzo& lienzo);
};
