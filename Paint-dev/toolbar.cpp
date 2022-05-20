#include "main_window.h"
#include "draw_area.h"


/**
 * @brief ToolBar::ToolBar - Wrapper class for QToolBar.
 *                           construct a ToolBar with icons & actions.
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
 * @brief ToolBar::createActions - Populate the toolbar with actions
 */
void ToolBar::createActions()
{
    addSeparator();
    addActions(toolActions);
}

