#include "mainwindow.h"
#include "ui_mainwindow.h"

/* MainWindow Constructor */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("Fun-Gram");
    //this->setWindowIcon();
    this->setMinimumSize(1280, 720);
}

/* MainWindow Destructor */
MainWindow::~MainWindow()
{
    delete ui;
}

