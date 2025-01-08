#pragma once

#include "FramelessTheme.h"
#include <QMainWindow>

class FramelessWindow : public QMainWindow
{
    Q_OBJECT;
    using Theme = FramelessTheme;

public:
    static void initialize();

public:
    explicit FramelessWindow(QWidget *parent = nullptr, Theme theme = Theme::Dark);
    virtual ~FramelessWindow() override;

public:
    void setMenuBar(QMenuBar *menubar);
    QMenuBar *menuBar() const;

protected:
    bool event(QEvent *event) override;

Q_SIGNALS:
    void themeChanged(Theme theme);

private:
    class FramelessHelper *helper_;
};
