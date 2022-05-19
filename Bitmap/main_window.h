#include <QtWidgets>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>

#ifndef _MAIN_WINDOW_H
#define _MAIN_WINDOW_H

class ToolBar : public QToolBar
{
    Q_OBJECT

// ToolBar para las  herramientas del Paint //
public:
    ToolBar(QWidget* parent);

public slots:

private:
    void CrearAcciones();
};

class MainWindow: public QMainWindow {

    Q_OBJECT

    public:
    MainWindow(QWidget* parent = 0, const char* Nombre = 0);
    ~MainWindow();

    // Evento encargado de sabe que sucede al presionar el click izquierdo del mouse //
    void EventoPresionarMouse (QMouseEvent *);

    // Evento encargado de Pintar en el lienzo o imagen //
    void EventoPintar(QPaintEvent *);

    public slots:
    // Opciones respecto a la imagen del lienzo//
    void NuevaImagen();
    void CargarImagen();
    void GuardarImagen();
    // Opciones para deshacer y rehacer las acciones mas recientes //
    void Undo();
    void Redo();
    // Opciones para el tamano y color del lienzo //
    void CambiarTamano();
    void EscogerColor(int);

    void Limpiar();

    private:
    QPixmap* imagen;
    ToolBar* toolbar;
    QColor ColorForeground;
    QColor ColorBackground;
};

#endif
