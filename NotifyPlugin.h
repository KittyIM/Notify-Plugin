#ifndef NOTIFYPLUGIN_H
#define NOTIFYPLUGIN_H

#include <IPlugin.h>

#include <QtCore/QDebug>

#define qDebug() qDebug() << "[Notify]"
#define qWarning() qWarning() << "[Notify]"

namespace Notify
{
	class SettingsPage;
	class Widget;

	class Plugin: public KittySDK::IPlugin
	{
		Q_OBJECT

		public:
			Plugin(KittySDK::IPluginCore *pcore);
			~Plugin();

			void init();
			void load();
			void unload();

		public slots:
			void execAction(const QString &name, const QMap<QString, QVariant> &args);

		private slots:
			void previewTheme(const QString &theme);
			void setTheme(const QString &theme);
			void handleLink(const QString &link);
			void applySettings();

		private:
			SettingsPage *m_settings;
			Widget *m_widget;
	};
}

#endif // NOTIFYPLUGIN_H

