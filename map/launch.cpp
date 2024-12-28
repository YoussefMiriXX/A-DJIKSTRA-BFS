#include <iostream>
#include <ostream>

#include "mainwindow.h" // Main window header file.
#include <QApplication> // Qt application class.

using std::cout;
using std::endl;
using std::vector;
using std::string;

int main(int argc, char* argv[])
{
    QApplication app(argc, argv); // Initializes the Qt application.
    app.setAttribute(Qt::AA_DontCreateNativeWidgetSiblings); // Prevents native widget siblings.

    MainWindow mainWindow; // Creates the main window.
    mainWindow.show(); // Displays the main window.

    return app.exec(); // Enters the Qt event loop.
}
