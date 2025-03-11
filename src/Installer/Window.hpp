#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <memory>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>

class MainWindow : public QMainWindow {
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void createUI(void);
    void connectUI(void);
    void setPathFO3(void);
    void setPathFNV(void);
    void setPathTTW(void);
    void setRawText(void);
    void exitWindow(void);

    // Window itself
    std::shared_ptr<QWidget>        m_window;
    std::shared_ptr<QVBoxLayout>    m_layout;
    
    // Text objects
    std::shared_ptr<QLabel>         m_infoHeader;
    std::shared_ptr<QLabel>         m_infoDetails;
    std::shared_ptr<QLabel>         m_infoFO3;
    std::shared_ptr<QLabel>         m_infoFNV;
    std::shared_ptr<QLabel>         m_infoTTW;
    
    // Input fields
    std::shared_ptr<QLineEdit>      m_pathFO3;
    std::shared_ptr<QPushButton>    m_browseFO3;

    std::shared_ptr<QLineEdit>      m_pathFNV;
    std::shared_ptr<QPushButton>    m_browseFNV;

    std::shared_ptr<QLineEdit>      m_pathTTW;
    std::shared_ptr<QPushButton>    m_browseTTW;

    // Buttons
    std::shared_ptr<QPushButton>    m_installButton;
    std::shared_ptr<QPushButton>    m_exitButton;
};

#endif
