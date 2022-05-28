#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QList>
#include <QAction>
#include <QWidget>
#include <QLabel>
#include "dialog_windows.h"
#include "draw_area.h"
#include "toolbar.h"


class Tool;
class QLabel;
class MainWindow: public QMainWindow
{
	Q_OBJECT

public:
    MainWindow(QWidget* parent = 0, const char* name = 0);
    ~MainWindow();
    QLabel * etiqueta = new QLabel( this);
    QLabel * estado = new QLabel (this);
    void virtual mousePressEvent (QMouseEvent*) override;
   // QString getSelectShape();

public slots:
    /** Metodos que se agregan al ToolBar */
    void OnNewImage();
	void OnLoadImage();
    void OnSaveImage();
    void OnResizeImage();
    void OnGetPixelColor();
    void OnPickColor(int);
    void OnChangeTool(int);
    void OnSelectRectangle();
    void OnSelectCircle();
    void OnSelectTriangle();
    /** tool dialogs */
    void OpenPencilDialog();
    void OpenPenDialog();
    void OpenEraserDialog();
    void OpenShapesDialog();
    void openToolDialog();


private:
    void createMenuActions();
    void createToolBarToggle();
    void createMenuAndToolBar();


    DrawArea* drawArea;

    QList<QAction*> toolActions;

    ToolBar* toolbar;

    Tool* currentTool;

    PencilDialog* pencilDialog;
    PenDialog* penDialog;
    EraserDialog* eraserDialog;
    ShapesDialog* shapesDialog;
    MainWindow(const MainWindow&);
    MainWindow& operator=(const MainWindow&);
};

#endif // MAIN_WINDOW_H
