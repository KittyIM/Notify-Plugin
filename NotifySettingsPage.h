#ifndef NOTIFY_SETTINGSPAGE_H
#define NOTIFY_SETTINGSPAGE_H

#include <ISettingsPage.h>

namespace Notify
{
	namespace Ui
	{
		class SettingsPage;
	}

	class PluginCore;

	class SettingsPage: public KittySDK::ISettingsPage
	{
		Q_OBJECT

		public:
			explicit SettingsPage(KittySDK::IPluginCore *core, QWidget *parent = 0);
			~SettingsPage();

			void apply();
			void reset();

		signals:
			void previewClicked(const QString &theme);
			void applied();

		private slots:
			void previewTheme();

		private:
			Ui::SettingsPage *m_ui;
	};
}

#endif // NOTIFY_SETTINGSPAGE_H
