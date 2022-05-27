#ifndef DIALOG_WINDOWS_H
#define DIALOG_WINDOWS_H

#include <QSpinBox>
#include <QGroupBox>
#include <QDialog>
#include <QSlider>
#include <QButtonGroup>

#include "constants.h"
#include "tool.h"


class DrawArea;

class CanvasSizeDialog : public QDialog
{
    Q_OBJECT

public:
    CanvasSizeDialog(QWidget* parent, const char* name = 0,
                     int width = DEFAULT_IMG_WIDTH,
                     int height = DEFAULT_IMG_HEIGHT);

    int getWidthValue() const { return widthSpinBox->value(); }
    int getHeightValue() const { return heightSpinBox->value(); }

private:
    QGroupBox* createSpinBoxes(int,int);

    QSpinBox *widthSpinBox;
    QSpinBox *heightSpinBox;
    QGroupBox *spinBoxesGroup;
};

class PencilDialog : public QDialog
{
    Q_OBJECT

public:
    PencilDialog(QWidget* parent, DrawArea* drawArea, int size = DEFAULT_PEN_THICKNESS);

private:

    DrawArea* drawArea;
    QSlider* pencilSizeSlider;
};

class PenDialog : public QDialog
{
    Q_OBJECT

public:
    PenDialog(QWidget* parent, DrawArea* drawArea,
                                LineStyle lineStyle = solid,
                                DrawType = single,
                                int thickness = DEFAULT_PEN_THICKNESS);

private:
    QGroupBox* createLineStyle(LineStyle);
    QGroupBox* createDrawType(DrawType);

    DrawArea* drawArea;
    QButtonGroup* penStyleG;
    QButtonGroup* drawTypeG;
    QSlider* lineThicknessSlider;
};

class EraserDialog : public QDialog
{
    Q_OBJECT

public:
    EraserDialog(QWidget* parent, DrawArea* drawArea,
                 int thickness = DEFAULT_ERASER_THICKNESS);

private:
    DrawArea* drawArea;
    QSlider* eraserThicknessSlider;
};

class ShapesDialog : public QDialog
{
    Q_OBJECT

public:
    ShapesDialog(QWidget* parent, DrawArea* drawArea,
                                LineStyle = solid,
                                FillColor = no_fill, int thickness = DEFAULT_PEN_THICKNESS);

private:
    QGroupBox* createBoundaryStyle(LineStyle);
    QGroupBox* createFillColor(FillColor);
    QGroupBox* createBoundaryType(BoundaryType);

    DrawArea* drawArea;
    QButtonGroup* boundaryStyleG;
    QButtonGroup* fillColorG;
    QButtonGroup* boundaryTypeG;
    QSlider* lineThicknessSlider;
};

#endif // DIALOGS_H
