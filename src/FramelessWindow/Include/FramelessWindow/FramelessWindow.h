#pragma once

#include <QtWidgets/QMainWindow>

namespace QWK {
    class WidgetWindowAgent;
}

class FramelessWindow : public QMainWindow {
    Q_OBJECT
public:
    enum Theme {
        Dark,
        Light,
    };
    Q_ENUM(Theme)

public:
    explicit FramelessWindow(Theme theme = Theme::Dark, QWidget *parent = nullptr);
    ~FramelessWindow() override;

public:
    void setMenuBar(QMenuBar *menubar);

Q_SIGNALS:
    void themeChanged();

protected:
    bool event(QEvent *event) override;

private:
    void installWindowAgent();
    void loadStyleSheet(Theme theme);

private:
    Theme theme_{};
    QWK::WidgetWindowAgent *agent_;
};
