#include "InputSection.hpp"
#include <QFileDialog>

InputSection::InputSection(QWidget* parent)
    : QWidget(parent) {
    m_header = std::make_unique<QLabel>(this);
    m_instructions = std::make_unique<QLabel>(this);
    m_path = std::make_unique<QLineEdit>(this);
    m_browse = std::make_unique<QPushButton>(this);
    m_layout = std::make_unique<QGridLayout>(this);
    createUI();
    connectUI();
}

InputSection::~InputSection() {
    m_header.reset();
    m_instructions.reset();
    m_path.reset();
    m_browse.reset();
}

void InputSection::setHeaderText(QString text) {
    m_header->setText(text);
}

void InputSection::setInstructionText(QString text) {
    m_instructions->setText(text);
}

void InputSection::createUI(void) {
    m_header->setTextFormat(Qt::MarkdownText);
    m_header->setWordWrap(true);
    m_instructions->setTextFormat(Qt::MarkdownText);
    m_instructions->setWordWrap(true);
    m_browse->setText("Browse");

    m_header->setSizeIncrement(1, 0);
    m_instructions->setSizeIncrement(1, 0);
    m_path->setSizeIncrement(1, 0);
    m_browse->setSizeIncrement(1, 0);
    m_layout->setColumnMinimumWidth(1, 450);

    // Add individual widgets to stacked widget 
    m_layout->addWidget(m_header.get(), 0, 0, 1, -1);
    m_layout->addWidget(m_instructions.get(), 1, 0, 1, -1);
    //m_pGridLayout->addWidget(m_path.get(), 0, 4, 1, 1);
    //m_pGridLayout->addWidget(m_pExitButton.get(), 0, 5, 1, 1);
    m_layout->addWidget(m_path.get(), 2, 0, 1, 5);
    m_layout->addWidget(m_browse.get(), 2, 5, 1, 1);
}

void InputSection::connectUI(void) {
    QObject::connect(m_browse.get(), &QAbstractButton::clicked, this, &InputSection::setPath);
}

void InputSection::setPath(void) {
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open FO3 Directory"));
    if (dir.isEmpty())
        return;
    m_path->setText(dir);
}

