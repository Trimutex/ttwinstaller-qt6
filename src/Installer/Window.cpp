#include "Window.hpp"
#include <QSizePolicy>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent) {
    m_window = std::make_shared<QWidget>(parent);
    m_layout = std::make_shared<QGridLayout>(m_window.get());
    m_bottomGap = std::make_shared<QWidget>(m_window.get());

    m_header= std::make_shared<QLabel>(m_window.get());

    m_instructions = std::make_shared<QLabel>(m_window.get());

    m_fo3 = std::make_shared<InputSection>(m_window.get());
    m_fnv = std::make_shared<InputSection>(m_window.get());
    m_ttw = std::make_shared<InputSection>(m_window.get());

    m_installButton = std::make_shared<QPushButton>(m_window.get());
    m_exitButton = std::make_shared<QPushButton>(m_window.get());


    createUI();
    connectUI();
    m_window->show();
}

// Clean up
MainWindow::~MainWindow() {
    m_bottomGap.reset();
    m_header.reset();
    m_instructions.reset();

    m_fo3.reset();
    m_fnv.reset();
    m_ttw.reset();

    m_installButton.reset();
    m_exitButton.reset();

    m_layout.reset();
    m_window.reset();
}

// Create objects for GUI
void MainWindow::createUI(void) {
    m_header->setTextFormat(Qt::MarkdownText);
    m_header->setWordWrap(true);
    m_instructions->setTextFormat(Qt::MarkdownText);
    m_instructions->setWordWrap(true);
    setRawText();

    m_bottomGap->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

    m_layout->addWidget(m_header.get(), 0, 0, 2, 7);
    m_layout->addWidget(m_instructions.get(), 2, 0, 2, 7);
    m_layout->addWidget(m_fo3.get(), 4, 0, 1, -1);
    m_layout->addWidget(m_fnv.get(), 5, 0, 1, -1);
    m_layout->addWidget(m_ttw.get(), 6, 0, 1, -1);
    m_layout->addWidget(m_installButton.get(), 8, 4, 1, 2);
    m_layout->addWidget(m_exitButton.get(), 8, 6, 1, 2);
    m_layout->addWidget(m_bottomGap.get(), 9, 0, -1, -1);
    m_window->setMinimumSize(784, 561);
}

// Connect buttons to implementations
void MainWindow::connectUI(void) {
    QObject::connect(m_exitButton.get(), &QAbstractButton::clicked, this, &MainWindow::exitWindow);
}

// Raw text for the window
void MainWindow::setRawText(void) {
    setWindowTitle("Tale of Two Wastelands");

    // TODO: probably adjust version dynamically or something? idk
    m_header->setText("Tale of Two Wastelands v3.3.3b");

    m_instructions->setText(
            "You need legal copies of both Fallout 3 (Steam, GOG, EGS, GFWL)"
            " and Fallout: New Vegas (Steam, GOG, EGS, PCR), including all DLCs"
            " and preorder packs. You also need at least 17GB of free space in"
            " the destination folder.\n\n"
            "Patched executables are supported but a file verification is"
            " required if you modified the BSA archives with the BSA"
            " Decompressor.\n\n"
            "Don't forget to re-run the executable patch after installation if"
            " you needed to verify files.\n\n"
            "Please read the"
            " [official guide](https://thebestoftimes.moddinglinked.com/)"
            " for a complete set of instructions and more.");

    m_fo3->setHeaderText("Fallout 3");
    m_fo3->setInstructionText(
            "Path to the Fallout 3 folder (run the game once if the field"
            " isn't automatically filled).");

    m_fnv->setHeaderText("Fallout: New Vegas");
    m_fnv->setInstructionText(
            "Path to the Fallout: New Vegas folder (run the game once if the"
            " field isn't automatically filled).");

    m_ttw->setHeaderText("Tale of Two Wastelands");
    m_ttw->setInstructionText(
            "Path where TTW files will be installed. Set it to an empty mod in"
            " your manager. Support will not be provided for installations in"
            " the data directory.");

    m_installButton->setText("INSTALL");
    m_exitButton->setText("EXIT");
}

void MainWindow::exitWindow(void) {
    exit(0);
}
