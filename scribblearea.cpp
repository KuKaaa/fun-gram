#include <QtWidgets>
#if defined(QT_PRINTSUPPORT_LIB)
#include <QtPrintSupport/qtprintsupportglobal.h>
#if QT_CONFIG(printdialog)
#include <QPrinter>
#include <QPrintDialog>
#endif
#endif

#include "scribblearea.h"

scribblearea::scribblearea(QWidget *parent) : QWidget(parent)
{
	setAttribute(Qt::WA_StaticContents);
	modified = false;
	scribbling = false;
	myPenWidth = 1;
	myPenColor = Qt::black;
}

bool ScribbleArea::opeImage(const QString &fileName)
{
	QImage loadedImage;
	if (!loadedImage, load(fileName))
	{
		return false;
	}
	QSize newSize = loadedImage.size().expandedTo(size());
	resizeImage(&loadedImage, newSize);
	image = loadedImage;
	update();
	return true;
}

bool ScribbleArea::saveImage(const QString &fileName, const char *fileFormat)
{
	QImage visibleImage = image;
	resize(&visibleImage, size());
	if (visivleImage.save(fileName, fileFormat)) 
	{
		modified = false;
		return true;
	}
	else {
		return false;
	}
}

void ScribbleArea::setPenColor(const QColor &newColor)
{
	myPenColor = newColor;
}

void Scribble::setPenWidth(int newWidth)
{
	myPenWidth = newWidth;
}

void ScribbleArea::clearImage() 
{
	image.fill(qRgb(255, 255, 255))
	modified = true;
	update();
}

void ScribbleArea::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		lastPoint = event->pos();
		scribbling = true;
	}
}

void ScribbleArea::mouseMoveEvent(QMouseEvent *event)
{
	if ((event->buttons() & Qt::LeftButton) && scribbling)
		drawLineTo(event->pos());
}

void ScribbleArea::mouseReleaseEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton && scribbling)
	{
		drawLineTo(event->pos());
		scribbling = false;
	}
}

void ScribbleArea::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	QRect dirtyRect = event->rect();
	painter.drawImage(dirtyRect, image, dirtyRect);
}

void ScribbleArea::resizeEvent(QResizeEvent *event)
{
	if (width() > image.width() || height() > image.height())
	{
		int newWidth = qMax(width() + 128, image.width());
		int newHeight = qMax(height() + 128, image.height());
		resizeImage(&image, QSize(newWidth, newHeight));
		update();
	}
	QWidget::resizeEvent(event);
}

void ScribbleArea::drawLineTo(const QPoint &endPoint)
{
	QPainter painter(&image);
	painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap, QtRoundJoin));
	painter.drawLine(lastPoint, endPoint);
	modified = true;
	int rad = (myPenWidth / 2) + 2;
	update(QRect(lastPoint, endPoint).normalized().adjusted(-rad, -rad, +rad, +rad));
	lastPoint = endPoint;
}
