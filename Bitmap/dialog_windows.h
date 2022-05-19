#include <qmessagebox.h>

#ifndef DIALOG_WINDOWS_H
#define DIALOG_WINDOWS_H

const int AnchoPorDefecto = 700;
const int AltoPorDefecto = 700;

class QSpinBox;
class QGroupBox;
class QLabel;

class CanvasSizeDialog : public QDialog
{
    Q_OBJECT

public:
    CanvasSizeDialog(QWidget* VentanaPrincipal, const char* Nombre = 0,
                     int ancho = AnchoPorDefecto,
                     int alto = AltoPorDefecto);

public slots:
    int ObtenerValorAncho() const;
    int ObtenerValorAlto() const;

private:
    void createSpinBoxes(int,int);

    QSpinBox *AnchoSpinBox;
    QSpinBox *AltoSpinBox;
    QGroupBox *GrupoSpinBoxes;
};

#endif // DIALOGS_H
