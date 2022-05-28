#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QToolBar>
#include <QAction>
#include <QList>
#include <QLabel>
#include "constants.h"


class QWidget;

class ToolBar : public QToolBar
{
public:
    ToolBar(QWidget* parent,
                             const QList<QAction*> &toolActions);
    QLabel * funcion;

private:
    void createActions();

    QList<QAction*> toolActions;
    ToolBar(const ToolBar&);
    ToolBar& operator=(const ToolBar&);
};

#endif // TOOLBAR_H
