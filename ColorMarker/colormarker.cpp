#include "colormarker.h"

ColorMarker::ColorMarker(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	ui.lblDisplay->installEventFilter(this);
	setWindowFlags(Qt::FramelessWindowHint);
	showFullScreen();
}

ColorMarker::~ColorMarker()
{

}

void ColorMarker::on_btnPrintScreen_clicked()
{
	showMinimized();
	QApplication::clipboard()->clear();
	Utility::printScreen();
	QClipboard *clipBorad = QApplication::clipboard();
	QImage image = clipBorad->image();
	if (!image.isNull())
	{
		QString tmpFilePath = QString("./%1.jpg").arg(QDateTime::currentMSecsSinceEpoch());
		bool isSaved =  image.save(tmpFilePath);
		if (!isSaved)
		{
			qDebug() << "save tmp image file error.";
		}
		else
		{
			ui.lblDisplay->setStyleSheet(QString("background-color:lightgreen;image:url('%1')").arg(tmpFilePath));
			QFile::remove(tmpFilePath);
		}
	}
	QApplication::clipboard()->clear();
	markedColor.clear();
	showNormal();
	ui.lblMarkedPoints->setText(QString::number(markedColor.count()));
}

bool ColorMarker::eventFilter(QObject *sender, QEvent *event)
{
	if (sender == ui.lblDisplay)
	{
		switch (event->type())
		{
		case QEvent::Enter:
		{
			currentImage = ui.lblDisplay->grab().toImage();
		}
			break;
		case QEvent::MouseMove:
		{
			if (!currentImage.isNull())
			{
				QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
				QPoint pos = mouseEvent->pos();
				QColor color = currentImage.pixel(pos);
				ui.txtR->setText(QString::number(color.red()));
				ui.txtG->setText(QString::number(color.green()));
				ui.txtB->setText(QString::number(color.blue()));
				ui.txtHex->setText(color.name(QColor::HexRgb).toUpper());
				ui.lblColor->setText(color.name(QColor::HexRgb).toUpper());
				ui.lblColor->setStyleSheet(QString("background-color:%1;color:%2;").arg(color.name(QColor::HexRgb).toUpper()).arg(color.dark().name(QColor::HexRgb).toUpper()));
				QImage scaledImg = fetchCenterImage(currentImage, pos);
				ui.lblScaleDisplay->setPixmap(QPixmap::fromImage(scaledImg));
				ui.lblScaleDisplay->setParent(ui.lblDisplay);
				ui.lblScaleDisplay->move(pos+QPoint(5,5));
				ui.lblScaleDisplay->show();
			}
		}
			break;
		case QEvent::MouseButtonRelease:
		{
			QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
			QPoint pos = mouseEvent->pos();
			QColor color = currentImage.pixel(pos);
			QPair<QPoint,QColor> pair;
			pair.first = pos;
			pair.second = color;
			markedColor << pair;
			ui.lblMarkedPoints->setText(QString::number(markedColor.count()));
			QApplication::clipboard()->setText(color.name(QColor::HexRgb).toUpper());
		}
			break;
		default:
			break;
		}
	}
	return QMainWindow::eventFilter(sender, event);
}

QImage ColorMarker::fetchCenterImage(QImage srcImg, QPoint pos)
{
	QSize size = ui.lblScaleDisplay->size();
	QImage newImg = srcImg.copy(pos.x() - size.width() / 2, pos.y() - size.height() / 2,size.width(),size.height());
	newImg = newImg.scaled(size * 2);
	QSize newSize = newImg.size();
	newImg = newImg.copy(newSize.width() / 2 - size.width()/2, newSize.height() / 2 - size.height()/2, size.width(), size.height());
	QPainter painter(&newImg);
	QPen pen = QPen(QColor(117, 30, 30));
	pen.setWidth(2);
	painter.setPen(pen);
	painter.drawLine(0, newImg.height() / 2, newImg.width(), newImg.height() / 2);
	painter.drawLine(newImg.width()/2, 0, newImg.width()/2, newImg.height());
	return newImg;
}

void ColorMarker::on_btnSaveImg_clicked()
{
	ui.lblScaleDisplay->hide();
	QPainter painter(&currentImage);
	painter.setRenderHint(QPainter::Antialiasing);
	for (int i = 0; i < markedColor.count(); ++i)
	{
		QColor color = markedColor[i].second;
		QPoint point = markedColor[i].first;
		//color.setAlpha(150);
		QColor frontColor;
		if (color.red()+color.green()+color.blue() >= 450 )
		{
			//Ç³É«
			frontColor = color.darker(300);
		}
		else
		{
			//ÉîÉ«
			frontColor = color.lighter(300);
		}
		QPen pen(frontColor);
		QBrush brush(color);
		painter.setBrush(brush);
		painter.setPen(pen);
		QFontMetrics metrics(painter.font());
		QRect markRect;
		int mx, my;
		if (point.x()>ui.lblDisplay->width()/2)
		{
			mx = -50;
		}
		else
		{
			mx = 50;
		}
		if (point.y() > ui.lblDisplay->height() / 2)
		{
			my = -50;
		}
		else
		{
			my = 50;
		}
		markRect.setTopLeft(point + QPoint(mx, my));
		markRect.setSize(QSize(metrics.width(color.name(QColor::HexRgb).toUpper()) + 10, metrics.height()));
		painter.drawRect(markRect);
		painter.drawText(markRect.x()+5, markRect.y() + metrics.height()-2, color.name(QColor::HexRgb).toUpper());
		painter.drawLine(point, markRect.bottomLeft());
		painter.drawEllipse(point, 2, 2);
	}
	QString tmpFilePath = QString("./%1.jpg").arg(QDateTime::currentMSecsSinceEpoch());
	bool isSaved = currentImage.save(tmpFilePath);
	if (!isSaved)
	{
		qDebug() << "save tmp image file error.";
	}
	else
	{
		ui.lblDisplay->setStyleSheet(QString("background-color:lightgreen;image:url('%1')").arg(tmpFilePath));
		QFile::remove(tmpFilePath);
	}
	tmpFilePath = QFileDialog::getSaveFileName(this, tr("Open File"), "", tr("Image (*.jpg)"));
	if (!tmpFilePath.isEmpty())
	{
		currentImage.save(tmpFilePath);
	}
	ui.lblScaleDisplay->show();
}

void ColorMarker::on_btnQuit_clicked()
{
	qApp->quit();
}
