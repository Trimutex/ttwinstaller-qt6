#pragma once

#include "InputSection.hpp"
#include <memory>
#include <QGridLayout>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>

class MainWindow : public QMainWindow {
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private:
    void createUI(void);
    void connectUI(void);
    void setRawText(void);
    void exitWindow(void);

    // Window itself
    std::shared_ptr<QWidget>        m_window;

    // Layout
    std::shared_ptr<QGridLayout>    m_layout;
    std::shared_ptr<QWidget>        m_bottomGap;
    
    // Text
    std::shared_ptr<QLabel>         m_header;
    std::shared_ptr<QLabel>         m_instructions;

    // Section
    std::shared_ptr<InputSection>   m_fo3;
    std::shared_ptr<InputSection>   m_fnv;
    std::shared_ptr<InputSection>   m_ttw;

    // Buttons
    std::shared_ptr<QPushButton>    m_installButton;
    std::shared_ptr<QPushButton>    m_exitButton;
};
