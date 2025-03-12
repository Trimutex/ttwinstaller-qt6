#include "Window.hpp"

MainWindow::MainWindow(QWidget* _parent)
    : QMainWindow(_parent) {
    m_window = std::make_shared<QWidget>(_parent);
    createUI();
    connectUI();
    m_window->show();
}

// Clean up
MainWindow::~MainWindow() {
    m_window.reset();
}

// Create objects for GUI
void MainWindow::createUI(void) {
}

// Connect buttons to implementations
void MainWindow::connectUI(void) {
}
