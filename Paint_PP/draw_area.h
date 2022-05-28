#ifndef DRAW_AREA_H
#define DRAW_AREA_H

#include <QUndoStack>


#include "constants.h"
#include "tool.h"


class DrawArea : public QWidget
{
    Q_OBJECT

public:
    DrawArea(QWidget *parent);
    ~DrawArea();
    QPixmap* getImage() { return image; }
    Tool* getCurrentTool() const { return currentTool; }
    QColor getForegroundColor() { return foregroundColor; }
    QColor getBackgroundColor() { return backgroundColor; }
    QPoint getPOINT(){ return punto;}
    void setDropperState(bool);
    Tool* setCurrentTool(int);
    void setLineMode(const DrawType mode);

    void createNewImage(const QSize&);
    void loadImage(const QString&);
    void saveImage(const QString&);
    void resizeImage(const QSize&);
    void clearImage();
    void updateColorConfig(const QColor&, int);

    void saveDrawCommand(const QPixmap&);

public slots:
    void OnUndo();
    void OnRedo();
    void OnClearAll();

    void OnPencilSizeConfig(int);

    void OnEraserConfig(int);

    void OnPenLineStyleConfig(int);
    void OnPenDrawTypeConfig(int);
    void OnPenLineThicknessConfig(int);

    void OnShapesBStyleConfig(int);
    void OnSelectShapeTypeConfig(int);
    void OnShapesFillConfig(int);
    void OnShapesBTypeConfig(int);
    void OnShapesLineConfig(int);

protected:
    void virtual mousePressEvent(QMouseEvent *event) override;
    void virtual mouseMoveEvent(QMouseEvent *event) override;
    void virtual mouseReleaseEvent(QMouseEvent *event) override;
    void virtual mouseDoubleClickEvent(QMouseEvent *event) override;

    void virtual paintEvent(QPaintEvent *event) override;

private:
    void createTools();

    QUndoStack* undoStack;

    Tool* currentTool;
    DrawType currentLineMode;

    QPixmap* image;
    QPixmap oldImage;

    QColor foregroundColor;
    QColor backgroundColor;

    PencilTool* pencilTool;
    PenTool* penTool;
    EraserTool* eraserTool;
    ShapesTool* shapesTool;

    bool drawing;
    bool drawingPoly;
    bool dropperState;
    QPoint punto;
    DrawArea(const DrawArea&);
    DrawArea& operator=(const DrawArea&);
};

extern bool imagesEqual(const QPixmap& image1, const QPixmap& image2);

#endif // DRAW_AREA_H
