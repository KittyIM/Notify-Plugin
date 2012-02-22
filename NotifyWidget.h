#ifndef NOTIFY_WIDGET_H
#define NOTIFY_WIDGET_H

#include <QtGui/QFrame>

class QVBoxLayout;
class QLabel;

namespace Notify
{
	class Label: public QFrame
	{
		Q_OBJECT

		public:
			Label(const QPixmap &icon, const QString &text, int timeout = 2500);

		signals:
			void timeout();
			void linkClicked(const QString &url);

		private:
			QLabel *m_icon;
			QLabel *m_text;
			int m_timeout;
	};

	class Widget: public QWidget
	{
		Q_OBJECT

		public:
			enum Position
			{
				TopLeft = 0,
				TopRight,
				BottomLeft,
				BottomRight
			};

		public:
			explicit Widget(QWidget *parent = 0);

			void addNotify(const QPixmap &icon, const QString &text, int timeout = 2500);

			void setAnimation(const bool &animation) { m_animation = animation; }
			bool animation() const { return m_animation; }

			void setPosition(const Position &position) { m_position = position; }
			Position position() const { return m_position; }

			void setLimit(const int &limit) { m_limit = limit; }
			int limit() const { return m_limit; }

		signals:
			void linkClicked(const QString &url);

		protected:
			void resizeEvent(QResizeEvent *event);
			void enterEvent(QEvent *event);
			void leaveEvent(QEvent *event);

		private slots:
			void maybeRemove();
			void removeLabel(Label *label);

		private:
			bool m_mouseInside;
			bool m_animation;
			Position m_position;
			int m_limit;
			QList<Label*> m_timedOut;
			QVBoxLayout *m_layout;
			QFrame *m_centralWidget;
	};

}

#endif // NOTIFY_WIDGET_H
