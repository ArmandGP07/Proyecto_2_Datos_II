#include <QPushButton>
#include <QRadioButton>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

#include "dialog_windows.h"
#include "main_window.h"
#include "draw_area.h"


/**
 * @brief CanvasSizeDialog::CanvasSizeDialog: Este metodo es el constructor del objeto QDialog que muestra las opciones para
 *                                            redefinir el tamaño del lienzo del editor de imagenes.
 */
CanvasSizeDialog::CanvasSizeDialog(QWidget* parent, const char* name, int width, int height)
    :QDialog(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(createSpinBoxes(width,height));
    setLayout(layout);

    setWindowTitle(tr(name));
}

/**
 * @brief NewCanvasDialog::createSpinBoxes: Este metodo instancia un objeto QGroupBox que muestra las opciones para
 *                                            redefinir el tamaño del lienzo, el cual se añade al QDialog cque muestra
 *                                            las opciones para redimensionar el tamaño del lienzo.
 */
QGroupBox* CanvasSizeDialog::createSpinBoxes(int width, int height)
{
    QGroupBox *spinBoxesGroup = new QGroupBox(tr("Image Size"), this);

    // Se define el ancho del lienzo.
    widthSpinBox = new QSpinBox(this);
    widthSpinBox->setRange(MIN_IMG_WIDTH, MAX_IMG_WIDTH);
    widthSpinBox->setSingleStep(1);
    widthSpinBox->setValue(width);
    widthSpinBox->setSuffix("px");

    // Se define la altura del lienzo.
    heightSpinBox = new QSpinBox(this);
    heightSpinBox->setRange(MIN_IMG_HEIGHT, MAX_IMG_HEIGHT);
    heightSpinBox->setSingleStep(1);
    heightSpinBox->setValue(height);
    heightSpinBox->setSuffix("px");

    // Se agregan los botones.
    QPushButton *okButton = new QPushButton(tr("OK"), this);
    QPushButton *cancelButton = new QPushButton(tr("Cancel"), this);
    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

    // Integra todo al QGroupBox.
    QFormLayout *spinBoxLayout = new QFormLayout(spinBoxesGroup);
    spinBoxLayout->addRow(tr("Width: "), widthSpinBox);
    spinBoxLayout->addRow(tr("Height: "), heightSpinBox);
    spinBoxLayout->addRow(okButton);
    spinBoxLayout->addRow(cancelButton);
    spinBoxesGroup->setLayout(spinBoxLayout);

    return spinBoxesGroup;
}

/**
 * @brief PenDialog::PencilDialog: Este metodo es el constructor del objeto QDialog que muestra un SizeSlider para redefinir el grosor
 *                                 del trazo de la funcion lapiz.
 */
PencilDialog::PencilDialog(QWidget* parent, DrawArea* drawArea, int size)
    :QDialog(parent)
{
    setWindowTitle(tr("Pencil Properties"));

    this->drawArea = drawArea;

    QLabel *penSizeLabel = new QLabel(tr("Pencil Size"), this);
    pencilSizeSlider = new QSlider(Qt::Horizontal, this);
    pencilSizeSlider->setMinimum(MIN_PEN_SIZE);
    pencilSizeSlider->setMaximum(MAX_PEN_SIZE);
    pencilSizeSlider->setSliderPosition(size);
    pencilSizeSlider->setTracking(false);
    connect(pencilSizeSlider, SIGNAL(valueChanged(int)),
            drawArea, SLOT(OnPencilSizeConfig(int)));

    QVBoxLayout *vbox = new QVBoxLayout(this);
    vbox->addWidget(penSizeLabel);
    vbox->addWidget(pencilSizeSlider);
    setLayout(vbox);
}

/**
 * @brief PenDialog::LineSizeDialog: Este metodo es el constructor  del objeto QDialog que muestra un SizeSlider para redefinir el grosor
 *                                   del trazo de la funcion lapicero.
 */
PenDialog::PenDialog(QWidget* parent, DrawArea* drawArea,
                                        LineStyle lineStyle,
                                        DrawType drawType,
                                        int thickness)
    :QDialog(parent)
{
    setWindowTitle(tr("Pen Dialog"));

    QGroupBox*left = new QGroupBox(this);

    this->drawArea = drawArea;

    QVBoxLayout *vboxL = new QVBoxLayout(this);
    vboxL->addWidget(createLineStyle(lineStyle));
    left->setLayout(vboxL);

    QGroupBox *right = new QGroupBox(this);

    QVBoxLayout *vboxR = new QVBoxLayout(this);
    vboxR->addWidget(createDrawType(drawType));
    right->setLayout(vboxR);

    QLabel *lineThicknessLabel = new QLabel(tr("Line Thickness"), this);
    lineThicknessSlider = new QSlider(Qt::Horizontal, this);
    lineThicknessSlider->setMinimum(MIN_PEN_SIZE);
    lineThicknessSlider->setMaximum(MAX_PEN_SIZE);
    lineThicknessSlider->setSliderPosition(thickness);
    lineThicknessSlider->setTracking(false);
    connect(lineThicknessSlider, SIGNAL(valueChanged(int)), drawArea, SLOT(OnPenLineThicknessConfig(int)));

    QGridLayout *grid = new QGridLayout(this);
    grid->addWidget(left, 0,0);
    grid->addWidget(right, 0, 1);
    grid->addWidget(lineThicknessLabel, 2, 0, 1, 2);
    grid->addWidget(lineThicknessSlider, 3, 0, 1, 2);
    setLayout(grid);
}
/**
 * @brief PenDialog::createLineStyle: Este metodo es el que se usa para instanciar un QGroupBox que tiene las opciones de
 *                                    el tipo de linea que va trazar la funcion lapicero.
 *                                   -SolidLine:     linea continua
 *                                   -DashLine:      linea de lineas espaciadas
 *                                   -DotLine:       linea punteada
 *                                   -DashDotLine:   linea de puntos y lineas
 *                                   -DashDotDotLine:linea de linea-punto-punto-linea
 */
QGroupBox* PenDialog::createLineStyle(LineStyle lineStyle)
{
    QGroupBox *lineStyles = new QGroupBox(tr("Line Style"), this);
    QRadioButton *solidButton = new QRadioButton(tr("Solid"), this);
    QRadioButton *dashedButton = new QRadioButton(tr("Dashed"), this);
    QRadioButton *dottedButton = new QRadioButton(tr("Dotted"), this);
    QRadioButton *dashDottedButton = new QRadioButton(tr("Dash-Dotted"), this);
    QRadioButton *dashDotDottedButton = new QRadioButton(tr("Dash-Dot-Dotted"), this);

    penStyleG = new QButtonGroup(this);
    penStyleG->addButton(solidButton, 0);
    penStyleG->addButton(dashedButton, 1);
    penStyleG->addButton(dottedButton, 2);
    penStyleG->addButton(dashDottedButton, 3);
    penStyleG->addButton(dashDotDottedButton, 4);

    connect(penStyleG, SIGNAL(buttonClicked(int)), drawArea, SLOT(OnPenLineStyleConfig(int)));

    switch(lineStyle)
    {
        case solid: solidButton->setChecked(true);                   break;
        case dashed: dashedButton->setChecked(true);                 break;
        case dotted: dottedButton->setChecked(true);                 break;
        case dash_dotted: dashDottedButton->setChecked(true);        break;
        case dash_dot_dotted: dashDotDottedButton->setChecked(true); break;
        default:                                                     break;
    }

    QVBoxLayout *vbox = new QVBoxLayout(lineStyles);
    vbox->addWidget(solidButton);
    vbox->addWidget(dashedButton);
    vbox->addWidget(dottedButton);
    vbox->addWidget(dashDottedButton);
    vbox->addWidget(dashDotDottedButton);
    lineStyles->setLayout(vbox);

    return lineStyles;
}
/**
 * @brief PenDialog::createDrawType:  Este metodo es el que se usa para instanciar un QGroupBox que tiene las opciones de
 *                                    de dos tipos de ejecucion de la funcion lapicero.
 *                                   -single: Traza una linea recta entre dos puntos.
 *                                   -poly: Traza una linea recta entre dos puntos
 *                                            toomando como punto inicial el ultimo
 *                                            punto de la ultima recta trazada.
 */
QGroupBox* PenDialog::createDrawType(DrawType drawType)
{
    QGroupBox *drawTypes = new QGroupBox(tr("Draw Type"), this);
    QRadioButton *singleButton = new QRadioButton(tr("Single"), this);
    QRadioButton *polyButton = new QRadioButton(tr("Poly"), this);

    drawTypeG = new QButtonGroup(this);
    drawTypeG->addButton(singleButton, 0);
    drawTypeG->addButton(polyButton, 1);

    connect(drawTypeG, SIGNAL(buttonClicked(int)), drawArea, SLOT(OnPenDrawTypeConfig(int)));

    switch(drawType)
    {
        case single: singleButton->setChecked(true); break;
        case poly: polyButton->setChecked(true);     break;
        default:                                     break;
    }

    QVBoxLayout *vbox = new QVBoxLayout(drawTypes);
    vbox->addWidget(singleButton);
    vbox->addWidget(polyButton);
    drawTypes->setLayout(vbox);

    return drawTypes;
}

/**
 * @brief EraserDialog::EraserDialog: Este metodo es el constructor el objeto QDialog que muestra un SizeSlider para redefinir el grosor
 *                                    del objeto Eraser que se usa para ejecutar la función borrador.
 */
EraserDialog::EraserDialog(QWidget* parent, DrawArea* drawArea, int thickness)
    :QDialog(parent)
{
    setWindowTitle(tr("Eraser Dialog"));

    this->drawArea = drawArea;

    QLabel *eraserThicknessLabel = new QLabel(tr("Eraser Thickness"), this);
    eraserThicknessSlider = new QSlider(Qt::Horizontal, this);
    eraserThicknessSlider->setMinimum(MIN_PEN_SIZE);
    eraserThicknessSlider->setMaximum(MAX_PEN_SIZE);
    eraserThicknessSlider->setSliderPosition(thickness);
    eraserThicknessSlider->setTracking(false);
    connect(eraserThicknessSlider, SIGNAL(valueChanged(int)), drawArea, SLOT(OnEraserConfig(int)));

    QVBoxLayout *vbox = new QVBoxLayout(this);
    vbox->addWidget(eraserThicknessLabel);
    vbox->addWidget(eraserThicknessSlider);
    setLayout(vbox);
}

/**
 * @brief RectDialog::ShapesDialog: Este metodo es el constructor del Objeto QDialog que muestra las opciones para configurar como se van a dibuijar
 *                                  las figuras que dispone Paint++.
 *
 */
ShapesDialog::ShapesDialog(QWidget* parent, DrawArea* drawArea,
                                        LineStyle boundaryStyle,
                                        FillColor fillColor,
                                        int thickness)
    :QDialog(parent)
{
    setWindowTitle(tr("Shapes Properties"));
    this->drawArea = drawArea;

    QGroupBox*left = new QGroupBox(this);

    QVBoxLayout *vboxL = new QVBoxLayout(this);
    vboxL->addWidget(createBoundaryStyle(boundaryStyle));
    left->setLayout(vboxL);

    QGroupBox *right = new QGroupBox(this);

    QVBoxLayout *vboxR = new QVBoxLayout(this);
    vboxR->addWidget(createFillColor(fillColor));
    right->setLayout(vboxR);
    // Se usa un Slider que se usa para determinar el grosor de los bordes de las figuras.
    QLabel *lineThicknessLabel = new QLabel(tr("Line Thickness"), this);
    lineThicknessSlider = new QSlider(Qt::Horizontal, this);
    lineThicknessSlider->setMinimum(MIN_PEN_SIZE);
    lineThicknessSlider->setMaximum(MAX_PEN_SIZE);
    lineThicknessSlider->setSliderPosition(thickness);
    lineThicknessSlider->setTracking(false);

    connect(lineThicknessSlider, SIGNAL(valueChanged(int)), drawArea, SLOT(OnShapesLineConfig(int)));

    QGridLayout *grid = new QGridLayout(this);
    grid->addWidget(left, 0,0);
    grid->addWidget(right, 0, 1);
    grid->addWidget(lineThicknessLabel, 1, 0, 1, 2);
    grid->addWidget(lineThicknessSlider, 2, 0, 1, 2);
    setLayout(grid);
}
/**
 * @brief ShapesDialog::createBoundaryStyle: Este metodo es el que se usa para instanciar un QGroupBox que tiene las opciones de
 *                                    el tipo de linea que va trazar la funcion que dibuja las figuras.
 *                                   -SolidLine:     linea continua
 *                                   -DashLine:      linea de lineas espaciadas
 *                                   -DotLine:       linea punteada
 *                                   -DashDotLine:   linea de puntos y lineas
 *                                   -DashDotDotLine:linea de linea-punto-punto-linea
 */
QGroupBox* ShapesDialog::createBoundaryStyle(LineStyle boundaryStyle)
{
    QGroupBox *boundaryStyles = new QGroupBox(tr("Boundary Style"), this);
    QRadioButton *solidButton = new QRadioButton(tr("Solid"), this);
    QRadioButton *dashedButton = new QRadioButton(tr("Dashed"), this);
    QRadioButton *dottedButton = new QRadioButton(tr("Dotted"), this);
    QRadioButton *dashDottedButton = new QRadioButton(tr("Dash-Dotted"), this);
    QRadioButton *dashDotDottedButton = new QRadioButton(tr("Dash-Dot-Dotted"), this);

    boundaryStyleG = new QButtonGroup(this);
    boundaryStyleG->addButton(solidButton, 0 );
    boundaryStyleG->addButton(dashedButton, 1);
    boundaryStyleG->addButton(dottedButton, 2);
    boundaryStyleG->addButton(dashDottedButton, 3);
    boundaryStyleG->addButton(dashDotDottedButton, 4);

    connect(boundaryStyleG, SIGNAL(buttonClicked(int)), drawArea, SLOT(OnShapesBStyleConfig(int)));

    switch(boundaryStyle)
    {
        case solid: solidButton->setChecked(true);                   break;
        case dashed: dashedButton->setChecked(true);                 break;
        case dotted: dottedButton->setChecked(true);                 break;
        case dash_dotted: dashDottedButton->setChecked(true);        break;
        case dash_dot_dotted: dashDotDottedButton->setChecked(true); break;
        default:                                                     break;
    }

    QVBoxLayout *vbox = new QVBoxLayout(boundaryStyles);
    vbox->addWidget(solidButton);
    vbox->addWidget(dashedButton);
    vbox->addWidget(dottedButton);
    vbox->addWidget(dashDottedButton);
    vbox->addWidget(dashDotDottedButton);
    boundaryStyles->setLayout(vbox);

    return boundaryStyles;
}
/**
 * @brief ShapesDialog::createFillColor:  Este metodo es el constructor de un objeto QDialog que tiene las opciones para configurar el relleno
 *                                        de las figuras geometricas.
 *                                       -foregraund: Dibuja una figura rellena de un mismo color al de sus bordes.
 *                                       -background: Dibuja una figura con el relleno del mismo color del fondo del lienzo.
 *                                       -no_fil:     Dibuja una figura sin relleno.
 */
QGroupBox* ShapesDialog::createFillColor(FillColor fillColor)
{
    QGroupBox *fillColors = new QGroupBox(tr("Fill Color"), this);
    QRadioButton *foregroundButton = new QRadioButton(tr("Foreground"), this);
    QRadioButton *backgroundButton = new QRadioButton(tr("Background"), this);
    QRadioButton *noFillButton = new QRadioButton(tr("No Fill"), this);

    fillColorG = new QButtonGroup(this);
    fillColorG->addButton(foregroundButton, 0);
    fillColorG->addButton(backgroundButton, 1);
    fillColorG->addButton(noFillButton, 2);

    connect(fillColorG, SIGNAL(buttonClicked(int)), drawArea, SLOT(OnShapesFillConfig(int)));

    switch(fillColor)
    {
        case foreground: foregroundButton->setChecked(true); break;
        case background: backgroundButton->setChecked(true); break;
        case no_fill: noFillButton->setChecked(true);        break;
        default:                                             break;
    }

    QVBoxLayout *vbox = new QVBoxLayout(fillColors);
    vbox->addWidget(foregroundButton);
    vbox->addWidget(backgroundButton);
    vbox->addWidget(noFillButton);
    fillColors->setLayout(vbox);

    return fillColors;
}


