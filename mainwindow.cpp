#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(ui->textEdit);
    this->isSaved = false;
    this->isUpdated = false;
    setWindowTitle("qtEdit 0.1");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionQuit_triggered()
{
    close();
}

void MainWindow::on_actionCut_triggered()
{
    ui->textEdit->cut();
}

void MainWindow::on_actionCopy_triggered()
{
    ui->textEdit->copy();
}

void MainWindow::on_actionPaste_triggered()
{
    ui->textEdit->paste();
}

void MainWindow::on_actionUndo_triggered()
{
    ui->textEdit->undo();
}

void MainWindow::on_actionRedo_triggered()
{
    ui->textEdit->redo();
}

void MainWindow::on_actionSave_triggered()
{
    if(!isSaved && isUpdated)
    {
        this->workingFileName = QFileDialog::getSaveFileName(this, tr("Save"),
                                                            "/home/trak/untitled.txt",
                                                            tr("Text Files (*.*)"));
        if(workingFileName.isEmpty()) return;
        else {
            workingFile->open(QIODevice::Text | QIODevice::Truncate | QIODevice::WriteOnly);
            workingFile = new QFile(workingFileName);
            QTextStream out(workingFile);
            out << ui->textEdit->toPlainText();
            workingFile->close();
            isSaved = true;
            isUpdated = false;
            setWindowTitle(workingFileName);
        }
    }
    else if(isSaved)
    {
        workingFile->open(QIODevice::Text | QIODevice::Truncate | QIODevice::WriteOnly);
        QTextStream out(workingFile);
        out << ui->textEdit->toPlainText();
        workingFile->close();
        isUpdated = false;
    }
}

void MainWindow::on_actionSave_As_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
                                                    "/home/trak/untitled.txt",
                                                    tr("Text Files (*.*)"));
    if(fileName.isEmpty()) return;
    else {
        QFile file(fileName);
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
        {
            qDebug() << "Could not save the file.";
            return;

        } else
        {
            QTextStream out(&file);
            out << ui->textEdit->toPlainText();
            file.flush();
            file.close();
        }
    }
}

void MainWindow::on_actionOpen_triggered()
{
    this->workingFileName = QFileDialog::getOpenFileName(this, tr("Open"),
                                                         "/home",
                                                         tr("Text Files (*.*)"));
    if(workingFileName.isEmpty()) return;
    else {
        workingFile = new QFile(workingFileName);
        if(!workingFile->open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qDebug() << "Could not open file.";
            return;
        } else
        {
            QTextStream in(workingFile);
            ui->textEdit->setText(in.readAll());
            workingFile->close();
            isSaved = true;
            setWindowTitle(workingFileName);
        }
    }
}

void MainWindow::on_actionNew_triggered()
{
    this->newWindow = new MainWindow;
    newWindow->show();
}
