#include "NotifyWidget.h"

#include <QtCore/QPropertyAnimation>
#include <QtCore/QTimer>
#include <QtCore/QDebug>
#include <QtGui/QDesktopWidget>
#include <QtGui/QApplication>
#include <QtGui/QHBoxLayout>
#include <QtGui/QGridLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QLabel>

namespace Notify
{

Label::Label(const QPixmap &icon, const QString &text, int timeout)
{
	m_icon = new QLabel(this);
	m_icon->setPixmap(icon);
	m_icon->setMaximumWidth(16);
	m_icon->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
	if(icon.width() > 16) {
		m_icon->setScaledContents(true);
	}

	m_text = new QLabel(text, this);
	connect(m_text, SIGNAL(linkActivated(QString)), SIGNAL(linkClicked(QString)));
	m_timeout = timeout;

	QHBoxLayout *lay = new QHBoxLayout(this);
	lay->setMargin(0);
	lay->addWidget(m_icon);
	lay->addWidget(m_text);

	if(timeout > 0) {
		QTimer::singleShot(timeout, this, SIGNAL(timeout()));
	}
}

Widget::Widget(QWidget *parent):
	QWidget(parent, Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint),
	m_mouseInside(false),
	m_animation(false),
	m_position(BottomRight),
	m_limit(20)
{
	m_centralWidget = new QFrame(this);
	m_centralWidget->setObjectName("centralWidget");

	QGridLayout *lay = new QGridLayout(this);
	lay->setMargin(0);
	lay->setSpacing(0);
	lay->addWidget(m_centralWidget);

	m_layout = new QVBoxLayout(m_centralWidget);
	m_layout->setMargin(0);
	m_layout->setSpacing(0);

	setAttribute(Qt::WA_TranslucentBackground);
}

void Widget::addNotify(const QPixmap &icon, const QString &text, int timeout)
{
	Label *label = new Label(icon, "<style type=\"text/css\">" + styleSheet() + "</style>" + text, timeout);
	connect(label, SIGNAL(timeout()), SLOT(maybeRemove()));
	connect(label, SIGNAL(linkClicked(QString)), SLOT(removeClicked(QString)));

	if((m_limit > 0) && (m_layout->count() >= m_limit)) {
		delete m_layout->takeAt(0)->widget();
	}

	m_layout->addWidget(label);

	if(!isVisible()) {
		move(-1000, -1000);
		show();
	}

	adjustSize();
}

void Widget::resizeEvent(QResizeEvent *event)
{
	if(m_layout->count()) {
		QRect desktop = qApp->desktop()->availableGeometry();

		bool animate = (x() < 0) && m_animation;
		if(!animate) {
			switch(m_position) {
				case TopLeft:
					move(desktop.left(), desktop.top());
				break;

				case TopRight:
					move(desktop.right() - width(), desktop.top());
				break;

				case BottomLeft:
					move(desktop.left(), desktop.bottom() - height());
				break;

				case BottomRight:
					move(desktop.right() - width(), desktop.bottom() - height());
				break;
			}
		} else {
			QPoint target;
			QPropertyAnimation *animation = new QPropertyAnimation(this, "pos");

			switch(m_position) {
				case TopLeft:
					move(desktop.left(), desktop.top() - height());
					target = QPoint(desktop.left(), desktop.top());
				break;

				case TopRight:
					move(desktop.right() - width(), desktop.top() - height());
					target = QPoint(desktop.right() - width(), desktop.top());
				break;

				case BottomLeft:
				{
					move(desktop.left(), desktop.bottom());
					target = QPoint(desktop.left(), desktop.bottom() - height());
				}
				break;

				case BottomRight:
				{
					move(desktop.right() - width(), desktop.bottom());
					target = QPoint(desktop.right() - width(), desktop.bottom() - height());
				}
				break;
			}

			animation->setStartValue(pos());
			animation->setEndValue(target);
			animation->setDuration(150);
			animation->start(QPropertyAnimation::DeleteWhenStopped);
		}
	}
}

void Widget::enterEvent(QEvent *event)
{
	QWidget::enterEvent(event);
	m_mouseInside = true;
}

void Widget::leaveEvent(QEvent *event)
{
	QWidget::leaveEvent(event);
	m_mouseInside = false;

	foreach(Label *label, m_timedOut) {
		removeLabel(label);
	}

	m_timedOut.clear();
}

void Widget::maybeRemove()
{
	if(Label *label = qobject_cast<Label*>(sender())) {
		if(m_mouseInside) {
			m_timedOut << label;
		} else {
			removeLabel(label);
		}
	}
}

void Widget::removeClicked(const QString &url)
{
	if(Label *label = qobject_cast<Label*>(sender())) {
		removeLabel(label);
	}

	emit linkClicked(url);
}

void Widget::removeLabel(Label *label)
{
	m_layout->removeWidget(label);
	delete label;

	m_centralWidget->adjustSize();
	adjustSize();

	if(!m_layout->count()) {
		hide();
	}
}

}
