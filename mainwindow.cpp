#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "scribblearea.h"

#include <QtWidgets>
#include <QMessageBox>

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
    connect(openAct, SIGNAL(triggered()), this, SLOT(openAct()));

    foreach(QByteArray format, QImageWriter::supportedImageFormats())
    {
        QString text = tr("%1...").arg(QString(format).toUpper());
        QAction *action = new QAction(text, this);
        action->setData(format);
        connect(action, SIGNAL(triggered()), scribbleArea, SLOT(printAct()));
        saveAsActs.append(action);
    }

    printAct = new QAction(tr("&Print..."), this);
    connect(printAct, SIGNAL(triggered()), scribbleArea, SLOT(printAct()));

    exitAct = new QAction(tr("&Exit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    connect(exitAct, SIGNAL(triggeres()), this, SLOT(exitAct()));
    
    penColorAct = new QAction(tr("&Pen Color..."), this);
    connect(penColorAct, SIGNAL(triggered()), this, SLOT(penColor()));

    penWidthAct = new QAction(tr("&Pen Width..."), this);
    penWidthAct->setShortcut(tr("Ctrl + Q"));
    connect(penWidthAct, SIGNAL(triggered()), this, SLOT(penWidth()));
    
    clsAct = new QAction(tr("&Cls"), this);
    clsAct->setShortcut(tr("Ctrl+C"));
    connect(clsAct, SIGNAL(triggered()), scribbleArea, SLOT(clearImg()));
    
    aboutAct = new QAction(tr("&About..."), this);
    connect(aboutAct, SIGNAL(triggered()), scribbleArea, SLOT(about()));

    gaussBlurAct = new QAction(tr("&Gauss Blur"), this);
    connect(aboutAct, SIGNAL(triggered()), scribbleArea, SLOT(gaussBlur()));
    
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


    optionMenu = new QMenu(tr("&Options"), this);
    optionMenu->addAction(penColorAct);
    optionMenu->addAction(penWidthAct);
    optionMenu->addSeparator();
    optionMenu->addAction(clsAct);
    
    helpMenu = new QMenu(tr("&Help"), this);
    helpMenu->addAction(aboutAct);
    
    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(optionMenu);
    menuBar()->addMenu(helpMenu);
}

bool MainWindow::maybeSave(){
    if(scribbleArea->isModified())
    {
        QMessageBox::StandardButton retVal;
        retVal = QMessageBox::warning(this, tr("Scribble"),
                                   tr("This image has been modified.\n"
                                    "Do you want to save your changes?"),
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
        initialPath,
        tr("%1 Files (*,%2);; All Files(*)")
        .arg(QString::fromLatin1(fileFormat.toUpper()))
        .arg(QString::fromLatin1(fileFormat)));

    if (fileName.isEmpty())
    {
        return false;
    }
    else
    {
        return scribbleArea->saveImg(fileName, fileFormat.constData());
    }
}

void MainWindow::gaussBlur(const QByteArray &file)
{

    //CODE
    import QtGraphicalEffects 1.14


}
