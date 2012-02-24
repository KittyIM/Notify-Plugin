#include "NotifySettingsPage.h"
#include "ui_NotifySettingsPage.h"

#include "NotifyConstants.h"

#include <IPluginCore.h>

#include <QtCore/QFileInfo>
#include <QtCore/QDir>

namespace Notify
{

SettingsPage::SettingsPage(KittySDK::IPluginCore *core, QWidget *parent):
	KittySDK::ISettingsPage(core, parent),
	m_ui(new Ui::SettingsPage)
{
	m_ui->setupUi(this);
	connect(m_ui->themePreviewPushButton, SIGNAL(clicked()), SLOT(previewTheme()));

	setIcon(Icons::I_NOTIFY);
	setId(SettingPages::S_NOTIFY);
}

SettingsPage::~SettingsPage()
{
	delete m_ui;
}

void SettingsPage::apply()
{
	core()->setSetting(Settings::S_ANIMATION, m_ui->animationCheckBox->isChecked());
	core()->setSetting(Settings::S_FULLSCREEN, m_ui->fullscreenCheckBox->isChecked());
	core()->setSetting(Settings::S_TIMEOUT, m_ui->displayTimeSpinBox->value());
	core()->setSetting(Settings::S_LIMIT, m_ui->limitSpinBox->value());
	core()->setSetting(Settings::S_POSITION, m_ui->positionComboBox->currentIndex());
	core()->setSetting(Settings::S_THEME, m_ui->themeComboBox->itemData(m_ui->themeComboBox->currentIndex()));

	emit applied();
}

void SettingsPage::reset()
{
	m_ui->animationCheckBox->setChecked(core()->setting(Settings::S_ANIMATION, false).toBool());
	m_ui->fullscreenCheckBox->setChecked(core()->setting(Settings::S_FULLSCREEN, true).toBool());
	m_ui->displayTimeSpinBox->setValue(core()->setting(Settings::S_TIMEOUT, 2500).toInt());
	m_ui->limitSpinBox->setValue(core()->setting(Settings::S_LIMIT, 20).toInt());
	m_ui->positionComboBox->setCurrentIndex(core()->setting(Settings::S_POSITION, 3).toInt());

	m_ui->themeComboBox->clear();
	m_ui->themeComboBox->addItem(tr("Default"), QString());

	QDir themeDir(core()->kittyDir() + "/themes/notify/");
	QFileInfoList themes = themeDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
	foreach(QFileInfo dir, themes) {
		m_ui->themeComboBox->addItem(dir.fileName(), dir.fileName());

		if(dir.fileName() == core()->setting(Settings::S_THEME, QString()).toString()) {
			m_ui->themeComboBox->setCurrentIndex(m_ui->themeComboBox->count() - 1);
		}
	}
}

void SettingsPage::retranslate()
{
	m_ui->retranslateUi(this);
}

void SettingsPage::previewTheme()
{
	emit previewClicked(m_ui->themeComboBox->itemData(m_ui->themeComboBox->currentIndex()).toString());
}

}
