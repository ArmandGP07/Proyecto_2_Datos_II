#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QPainter;
class QImage;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:

    void paintEvent(QPaintEvent * e) override;
    void mousePressEvent(QMouseEvent * e) override;
    void mouseMoveEvent(QMouseEvent * e) override;
    void mouseReleaseEvent(QMouseEvent * e) override;
private slots:
    void on_actionTamano_triggered();

    void on_actionColor_triggered();

    void on_actionQuitar_triggered();

private:
    Ui::MainWindow *ui;
    QPainter * mPainter; //pinta
    QImage * mImage;     //imagen
    QPoint mBegin;       //inicio
    QPoint mEnd;         //final
    QColor mColor;       //color
    bool mEnabled;       //activado
    int mSize;           //tamaño
    int puntos;
};
#endif // MAINWINDOW_H
