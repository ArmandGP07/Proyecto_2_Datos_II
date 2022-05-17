#include <qmainwindow.h>
#include <qmessagebox.h>
//#include <qpopupmenu.h>
#include <QMenu>
#include <qmenubar.h>
#include <qapplication.h>
#include <qpixmap.h>

#ifndef _WIDGET_H
#define _WIDGET_H

class MyMainWindow: public QMainWindow {
    // All classes that contain signals or slots
    // must mention Q_OBJECT in their declaration.
    Q_OBJECT

    public:
    MyMainWindow(QWidget* VentanaPrincipal = 0, const char* Nombre = 0);
    ~MyMainWindow();

    // Evento encargado de sabe que sucede al presionar el click izquierdo del mouse //
    void EventoPresionarMouse (QMouseEvent *);

    // Evento encargado de Pintar en el lienzo o imagen //
    void EventoPintar(QPaintEvent *);

    public slots:
    void CargarImagen();

    private:
    QPixmap* imagen;
};

#endif
