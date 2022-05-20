#include <QPainter>
#include <QPaintEvent>

#include "commands.h"
#include "draw_area.h"
#include "main_window.h"


/**
 * @brief DrawArea::DrawArea - constructor for our Draw Area.
 *                             Pointers to the MainWindow's
 *                             tools and image are mandatory
 *
 */
DrawArea::DrawArea(QWidget *parent)
    : QWidget(parent)
{
    // initialize the undo stack
    undoStack = new QUndoStack(this);
    undoStack->setUndoLimit(UNDO_LIMIT);

    // initialize image
    image = new QPixmap();

    //create the pen, line, eraser, & rect tools
    createTools();

    // initialize colors
    foregroundColor = Qt::black;
    backgroundColor = Qt::white;

    // initialize state variables
    drawing = false;
    drawingPoly = false;
    currentLineMode = single;

    // small optimizations
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


/**
 * @brief DrawArea::paintEvent - redraw part of the image based
 *                               on what was modified
 *
 */
void DrawArea::paintEvent(QPaintEvent *e)

{
    QPainter painter(this);
    QRect modifiedArea = e->rect(); // only need to redraw a small area
    painter.drawPixmap(modifiedArea, *image, modifiedArea);
}

/**
 * @brief DrawArea::mousePressEvent - left-click initiates a draw
 *
 *                                  - right-click opens a dialog
 *                                    menu for the current tool
 *
 */
void DrawArea::mousePressEvent(QMouseEvent *e)
{

    if(e->button() == Qt::RightButton)
    {
        // open the dialog menu
        static_cast<MainWindow*>(parent())->mousePressEvent(e);
    }
    else if (e->button() == Qt::LeftButton)
    {
        if(image->isNull())
            return;

        drawing = true;

        if(!drawingPoly)
            currentTool->setStartPoint(e->pos());

        // save a copy of the old image
        oldImage = image->copy(QRect());
    }
}

/**
 * @brief DrawArea::mouseMoveEvent - draw
 *
 */
void DrawArea::mouseMoveEvent(QMouseEvent *e)
{
    if (e->buttons() & Qt::LeftButton && drawing)
    {
        if(image->isNull())
            return;

        ToolType type = currentTool->getType();
        if(type == line || type == shapes_tool)
        {
            *image = oldImage;
            if(type == line && currentLineMode == poly)
            {
                drawingPoly = true;
            }
        }
        currentTool->drawTo(e->pos(), this, image);
    }
}

/**
 * @brief DrawArea::mouseReleaseEvent - finish drawing
 *
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

        // for undo/redo - make sure there was a change
        // (in case drawing began off-image)
        if(oldImage.toImage() != image->toImage())
            saveDrawCommand(oldImage);
    }
}

/**
 * @brief DrawArea::mouseDoubleClickEvent - cancel poly mode
 *
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
 * @brief DrawArea::OnSaveImage - Undo a previous action
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
 * @brief DrawArea::OnRedo - Redo a previously undone action
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
 * @brief DrawArea::OnClearAll - Clear the image
 *
 */
void DrawArea::OnClearAll()
{
    if(image->isNull())
        return;

    clearImage();
}

/**
 * @brief DrawArea::OnPenCapConfig - Update cap style for pen tool
 *
 */


/**
 * @brief DrawArea::OnPenSizeConfig - Update pen size
 *
 */
void DrawArea::OnPencilSizeConfig(int value)
{
    pencilTool->setWidth(value);
}

/**
 * @brief DrawArea::OnEraserConfig - Update eraser thickness
 *
 */
void DrawArea::OnEraserConfig(int value)
{
    eraserTool->setWidth(value);
}

/**
 * @brief DrawArea::OnLineStyleConfig - Update line style for line tool
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
 * @brief DrawArea::OnLineCapConfig - Update cap style for line tool
 *
 */


/**
 * @brief DrawArea::OnDrawTypeConfig - Update draw type for line tool
 *
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
 * @brief DrawArea::OnLineThicknessConfig - Update line thickness for line tool
 *
 */
void DrawArea::OnPenLineThicknessConfig(int value)
{
    penTool->setWidth(value);
}

/**
 * @brief DrawArea::OnRectBStyleConfig - Update rectangle boundary line style
 *
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
 * @brief DrawArea::OnRectShapeTypeConfig - Update rectangle shape setting
 *
 */
void DrawArea::OnSelectShapeTypeConfig(int shape)
{
    switch (shape)
    {
        case rectangle: shapesTool->setShapeType(rectangle);                 break;
        case triangle: shapesTool->setShapeType(triangle); break;
        case ellipse: shapesTool->setShapeType(ellipse);                     break;
        default:                                                           break;
    }
}

/**
 * @brief DrawArea::OnRectFillConfig - Update rectangle fill setting
 *
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
 * @brief DrawArea::OnRectBTypeConfig - Update rectangle join style
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
 * @brief DrawArea::OnRectLineConfig - Update rectangle line width
 *
 */
void DrawArea::OnShapesLineConfig(int value)
{
    shapesTool->setWidth(value);
}


/**
 * @brief DrawArea::createNewImage - creates a new image of
 *                                   user-specified dimensions
 *
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
 * @brief DrawArea::loadImage - Load an image from a user-specified file
 *
 */
void DrawArea::loadImage(const QString &fileName)
{
    // save a copy of the old image
    oldImage = image->copy();

    image->load(fileName);
    update();

    // for undo/redo
    if(!imagesEqual(oldImage, *image))
        saveDrawCommand(oldImage);
}

/**
 * @brief DrawArea::saveImage - Save an image to user-specified file
 *
 */
void DrawArea::saveImage(const QString &fileName)
{
    image->save(fileName, "BMP");
}

/**
 * @brief DrawArea::resizeImage - Resize image to user-specified dimensions
 *
 */
void DrawArea::resizeImage(const QSize &size)
{
    // save a copy of the old image
    oldImage = image->copy();

    // if no change, do nothing
    if(image->size() == size)
    {
        return;
    }

    // else re-scale the image
    *image = image->scaled(size, Qt::IgnoreAspectRatio);
    update();

    // for undo/redo
    saveDrawCommand(oldImage);
}

/**
 * @brief DrawArea::clearImage - clears an image by filling it with
 *                               the background color
 *
 */
void DrawArea::clearImage()
{
    // save a copy of the old image
    oldImage = image->copy();

    image->fill(backgroundColor);
    update(image->rect());

    // for undo/redo
    if(!imagesEqual(oldImage, *image))
        saveDrawCommand(oldImage);
}

/**
 * @brief DrawArea::updateColorConfig - Updates the tools' colors
 *                                      as appropriate
 *
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
 * @brief DrawArea::setCurrentTool - Sets the current tool, unsetting
 *                                   poly mode if necessary
 *
 */
Tool* DrawArea::setCurrentTool(int newType)
{
    // get the current tool's type
    int currType = currentTool->getType();

    // if no change, return --else cancel poly mode & set tool
    if(newType == currType)
        return currentTool;

    if(currType == line)
        drawingPoly = false;

    switch(newType)
    {
        case pencil: currentTool = pencilTool;        break;
        case line: currentTool = penTool;      break;
        case eraser: currentTool = eraserTool;  break;
        case shapes_tool: currentTool = shapesTool; break;
        default:                                break;
    }
    return currentTool;
}

/**
 * @brief DrawArea::setLineMode - Sets the current line draw mode,
 *                                unsetting poly mode if necessary
 *
 */
void DrawArea::setLineMode(const DrawType mode)
{
    if(mode == single)
        drawingPoly = false;

    currentLineMode = mode;
}

/**
 * @brief DrawArea::SaveDrawCommand - Put together a DrawCommand
 *                                  and save it on the undo/redo stack.
 *
 */
void DrawArea::saveDrawCommand(const QPixmap &old_image)
{
    // put the old and new image on the stack for undo/redo
    QUndoCommand *drawCommand = new DrawCommand(old_image, image);
    undoStack->push(drawCommand);
}

/**
 * @brief DrawArea::createTools - takes care of creating the tools
 *
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
 * @brief imagesEqual - returns true if the two images are the same
 *
 */
bool imagesEqual(const QPixmap &image1, const QPixmap &image2)
{
    return image1.toImage() == image2.toImage();
}
