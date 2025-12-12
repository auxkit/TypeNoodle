#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QIcon>

#include "version.h"
#include "core/FontManager.h"
#include "models/FontListModel.h"
#include "models/CollectionModel.h"
#include "utils/Config.h"
#include "utils/SettingsManager.h"
#include "utils/Logger.h"

int main(int argc, char *argv[]) {
    // Initialize logger first
    TypeNoodle::Logger::initialize();
    TN_INFO("Application starting...");

    // Load configuration
    TypeNoodle::Config::instance().load();

    // Set up Qt application
    QGuiApplication app(argc, argv);
    app.setOrganizationName(QString(PROJECT_ORG_NAME));
    app.setOrganizationDomain(QString(PROJECT_ORG_DOMAIN));
    app.setApplicationName(QString(PROJECT_NAME));
    app.setApplicationVersion(QString(PROJECT_VERSION));

    // Set QuickControls style
    QQuickStyle::setStyle("Basic");

    // Create core objects
    TypeNoodle::FontManager fontManager;
    TypeNoodle::FontListModel fontListModel;
    TypeNoodle::CollectionModel collectionModel;
    TypeNoodle::SettingsManager settingsManager;

    // Create QML engine
    QQmlApplicationEngine engine;

    // Expose C++ objects to QML
    engine.rootContext()->setContextProperty("FontManager", &fontManager);
    engine.rootContext()->setContextProperty("_fontListModel", &fontListModel);
    engine.rootContext()->setContextProperty("_collectionModel", &collectionModel);
    engine.rootContext()->setContextProperty("Settings", &settingsManager);

    // Load main QML file from module
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [](QObject *obj, const QUrl &objUrl) {
        if (!obj) {
            TN_ERROR("Failed to load QML");
            QCoreApplication::exit(-1);
        }
    }, Qt::QueuedConnection);

    engine.loadFromModule("TypeNoodle", "Main");

    if (engine.rootObjects().isEmpty()) {
        // TN_ERROR(engine.rootObjects());
        TN_ERROR("No root objects loaded from QML");
        return -1;
    }

    TN_INFO("TypeNoodle started successfully");

    // Run application
    int result = app.exec();

    // Cleanup
    TN_INFO("TypeNoodle shutting down...");
    TypeNoodle::Config::instance().save();
    TypeNoodle::Logger::shutdown();

    return result;
}
