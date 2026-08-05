#include <QApplication>
#include <QDir>
#include "MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    
    QDir().mkpath("data");
    
    MainWindow window;
    window.show();
    
    return app.exec();
}