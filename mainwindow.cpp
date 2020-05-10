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
    createActs();
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

void MainWindow::open()
{
    if(maybeSave())
    {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath());
        if(!fileName.isEmpty())
        {
            scribbleArea->openImg(fileName);
        }
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
    {
        scribbleArea->setPenColor(newColor);
    }
}       

void MainWindow::penWidth()
{
    bool ok;
    int newWidth = QInputDialog::getInt(this, tr("Scribble"), tr("Select pen width : "), scribbleArea->penWidth(), 1, 50, 1, &ok);
    if(ok)
    {
        scribbleArea->setPenWidth(newWidth);
    }
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Scribble"), tr("<p>The <b>Scribble</b> example is awesome</p>"));
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
        connect(action, SIGNAL(triggered()), scribbleArea, SLOT(print()));
        saveAsActs.append(action);
    }

    printAct = new QAction(tr("&Print..."), this);
    connect(printAct, SIGNAL(triggered()), scribbleArea, SLOT(print()));
    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    connect(exitAct, SIGNAL(triggeres()), this, SLOT(print()));
    
    penColorAct = new QAction(tr("&Pen Color..."), this);
    connect(penColorAct, SIGNAL(triggered()), scribbleArea, SLOT(penColor()));
    penWidthAct = new QAction(tr("Pen &Width..."), this);
    connect(penWidthAct, SIGNAL(triggered()), scribbleArea, SLOT(penWidth()));
    
    clsAct = new QAction(tr("&Clear Screen"), this);
    clsAct->setShortcut(tr("Ctrl+L"));
    connect(clsAct, SIGNAL(triggered()), scribbleArea, SLOT(clearImage()));
    
    aboutAct = new QAction(tr("&About..."), this);
    connect(aboutAct, SIGNAL(triggered()), scribbleArea, SLOT(about()));
    
    aboutQtAct = new QAction(tr("&About &Qt..."), this);
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(about()));    
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
    optionMenu = new QMenu(tr("&Options"), this);
    optionMenu->addAction(penColorAct);
    optionMenu->addAction(penWidthAct);
    fileMenu->addSeparator();
    optionMenu->addAction(clsAct);
    
    helpMenu = new QMenu(tr("&Help"), this);
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);
    
    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(optionMenu);
    menuBar()->addMenu(helpMenu);
}

bool MainWindow::maybeSave(){
    if(scribbleArea->isModified())
    {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("Scribble"),
                                   tr("This image has been modified.\n"
                                    "Do you want to save your changes?"),
                                   QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if(ret == QMessageBox::Save)
        {
            return saveFile("png");
        }
        else if (ret == QMessageBox::Cancel)
        {
            return false;
        }
    }
    return true;
}

bool MainWindow::saveFile(const QByteArray &fileFormat)
{
    QString initialPath = QDir::currentPath() + "/untilted." + fileFormat;
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


























