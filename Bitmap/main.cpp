#include <qapplication.h>
#include "widget.h"

int main(int argc, char* argv[])
{
    QApplication aplicacion(argc, argv);
    QWidget* widget = new MyMainWindow();
    widget->show();
    return aplicacion.exec();
};
