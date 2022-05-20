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
 * @brief MainWindow::MainWindow - the main window, parent to every other
 *                                 widget.
 */
MainWindow::MainWindow(QWidget* parent, const char* name)
    :QMainWindow(parent)
{
    // create the DrawArea, which will receive the draw mouse events
    drawArea = new DrawArea(this);
    drawArea->setStyleSheet("background-color:transparent");

    // get default tool
    currentTool = drawArea->getCurrentTool();
    // create the menu and toolbar
    createMenuAndToolBar();

    // init dialog pointers to 0
    penDialog = 0;
    lineDialog = 0;
    eraserDialog = 0;
    rectDialog = 0;

    // adjust window size, name, & stop context menu
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
 * @brief MainWindow::mousePressEvent - On mouse right click, open dialog menu.
 *
 */

void MainWindow::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::RightButton) {
        openToolDialog();
    }
}

/**
 * @brief MainWindow::OnNewImage - Open a NewCanvasDialogue prompting user to
 *                                 enterthe dimensions for a new image.
 */
void MainWindow::OnNewImage()
{
    CanvasSizeDialog* newCanvas = new CanvasSizeDialog(this, "New Canvas");
    newCanvas->exec();
    // if user hit 'OK' button, create new image
    if (newCanvas->result())
    {
        QSize size = QSize(newCanvas->getWidthValue(),
                           newCanvas->getHeightValue());
        drawArea->createNewImage(size);
    }
    // done with the dialog, free it
    delete newCanvas;
}

/**
 * @brief MainWindow::OnLoadImage - Open a QFileDialogue prompting user to
 *                                  browse for a file to open.
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
 * @brief MainWindow::OnSaveImage - Open a QFileDialogue prompting user to
 *                                  enter a filename and save location.
 */
void MainWindow::OnSaveImage()
{
    if(drawArea->getImage()->isNull())
        return;

    // use custom dialog settings for appending suffixes
    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setAcceptMode(QFileDialog::AcceptSave);
    fileDialog->setDirectory(".");
    fileDialog->setNameFilter("BMP image (*.bmp)");
    fileDialog->setDefaultSuffix("bmp");
    fileDialog->exec();

    // if user hit 'OK' button, save file
    if (fileDialog->result())
    {
        QString s = fileDialog->selectedFiles().first();

        if (! s.isNull())
        {
            drawArea->saveImage(s);
        }
    }
    // done with the dialog, free it
    delete fileDialog;
}

/**
 * @brief MainWindow::OnResizeImage - Change the dimensions of the image.
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
    // if user hit 'OK' button, create new image
    if (newCanvas->result())
    {
         drawArea->resizeImage(QSize(newCanvas->getWidthValue(),
                                     newCanvas->getHeightValue()));
    }
    // done with the dialog, free it
    delete newCanvas;
}

/**
 * @brief MainWindow::OnPickColor - Open a QColorDialog prompting the user to
 *                                  select a color.
 *
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

    // if user hit 'OK' button, change the color
    if (color.isValid())
        drawArea->updateColorConfig(color, which);

    // done with the dialog, free it
    delete colorDialog;
}

/**
 * @brief MainWindow::OnChangeTool - Sets the current tool based on argument.
 *
 */
void MainWindow::OnChangeTool(int newTool)
{
    currentTool = drawArea->setCurrentTool(newTool); // notify observer
}

/**
 * @brief MainWindow::OnPenDialog - Open a PenDialog prompting the user to
 *                                  change pen settings.
 *
 */
void MainWindow::OnPenDialog()
{
    if(!penDialog)
        penDialog = new PenDialog(this, drawArea);

    if(penDialog && penDialog->isVisible())
        return;

    penDialog->show();
}

/**
 * @brief MainWindow::OnLineDialog - Open a LineDialog prompting the user to
 *                                   change line tool settings.
 *
 */
void MainWindow::OnLineDialog()
{
    if(!lineDialog)
        lineDialog = new LineDialog(this, drawArea);

    if(lineDialog && lineDialog->isVisible())
        return;

    lineDialog->show();
}

/**
 * @brief MainWindow::OnEraserDialog - Open a EraserDialog prompting the user
 *                                     to change eraser settings.
 *
 */
void MainWindow::OnEraserDialog()
{
    if (!eraserDialog)
        eraserDialog = new EraserDialog(this, drawArea);

    if(eraserDialog->isVisible())
        return;

    eraserDialog->show();
}

/**
 * @brief MainWindow::OnRectangleDialog - Open a RectDialog prompting the user
 *                                        to change rect tool settings.
 *
 */
void MainWindow::OnRectangleDialog()
{
    if (!rectDialog)
        rectDialog = new RectDialog(this, drawArea);

    if(rectDialog->isVisible())
        return;

    rectDialog->show();
}

/**
 * @brief MainWindow::openToolDialog - call the appropriate dialog function
 *                                     based on the current tool.
 *
 */
void MainWindow::openToolDialog()
{
    switch(currentTool->getType())
    {
        case pen: OnPenDialog();             break;
        case line: OnLineDialog();           break;
        case eraser: OnEraserDialog();       break;
        case shapes_tool: OnRectangleDialog(); break;
    }
}

/**
 * @brief ToolBar::createMenuAndToolBar() - ensure that everything gets
 *                                          created in the correct order
 *
 */
void MainWindow::createMenuAndToolBar()
{
    // create actions and add them to the menu
    createMenuActions();

    // create the toolbar
    toolbar = new ToolBar(this, toolActions);
    addToolBar(toolbar);

}

/**
 * @brief MainWindow::createMenu - create the actions that appear in the menu
 *
 */
void MainWindow::createMenuActions()
{
    // load button icons from files
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
    QIcon rectIcon(":/icons/rectIcon");
    QIcon propertiesIcon(":/icons/propiedadesIcon");

    QMenu* barra_herramientas = new QMenu(tr("File"), this);

    QAction* newAction = barra_herramientas->addAction(new_canvas_Icon, tr("New image..."),this, SLOT(OnNewImage()), tr("Ctrl+N"));

    QAction* openAction = barra_herramientas->addAction(open_image_Icon, tr("Load image..."),this, SLOT(OnLoadImage()), tr("Ctrl+O"));

    QAction* saveAction = barra_herramientas->addAction(save_image_Icon, tr("Save image..."),this, SLOT(OnSaveImage()), tr("Ctrl+S"));

    QAction* undoAction = barra_herramientas->addAction(undo_command_Icon, tr("Undo"),drawArea, SLOT(OnUndo()), tr("Ctrl+Z"));

    QAction* redoAction = barra_herramientas->addAction(redo_command_Icon, tr("Redo"),drawArea, SLOT(OnRedo()), tr("Ctrl+Y"));

    QAction* clear_canvas_Action = barra_herramientas->addAction(clear_canvas_Icon, tr("Clear Canvas"),drawArea, SLOT(OnClearAll()), tr("Ctrl+C"));

    QAction* resizeAction = barra_herramientas->addAction( custom_size_canvas_Icon, tr("Custom Canvas"),this, SLOT(OnResizeImage()), tr("Ctrl+R"));

    QAction* propertiesAction = barra_herramientas->addAction(propertiesIcon, tr("Draw Config"),this, SLOT(openToolDialog()));




    // color pickers (still under >Edit)
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

    // Tool pickers

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

    QAction* rectAction = new QAction(rectIcon, tr("Shapes Tool"), this);
    connect(rectAction, SIGNAL(triggered()),
            signalMapperT, SLOT(map()));
    rectAction->setShortcut(tr("R"));


    signalMapperT->setMapping(penAction, pen);
    signalMapperT->setMapping(lineAction, line);
    signalMapperT->setMapping(eraserAction, eraser);
    signalMapperT->setMapping(rectAction, shapes_tool);

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
    toolActions.append(penAction);
    toolActions.append(lineAction);
    toolActions.append(eraserAction);
    toolActions.append(rectAction);
    toolActions.append(propertiesAction);

}

