#include <QApplication>
#include <QPalette>
#include <QStyleFactory>

#include "UI/MainWindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    app.setStyle(QStyleFactory::create("Fusion"));

    QPalette lightPalette;
    lightPalette.setColor(QPalette::Window, QColor(240, 240, 240));
    lightPalette.setColor(QPalette::WindowText, Qt::black);
    lightPalette.setColor(QPalette::Base, Qt::white);
    lightPalette.setColor(QPalette::AlternateBase, QColor(240, 240, 240));
    lightPalette.setColor(QPalette::ToolTipBase, Qt::white);
    lightPalette.setColor(QPalette::ToolTipText, Qt::black);
    lightPalette.setColor(QPalette::Text, Qt::black);
    lightPalette.setColor(QPalette::Button, QColor(240, 240, 240));
    lightPalette.setColor(QPalette::ButtonText, Qt::black);
    lightPalette.setColor(QPalette::BrightText, Qt::red);

    app.setPalette(lightPalette);

    MainWindow window;
    window.show();

    return app.exec();
}