#ifndef SCRIBBLEAREA_H
#define SCRIBBLEAREA_H

#include <QColor>
#include <QImage>
#include <QPoint>
#include <QWidget>

class ScribbleArea : public QWidget
{
    Q_OBJECT

public:
    ScribbleArea(QWidget *parent = nullptr);
    /* sets pencil color, e.g. red(#FF0000) */
    void setPenColor(const QColor &newColor);

    /* sets pencil width */
    void setPenWidth(int newWidth);

    /* open image file */
    bool openImg(const QString &fileName);

    /* save img file */
    bool saveImg(const QString &fileName, const char *fileFormat);

    /* checks if file is modified */
    bool isModified() const {return modified;}

    /* pencil color */
    QColor penColor() const {return myPenColor;}

    /* pencil width */
    int penWidth() const {return myPenWidth;}

private slots:
    void clearImg();
    void printImg();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void drawLineTo(const QPoint &endPoint);
    void resizeImg(QImage *image, const QSize &newSize);

    bool modified;
    bool scribbling;

    QColor myPenColor;
    QImage image;
    QPoint lastPoint;

    int myPenWidth;

};
#endif // SCRIBBLEAREA_H
