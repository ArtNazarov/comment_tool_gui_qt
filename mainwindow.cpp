#include "mainwindow.h"

MainWindow::MainWindow(const QString &initialDir, QWidget *parent)
    : QMainWindow(parent), pathToDir(initialDir.isEmpty() ? QDir::currentPath() : initialDir)
{
    setupUI();
    runInitialCommand();
}

MainWindow::~MainWindow()
{
    if (process) {
        process->terminate();
        process->waitForFinished();
        delete process;
    }
}

void MainWindow::setupUI()
{
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // Path selection
    QHBoxLayout *pathLayout = new QHBoxLayout();
    QLabel *pathLabel = new QLabel("Путь к каталогу:", this);
    pathLineEdit = new QLineEdit(pathToDir, this);
    browseButton = new QPushButton("Выбрать каталог...", this);

    pathLayout->addWidget(pathLabel);
    pathLayout->addWidget(pathLineEdit);
    pathLayout->addWidget(browseButton);

    // Text area
    QLabel *textLabel = new QLabel("Считано командой comment_tool:", this);
    textEdit = new QTextEdit(this);
    textEdit->setReadOnly(false);

    // Save button
    saveButton = new QPushButton("Записать командой comment_tool", this);

    mainLayout->addLayout(pathLayout);
    mainLayout->addWidget(textLabel);
    mainLayout->addWidget(textEdit);
    mainLayout->addWidget(saveButton, 0, Qt::AlignRight);

    setCentralWidget(centralWidget);
    resize(600, 400);

    // Connections
    connect(browseButton, &QPushButton::clicked, this, [this]() {
        QString dir = QFileDialog::getExistingDirectory(this, "Select Directory", pathToDir);
        if (!dir.isEmpty()) {
            pathToDir = dir;
            pathLineEdit->setText(pathToDir);
        }
    });

    connect(saveButton, &QPushButton::clicked, this, &MainWindow::onSaveClicked);

    connect(pathLineEdit, &QLineEdit::textChanged, this, [this](const QString &text) {
        pathToDir = text;
        runInitialCommand();
    });

    connect(textEdit, &QTextEdit::textChanged, this, [this]() {
        commentsText = textEdit->toPlainText();
    });
}

void MainWindow::runInitialCommand()
{
    /*
    if (process) {
        process->terminate();
        process->waitForFinished();
        delete process;
    }
    */

    process = new QProcess(this);
    connect(process, &QProcess::readyReadStandardOutput, this, &MainWindow::handleCommandOutput);
    connect(process, &QProcess::errorOccurred, this, &MainWindow::handleCommandError);

    QStringList arguments;
    arguments << "--view-comment";
    arguments << pathToDir;

    process->start("/usr/bin/comment_tool", arguments);
}

void MainWindow::handleCommandOutput()
{
    QByteArray output = process->readAllStandardOutput();
    QStringList lines = QString::fromUtf8(output).split('\n');

    // Skip first two rows if there are at least two lines
    if (lines.size() > 2) {
        lines.removeFirst(); // Remove first line
        lines.removeFirst(); // Remove second line
    }

    QString filteredOutput = lines.join('\n');
    textEdit->setPlainText(filteredOutput);
    commentsText = filteredOutput;
}

void MainWindow::handleCommandError(QProcess::ProcessError error)
{
    Q_UNUSED(error);
    textEdit->setPlainText("Error running command: " + process->errorString());
}

void MainWindow::onSaveClicked()
{
    /*
    if (process) {
        process->terminate();
        process->waitForFinished();
    }
    */
    process = new QProcess(this);
    connect(process, &QProcess::errorOccurred, this, &MainWindow::handleCommandError);

    QStringList arguments;
    arguments << "--comment";
    arguments << pathToDir;
    arguments << "--from-command-line" << commentsText;

    process->start("/usr/bin/comment_tool", arguments);

    if (!process->waitForStarted()) {
        QMessageBox::critical(this, "Error", "Failed to start the command: " + process->errorString());
        return;
    }

    if (!process->waitForFinished()) {
        QMessageBox::critical(this, "Error", "Command failed: " + process->errorString());
        return;
    }

    if (process->exitCode() != 0) {
        QMessageBox::warning(this, "Warning", "Command completed with exit code " +
                                                  QString::number(process->exitCode()));
    } else {
        QMessageBox::information(this, "Success", "Command executed successfully");
    }
}
