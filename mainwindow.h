#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFileDialog>
#include <QMessageBox>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(const QString &initialDir = QString(), QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onSaveClicked();
    void handleCommandOutput();
    void handleCommandError(QProcess::ProcessError error);

private:
    void setupUI();
    void runInitialCommand();

    QString pathToDir;
    QString commentsText;

    QLineEdit *pathLineEdit;
    QTextEdit *textEdit;
    QPushButton *saveButton;
    QPushButton *browseButton;
    QProcess *process;
};

#endif // MAINWINDOW_H
