#include <iostream>
using namespace std;

#include <QDesktopWidget>
#include <QMouseEvent>
#include <QFileDialog>
#include <QColorDialog>
#include <QSignalMapper>
#include <QMenuBar>
#include <QMenu>
#include "main_window.h"
#include "commands.h"
#include "draw_area.h"


/**
 * @brief MainWindow::MainWindow: En este metodo se instancia la ventana donde se va desarrrollar todo el programa
 *                                basicamente es una ventana con un Toolbar incrustado en la parte superior de la
 *                                ventana, los cuales accionan las funciones requeridas del Paint++ y algunas despliegan
 *                                ventanas tipo "dialog".
 */
MainWindow::MainWindow(QWidget* parent, const char* name)
    :QMainWindow(parent)
{
    // crea el drawarea, la cual recive los eventos del mouse para implementar las herramientas y usus funciones.
    drawArea = new DrawArea(this);
    drawArea->setStyleSheet("background-color:transparent");
    // se asignan las herramientas que tiene drawArea en una variable aparte dentro de la clase que defina la ventana
    // donde se desarrola el programa.
    currentTool = drawArea->getCurrentTool();
    // crea el ToolBar
    createMenuAndToolBar();

    pencilDialog = 0;
    penDialog = 0;
    eraserDialog = 0;
    shapesDialog = 0;
    etiqueta->setStyleSheet("background-color:"+ drawArea->getForegroundColor().name() );
    etiqueta->setFixedSize(25,25);
    estado->setFixedSize(65,25);
    estado->setText("Lapiz");
    setWindowTitle(name);
    resize(QDesktopWidget().availableGeometry(this).size()*.6);
    setContextMenuPolicy(Qt::PreventContextMenu);
    setCentralWidget(drawArea);
}

MainWindow::~MainWindow()
{
    toolActions.clear();
}

/**
 * @brief MainWindow::mousePressEvent: Este metodo despliega el "dialog" correspondiente a cada herramienta con sus propiedades
 *                                     al presionar click derecho, de la herramiente que se haya seleccionado.
 */

void MainWindow::mousePressEvent(QMouseEvent *e)
{

    if(e->button() == Qt::RightButton) {
        openToolDialog();
        estado->setText("DERECHA");
    }
}

/**
 * @brief MainWindow::OnNewImage: En este metodo se crea el nuevo objeto "image" que hace de lienzo, en él se definen las dimensiones
 *                                del nuevo lienzo.
 */
void MainWindow::OnNewImage()
{
    CanvasSizeDialog* newCanvas = new CanvasSizeDialog(this, "New Canvas");
    newCanvas->exec();
    if (newCanvas->result())
    {
        QSize size = QSize(newCanvas->getWidthValue(),
                           newCanvas->getHeightValue());
        drawArea->createNewImage(size);
    }
    delete newCanvas;
}

/**
 * @brief MainWindow::OnLoadImage: Este metodo se encarga de cargar una imagen formato Bitmap que este en el equipo, abre un objeto de tipo
 *                                  QFileDialogue el cual provee la interfaz para explorar en los archivos del equipo y abrir el arechivo necesario.
 */
void MainWindow::OnLoadImage()
{
    QString s = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    ".",
                                                    tr("BMP image (*.bmp)"));
	if (! s.isNull())
	{
        drawArea->loadImage(s);
	}
}

/**
 * @brief MainWindow::OnSaveImage Este metodo se encarga de guardar una imagen formato Bitmap con todo lo que se haya realizado en el editor de imagenes Paint++
 *                                abre un objeto de tipo qFileDialogue el cual provee la interfaz para ejecutar el explorador de archivos del equipo y }guardar
 *                                el archivo en la ubicación deseada.
 */
void MainWindow::OnSaveImage()
{
    if(drawArea->getImage()->isNull())
        return;

    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setAcceptMode(QFileDialog::AcceptSave);
    fileDialog->setDirectory(".");
    fileDialog->setNameFilter("BMP image (*.bmp)");
    fileDialog->setDefaultSuffix("bmp");
    fileDialog->exec();

    // Si el usuario presiona el boton Ok se guarda la imagen.
    if (fileDialog->result())
    {
        QString s = fileDialog->selectedFiles().first();

        if (! s.isNull())
        {
            drawArea->saveImage(s);
        }
    }
    delete fileDialog;
}

/**
 * @brief MainWindow::OnResizeImage: Este metodo se encarga de redimiensionar el tamaño del lienzo, provee un objeto de tipo Qdialog
 *                                   para proveer la interfaz necesaria pra reconfigurar el tam.
 *
 */
void MainWindow::OnResizeImage()
{
    QPixmap *image = drawArea->getImage();
    if(image->isNull())
        return;

    CanvasSizeDialog* newCanvas = new CanvasSizeDialog(this, "Resize Image",
                                                       image->width(),
                                                       image->height());
    newCanvas->exec();
    // Si el usuario presiona el boton Ok crea la nueva imagen con el nuevo tamaño.
    if (newCanvas->result())
    {
         drawArea->resizeImage(QSize(newCanvas->getWidthValue(),
                                     newCanvas->getHeightValue()));
    }
    delete newCanvas;
}

/**
 * @brief MainWindow:: Este metodo se encarga de accionar la funcion Picker del programa, primero le asigna valor verdadero a la variable
 *                     booleana dropperState que se encarga de determinar que esta funcion esta en ejecución.
 *                     Ademas cambia el color de la etiqueta que muestra cual color ha asignado a los trazos.
 */

void MainWindow::OnGetPixelColor(){

    drawArea->setDropperState(true);
    etiqueta->setStyleSheet("background-color:"+ drawArea->getForegroundColor().name() );
    estado->setText("Picker");
}
/**
 * @brief MainWindow::OnPickColor: Abre un QColorDialog ya sea el encargado de la configuracion de los colores de los trazos o el encargado
 *                                 de la configuracion del color del fondo del lienzo.
 */
void MainWindow::OnPickColor(int which)
{
    QColorDialog* colorDialog = new QColorDialog(this);
    QColor foregroundColor = drawArea->getForegroundColor();
    QColor backgroundColor = drawArea->getBackgroundColor();
    QColor color = colorDialog->getColor(which == foreground ? foregroundColor
                                                              : backgroundColor,
                                          this,
                                          which == foreground ? "Foreground Color"
                                                              : "Background Color",
                                          QColorDialog::DontUseNativeDialog);
    if (color.isValid())
        drawArea->updateColorConfig(color, which);

    delete colorDialog;
    etiqueta->setStyleSheet("background-color:"+ drawArea->getForegroundColor().name() );

}
/**
 * @brief MainWindow::OnChangeTool: Este metodo se encarga de abstraer la seleccion de cuatro herramientas diferentes en un solo
 *                                  metodo.
 *                                  -Pencil: Es el objeto que abstrae la funcion Lapiz, se asigna este si el parametro newTool es 0.
 *                                  -Pen:    Es el objeto que abstrae la funcion Lapicero, se asigna este si el parametro newTool es 1.
 *                                  -Eraser: Es el objeto que abstrae la funcion Borrador, se asigna este si el parametro newTool es 2.
 *                                  -Shapes: Es el objeto que abstrae la funcion de dibujar las figuras, se asigna este si el parametro newTool es 3.
 */
void MainWindow::OnChangeTool(int newTool)
{
    drawArea->setDropperState(false);
    currentTool = drawArea->setCurrentTool(newTool); // notify observer
    if(newTool == 0){estado->setText("Lapiz");}
    else if(newTool == 1){estado->setText("Lapicero");}
    else if(newTool == 2){estado->setText("Borrador");}
}
/**
 * @brief MainWindow::OnSelectRectangle: Este metodo ejecuta el metodo OnChangeTool con el parametro 3 para implementar la Herramienta Shapes que se encarga de
 *                                       ejecutar las funciones para dibujar las figuras, luego ejecuta el metodo propio de drawArea, con el parametro 0, que se encarga de asignar
 *                                       que la figura a dibujar es el rectangulo.
 */
void MainWindow::OnSelectRectangle(){
    OnChangeTool(3);
    drawArea->OnSelectShapeTypeConfig(0);
    estado->setText("Rectangulo");
}
/**
 * @brief MainWindow::OnSelectCircle:  Este metodo ejecuta el metodo OnChangeTool con el parametro 3 para implementar la Herramienta Shapes que se encarga de
 *                                       ejecutar las funciones para dibujar las figuras, luego ejecuta el metodo propio de drawArea, con el parametro 1, que se encarga de asignar
 *                                       que la figura a dibujar es el circulo.
 */
void MainWindow::OnSelectCircle(){
    OnChangeTool(3);
    drawArea->OnSelectShapeTypeConfig(1);
    estado->setText("Circulo");

}
/**
 * @brief MainWindow::OnSelectTriangle: Este metodo ejecuta el metodo OnChangeTool con el parametro 3 para implementar la Herramienta Shapes que se encarga de
 *                                       ejecutar las funciones para dibujar las figuras, luego ejecuta el metodo propio de drawArea, con el parametro 2, que se encarga de asignar
 *                                       que la figura a dibujar es el triangulo.
 */
void MainWindow::OnSelectTriangle(){
    OnChangeTool(3);
    drawArea->OnSelectShapeTypeConfig(2);
    estado->setText("Triangulo");
}
/**
 * @brief MainWindow::OnPenDialog: Abre el QDialog que se encarga de la configuracion del objeto Pencil, encargado de la funcion Lapiz.
 */
void MainWindow::OpenPencilDialog()
{
    if(!pencilDialog)
        pencilDialog = new PencilDialog(this, drawArea);

    if(pencilDialog && pencilDialog->isVisible())
        return;

    pencilDialog->show();
}

/**
 * @brief MainWindow::OnLineDialog: Abre el QDialog que se encarga de la configuracion del objeto Pencil, encargado de la funcion Lapiz.
 */
void MainWindow::OpenPenDialog()
{
    if(!penDialog)
        penDialog = new PenDialog(this, drawArea);

    if(penDialog && penDialog->isVisible())
        return;

    penDialog->show();

}

/**
 * @brief MainWindow::OnEraserDialog:  Abre el QDialog que se encarga de la configuracion del objeto Pencil, encargado de la funcion Lapiz.
 */
void MainWindow::OpenEraserDialog()
{
    if (!eraserDialog)
        eraserDialog = new EraserDialog(this, drawArea);

    if(eraserDialog->isVisible())
        return;

    eraserDialog->show();
}

/**
 * @brief MainWindow::OnShapesDialog:  Abre el QDialog que se encarga de la configuracion del objeto Shapes, encargado de la funcion que dibuja las figuras.
 */
void MainWindow::OpenShapesDialog()
{
    if (!shapesDialog)
        shapesDialog = new ShapesDialog(this, drawArea);

    if(shapesDialog->isVisible())
        return;

    shapesDialog->show();
}

/**
 * @brief MainWindow::openToolDialog: Este metodo se encarga de determinar que QDialog, se debe de abrir segun la herramienta que haya sido seleccionado.
 */
void MainWindow::openToolDialog()
{
    switch(currentTool->getType())
    {
        case pencil: OpenPencilDialog();             break;
        case pen: OpenPenDialog();           break;
        case eraser: OpenEraserDialog();       break;
        case shapes_tool: OpenShapesDialog(); break;
    }
}
/**
 * @brief ToolBar::createMenuAndToolBar:  Este metodo instancia el TollBar que tiene los botones encargados, de ejecutar las funciones del Paint++, le
 *                                        adiere una etiqueta para mostrar un texto que señala cual funcion se esta ejecutando y otra que indica  cual color
 *                                        tiene asignado los trazos.
 */
void MainWindow::createMenuAndToolBar()
{
    createMenuActions();
    toolbar = new ToolBar(this, toolActions);
    toolbar->layout();
    toolbar->addWidget(etiqueta);
    toolbar->addWidget(estado);
    addToolBar(toolbar);
}

/**
 * @brief MainWindow::createMenu:  En este metodo se instancian objetos de tipo QAction, a los cuales se les asigna un icono que sea significativo a las fuinciones que
 *                                 y herramientas que posee Paint++ y su respectiva funcion dentro de los objetos mencionados para luego añadirlos a una lista que se añade
 *                                 al ToolBar.
 *
 */
void MainWindow::createMenuActions()
{
    // Se cargan los archivos PNG que van ser usados como la imagen de los iconos que haran de botones dentro del ToolBar.
    QIcon new_canvas_Icon(":/icons/newIcon");
    QIcon open_image_Icon(":/icons/openIcon");
    QIcon save_image_Icon(":/icons/saveIcon");
    QIcon undo_command_Icon(":/icons/undoIcon");
    QIcon redo_command_Icon(":/icons/redoIcon");
    QIcon clear_canvas_Icon(":/icons/clearAllIcon");
    QIcon custom_size_canvas_Icon(":/icons/resizeIcon");
    QIcon fColorIcon(":/icons/fColorIcon");
    QIcon bColorIcon(":/icons/bColorIcon");
    QIcon penIcon(":/icons/penIcon");
    QIcon lineIcon(":/icons/lineIcon");
    QIcon eraserIcon(":/icons/eraserIcon");
    //QIcon rectIcon(":/icons/rectIcon");
    QIcon rectangleIcon(":/icons/rectangleIcon");
    QIcon circleIcon(":/icons/circleIcon");
    QIcon triangleIcon(":/icons/triangleIcon");
    QIcon dropperIcon(":/icons/dropperIcon");
    QIcon propertiesIcon(":/icons/propiedadesIcon");

    QMenu* barra_herramientas = new QMenu(tr("File"), this);

    QAction* newAction = barra_herramientas->addAction(new_canvas_Icon, tr("New image..."),this, SLOT(OnNewImage()), tr("Ctrl+N"));

    QAction* openAction = barra_herramientas->addAction(open_image_Icon, tr("Load image..."),this, SLOT(OnLoadImage()), tr("Ctrl+O"));

    QAction* saveAction = barra_herramientas->addAction(save_image_Icon, tr("Save image..."),this, SLOT(OnSaveImage()), tr("Ctrl+S"));

    QAction* undoAction = barra_herramientas->addAction(undo_command_Icon, tr("Undo"),drawArea, SLOT(OnUndo()), tr("Ctrl+Z"));

    QAction* redoAction = barra_herramientas->addAction(redo_command_Icon, tr("Redo"),drawArea, SLOT(OnRedo()), tr("Ctrl+Y"));

    QAction* clear_canvas_Action = barra_herramientas->addAction(clear_canvas_Icon, tr("Clear Canvas"),drawArea, SLOT(OnClearAll()), tr("Ctrl+C"));

    QAction* resizeAction = barra_herramientas->addAction( custom_size_canvas_Icon, tr("Custom Canvas"),this, SLOT(OnResizeImage()), tr("Ctrl+R"));

    QAction* dropperAction = barra_herramientas->addAction( dropperIcon, tr("Fill Picker"),this,SLOT(OnGetPixelColor()));

    QAction* rectangleAction = barra_herramientas->addAction(rectangleIcon, tr("Rectangle"),this, SLOT(OnSelectRectangle()));

    QAction* circleAction = barra_herramientas->addAction(circleIcon, tr("Circle"),this, SLOT(OnSelectCircle()));

    QAction* triangleAction = barra_herramientas->addAction(triangleIcon, tr("Triangle"),this, SLOT(OnSelectTriangle()));

    QAction* propertiesAction = barra_herramientas->addAction(propertiesIcon, tr("Draw Config"),this, SLOT(openToolDialog()));

    QSignalMapper *signalMapper = new QSignalMapper(this);

    QAction* fColorAction = new QAction(fColorIcon, tr("Foreground Color..."), this);
    connect(fColorAction, SIGNAL(triggered()),
            signalMapper, SLOT(map()));
    fColorAction->setShortcut(tr("Ctrl+F"));

    QAction* bColorAction = new QAction(bColorIcon, tr("Background Color..."), this);
    connect(bColorAction, SIGNAL(triggered()),
            signalMapper, SLOT(map()));
    bColorAction->setShortcut(tr("Ctrl+B"));

    signalMapper->setMapping(fColorAction, foreground);
    signalMapper->setMapping(bColorAction, background);

    connect(signalMapper, SIGNAL(mapped(int)),
            this, SLOT(OnPickColor(int)));

    barra_herramientas->addAction(fColorAction);
    barra_herramientas->addAction(bColorAction);


    QSignalMapper *signalMapperT = new QSignalMapper(this);

    QAction* penAction = new QAction(penIcon, tr("Pencil"), this);
    connect(penAction, SIGNAL(triggered()),
            signalMapperT, SLOT(map()));
    penAction->setShortcut(tr("W"));

    QAction* lineAction = new QAction(lineIcon, tr("Pen"), this);
    connect(lineAction, SIGNAL(triggered()),
            signalMapperT, SLOT(map()));
    lineAction->setShortcut(tr("F"));

    QAction* eraserAction = new QAction(eraserIcon, tr("Eraser"), this);
    connect(eraserAction, SIGNAL(triggered()),
            signalMapperT, SLOT(map()));
    eraserAction->setShortcut(tr("E"));




    signalMapperT->setMapping(penAction, pencil);
    signalMapperT->setMapping(lineAction, pen);
    signalMapperT->setMapping(eraserAction, eraser);

    connect(signalMapperT, SIGNAL(mapped(int)), this, SLOT(OnChangeTool(int)));

    toolActions.append(newAction);
    toolActions.append(openAction);
    toolActions.append(saveAction);
    toolActions.append(undoAction);
    toolActions.append(redoAction);
    toolActions.append(clear_canvas_Action);
    toolActions.append(resizeAction);
    toolActions.append(fColorAction);
    toolActions.append(bColorAction);
    toolActions.append(dropperAction);
    toolActions.append(penAction);
    toolActions.append(lineAction);
    toolActions.append(eraserAction);
    toolActions.append(rectangleAction);
    toolActions.append(circleAction);
    toolActions.append(triangleAction);
    toolActions.append(propertiesAction);
}

