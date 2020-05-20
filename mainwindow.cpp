#include "ui_mainwindow.h"
#include "scribblearea.h"
#include "mainwindow.h"

#include <QtWidgets>
#include <QMessageBox>
#include <QMatrix3x3>

#include <QTextStream>

/* MainWindow Constructor */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    scribbleArea = new ScribbleArea;
    //QIcon icon(":/icons/appIcon.png");

    setCentralWidget(scribbleArea);
    createActs();
    createMenu();
    setWindowTitle("Fun-Gram");
    setMinimumSize(1280, 720);
    //setWindowIcon(icon);

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

void MainWindow::open()
{
    if(maybeSave())
    {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath());
        if(!fileName.isEmpty())
            scribbleArea->openImg(fileName);
    }
}

void MainWindow::save()
{
    QAction *action = qobject_cast<QAction *>(sender());
    QByteArray fileFormat = action->data().toByteArray();

    saveFile(fileFormat);
}

void MainWindow::penColor()
{
    QColor newColor = QColorDialog::getColor(scribbleArea->penColor());

    if(newColor.isValid())
        scribbleArea->setPenColor(newColor);
}       

void MainWindow::penWidth()
{
    bool ok;

    int newWidth = QInputDialog::getInt(this, tr("Pen Width"), tr("Please select your pen width : "), scribbleArea->penWidth(), 1, 50, 1, &ok);

    if(ok)
        scribbleArea->setPenWidth(newWidth);
}

void MainWindow::about()
{
    QMessageBox aboutBox;
    aboutBox.setWindowTitle("About Fun-Gram");
    aboutBox.setInformativeText("Fun-Gram is a free paint type application, that let's you draw and edit raster graphics");
    aboutBox.setStyleSheet("background: #FFFFFF; border: none; font-family: Cambria; font-style: bold; font-size: 16px; color: #000000;");

    QAbstractButton* bttn;
    bttn = aboutBox.addButton(tr("Great!"), QMessageBox::YesRole);
    bttn->setStyleSheet("background: #FFFFFF; font-family: Cambria; color: #000000");

    aboutBox.exec();
}

void MainWindow::createActs()
{
    openAct = new QAction(tr("&Open"), this);
    openAct->setShortcuts(QKeySequence::Open);
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    foreach(QByteArray format, QImageWriter::supportedImageFormats())
    {
        QString text = tr("%1...").arg(QString(format).toUpper());
        QAction *action = new QAction(text, this);
        action->setData(format);
        connect(action, SIGNAL(triggered()), this, SLOT(save()));
        saveAsActs.append(action);
    }

    printAct = new QAction(tr("&Print..."), this);
    connect(printAct, SIGNAL(triggered()), scribbleArea, SLOT(printImg()));

    exitAct = new QAction(tr("&Exit"), this);
    connect(exitAct, SIGNAL(triggered()), this, SLOT(exit()));
    
    penColorAct = new QAction(tr("&Pen Color..."), this);
    connect(penColorAct, SIGNAL(triggered()), this, SLOT(penColor()));

    penWidthAct = new QAction(tr("&Pen Width..."), this);
    penWidthAct->setShortcut(tr("Ctrl + Q"));
    connect(penWidthAct, SIGNAL(triggered()), this, SLOT(penWidth()));
    
    clsAct = new QAction(tr("&Cls"), this);
    clsAct->setShortcut(tr("Ctrl+C"));
    connect(clsAct, SIGNAL(triggered()), scribbleArea, SLOT(clearImg()));
    
    aboutAct = new QAction(tr("&About..."), this);
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    gaussBlurAct = new QAction(tr("&Gauss Blur"), this);
    connect(gaussBlurAct, SIGNAL(triggered()), this, SLOT(gaussBlur()));

    lapFilterAct = new QAction(tr("&Laplacian Edge Detection"), this);
    connect(lapFilterAct, SIGNAL(triggered()), this, SLOT(lapFilter()));

    gradientFilterAct = new QAction(tr("&Gradient Directional Filter"), this);
    connect(gradientFilterAct, SIGNAL(triggered()), this, SLOT(gradientFilter()));

    outlineFilterAct = new QAction(tr("&Outline Filter"), this);
    connect(outlineFilterAct, SIGNAL(triggered()), this, SLOT(outlineFilter()));
}

void MainWindow::createMenu()
{
    saveAsMenu = new QMenu(tr("&Save As"), this);

    foreach(QAction *action, saveAsActs)
    {
        saveAsMenu->addAction(action);
    }

    fileMenu = new QMenu(tr("&File"), this);
    fileMenu->addAction(openAct);
    fileMenu->addMenu(saveAsMenu);
    fileMenu->addAction(printAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    filterMenu = new QMenu(tr("&Filters"),  this);
    filterMenu->addAction(gaussBlurAct);
    filterMenu->addAction(lapFilterAct);
    filterMenu->addAction(gradientFilterAct);
    filterMenu->addAction(outlineFilterAct);

    optionMenu = new QMenu(tr("&Options"), this);
    optionMenu->addAction(penColorAct);
    optionMenu->addAction(penWidthAct);
    optionMenu->addSeparator();
    optionMenu->addAction(clsAct);
    

    helpMenu = new QMenu(tr("&About"), this);
    //helpMenu->addAction(helpAct);
    helpMenu->addSeparator();
    helpMenu->addAction(aboutAct);
    
    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(filterMenu);
    menuBar()->addMenu(optionMenu);
    menuBar()->addMenu(helpMenu);

}

bool MainWindow::maybeSave(){
    if(scribbleArea->isModified())
    {
        QMessageBox::StandardButton retVal;
        retVal = QMessageBox::warning(this, tr("Fun-Gram"),
                                   tr("This image has been modified.\nDo you want to save your changes?"),
                                   QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

        if(retVal == QMessageBox::Save)
            return saveFile("png");
        else if (retVal == QMessageBox::Cancel)
            return false;
    }

    return true;
}

bool MainWindow::saveFile(const QByteArray &fileFormat)
{
    QString initialPath = QDir::currentPath() + "/*." + fileFormat;

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
        initialPath, tr("%1 Files (*,%2);; All Files(*)").arg(QString::fromLatin1(fileFormat.toUpper())).arg(QString::fromLatin1(fileFormat)));

    if (fileName.isEmpty())
    {
        return false;
    }
    else
    {
        return scribbleArea->saveImg(fileName, fileFormat.constData());
    }
}

void MainWindow::exit()
{
    QMessageBox closeMsg;
    closeMsg.setWindowTitle(tr("Exit"));
    //closeMsg.setIcon();
    closeMsg.setInformativeText("Are you really want to exit?");

    QAbstractButton* bttnY = closeMsg.addButton(tr("Yes"), QMessageBox::YesRole);
    closeMsg.addButton(tr("No"), QMessageBox::NoRole);

    closeMsg.setStyleSheet("background: #FFFFFF; border: none; font-family: Cambria; font-style: bold; font-size: 16px; color: #000000;");
    bttnY->setStyleSheet("background: #FFFFFF; font-family: Cambria; color: #000000");

    closeMsg.exec();

    if (closeMsg.clickedButton() == bttnY)
    {
        QApplication::quit();
    }
}

void MainWindow::gaussBlur()
{
    //For testing only
    //QString fileName = QFileDialog::getOpenFileName(this,tr("Select files"),"",tr("JPG (*.jpg);;PNG (*.png);;All file types (*.*)"));

    QMessageBox aboutBox;
    aboutBox.setWindowTitle("About Fun-Gram");
    aboutBox.setInformativeText("Fun-Gram is a free paint type application, that let's you draw and edit raster graphics");
    aboutBox.setStyleSheet("background: #FFFFFF; border: none; font-family: Cambria; font-style: bold; font-size: 16px; color: #000000;");

    aboutBox.exec();


    QMatrix3x3 gaussKernel;
    gaussKernel(0, 0) = 1;  gaussKernel(0, 1) = 2;  gaussKernel(0, 2) = 1;
    gaussKernel(1, 0) = 2;  gaussKernel(1, 1) = 4;  gaussKernel(1, 2) = 2;
    gaussKernel(2, 0) = 1;  gaussKernel(2, 1) = 2;  gaussKernel(2, 2) = 1;

    float gauss = 16.0;


}

void MainWindow::lapFilter()
{
    //For testing only
    //QString fileName = QFileDialog::getOpenFileName(this,tr("Select files"),"",tr("JPG (*.jpg);;PNG (*.png);;All file types (*.*)"));

    QMessageBox aboutBox;
    aboutBox.setWindowTitle("About Fun-Gram");
    aboutBox.setInformativeText("Fun-Gram is a free paint type application, that let's you draw and edit raster graphics");
    aboutBox.setStyleSheet("background: #FFFFFF; border: none; font-family: Cambria; font-style: bold; font-size: 16px; color: #000000;");

    aboutBox.exec();

    QMatrix3x3 lapKernel;
    lapKernel(0, 0) = -1;    lapKernel(0, 1) = -1;    lapKernel(0, 2) = -1;
    lapKernel(1, 0) = -1;    lapKernel(1, 1) = 8;    lapKernel(1, 2) = -1;
    lapKernel(2, 0) = -1;    lapKernel(2, 1) = -1;    lapKernel(2, 2) = -1;

}


void MainWindow::gradientFilter()
{

    QMessageBox aboutBox;
    aboutBox.setWindowTitle("About Fun-Gram");
    aboutBox.setInformativeText("Fun-Gram is a free paint type application, that let's you draw and edit raster graphics");
    aboutBox.setStyleSheet("background: #FFFFFF; border: none; font-family: Cambria; font-style: bold; font-size: 16px; color: #000000;");

    aboutBox.exec();

    QMatrix3x3 gradientKernel;
    gradientKernel(0, 0) = -1;    gradientKernel(0, 1) = 1;    gradientKernel(0, 2) = 1;
    gradientKernel(1, 0) = -1;    gradientKernel(1, 1) = -2;    gradientKernel(1, 2) = 1;
    gradientKernel(2, 0) = -1;    gradientKernel(2, 1) = 1;    gradientKernel(2, 2) = 1;


}

void MainWindow::outlineFilter()
{

    QMessageBox aboutBox;
    aboutBox.setWindowTitle("About Fun-Gram");
    aboutBox.setInformativeText("Fun-Gram is a free paint type application, that let's you draw and edit raster graphics");
    aboutBox.setStyleSheet("background: #FFFFFF; border: none; font-family: Cambria; font-style: bold; font-size: 16px; color: #000000;");

    aboutBox.exec();

    QMatrix3x3 outlineKernel;
    outlineKernel(0, 0) = 1;    outlineKernel(0, 1) = 2;    outlineKernel(0, 2) = 1;
    outlineKernel(1, 0) = 0;    outlineKernel(1, 1) = 0;    outlineKernel(1, 2) = 0;
    outlineKernel(2, 0) = -1;    outlineKernel(2, 1) = -2;    outlineKernel(2, 2) = -1;


}
