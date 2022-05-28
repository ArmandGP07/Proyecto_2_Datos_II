#include <QPainter>

#include "tool.h"
#include "draw_area.h"


/**
 * @brief PencilTool::drawTo: Este es el metodo que se usa para dibujar con el objeto PencilTool el cual es la herramienta que se usa para ejecutar la función
 *                            lapiz donde va dibujar desd el primer pounto donde se hace clic un trazo continuo mientras se tenga presionado el mouse, hasta el ultimo
 *                            punto donde se deje de poresionar.
 */
void PencilTool::drawTo(const QPoint &endPoint, DrawArea *drawArea, QPixmap *image)
{
    QPainter painter(image);
    painter.setPen(static_cast<QPen>(*this));
    painter.drawLine(getStartPoint(), endPoint);

    int rad = (this->width() / 2) + 2;
    drawArea->update(QRect(getStartPoint(), endPoint).normalized()
                                .adjusted(-rad, -rad, +rad, +rad));
    setStartPoint(endPoint);
}

/**
 * @brief PenTool::drawTo: Este es el metodo que se usa para dibujar con el objeto PenTool el cual es la herramienta que se usa para ejecutar la función
 *                            lapicero donde va dibujar una linea recta desde el primer punto donde se haga clic hasta donde se mueva el mouse y se deje de presionar.
 */
void PenTool::drawTo(const QPoint &endPoint,  DrawArea *drawArea, QPixmap *image)
{
    QPainter painter(image);
    painter.setPen(static_cast<QPen>(*this));
    painter.drawLine(getStartPoint(), endPoint);
    drawArea->update();
}
/**
 * @brief ShapesTool::ShapesTool: Es el constructor de ShapesTool que es el objeto que se encarga de dibujar las Figuras.
 */
ShapesTool::ShapesTool(const QBrush &brush, qreal width, Qt::PenStyle s,
                   Qt::PenCapStyle c, Qt::PenJoinStyle j, QColor fill,
                   ShapeType shape, FillColor mode)
    : Tool(brush, width, s, c, j)
{
    fillColor = fill;
    fillMode = mode;
    shapeType = shape;
}

/**
 * @brief ShapesTool::drawTo:  Este es el metodo que se usa para dibujar con el objeto ShapesTool el cual es la herramienta que se usa para ejecutar la función
 *                            que dibuja las figuras donde toma como referencia una linea recta desde el primer punto donde se haga clic hasta donde se mueva
 *                            el mouse y se deje de presionar, dicha recta se va usar de forma diferente segun sea la figura que se vaya a dibujar pero en todas
 *                            se usa como referencia.
 */
void ShapesTool::drawTo(const QPoint &endPoint,  DrawArea *drawArea, QPixmap *image)
{
    QPainter painter(image);
    painter.setPen(static_cast<QPen>(*this));
    QPoint temp_point = endPoint;
    QRect rect = adjustPoints(endPoint);

    switch(shapeType)
    {   //La recta que se traza con los eventos del mouse se susa como la diagonal del rectangulo
        case rectangle:
        {
            if(fillColor != no_fill)
                painter.fillRect(rect, fillColor);
            painter.drawRect(rect);
        } break;
        //La recta que se traza con los eventos del mouse se susa de referencia de esta forma:
        // -Primer punto de la base del triangulo:se usa el primer punto de lña recta.
        // -Segundo punto de la base del triangulo: se usa el eje x del ultimo punto de la recta, con el eje y del primer punto de la recta.
        // -Tercer punto (altura del triangulo): Se usa como eje x el punto medio de la base y como eje y el mismo de ultimo punto de la recta.
        case triangle:
        {
            if(fillMode != no_fill){
                painter.setBrush(QBrush(fillColor));}
            if(rect.topLeft().rx()!=temp_point.rx()){
            polygon << QPoint(rect.topLeft()) << QPoint(rect.center().rx(),temp_point.ry()) << QPoint(rect.topRight());}
            else{
                polygon  << QPoint(temp_point) << QPoint(rect.center().rx(),rect.center().ry()-(rect.topRight().rx()-rect.center().rx()))<< QPoint(rect.topRight());
            }

            painter.drawPolygon(polygon);
            polygon.clear();

        } break;
        //La recta que se traza con los eventos del mouse se susa como el diametro del circulo.
        case ellipse:
        {
            if(fillMode != no_fill)
                painter.setBrush(QBrush(fillColor));
            painter.drawEllipse(rect);
        }
        break;
        default:
          break;
    }
    drawArea->update();
}

/**
 * @brief RectTool::adjustPoints: Este metodo instancia una recta entre el primer punto donde se recibio el evento de que se presiono el boton izquierdo del mouse
 *                                y el ultimo punto donde se recibe este evento.
 */
QRect ShapesTool::adjustPoints(const QPoint &endPoint)
{
    QRect rect;
    if(endPoint.x() < getStartPoint().x())
        rect = QRect(endPoint, getStartPoint());
    else
        rect = QRect(getStartPoint(), endPoint);
    return rect;
}
