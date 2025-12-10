#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QIcon>

#include "core/FontManager.h"
#include "models/FontListModel.h"
#include "models/CollectionModel.h"
#include "utils/Config.h"
#include "utils/Logger.h"

int main(int argc, char *argv[]) {
    // Initialize logger first
    TypeNoodle::Logger::initialize();
    TN_INFO("TypeNoodle starting...");

    // Load configuration
    TypeNoodle::Config::instance().load();

    // Set up Qt application
    QGuiApplication app(argc, argv);
    app.setOrganizationName("TypeNoodle");
    app.setOrganizationDomain("typenoodle.app");
    app.setApplicationName("TypeNoodle");
    app.setApplicationVersion("1.0.0");

    // Set QuickControls style
    QQuickStyle::setStyle("Basic");

    // Create core objects
    TypeNoodle::FontManager fontManager;
    TypeNoodle::FontListModel fontListModel;
    TypeNoodle::CollectionModel collectionModel;

    // Create QML engine
    QQmlApplicationEngine engine;

    // Register types
    qmlRegisterSingletonType(QUrl("qrc:/qt/qml/TypeNoodle/Theme.qml"), "TypeNoodle", 1, 0, "Theme");

    // Expose C++ objects to QML
    engine.rootContext()->setContextProperty("FontManager", &fontManager);
    engine.rootContext()->setContextProperty("fontListModel", &fontListModel);
    engine.rootContext()->setContextProperty("collectionModel", &collectionModel);

    // Load main QML file
    const QUrl url(u"qrc:/qt/qml/TypeNoodle/Main.qml"_qs);

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl) {
            TN_ERROR("Failed to load QML");
            QCoreApplication::exit(-1);
        }
    }, Qt::QueuedConnection);

    engine.load(url);

    if (engine.rootObjects().isEmpty()) {
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
