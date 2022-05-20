#ifndef TOOL_H
#define TOOL_H

#include <QWidget>
#include <QPen>

#include "constants.h"


class DrawArea;


class Tool : public QPen
{
public:
    Tool(const QBrush &brush, qreal width, Qt::PenStyle s = Qt::SolidLine,
         Qt::PenCapStyle c = Qt::RoundCap,
         Qt::PenJoinStyle j = Qt::BevelJoin)
        : QPen(brush, width, s, c, j) {}
    virtual ~Tool() {}

    virtual ToolType getType() const = 0;
    virtual void drawTo(const QPoint&, DrawArea*, QPixmap*) {}

    QPoint getStartPoint() const { return startPoint; }
    void setStartPoint(QPoint point) { startPoint = point; }

private:
    QPoint startPoint;

    /** Don't allow copying */
    Tool(const Tool&);
    Tool& operator=(const Tool&);
};


/**
 * Herramientas de la funcion Lapiz
 */

class PencilTool : public Tool
{
public:
    PencilTool(const QBrush &brush, qreal width, Qt::PenStyle s = Qt::SolidLine,
            Qt::PenCapStyle c = Qt::RoundCap,
            Qt::PenJoinStyle j = Qt::BevelJoin)
       : Tool(brush, width, s, c, j) {}

    virtual ToolType getType() const { return pencil; }
    virtual void drawTo(const QPoint&, DrawArea*, QPixmap*);

private:
    /** Don't allow copying */
    PencilTool(const PencilTool&);
    PencilTool& operator=(const PencilTool&);
};



/**
 * Herramientas de la funcion Lapicero
 */

class PenTool : public Tool
{
public:
    PenTool(const QBrush &brush, qreal width, Qt::PenStyle s = Qt::SolidLine,
             Qt::PenCapStyle c = Qt::RoundCap,
             Qt::PenJoinStyle j = Qt::BevelJoin)
       : Tool(brush, width, s, c, j) {}
    virtual ToolType getType() const { return line; }
    virtual void drawTo(const QPoint&, DrawArea*, QPixmap*);

private:
    /** Don't allow copying */
    PenTool(const PenTool&);
    PenTool& operator=(const PenTool&);
};



/**
 * Herramientas de la funcion Borrador
 */

class EraserTool : public PencilTool
{
public:
    EraserTool(const QBrush &brush, qreal width,
               Qt::PenStyle s = Qt::SolidLine,
               Qt::PenCapStyle c = Qt::RoundCap,
               Qt::PenJoinStyle j = Qt::BevelJoin)
       : PencilTool(brush, width, s, c, j) {}

    virtual ToolType getType() const { return eraser; }

private:
    /** Don't allow copying */
    EraserTool(const EraserTool&);
    EraserTool& operator=(const EraserTool&);
};



/**
 * Herramientas de la funcion que dibuja Figuras
 */

class ShapesTool : public Tool
{
public:
    ShapesTool(const QBrush &brush, qreal width, Qt::PenStyle s = Qt::SolidLine,
             Qt::PenCapStyle c = Qt::RoundCap,
             Qt::PenJoinStyle j = Qt::BevelJoin,
             QColor fill = QColor(Qt::transparent),
             ShapeType shape = rectangle,
             FillColor mode = no_fill,
             int roundedCurve = DEFAULT_RECT_CURVE);

    virtual ToolType getType() const { return shapes_tool; }
    virtual void drawTo(const QPoint&, DrawArea*, QPixmap*);

    FillColor getFillMode() const { return fillMode; }
    void setFillMode(FillColor mode) { fillMode = mode; }
    void setShapeType(ShapeType shape) { shapeType = shape; }
    void setFillColor(QColor color) { fillColor = color; }
    void setCurve(int value) { roundedCurve = value; }
    QRect adjustPoints(const QPoint&);

    QPolygon polygon;
private:
    ShapeType shapeType;
    QColor fillColor;
    FillColor fillMode;
    int roundedCurve;

    /** Don't allow copying */
    ShapesTool(const ShapesTool&);
    ShapesTool& operator=(const ShapesTool&);
};

#endif // TOOL_H
