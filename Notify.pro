TARGET     = Notify
TEMPLATE   = lib

SOURCES   += NotifyPlugin.cpp \
    NotifySettingsPage.cpp \
    NotifyWidget.cpp

HEADERS   += NotifyPlugin.h \
    NotifyConstants.h \
    NotifySettingsPage.h \
    NotifyWidget.h

RESOURCES += res/res.qrc

isEmpty(SDK_PATH):error(Set the SDK_PATH variable!)
include($$SDK_PATH/KittySDK.pri)

FORMS += \
    NotifySettingsPage.ui
