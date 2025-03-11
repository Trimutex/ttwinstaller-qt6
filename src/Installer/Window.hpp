#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <memory>
#include <QMainWindow>

class MainWindow : public QMainWindow {
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void createUI(void);
    void connectUI(void);

    // Window itself
    std::shared_ptr<QWidget>        m_window;
};

#endif
