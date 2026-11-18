#include "FramelessWindow/FramelessWindow.h"
#include "FramelessHelper.hpp"
#include "widgets/widgetwindowagent.h"
#include "windowbar.h"
#include "windowbutton.h"

#include <QApplication>
#include <QtWidgets/QStyle>

void FramelessWindow::initialize()
{
    QApplication::setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);
}

FramelessWindow::FramelessWindow(QWidget *parent, Theme theme)
    : QMainWindow(parent)
    , helper_(new FramelessHelper(this, theme))
{
    setMenuWidget(helper_->titleBar());
    connect(helper_, &FramelessHelper::themeChanged, this, &FramelessWindow::themeChanged);

    // Keep a sane minimum size. The window bar layout (window icon + menu bar +
    // caption + window buttons) needs ~280 logical px to show even a small menu
    // bar; below that the menu bar collapses into its overflow ("...") extension
    // button and the window buttons start overlapping each other. See issue #1.
    setMinimumSize(400, 160);
}

FramelessWindow::~FramelessWindow()
{
}

QSize FramelessWindow::sizeHint() const
{
    // An empty QMainWindow has an almost empty layout, so its default size hint
    // is tiny (200x100 without a central widget). A window shown with no explicit
    // size then squeezes the window bar and the menu bar collapses into its
    // overflow button. Fall back to a regular main window size until the user
    // resizes or sets a central widget that wants more space.
    return QMainWindow::sizeHint().expandedTo(QSize(960, 600));
}

void FramelessWindow::setMenuBar(QMenuBar *menubar)
{
    q_check_ptr(menubar);
    helper_->setMenuBar(menubar);
}

QMenuBar *FramelessWindow::menuBar() const
{
    return helper_->menuBar();
}

bool FramelessWindow::event(QEvent *event)
{
    switch (event->type())
    {
    case QEvent::WindowActivate: {
        auto menu = menuWidget();
        if (menu)
        {
            menu->setProperty("bar-active", true);
            style()->polish(menu);
        }
        break;
    }

    case QEvent::WindowDeactivate: {
        auto menu = menuWidget();
        if (menu)
        {
            menu->setProperty("bar-active", false);
            style()->polish(menu);
        }
        break;
    }

    default:
        break;
    }
    return QMainWindow::event(event);
}
