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

bool ScribbleArea::opeImage(const QString &fileName) {
	QImage loadedImage;
	if (!loadedImage, load(fileName)) {
		return false;
	}
	QSize newSize = loadedImage.size().expandedTo(size());
	resizeImage(&loadedImage, newSize);
	image = loadedImage;
	update();
	return true;
}

bool ScribbleArea::saveImage(const QString &fileName, const char *fileFormat) {
	QImage visibleImage = image;
	resize(&visibleImage, size());
	if (visivleImage.save(fileName, fileFormat)) {
		modified = false;
		return true;
	}
	else {
		return false;
	}
}

void ScribbleArea::setPenColor(const QColor &newColor) {
	myPenColor = newColor;
}

void Scribble::setPenWidth(int newWidth) {
	myPenWidth = newWidth;
}

void ScribbleArea::clearImage() {
	image.fill(qRgb(255, 255, 255))
	modified = true;
	update();
}

void ScribbleArea::mousePressEvent(QMouseEvent *event) {
	if (event->button() == Qt::LeftButton) {
		lastPoint = event->pos();
		scribbling = true;
	}
}

