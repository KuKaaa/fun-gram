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

void MainWindow::open(){
    if(maybeSave()){
        Qstring fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath());
        if(!fileName.isEmpty()){
            scribbleArea->openImage(fileName);
        }
    }
}

void Mainwindow::save(){
    QAction *action = qobject_cast<QAction *>(sender());
    QByte fileFormat = action->data().toByteArray();
    saveFile(fileFormat);
}

void MainWindow::penColor(){
    QColor newColor = QColorDialog::getColor(scribbleArea->penColor());
    if(newColor.isValid()){
        scribbleArea->setPenColor(newColor);
    }
}       

void MainWindow::penWidth(){
    bool ok;
    int newWidth = QInputDialog::getInt(this, tr("Scribble"), tr("Select pen width : "), scribbleArea->penWidth(), 1, 50, 1, &ok);
    if(ok){
        scribbleArea->setPenWidth(newWidth);
    }
}

void MainWindow::about(){
    QMessageBox::about(this, tr("About Scribble"), tr("<p>The <b>Scribble</b> example is awesome</p>"));
}

void MainWindow::createActions(){
    openAct = new QAction(tr("&Open"), this);
    openAct->setShortcuts(QKeySequence::Open);
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));
    foreach(QByteArray format, QImageWriter::supportedImageFormats()){
        QString text = tr("%1...").arg(QString(format).toUpper());
        QAction *action = new QAction(text, this);
        action->setData(format);
        connect(action, SIGNAL(triggered()), scribbleArea, SLOT(print()));
        saveAsActs.append(action);
    }
    printAct = new QAction(tr("&Print..."), this);
    connect(printAct, SIGNAL(triggered()), scribleArea, SLOT(print()));
    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    connect(exitAct, SIGNAL(triggeres()), this, SLOT(print()));
    
    penColorAct = new QAction(tr("&Pen Color..."), this);
    connect(penColorAct, SIGNAL(triggered()), scribbleArea, SLOT(penColor()));
    penWidthAct = new QAction(tr("Pen &Width..."), this);
    connect(penWidthAct, SIGNAL(triggered()), scribbleArea, SLOT(penWidth()));
    
    clearScreenAct = new QAction(tr("&Clear Screen"), this);
    clearScreenAct->setShortcut(tr("Ctrl+L"));
    connect(clearScreenAct, SIGNAL(triggered()), scribbleArea, SLOT(clearImage()));
    
    aboutAct = new QAction(tr("&About..."), this);
    connect(aboutAct, SIGNAL(triggered()), scribbleArea, SLOT(about()));
    
    aboutQtAct = new QAction(tr("&About &Qt..."), this);
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(about()));    
}

void Main Window::createMenus(){
    saveMenu = new QMenu(tr("&Save As"), this);
    foreach(QAction *action, saveAsActs)
        saveAsMenu->addAction(action);
    fileMenu = new QMenu(tr("&File"), this);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAsMenu);
    fileMenu->addAction(printAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);
    optionMenu = new QMenu(tr("&Options"), this);
    optionMenu->addAction(penColorAct);
    optionMenu->addAction(penWidthAct);
    fileMenu->addSeparator();
    optionMenu->addAction(clearScreenAct);
    
    helpMenu = new QMenu(tr("&Help"), this);
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);
    
    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(optionMenu);
    menuBar()->addMenu(helpMenu);
}

bool MainWindow::maybeSave(){
    if(scribbleArea->isModified()){
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("Scribble"),
                                   tr("This image has been modified.\n"
                                    "Do you want to save your changes?"),
                                   QMessegeBox::Save | QMessageBox::Discard | QmessageBox::Cancel);
        if(ret == QMessage::Save){
            return saveFile("png");
		}
		else if (ret == QMessageBox::Cancel) {
			return false;
		}
    }
	return true;
}

bool MainWindow::saveFile(const QByteArray &fileFormat) {
	QString initialPath = QDir::currentPath() + "/untilted." + fileFormat;
	QString fileName = QFilfeDialog::getSaveFileName(this, tr("Save As"),
		initialPath,
		tr("%1 Files (*,%2);; All Files(*)")
		.arg(QString::fromLatin1(fileFormat.toUpper()))
		.arg(QString::fromLatin1(fileFormat)));
	if (fileName.isEmpty()) {
		return false;
	}
	else {
		return scribbleArea->saveImage(fileName, fileFormat.constData());
	}
}
