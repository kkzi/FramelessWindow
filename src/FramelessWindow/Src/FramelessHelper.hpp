#pragma once

#include "widgets/widgetwindowagent.h"
#include "windowbar.h"
#include "windowbutton.h"
#include <FramelessWindow/FramelessTheme.h>
#include <QGuiApplication>
#include <QTimer>
#include <QWidget>

static inline void emulateLeaveEvent(QWidget *widget)
{
    Q_ASSERT(widget);
    if (!widget)
    {
        return;
    }
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
    const QScreen *screen = widget->screen();
#else
    const QScreen *screen = widget->windowHandle()->screen();
#endif
    const QPoint globalPos = QCursor::pos(screen);
    if (!QRect(widget->mapToGlobal(QPoint{0, 0}), widget->size()).contains(globalPos))
    {
        QCoreApplication::postEvent(widget, new QEvent(QEvent::Leave));
        if (widget->testAttribute(Qt::WA_Hover))
        {
            const QPoint localPos = widget->mapFromGlobal(globalPos);
            const QPoint scenePos = widget->window()->mapFromGlobal(globalPos);
            static constexpr const auto oldPos = QPoint{};
            const Qt::KeyboardModifiers modifiers = QGuiApplication::keyboardModifiers();
#if (QT_VERSION >= QT_VERSION_CHECK(6, 4, 0))
            const auto event = new QHoverEvent(QEvent::HoverLeave, scenePos, globalPos, oldPos, modifiers);
            Q_UNUSED(localPos);
#elif (QT_VERSION >= QT_VERSION_CHECK(6, 3, 0))
            const auto event = new QHoverEvent(QEvent::HoverLeave, localPos, globalPos, oldPos, modifiers);
            Q_UNUSED(scenePos);
#else
            const auto event = new QHoverEvent(QEvent::HoverLeave, localPos, oldPos, modifiers);
            Q_UNUSED(scenePos);
#endif
            QCoreApplication::postEvent(widget, event);
        }
    }
}

class FramelessHelper : public QObject
{
    Q_OBJECT;

  public:
    FramelessHelper(QWidget *target, FramelessTheme theme)
        : QObject(target), theme_(theme), target_(target), bar_(new QWK::WindowBar(target_)),
          agent_(new QWK::WidgetWindowAgent(target_))
    {
        target_->setAttribute(Qt::WA_DontCreateNativeAncestors);

        loadStyleSheet();
        installWindowAgent();

        bar_->setFixedHeight(32);
        bar_->setIconFollowWindow(true);
        bar_->setTitleFollowWindow(true);
    }

  public:
    QWidget *titleBar() const
    {
        return bar_;
    }

    void setMenuBar(QMenuBar *menuBar)
    {
        menuBar->setObjectName(QStringLiteral("win-menu-bar"));
        bar_->setMenuBar(menuBar);
        agent_->setHitTestVisible(menuBar, true);
    }

    QMenuBar *menuBar() const
    {
        return bar_->menuBar();
    }

  private:
    void installWindowAgent()
    {
        agent_->setup(target_);

        auto titleLabel = new QLabel(target_);
        titleLabel->setAlignment(Qt::AlignCenter);
        titleLabel->setObjectName(QStringLiteral("win-title-label"));

#ifndef Q_OS_MAC
        {
            auto iconButton = new QWK::WindowButton();
            iconButton->setObjectName(QStringLiteral("icon-button"));
            iconButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
            bar_->setIconButton(iconButton);
            agent_->setSystemButton(QWK::WindowAgentBase::WindowIcon, iconButton);
        }

        auto flags = target_->windowFlags();
        if (flags.testFlag(Qt::WindowMinimizeButtonHint))
        {
            auto minButton = new QWK::WindowButton();
            minButton->setObjectName(QStringLiteral("min-button"));
            minButton->setProperty("system-button", true);
            minButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
            bar_->setMinButton(minButton);
            agent_->setSystemButton(QWK::WindowAgentBase::Minimize, minButton);
        }

        if (flags.testFlag(Qt::WindowMaximizeButtonHint))
        {
            auto maxButton = new QWK::WindowButton();
            maxButton->setCheckable(true);
            maxButton->setObjectName(QStringLiteral("max-button"));
            maxButton->setProperty("system-button", true);
            maxButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
            bar_->setMaxButton(maxButton);
            agent_->setSystemButton(QWK::WindowAgentBase::Maximize, maxButton);
        }

        if (flags.testFlag(Qt::WindowCloseButtonHint))
        {
            auto closeButton = new QWK::WindowButton();
            closeButton->setObjectName(QStringLiteral("close-button"));
            closeButton->setProperty("system-button", true);
            closeButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
            bar_->setCloseButton(closeButton);
            agent_->setSystemButton(QWK::WindowAgentBase::Close, closeButton);
        }
#endif

        bar_->setTitleLabel(titleLabel);
        bar_->setHostWidget(target_);

        agent_->setTitleBar(bar_);

#ifdef Q_OS_MAC
        agent_->setSystemButtonAreaCallback([](const QSize &size) {
            static constexpr const int width = 75;
            return QRect(QPoint(size.width() - width, 0), QSize(width, size.height())); //
        });
#endif

#ifndef Q_OS_MAC
        QObject::connect(bar_, &QWK::WindowBar::minimizeRequested, target_, &QWidget::showMinimized);
        QObject::connect(bar_, &QWK::WindowBar::maximizeRequested, target_, [this](bool max) {
            if (max)
            {
                target_->showMaximized();
            }
            else
            {
                target_->showNormal();
            }

            // It's a Qt issue that if a QAbstractButton::clicked triggers a window's maximization,
            // the button remains to be hovered until the mouse move. As a result, we need to
            // manually send leave events to the button.

            QTimer::singleShot(0, [btn = bar_->maxButton()] { emulateLeaveEvent(btn); });
        });
        QObject::connect(bar_, &QWK::WindowBar::closeRequested, target_, &QWidget::close);
#endif
    }

    void loadStyleSheet()
    {
        Q_INIT_RESOURCE(windowbar);
        auto filepath = theme_ == FramelessTheme::Dark ? QStringLiteral(":/windowbar/dark-style.qss")
                                                       : QStringLiteral(":/windowbar/light-style.qss");
        if (QFile qss(filepath); qss.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            target_->setStyleSheet(qss.readAll());
            Q_EMIT themeChanged(theme_);
            qss.close();
        }
    }

  signals:
    void themeChanged(FramelessTheme);

  private:
    FramelessTheme theme_;
    QWidget *target_{nullptr};
    QWK::WindowBar *bar_;
    QWK::WidgetWindowAgent *agent_;
};
