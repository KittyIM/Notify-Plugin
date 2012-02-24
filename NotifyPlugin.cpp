#include "NotifyPlugin.h"

#include "NotifySettingsPage.h"
#include "NotifyConstants.h"
#include "NotifyWidget.h"

#include <SDKConstants.h>

#include <QtCore/QFile>
#include <QtCore/QUrl>
#include <QtGui/QMessageBox>
#include <QtGui/QPixmap>

namespace Notify
{

Plugin::Plugin(KittySDK::IPluginCore *pcore): KittySDK::IPlugin(pcore)
{
	m_info = new KittySDK::IPluginInfo();
	m_info->setName(tr("Notify"));
	m_info->setId("notify");
	m_info->setVersion("0.0.1");
	m_info->setAuthor("arturo182");
	m_info->setEmail("arturo182@tlen.pl");
	m_info->setURL("http://www.arturpacholec.pl/");
}

Plugin::~Plugin()
{
	delete m_info;
}

void Plugin::init()
{
	core()->addIcon(Icons::I_NOTIFY, QPixmap(":/glyphs/notify.png"));

	m_widget = new Widget();
	connect(m_widget, SIGNAL(linkClicked(QString)), SLOT(handleLink(QString)));
	applySettings();
}

void Plugin::load()
{
	m_settings = new SettingsPage(core());
	connect(m_settings, SIGNAL(previewClicked(QString)), SLOT(previewTheme(QString)));
	connect(m_settings, SIGNAL(applied()), SLOT(applySettings()));
	core()->addSettingPage(m_settings, KittySDK::SettingPages::S_DISPLAY);
}

void Plugin::unload()
{
	delete m_settings;
	delete m_widget;
}

void Plugin::execAction(const QString &name, const QMap<QString, QVariant> &args)
{
	if(name == "addNotify") {
		QPixmap icon = args.value("icon").value<QPixmap>();
		QString text = args.value("text").toString();
		int timeout = args.value("timeout", core()->setting(Settings::S_TIMEOUT, 2500).toInt()).toInt();

		m_widget->addNotify(icon, text, timeout);
	} else if(name == "showMessage") {
		QMessageBox::information(0, tr("Notify clicked"), tr("You clicked message from Michael."));
	} else if(name == "retranslate") {
		m_info->setName(tr("Notify"));
	}
}

void Plugin::previewTheme(const QString &theme)
{
	setTheme(theme);
	m_widget->addNotify(core()->icon(KittySDK::Icons::I_STATUS_ONLINE), tr("<b>Kate</b> is online"));
	m_widget->addNotify(core()->icon(KittySDK::Icons::I_MESSAGE), "<a href=\"notify://showMessage\"><span class=\"notifyText\">" + tr("Message from <b>Michael</b>") + "</span><br><span class=\"notifyLink\">" + tr("\"Hey, I was wondering if the...\"") + "</span></a>");
	m_widget->addNotify(core()->icon(KittySDK::Icons::I_STATUS_AWAY), tr("<b>Mathew</b> is away"));
	m_widget->addNotify(core()->icon(KittySDK::Icons::I_STATUS_FFC), tr("<b>Peter</b> changed his description<br>\"Runnin' wild.\""));
	m_widget->addNotify(core()->icon(KittySDK::Icons::I_SMILEY), tr("kitty.im is happy"));

	//setTheme(core()->setting(Settings::S_THEME, tr("Default")).toString());
}

void Plugin::setTheme(const QString &theme)
{
	QString fileName = ":/themes/default/style.css";
	if(!theme.isEmpty()) {
		fileName = core()->kittyDir() + "/themes/notify/" + theme + "/style.css";
	}

	QFile cssFile(fileName);
	if(cssFile.open(QFile::ReadOnly)) {
		QString css = cssFile.readAll();
		cssFile.close();

		m_widget->setStyleSheet(css);
	}
}

void Plugin::handleLink(const QString &link)
{
	QRegExp regex("([-A-z0-9]+)://([A-z0-9]+)(/.*)?");
	if(regex.indexIn(link) > -1) {
		QUrl url(link);

		QMap<QString, QVariant> args;
		for(int i = 0; i < url.queryItems().count(); ++i) {
			QPair<QString, QString> pair = url.queryItems().at(i);
			args.insert(pair.first, pair.second);
		}

		QString pluginId = regex.cap(1);
		QString action = regex.cap(2);
		if(!pluginId.isEmpty() && !action.isEmpty()) {
			core()->execPluginAction(pluginId, action, args);
		}
	}
}

void Plugin::applySettings()
{
	setTheme(core()->setting(Settings::S_THEME, "").toString());

	m_widget->setAnimation(core()->setting(Settings::S_ANIMATION, false).toBool());
	m_widget->setLimit(core()->setting(Settings::S_LIMIT, 20).toInt());
	m_widget->setPosition(static_cast<Widget::Position>(core()->setting(Settings::S_POSITION, 3).toInt()));

	//m_ui->fullscreenCheckBox->setChecked(core()->setting(Settings::S_FULLSCREEN, true).toBool());
}

KITTY_PLUGIN(Plugin)
}
