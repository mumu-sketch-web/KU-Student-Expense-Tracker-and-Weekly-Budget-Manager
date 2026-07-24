#include <QApplication>
#include <QDir>
#include "MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    // Ensure the "data" directory exists so Mumuchhyu's CSV saving works smoothly
    QDir().mkpath("data");
    
    MainWindow window;
    window.show();
    
    return app.exec();
}