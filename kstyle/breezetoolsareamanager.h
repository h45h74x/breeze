#ifndef breezetoolsareamanager_h
#define breezetoolsareamanager_h

#include <QApplication>
#include <QObject>
#include <KSharedConfig>
#include <KConfigWatcher>
#include "breezestyle.h"
#include "breezehelper.h"

namespace Breeze {
    class ToolsAreaManager;

    // Trying to discriminate QApplication events from events from all QObjects
    // belonging to it is impractical with everything going through a single
    // eventFilter, so we have this class which provides a second one that allows
    // us to filter for the events we want.
    class AppListener : public QObject
    {
        Q_OBJECT

        bool eventFilter(QObject *watched, QEvent *event) override;

        ToolsAreaManager *manager;
        friend class ToolsAreaManager;
    };

    class ToolsAreaManager : public QObject
    {
        Q_OBJECT

    private:
        Helper* _helper;
        QHash<QMainWindow*,QVector<QPointer<QToolBar>>> _windows;
        KSharedConfigPtr _config;
        KConfigWatcher::Ptr _watcher;
        QPalette _palette = QPalette();
        AppListener* _listener;

        friend class AppListener;

    protected:
        bool tryRegisterToolBar(QPointer<QMainWindow> window, QPointer<QWidget> widget);
        void tryUnregisterToolBar(QPointer<QMainWindow> window, QPointer<QWidget> widget);
        void configUpdated();

    public:
        explicit ToolsAreaManager(Helper *helper, QObject *parent = nullptr);
        ~ToolsAreaManager();

        bool eventFilter(QObject *watched, QEvent *event) override;

        const QPalette& palette() const { return _palette; }

        void registerApplication(QApplication *application);
        void registerWidget(QWidget *widget);
        void unregisterWidget(QWidget *widget);

        QRect toolsAreaRect(const QMainWindow *window);
    };
}

#endif
