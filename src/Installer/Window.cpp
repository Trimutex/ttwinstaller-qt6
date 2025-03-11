#include "Window.hpp"
#include <QFileDialog>

MainWindow::MainWindow(QWidget* _parent)
    : QMainWindow(_parent) {
    m_window = std::make_shared<QWidget>(_parent);
    m_layout = std::make_shared<QVBoxLayout>(m_window.get());

    m_infoHeader = std::make_shared<QLabel>(m_window.get());
    m_infoDetails = std::make_shared<QLabel>(m_window.get());
    m_infoFO3 = std::make_shared<QLabel>(m_window.get());
    m_infoFNV = std::make_shared<QLabel>(m_window.get());
    m_infoTTW = std::make_shared<QLabel>(m_window.get());

    // TODO: Each of these three pieces should be a standalone widget instead
    //      e.g. m_fo3 m_fnv m_ttw
    m_pathFO3 = std::make_shared<QLineEdit>(m_window.get());
    m_browseFO3 = std::make_shared<QPushButton>(m_window.get());
    m_pathFNV = std::make_shared<QLineEdit>(m_window.get());
    m_browseFNV = std::make_shared<QPushButton>(m_window.get());
    m_pathTTW = std::make_shared<QLineEdit>(m_window.get());
    m_browseTTW = std::make_shared<QPushButton>(m_window.get());

    m_installButton = std::make_shared<QPushButton>(m_window.get());
    m_exitButton = std::make_shared<QPushButton>(m_window.get());

    createUI();
    connectUI();
    m_window->show();
}

// Clean up
MainWindow::~MainWindow() {
    m_window.reset();
    m_infoHeader.reset();
    m_infoDetails.reset();
    m_infoFO3.reset();
    m_infoFNV.reset();
    m_infoTTW.reset();
    m_pathFO3.reset();
    m_browseFO3.reset();
    m_pathFNV.reset();
    m_browseFNV.reset();
    m_pathTTW.reset();
    m_browseTTW.reset();
    m_installButton.reset();
    m_exitButton.reset();
}

// Create objects for GUI
void MainWindow::createUI(void) {
    setWindowTitle("Tale of Two Wastelands");
    setRawText();
    m_layout->addWidget(m_infoHeader.get());
    m_layout->addWidget(m_infoDetails.get());
    m_layout->addWidget(m_infoFO3.get());
    m_layout->addWidget(m_infoFNV.get());
    m_layout->addWidget(m_infoTTW.get());
    m_layout->addWidget(m_pathFO3.get());
    m_layout->addWidget(m_browseFO3.get());
    m_layout->addWidget(m_pathFNV.get());
    m_layout->addWidget(m_browseFNV.get());
    m_layout->addWidget(m_pathTTW.get());
    m_layout->addWidget(m_browseTTW.get());
    m_layout->addWidget(m_installButton.get());
    m_layout->addWidget(m_exitButton.get());
    m_window->setMinimumSize(784, 561);
}

// Connect buttons to implementations
void MainWindow::connectUI(void) {
    QObject::connect(m_browseFO3.get(), &QAbstractButton::clicked, this, &MainWindow::setPathFO3);
    QObject::connect(m_browseFNV.get(), &QAbstractButton::clicked, this, &MainWindow::setPathFNV);
    QObject::connect(m_browseTTW.get(), &QAbstractButton::clicked, this, &MainWindow::setPathTTW);
    QObject::connect(m_exitButton.get(), &QAbstractButton::clicked, this, &MainWindow::exitWindow);
}

void MainWindow::setPathFO3(void) {
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open FO3 Directory"));
    if (dir.isEmpty())
        return;
    m_pathFO3->setText(dir);
}

void MainWindow::setPathFNV(void) {
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open FNV Directory"));
    if (dir.isEmpty())
        return;
    m_pathFNV->setText(dir);
}

void MainWindow::setPathTTW(void) {
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open TTW Directory"));
    if (dir.isEmpty())
        return;
    m_pathTTW->setText(dir);
}

// Raw text for the window
void MainWindow::setRawText(void) {
    m_infoHeader->setTextFormat(Qt::MarkdownText);
    m_infoHeader->setWordWrap(true);
    m_infoHeader->setText(
            "Tale of Two Wastelands v3.3.3b");

    m_infoDetails->setTextFormat(Qt::MarkdownText);
    m_infoDetails->setWordWrap(true);
    m_infoDetails->setText(
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

    m_infoFO3->setTextFormat(Qt::MarkdownText);
    m_infoFO3->setWordWrap(true);
    m_infoFO3->setText(
            "Fallout 3\n"
            "Path to the Fallout 3 folder (run the game once if the field"
            " isn't automatically filled).");

    m_infoFNV->setTextFormat(Qt::MarkdownText);
    m_infoFNV->setWordWrap(true);
    m_infoFNV->setText(
            "Path to the Fallout: New Vegas folder (run the game once if the"
            " field isn't automatically filled).");

    m_infoTTW->setTextFormat(Qt::MarkdownText);
    m_infoTTW->setWordWrap(true);
    m_infoTTW->setText(
            "Path where TTW files will be installed. Set it to an empty mod in"
            " your manager. Support will not be provided for installations in"
            " the data directory.");

    m_installButton->setText("INSTALL");
    m_exitButton->setText("EXIT");

    m_browseFO3->setText("Browse");
    m_browseFNV->setText("Browse");
    m_browseTTW->setText("Browse");
}

void MainWindow::exitWindow(void) {
    exit(0);
}
