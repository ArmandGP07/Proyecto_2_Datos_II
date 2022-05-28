#include "main_window.h"
#include "draw_area.h"


/**
 * @brief ToolBar::ToolBar: Define los atributos de la clase ToolBar, a la cual se le agreagan los iconos de
 *                          las diferentes herramientas de Paint++.
 */
ToolBar::ToolBar(QWidget *parent,const QList<QAction*> &toolActions)
    : QToolBar(parent)
{
    this->toolActions = toolActions;
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setContextMenuPolicy(Qt::PreventContextMenu);
    setMovable(false);
    createActions();

}

/**
 * @brief ToolBar::createActions: Se le agregan los objetos de tipo QAction los cuales ya contioenen los metodos correspondientes
 *                                a las herramientas con sus respectivos iconos.
 */
void ToolBar::createActions()
{
    addSeparator();
    addActions(toolActions);
}

