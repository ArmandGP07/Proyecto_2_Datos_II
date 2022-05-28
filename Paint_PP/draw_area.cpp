#include <QPainter>
#include <QPaintEvent>

#include "commands.h"
#include "draw_area.h"
#include "main_window.h"


/**
 * @brief DrawArea::DrawArea - Es el constructor del objeto drawArea que vendria a ser una super clase
 * que contiene el lienzo donde se va dibujar, ademas de distintos atributos de este, como de las funciones, ademas
 * de variables de estado de ciertos eventos que se pueden desarrollar en el programa que tienen que ver directamente
 * con todo lo de la edicion de las imagenes en sí.
 */
DrawArea::DrawArea(QWidget *parent)
    : QWidget(parent)
{
    // inicializa el objeto undoStack que es un tipo de pila que permite almacenar los diferentes estados de una
    // imagen que contiene todo lo que se haya editado dentro del programa.
    undoStack = new QUndoStack(this);
    undoStack->setUndoLimit(UNDO_LIMIT);

    // inicializa la imagen que vendria a ser el lienzo
    image = new QPixmap();

    //create the pen, line, eraser, & rect tools
    createTools();

    // iniciliza los colores por defecto
    foregroundColor = Qt::black;
    backgroundColor = Qt::white;
    // inicializa las variables de los estados de ciertos eventos o funciones que se estan ejecutando
    drawing = false;
    drawingPoly = false;
    dropperState = false;
    currentLineMode = single;

    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_StaticContents);
}

DrawArea::~DrawArea()
{
    delete image;
    delete pencilTool;
    delete penTool;
    delete eraserTool;
    delete shapesTool;
}

void DrawArea::paintEvent(QPaintEvent *e)

{
    QPainter painter(this);
    QRect modifiedArea = e->rect(); // only need to redraw a small area
    painter.drawPixmap(modifiedArea, *image, modifiedArea);
}

/**
 * @brief DrawArea::mousePressEvent Este metodo maneja los eventos correspondientes a ejecutarse, según la función
 *                                  del program en ejecucion cuando cuando se presiona algun bonton en el mouse.
 *                                  - clic-izquierdo inicia a dibujar con la funcion lapiz un trazo continuo.
 *                                  -clic-izquierdo ingresa el punto inicial y final en la función lapicero.
 *                                  -clic-izquierdo ingresa el punto inicial y final en la funciones que
 *                                   dibujan las figuras.
 *                                  - clic-derecho abre un "dialog" que tien las propiedades de la función
 *                                  seleccionada por el usuario.
 *
 */
void DrawArea::mousePressEvent(QMouseEvent *e)
{

    if(e->button() == Qt::RightButton)
    {
        // Abre el "dialog menu" segun la función seleccionada por el usuario.
        static_cast<MainWindow*>(parent())->mousePressEvent(e);
    }
    else if (e->button() == Qt::LeftButton)
    {
        if(image->isNull())
            return;
        if (dropperState){
            punto =e->pos();
            QColor color_temp = this->getImage()->toImage().pixelColor(this->getPOINT());
            if (color_temp.isValid())
                this->updateColorConfig(color_temp, foreground);
            static_cast<MainWindow*>(parent())->OnGetPixelColor();
            return;
        }
        drawing = true;

        if (dropperState){
            punto =e->pos();
        }

        if(!drawingPoly)
            currentTool->setStartPoint(e->pos());

        // guarda una copia de la anterior imagen a la nueva edicion.
        oldImage = image->copy(QRect());
    }
}

/**
 * @brief DrawArea::mouseMoveEvent:  Este metodo maneja los eventos correspondientes a ejecutarse, según la función
 *                                  del programa en ejecución cuando cuando se mueve el cursor del mouse.
 *                                  -Estira las figuras cuando se estan dibujando.
 *                                  -Estira la linea que se traza con la función lapicero.
 */
void DrawArea::mouseMoveEvent(QMouseEvent *e)
{
    if (e->buttons() & Qt::LeftButton && drawing)
    {
        if(image->isNull())
            return;

        ToolType type = currentTool->getType();
        if(type == pen || type == shapes_tool)
        {
            *image = oldImage;
            if(type == pen && currentLineMode == poly)
            {
                drawingPoly = true;
            }
        }
        currentTool->drawTo(e->pos(), this, image);
    }
}

/**
 * @brief DrawArea::mouseReleaseEvent: Este metodo maneja los eventos correspondientes a ejecutarse, según la función
 *                                  del programa en ejecución cuando cuando se deja de presionar el mouse.
 */
void DrawArea::mouseReleaseEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton && drawing)
    {
        drawing = false;

        if(image->isNull())
            return;

        if(drawingPoly)
        {
            currentTool->setStartPoint(e->pos());
            //return;
        }
        if(currentTool->getType() == pencil)
            currentTool->drawTo(e->pos(), this, image);

        if(oldImage.toImage() != image->toImage())
            saveDrawCommand(oldImage);
    }
}

/**
 * @brief DrawArea::mouseDoubleClickEvent:  Si en la funcion lapicero se escogio hacer trazos de un poligono
 *                                          al hacer doble clic se vuelve al trazo normal de una simple
 *                                          linea recta.
 */
void DrawArea::mouseDoubleClickEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        if(drawingPoly)
            drawingPoly = false;
    }
}
/**
 * @brief DrawArea::OnSaveImage: Este metode devuleve a su estado original la imagen antes del utltimo cambio
 *                              -Función "undo"
 *
 */
void DrawArea::OnUndo()
{
    if(!undoStack->canUndo())
        return;

    undoStack->undo();
    update();
}

/**
 * @brief DrawArea::OnRedo: Este metodo devuelve a un estado posterior la imagen si se a retrocedido a estados
 *                         previos de el ultimo cambio
 *                         -Función "redo"
 *
 */
void DrawArea::OnRedo()
{
    if(!undoStack->canRedo())
        return;

    undoStack->redo();
    update();
}

/**
 * @brief DrawArea::OnClearAll:  Limpia por completo la imagen dejandola a su estado original, dejando solo el color de fondo seleccionado
 *                               o el fondo blanco que es qel que se asigna por defecto.
 *
 */
void DrawArea::OnClearAll()
{
    if(image->isNull())
        return;

    clearImage();
}

/**
 * @brief DrawArea::OnPencilSizeConfig:  Este metodo configura el grozor del trazo de la clase "Pencil" que pertenece a la clase
 *                                       que abstrae la función Lapiz.
 */
void DrawArea::OnPencilSizeConfig(int value)
{
    pencilTool->setWidth(value);
}

/**
 * @brief DrawArea::OnEraserConfig:  Este metodo configura el grozor de la clase "Eraser" que pertenece a la clase
 *                                   que abstrae la función Borrador.
 */
void DrawArea::OnEraserConfig(int value)
{
    eraserTool->setWidth(value);
}

/**
 * @brief DrawArea::OnPenStyleConfig: Cambia el estilo del trazado del objeto Pen
 *                                   -SolidLine:     linea continua
 *                                   -DashLine:      linea de lineas espaciadas
 *                                   -DotLine:       linea punteada
 *                                   -DashDotLine:   linea de puntos y lineas
 *                                   -DashDotDotLine:linea de linea-punto-punto-linea
 *
 */
void DrawArea::OnPenLineStyleConfig(int lineStyle)
{
    switch (lineStyle)
    {
        case solid: penTool->setStyle(Qt::SolidLine);                break;
        case dashed: penTool->setStyle(Qt::DashLine);                break;
        case dotted: penTool->setStyle(Qt::DotLine);                 break;
        case dash_dotted: penTool->setStyle(Qt::DashDotLine);        break;
        case dash_dot_dotted: penTool->setStyle(Qt::DashDotDotLine); break;
        default:                                                      break;
    }
}
/**
 * @brief DrawArea::OnDrawTypeConfig: Configura las dos formas de implementar la funcion Lapicero(Pen)
 *                                   -single: Traza una linea recta entre dos puntos
 *                                   -poly: Traza una linea recta entre dos puntos
 *                                            toomando como punto inicial el ultimo
 *                                            punto de la ultima recta trazada.
 */
void DrawArea::OnPenDrawTypeConfig(int drawType)
{
    switch (drawType)
    {
        case single: setLineMode(single); break;
        case poly:   setLineMode(poly);   break;
        default:     break;
    }
}

/**
 * @brief DrawArea::OnPenLineThicknessConfig: Configura el grozor del trazo del Objeto Pen.
 */
void DrawArea::OnPenLineThicknessConfig(int value)
{
    penTool->setWidth(value);
}

/**
 * @brief DrawArea::OnShapesBStyleConfig:  Cambia el estilo del trazado del Objeto Shapes que se encarga de dibujar
 *                                         alguna de las figuras disponibles.
 *                                          -SolidLine:     linea continua.
 *                                          -DashLine:      linea de lineas espaciadas.
 *                                          -DotLine:       linea punteada.
 *                                          -DashDotLine:   linea de puntos y lineas.
 *                                          -DashDotDotLine:linea de linea-punto-punto-linea.
 */
void DrawArea:: OnShapesBStyleConfig(int boundaryStyle)
{
    switch (boundaryStyle)
    {
        case solid: shapesTool->setStyle(Qt::SolidLine);                break;
        case dashed: shapesTool->setStyle(Qt::DashLine);                break;
        case dotted: shapesTool->setStyle(Qt::DotLine);                 break;
        case dash_dotted: shapesTool->setStyle(Qt::DashDotLine);        break;
        case dash_dot_dotted: shapesTool->setStyle(Qt::DashDotDotLine); break;
        default:                                                      break;
    }
}

/**
 * @brief DrawArea::OnSelectShapeTypeConfig: Este metodo se encarga de asignar el tipo de figura
 *                                           que debe dibujar el objeto Shapes.
 *
 */
void DrawArea::OnSelectShapeTypeConfig(int shape)
{
    switch (shape)
    {
        case rectangle: shapesTool->setShapeType(rectangle);        break;
        case ellipse: shapesTool->setShapeType(ellipse);        break;
        case triangle: shapesTool->setShapeType(triangle);        break;
    default:                                                           break;
    }
}

/**
 * @brief DrawArea::OnShapesFillConfig:  Este metodo se encarga de configurar el relleno de las figuras geometricas.
 *                                       -foregraund: Dibuja una figura rellena de un mismo color al de sus bordes.
 *                                       -background: Dibuja una figura con el relleno del mismo color del fondo del lienzo.
 *                                       -no_fil:     Dibuja una figura sin relleno.
 */
void DrawArea::OnShapesFillConfig(int fillType)
{
    switch (fillType)
    {
        case foreground: shapesTool->setFillMode(foreground);
                         shapesTool->setFillColor(foregroundColor);      break;
        case background: shapesTool->setFillMode(background);
                         shapesTool->setFillColor(backgroundColor);      break;
        case no_fill: shapesTool->setFillMode(no_fill);
                      shapesTool->setFillColor(QColor(Qt::transparent)); break;
        default:                                                       break;
    }
}

/**
 * @brief DrawArea::OnShapesBTypeConfig: - Update rectangle join style
 *
 */
void DrawArea::OnShapesBTypeConfig(int boundaryType)
{
    switch (boundaryType)
    {
        case miter_join: shapesTool->setJoinStyle(Qt::MiterJoin);  break;
        case bevel_join: shapesTool->setJoinStyle(Qt::BevelJoin);  break;
        case round_join: shapesTool->setJoinStyle(Qt::RoundJoin);  break;
        default:                                                 break;
    }
}

/**
 * @brief DrawArea::OnShapesLineConfig: Este metodo confugra el grozor del trazo de las figuras.
 */
void DrawArea::OnShapesLineConfig(int value)
{
    shapesTool->setWidth(value);
}


/**
 * @brief DrawArea::createNewImage: Meto que crea el nuevo objeto de timo QPixmap que sera de lienzo
 *                                  el cual se le asigna a la variable "image".
 */
void DrawArea::createNewImage(const QSize &size)
{
    // save a copy of the old image
    oldImage = image->copy();

    *image = QPixmap(size);
    image->fill(backgroundColor);
    update();

    // for undo/redo
    if(!imagesEqual(oldImage, *image))
        saveDrawCommand(oldImage);
}

/**
 * @brief DrawArea::loadImage: Este metodo se encarga de abrir una imagen que este en el equipo, siempre
 *                             que esté en formato BitMap.
 */
void DrawArea::loadImage(const QString &fileName)
{
    // guarda una copia de "image" antes de que se cagrgue la imagen.
    oldImage = image->copy();

    image->load(fileName);
    update();

    // Guarda la copia hecha antes, en la lista que almacena
    //los estados para los comandos "undo" y "redo".
    if(!imagesEqual(oldImage, *image))
        saveDrawCommand(oldImage);
}

/**
 * @brief DrawArea::saveImage: Este metodo guarda todo lo realizado en el editor de imagenes
 *                             en un archivo en formato Bitmap.
 */
void DrawArea::saveImage(const QString &fileName)
{
    image->save(fileName, "BMP");
}

/**
 * @brief DrawArea::resizeImage: Este metodo se encarga de reconfigurar las dimensiones de "image"
 *                               que hace de lienzo.
 */
void DrawArea::resizeImage(const QSize &size)
{
    // Guarda una copia de "image" antes de que se realicen los cambios.
    oldImage = image->copy();

    // Se evalua si no hayc cambios algunos en la escogencia del usuario
    // para no hacer nada.
    if(image->size() == size)
    {
        return;
    }

    // "Si no" erealiza los cambios en las dimensiones
    *image = image->scaled(size, Qt::IgnoreAspectRatio);
    update();
    // Guarda la copia hecha antes, en la lista que almacena
    //los estados para los comandos "undo" y "redo".
    saveDrawCommand(oldImage);
}
/**
 * @brief DrawArea::clearImage: Borra todo lo hecho en elñ editor de imagenes.
 */
void DrawArea::clearImage()
{
    // Guarda una copia de "image" antes de que se realicen los cambios.
    oldImage = image->copy();
    image->fill(backgroundColor);
    update(image->rect());
    // Guarda la copia hecha antes, en la lista que almacena
    //los estados para los comandos "undo" y "redo".    if(!imagesEqual(oldImage, *image))
    saveDrawCommand(oldImage);
}
/**
 * @brief DrawArea::updateColorConfig: Este metodo se encarga de asignar los valores de los calores a las variables que se encargan
 *                                     tanto del color del fondo del lienzo como del
 *                                     color de los trazos.
 *                                     -foreground: variable que almacena el color de los trazos.
 *                                     -background: variable que almacena el color del fonmdo del lienzo.
 */
void DrawArea::updateColorConfig(const QColor &color, int which)
{
    if(which == foreground)
    {
         foregroundColor = color;
         pencilTool->setColor(foregroundColor);
         penTool->setColor(foregroundColor);
         shapesTool->setColor(foregroundColor);

         if(shapesTool->getFillMode() == foreground)
             shapesTool->setFillColor(foregroundColor);
    }
    else
    {
        backgroundColor = color;
        eraserTool->setColor(backgroundColor);

        if(shapesTool->getFillMode() == background)
            shapesTool->setFillColor(backgroundColor);
    }
}

/**
 * @brief DrawArea:setDropperState: Este metodo asigna el valor acttivado o desactivado
 *                                  a dropperStae, la cual va definir si se uysa o no
 *                                  la logica necesaria para usar todo lo involucrado
 *                                  a la funcion Picker.
 */
void DrawArea::setDropperState(bool state){
    dropperState = state;
}
/**
 * @brief DrawArea::setCurrentTool: Este metdo despoues de recibir cual herramienta
 *                                  se selecciona segun el boton que se encarga de esto
 *                                  se lo asigna "currentool" que es lña variable que se
 *                                  encarga de guardar cual herramienta se seleccionó.
 *
 */
Tool* DrawArea::setCurrentTool(int newType)
{
    // get the current tool's type
    int currType = currentTool->getType();

    // if no change, return --else cancel poly mode & set tool
    if(newType == currType)
        return currentTool;

    if(currType == pen)
        drawingPoly = false;

    switch(newType)
    {
        case pencil: currentTool = pencilTool;        break;
        case pen: currentTool = penTool;      break;
        case eraser: currentTool = eraserTool;  break;
        case shapes_tool: currentTool = shapesTool; break;
        default:                                break;
    }
    return currentTool;
}

/**
 * @brief DrawArea::setPenMode:
 */
void DrawArea::setLineMode(const DrawType mode)
{
    if(mode == single)
        drawingPoly = false;

    currentLineMode = mode;
}

/**
 * @brief DrawArea::SaveDrawCommand: Se encarga de apilar objetos de tipo QUndoComand, dentor de la pila undoStack, que es la que se encarga de almacenar los dieferentes estados del lienzo
 *                                   para hacer las funciones "undo" y "redo".
 *
 */
void DrawArea::saveDrawCommand(const QPixmap &old_image)
{
    // put the old and new image on the stack for undo/redo
    QUndoCommand *drawCommand = new DrawCommand(old_image, image);
    undoStack->push(drawCommand);
}

/**
 * @brief DrawArea::createTools: Este metodo es el que se encarga d e instanciar los objetos
 *                               que son las herramientas del Paint++.
 *                               -PencilTool: Objeto que se encarga de la función Lapiz.
 *                               -PenTool: Objeto que se encarda de la función Lapicero.
 *                               -EraserTool: Objeto que se encarga de la función Borrador.
 *                               -ShapesTool: Objeto que se encarga de dibujar las tres diferentes figuras
 *                                            rectangulo, círculo y triángulo.
 */
void DrawArea::createTools()
{
    // create the tools
    pencilTool = new PencilTool(QBrush(Qt::black), DEFAULT_PEN_THICKNESS);
    penTool = new PenTool(QBrush(Qt::black), DEFAULT_PEN_THICKNESS);
    eraserTool = new EraserTool(QBrush(Qt::white), DEFAULT_ERASER_THICKNESS);
    shapesTool = new ShapesTool(QBrush(Qt::black), DEFAULT_PEN_THICKNESS);
    // set default tool
    currentTool = static_cast<Tool*>(pencilTool);
}
/**
 * @brief imagesEqual: Este metodo antes de agregar el ultimo estado de "image", a la pila que guarda los estados para los
 *                      comandos "undo" y "redo", que el nuevo esatdo a guardar no sea igual que el ultimo que se guardo,
 *                      para asi evitar guardarlo dos veces
**/

bool imagesEqual(const QPixmap &image1, const QPixmap &image2)
{
    return image1.toImage() == image2.toImage();
}
