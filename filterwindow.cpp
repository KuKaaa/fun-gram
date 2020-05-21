#include "filterwindow.h"
#include "ui_filterwindow.h"

#include <QtWidgets>

#include <QDesktopServices>
#include <QMessageBox>

QImage filteredImg;
QLabel* label;

FilterWindow::FilterWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FilterWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("Fun-Gram(Filter)");

    QIcon icon("");
    this->setWindowIcon(icon);
    this->setMinimumSize(1280, 720);

}

FilterWindow::~FilterWindow()
{
    delete ui;
}

void FilterWindow::on_action_Open_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath());

    if(!fileName.isEmpty())
    {
        QImage img(fileName);
        ui->label->setPixmap(QPixmap::fromImage(img));
    }

}

void FilterWindow::on_action_Save_file_triggered()
{

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save as"), QDir::currentPath(), tr("JPG (*.jpg);;PNG (*.png);;All file types (*.*)"));

    if(!fileName.isEmpty())
    {
        filteredImg.save(fileName);
    }

}

void FilterWindow::on_action_Quit_triggered()
{

    QMessageBox closeMsg;
    closeMsg.setWindowTitle(tr("Exit"));
    //closeMsg.setIcon();
    closeMsg.setText(tr("Are you really want to exit?"));

    QAbstractButton* bttnY = closeMsg.addButton(tr("Yes"), QMessageBox::YesRole);
    closeMsg.addButton(tr("No"), QMessageBox::NoRole);

    closeMsg.exec();

    if (closeMsg.clickedButton() == bttnY)
    {
        QApplication::quit();
    }

}


void FilterWindow::on_action_Gauss_Blur_triggered()
{

}

void FilterWindow::on_action_Laplacian_Edge_Detection_triggered()
{

}

void FilterWindow::on_action_Gradient_Directional_Filter_triggered()
{

}

void FilterWindow::on_action_Outline_Filter_triggered()
{

}

void FilterWindow::on_action_Help_triggered()
{
    QMessageBox helpMsg;
    helpMsg.setWindowTitle("Help");
    helpMsg.setText(tr("If you want to filter image, firstly you have to load that image simply by pressing \"Open\" in menu bar then search for \"Filter\""
                       "and choose filter you want to use!"));

    QAbstractButton* bttn;
    bttn = helpMsg.addButton(tr("Got it!"), QMessageBox::YesRole);

    helpMsg.exec();
}

void FilterWindow::on_action_About_filters_triggered()
{
    QMessageBox redirectMsg;
    redirectMsg.setWindowTitle(tr("About filtering"));
    //closeMsg.setIcon();
    redirectMsg.setText(tr("You are about to be redirected to an external page!\nContinue?"));

    QAbstractButton* bttn;
    bttn = redirectMsg.addButton(tr("Yes"), QMessageBox::YesRole);
    redirectMsg.addButton(tr("No"), QMessageBox::NoRole);

    redirectMsg.exec();

    if (redirectMsg.clickedButton() == bttn)
    {
        QDesktopServices::openUrl(QUrl("https://en.wikipedia.org/wiki/Kernel_(image_processing)"));
    }
}
