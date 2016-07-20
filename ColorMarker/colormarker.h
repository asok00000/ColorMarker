#ifndef COLORMARKER_H
#define COLORMARKER_H

#include <QtWidgets/QMainWindow>
#include <QClipboard>
#include <QDebug>
#include <QDateTime>
#include <QImage>
#include <QMouseEvent>
#include <QPixmap>
#include <QPainter>
#include <QBrush>
#include <QFontMetrics>
#include <QPair>
#include <QFileDialog>
#include "ui_colormarker.h"
#include "utility.h"
class ColorMarker : public QMainWindow
{
	Q_OBJECT

public:
	ColorMarker(QWidget *parent = 0);
	~ColorMarker();
private slots:
	void on_btnPrintScreen_clicked();
	void on_btnSaveImg_clicked();
	void on_btnQuit_clicked();
	virtual bool eventFilter(QObject *, QEvent *);
private:
	Ui::ColorMarkerClass ui;
	QImage currentImage;
	QImage fetchCenterImage(QImage srcImg, QPoint pos);
	QList<QPair<QPoint,QColor>> markedColor;
};

#endif // COLORMARKER_H
