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
}

FramelessWindow::~FramelessWindow()
{
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
