#include "commands.h"
#include "qrect.h"


/**
 * @Luis Pablo DrawCommand::DrawCommand - A command that keeps a copy of the image
 *
 *                                  before and after something is drawn
 *
 *
 */
DrawCommand::DrawCommand(const QPixmap &oldImage, QPixmap *image,
                               QUndoCommand *parent)
    : QUndoCommand(parent)
{
    this->image = image;
    this->oldImage = oldImage;
    newImage = image->copy(QRect());
}

/**
 * @Luis Pablo DrawCommand::undo - Restaura la imagen anterior almacenada en
 * oldImage que almacena el estado anterior del ultimo cambio hecho en el lienzo
 */
void DrawCommand::undo()
{
    *image = oldImage.copy(QRect());
}

/**
 * @Luis Pablo DrawCommand::redo - Almacena cada nuevo cambio en la variable neImage
 * por si se vuelve algun estado anterior, sea posible regresar al ultimo cambio
 * nuevamente
 */
void DrawCommand::redo()
{
    *image = newImage.copy(QRect());
}
