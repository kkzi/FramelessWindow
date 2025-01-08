#include <FramelessWindow/FramelessDialog.h>
#include <FramelessWindow/FramelessWindow.h>
#include <QApplication>
#include <QLabel>
#include <QMenuBar>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    qputenv("QT_WIN_DEBUG_CONSOLE", "attach");
    qputenv("QSG_INFO", "1");
    // qputenv("QT_WIDGETS_HIGHDPI_DOWNSCALE", "1");
    // qputenv("QT_WIDGETS_RHI", "1");
    // qputenv("QSG_RHI_BACKEND", "d3d12");
    // qputenv("QSG_RHI_HDR", "scrgb");
    // qputenv("QT_QPA_DISABLE_REDIRECTION_SURFACE", "1");

#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
#endif

    QCoreApplication::setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);
    QApplication a(argc, argv);

    FramelessWindow w;
    w.setWindowIcon(QIcon(":/app/example.png"));
    w.resize(900, 480);
    auto bar = new QMenuBar(&w);
    auto filemenu = bar->addMenu("File");
    filemenu->addAction("&Open");
    auto helpmenu = bar->addMenu("Help");
    helpmenu->addAction("About Qt", [&w] { QMessageBox::aboutQt(&w); });
    helpmenu->addAction("About", [&w] {
        auto label = new QLabel("Hello world");
        label->setAlignment(Qt::AlignCenter);
        FramelessDialog dialog(&w);
        dialog.setWindowTitle("Help");
        dialog.setFixedSize(400, 240);
        dialog.setCentralWidget(label);
        dialog.exec();
    });
    w.setMenuBar(bar);
    w.show();

    return a.exec();
}
