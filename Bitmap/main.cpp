#include <qapplication.h>
#include "main_window.h"

int main(int argc, char* argv[])
{
    QApplication aplicacion(argc, argv);
    QWidget* widget = new MainWindow(0, "Paint++");
    widget->show();
    return aplicacion.exec();
}
