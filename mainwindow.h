#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>

//Widget for drawing
class ScribbleArea;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

protected:
    /* stops events from executing*/
    void closeEvent(QCloseEvent *event) override;

private slots:
    /* open image file */
    void open();

    /* save image file */
    void save();

    /* changes pencil color */
    void pencilColor();

    /* changes pencil width */
    void pencilWidth();

    void about();

private:
    /* creates actions */
    void createActions();

    /* creates menu system*/
    void createMenu();

    /* save if changes occur */
    bool maybeSave();

    /* save file */
    bool saveFile(const QByteArray &file);

    /* paint area where draw */
    ScribbleArea *scribbleArea;

    QMenu *saveAsMenu;
    QMenu *fileMenu;
    QMenu *optionMenu;
    QMenu *helpMenu;

    QAction *openAction;

    QList<QAction *> saveAsAction;

    QAction *pencilColorAction;
    QAction *pencilWidthAction;
    QAction *printAction;
    QAction *clsAction;
    QAction *aboutQtAction;

    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
