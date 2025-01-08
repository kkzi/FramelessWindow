#pragma once

#include "FramelessTheme.h"
#include <QDialog>
#include <QDialogButtonBox>

class FramelessDialog : public QDialog
{
    Q_OBJECT;

    using Theme = FramelessTheme;

public:
    explicit FramelessDialog(QWidget *parent, Theme theme = Theme::Dark);
    explicit FramelessDialog(QDialogButtonBox::StandardButtons, QWidget *parget, Theme theme = Theme::Dark);
    virtual ~FramelessDialog() override;

public:
    void setCentralWidget(QWidget *widget);
    QWidget *centralWidget() const;

protected:
    virtual void onAccepted();
    virtual void onRejected();

Q_SIGNALS:
    void themeChanged(Theme theme);

private:
    class FramelessHelper *helper_{ nullptr };
    class QBoxLayout *content_;
    QWidget *central_{ nullptr };
};
