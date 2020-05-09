#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "scribblearea.h"

#include <QtWidgets>

/* MainWindow Constructor */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    scribbleArea = new ScribbleArea;

    setCentralWidget(scribbleArea);
    createActions();
    createMenu();
    setWindowTitle("Fun-Gram");
    setMinimumSize(1280, 720);
    //setWindowIcon();

    //ui->setupUi(this);
    //this->setWindowTitle("Fun-Gram");
    //this->setWindowIcon();
    //this->setMinimumSize(1280, 720);
}

/* MainWindow Destructor */
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(maybeSave())
        event->accept();
    else
        event->ignore();
}


