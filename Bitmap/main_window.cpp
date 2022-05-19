#include <iostream>
using namespace std;

#include "main_window.h"
#include "dialog_windows.h"

const int AlturaToolbar = 39;
const int BACKGROUND = 1;
const int FOREGROUND = 0;

/**
 * @brief MainWindow::MainWindow - the main window, parent to every other widget.
 */
MainWindow::MainWindow(QWidget* parent, const char* Nombre)
    :QMainWindow(parent)
{
    // create menu items
    QMenu* archivo = new QMenu("File", this);
    archivo->addAction("New image...", this, SLOT(NuevaImagen()));
    archivo->addAction("Load image...", this, SLOT(CargarImagen()));
    archivo->addAction("Save image...", this, SLOT(SalvarImagen()));
    menuBar()->addMenu(archivo);
    menuBar()->setNativeMenuBar(false);

    // create the toolbar
    toolbar = new ToolBar(this);
    addToolBar(toolbar);
    imagen = new QPixmap();

    setWindowTitle(Nombre);
    resize(QDesktopWidget().availableGeometry(this).size()*.6);
    setContextMenuPolicy(Qt::PreventContextMenu);
}

MainWindow::~MainWindow()
{
    delete imagen;
}

void MainWindow::EventoPintar(QPaintEvent* e)
{
    QPainter Pintar(this);
    if (! imagen->isNull())
    {
        Pintar.drawPixmap(0, menuBar()->height() + AlturaToolbar, (*imagen));
    }
}

/**
 * @brief MainWindow::OnNewImage - Open a NewCanvasDialogue prompting user to enter
 *                                  the dimensions for a new image.
 */
void MainWindow::NuevaImagen()
{
    CanvasSizeDialog* CanvasNuevo = new CanvasSizeDialog(this, "Canvas nuevo");
    CanvasNuevo->exec();
    // if user hit 'OK' button, create new image
    if (CanvasNuevo->result())
    {
        delete imagen;
        int ancho = CanvasNuevo->ObtenerValorAncho();
        int alto = CanvasNuevo->ObtenerValorAlto();
        imagen = new QPixmap(QSize(ancho,alto));
        imagen->fill(Qt::white);
        this->repaint();
    }
    // done with the dialog, free it
    delete CanvasNuevo;
}

/**
 * @brief MainWindow::OnLoadImage - Open a QFileDialogue prompting user to
 *                                  browse for a file to open.
 */
void MainWindow::CargarImagen()
{
    QString archivo = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("BMP image (*.bmp)"));
    if (!archivo.isNull())
    {
        imagen->load(archivo);
    }

    this->repaint();
}

/**
 * @brief MainWindow::OnSaveImage - Open a QFileDialogue prompting user to enter
 *                                  a filename and save location.
 */
void MainWindow::GuardarImagen()
{
    if(imagen->isNull())
        return;

    // appending suffixes doesn't work on native version, so use QT's instead
    QFileDialog *ArchivoDialog = new QFileDialog(this);
    ArchivoDialog->setAcceptMode(QFileDialog::AcceptSave);
    ArchivoDialog->setNameFilter("BMP image (*.bmp)");
    ArchivoDialog->setDefaultSuffix("bmp");
    ArchivoDialog->exec();

    // if user hit 'OK' button, save file
    if (ArchivoDialog->result())
    {
        QString archivo = ArchivoDialog->selectedFiles().first();

        if (! archivo.isNull())
        {
            imagen->save(archivo, "BMP");
        }
    }
    // done with the dialog, free it
    delete ArchivoDialog;
}

/**
 * @brief MainWindow::OnSaveImage - Undo a previous action.
 *
 */
void MainWindow::Undo()
{
    //stub
}

/**
 * @brief MainWindow::OnRedo - Redo a previously undone action.
 *
 */
void MainWindow::Redo()
{
    //stub
}

/**
 * @brief MainWindow::OnClearAll - Clear the image.
 *
 */
void MainWindow::Limpiar()
{
    if(imagen->isNull())
        return;

    imagen->fill(); // default is white
    this->repaint();
}

/**
 * @brief MainWindow::OnResizeImage - Change the dimensions of the image.
 *
 */
void MainWindow::CambiarTamano()
{
    if(imagen->isNull())
        return;

    CanvasSizeDialog* CanvasNuevo = new CanvasSizeDialog(this, "Cambiar tamano de Canvas",
                                                       imagen->width(), imagen->height());
    CanvasNuevo->exec();
    // if user hit 'OK' button, create new image
    if (CanvasNuevo->result())
    {
        int ancho = CanvasNuevo->ObtenerValorAncho();
        int alto = CanvasNuevo->ObtenerValorAlto();
        *imagen = imagen->scaled(QSize(ancho,alto), Qt::IgnoreAspectRatio);
        this->repaint();
    }
    // done with the dialog, free it
    delete CanvasNuevo;
}

/**
 * @brief MainWindow::OnPickColor - Open a QColorDialog prompting the user to select a color.
 *
 */
void MainWindow::EscogerColor(int ColorAEscoger)
{
    QColorDialog* colorDialog = new QColorDialog(this);
    QColor Color = colorDialog->getColor(ColorAEscoger == FOREGROUND ? ColorForeground : ColorBackground, this,"Foreground Color", QColorDialog::DontUseNativeDialog);

    // if user hit 'OK' button, change the color
    if (Color.isValid())
    {
       if(ColorAEscoger == FOREGROUND)
           ColorForeground = Color;
       else
           ColorBackground = Color;
    }
    // done with the dialog, free it
    delete colorDialog;
}

void MainWindow::EventoPresionarMouse(QMouseEvent * e)
{
    //paintEvent(new QPaintEvent(QRect(0,0,640,480)));
}

/**
 * @brief ToolBar::ToolBar - Wrapper class for QToolBar.
 *                           construct a ToolBar with icons & actions.
 */
ToolBar::ToolBar(QWidget *parent)
    : QToolBar(parent)
{
    // make sure we can't move or hide the toolbar
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setContextMenuPolicy(Qt::PreventContextMenu);
    setMovable(false);
    CrearAcciones();
}

void ToolBar::CrearAcciones()
{
    // load button icons from files
    QIcon new_icon("icons/new_icon.png");
    QIcon open_icon("icons/open_icon.png");
    QIcon save_icon("icons/save_icon.png");
    QIcon undo_icon("icons/undo_icon.png");
    QIcon redo_icon("icons/redo_icon.png");
    QIcon clear_icon("icons/clearall_icon.png");
    QIcon resize_icon("icons/resize_icon.png");
    QIcon fcolor_icon("icons/fcolor_icon.png");
    QIcon bcolor_icon("icons/bcolor_icon.png");
    QIcon pen_icon("icons/pen_icon.png");
    QIcon line_icon("icons/line_icon.png");
    QIcon eraser_icon("icons/eraser_icon.png");
    QIcon rect_icon("icons/rect_icon.png");

    MainWindow* mainWindow = (MainWindow*)this->parent();

    // create the actions for each button
    QAction *new_action = new QAction(new_icon, "New File", this);
    connect(new_action, &QAction::triggered, mainWindow, &MainWindow::NuevaImagen);

    QAction *open_action = new QAction(open_icon, "Open File", this);
    connect(open_action, &QAction::triggered, mainWindow, &MainWindow::CargarImagen);

    QAction *save_action = new QAction(save_icon, "Save File", this);
    connect(save_action, &QAction::triggered, mainWindow, &MainWindow::GuardarImagen);

    QAction *clear_action = new QAction(clear_icon, "Clear", this);
    connect(clear_action, &QAction::triggered, mainWindow, &MainWindow::Limpiar);

    QAction *resize_action = new QAction(resize_icon, "Resize", this);
    connect(resize_action, &QAction::triggered, mainWindow, &MainWindow::CambiarTamano);

    QAction *undo_action = new QAction(undo_icon, "Undo", this);
    connect(undo_action, &QAction::triggered, mainWindow, &MainWindow::Undo);

    QAction *redo_action = new QAction(redo_icon, "Redo", this);
    connect(redo_action, &QAction::triggered, mainWindow, &MainWindow::Redo);

    // color pickers
    QSignalMapper *signalMapper = new QSignalMapper(this);

    QAction *fcolor_action = new QAction(fcolor_icon, "Foreground Color", this);
    connect(fcolor_action, SIGNAL(triggered()), signalMapper, SLOT(map()));
    QAction *bcolor_action = new QAction(bcolor_icon, "Background Color", this);
    connect(bcolor_action, SIGNAL(triggered()), signalMapper, SLOT(map()));

    signalMapper->setMapping(fcolor_action, FOREGROUND);
    signalMapper->setMapping(bcolor_action, BACKGROUND);

    connect(signalMapper, SIGNAL(mapped(int)), mainWindow, SLOT(OnPickColor(int)));

    // add the actions
    this->addAction(new_action);
    this->addAction(open_action);
    this->addAction(save_action);
    this->addAction(undo_action);
    this->addAction(redo_action);
    this->addAction(clear_action);
    this->addAction(resize_action);
    this->addAction(fcolor_action);
    this->addAction(bcolor_action);
    this->addSeparator();
    this->addAction(pen_icon, "Pen Tool");
    this->addAction(line_icon, "Line Tool");
    this->addAction(eraser_icon, "Eraser");
    this->addAction(rect_icon, "Rectangle Tool");
}

/**
 * @brief CanvasSizeDialog::CanvasSizeDialog - Dialogue for creating a new canvas.
 *                                             Construct a dialog box containing
 *                                             QSpinBoxes and QPushButtons for this.
 */
CanvasSizeDialog::CanvasSizeDialog(QWidget* parent, const char* name, int width, int height)
    :QDialog(parent)
{
    createSpinBoxes(width,height);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(GrupoSpinBoxes);
    setLayout(layout);

    setWindowTitle(tr(name));
}

/**
 * @brief NewCanvasDialog::createSpinBoxes - Create the QSpinBoxes for the dialog
 *                                           box as well as the buttons.
 */
void CanvasSizeDialog::createSpinBoxes(int width, int height)
{
    GrupoSpinBoxes = new QGroupBox(tr("Tamano de imagen"), this);

    // the width field
    AnchoSpinBox = new QSpinBox(this);
    AnchoSpinBox ->setRange(1, 10000);
    AnchoSpinBox ->setSingleStep(1);
    AnchoSpinBox ->setValue(width);
    AnchoSpinBox ->setSuffix("px");

    // the height field
    AltoSpinBox = new QSpinBox(this);
    AltoSpinBox ->setRange(1, 10000);
    AltoSpinBox ->setSingleStep(1);
    AltoSpinBox ->setValue(height);
    AltoSpinBox ->setSuffix("px");

    // the buttons
    QPushButton *okButton = new QPushButton("OK", this);
    QPushButton *cancelButton = new QPushButton("Cancel", this);
    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

    // put it all together
    QFormLayout *LayoutSpinBox = new QFormLayout;
    LayoutSpinBox->addRow(tr("Ancho: "), AnchoSpinBox);
    LayoutSpinBox->addRow(tr("Alto: "), AltoSpinBox);
    LayoutSpinBox->addRow(okButton);
    LayoutSpinBox->addRow(cancelButton);
    GrupoSpinBoxes->setLayout(LayoutSpinBox);
}

/**
 * @brief NewCanvasDialog::getWidthValue
 *
 * @return the width selected by the user
 */
int CanvasSizeDialog::ObtenerValorAncho() const
{
    return AnchoSpinBox->value();
}

/**
 * @brief NewCanvasDialog::getHeightValue
 *
 * @return the height selected by the user
 */
int CanvasSizeDialog::ObtenerValorAlto() const
{
    return AltoSpinBox->value();
}
