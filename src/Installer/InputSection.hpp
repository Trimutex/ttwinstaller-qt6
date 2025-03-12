#pragma once

#include <memory>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>

class InputSection : public QWidget {
public:
    explicit InputSection(QWidget* parent = nullptr);
    ~InputSection();
    void setHeaderText(QString text);
    void setInstructionText(QString text);

private:
    void createUI(void);
    void connectUI(void);
    void setPath(void);
    
    // Objects
    std::unique_ptr<QLabel>         m_header;
    std::unique_ptr<QLabel>         m_instructions;
    std::unique_ptr<QLineEdit>      m_path;
    std::unique_ptr<QPushButton>    m_browse;
    
    // Layout
    std::shared_ptr<QGridLayout>    m_layout;
};
