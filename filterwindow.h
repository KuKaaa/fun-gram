#ifndef FILTERWINDOW_H
#define FILTERWINDOW_H

#include <QMainWindow>

namespace Ui {
class FilterWindow;
}

class FilterWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit FilterWindow(QWidget *parent = nullptr);
    ~FilterWindow();

private slots:

    void on_action_Open_triggered();

    void on_action_Save_file_triggered();

    void on_action_Quit_triggered();

    void on_action_Gauss_Blur_triggered();

    void on_action_Laplacian_Edge_Detection_triggered();

    void on_action_Gradient_Directional_Filter_triggered();

    void on_action_Outline_Filter_triggered();

    void on_action_Help_triggered();

    void on_action_About_filters_triggered();

private:
    Ui::FilterWindow *ui;
};

#endif // FILTERWINDOW_H
