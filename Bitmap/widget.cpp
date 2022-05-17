#include <iostream>
#include "widget.h"
#include <qfiledialog.h>
#include <qpainter.h>
#include <qmessagebox.h>
#include <QMenuBar>
using namespace std;

// Funcion que permite cargar imagen //
void MyMainWindow::CargarImagen()
{
    // El QString permite que el programa detecte imagenes en formato Bitmap (.bmp) //
    QString archivo = QFileDialog::getOpenFileName(this, tr("Open File"),"/home", tr("Images (*.bmp)"));
    if (! archivo.isNull())
    {
        // Cargar el archivo .bmp //
        imagen->load(archivo);
    }

     this->repaint();
}

// Ventana principal del paint //
MyMainWindow::MyMainWindow(QWidget* VentanaPrincipal, const char* Nombre)
:QMainWindow(VentanaPrincipal)
{
    // Opcion que permite acceder a un archivo .bmp y cargarlo con un boton //
    QMenu* BotonArchivo = new QMenu("Archivo", this);
    BotonArchivo ->addAction("Cargar Imagen", this, SLOT(CargarImagen()));
    menuBar()->addMenu(BotonArchivo);
    imagen = new QPixmap();
}

//  //
MyMainWindow::~MyMainWindow()
{
    delete imagen;
}

// Funcion para pintar //
void MyMainWindow::EventoPintar(QPaintEvent* e)
{
    // Permite saber si hay una imagen en la cual se pueda pintar encima //
    QPainter pintar(this);
    if (! imagen->isNull())
    {
        pintar.drawPixmap(0, menuBar()->height(), (*imagen));
    }
}

// Funcion que detecta el click del mouse en el lienzo //
void MyMainWindow::EventoPresionarMouse (QMouseEvent * e)
{
    QMessageBox::about( this, "bitmap", "Mouse clickeado");
}
