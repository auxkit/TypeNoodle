#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QIcon>
#include <QQuickWindow>
#include <QPixmap>
#include <QPainter>
#include <QFont>
#include <QSplashScreen>
#include <QTimer>

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

    // Set up Qt application (use QApplication for QSplashScreen support)
    QApplication app(argc, argv);
    app.setOrganizationName("TypeNoodle");
    app.setOrganizationDomain("typenoodle.app");
    app.setApplicationName("TypeNoodle");
    app.setApplicationVersion("1.0.0");

    // Set QuickControls style
    QQuickStyle::setStyle("Basic");

    // Create splash screen
    QPixmap splashPixmap(600, 400);
    splashPixmap.fill(QColor("#1e1e2e")); // Theme.background color
    
    QPainter painter(&splashPixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // Draw logo/icon
    painter.setBrush(QColor("#89b4fa")); // Theme.primary color
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(240, 80, 120, 120, 16, 16);
    
    QFont logoFont;
    logoFont.setPixelSize(48);
    logoFont.setBold(true);
    painter.setFont(logoFont);
    painter.setPen(QColor("#1e1e2e")); // Background color for contrast
    painter.drawText(QRect(240, 80, 120, 120), Qt::AlignCenter, "TN");
    
    // Draw app name
    QFont titleFont;
    titleFont.setPixelSize(24);
    titleFont.setBold(true);
    painter.setFont(titleFont);
    painter.setPen(QColor("#cdd6f4")); // Theme.foreground color
    painter.drawText(QRect(0, 220, 600, 30), Qt::AlignCenter, "TypeNoodle");
    
    // Draw version
    QFont versionFont;
    versionFont.setPixelSize(12);
    painter.setFont(versionFont);
    painter.setPen(QColor("#a6adc8")); // Theme.foregroundDim color
    painter.setOpacity(0.5);
    painter.drawText(QRect(0, 360, 600, 20), Qt::AlignCenter, "v1.0.0");
    
    painter.end();
    
    QSplashScreen *splash = new QSplashScreen(splashPixmap, Qt::WindowStaysOnTopHint);
    splash->show();
    app.processEvents();

    // Create core objects
    TypeNoodle::FontManager fontManager;
    TypeNoodle::FontListModel fontListModel;
    TypeNoodle::CollectionModel collectionModel;

    // Connect to FontManager signals to update splash screen
    QObject::connect(&fontManager, &TypeNoodle::FontManager::scanStatusChanged,
                     [splash](const QString& status) {
        splash->showMessage(status, Qt::AlignHCenter | Qt::AlignBottom, QColor("#cdd6f4"));
        QGuiApplication::processEvents();
    });

    // Create QML engine
    QQmlApplicationEngine engine;

    // Expose C++ objects to QML
    engine.rootContext()->setContextProperty("FontManager", &fontManager);
    engine.rootContext()->setContextProperty("_fontListModel", &fontListModel);
    engine.rootContext()->setContextProperty("_collectionModel", &collectionModel);

    // Load main QML file from module first (but hidden)
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [](QObject *obj, const QUrl &objUrl) {
        if (!obj) {
            TN_ERROR("Failed to load QML");
            QCoreApplication::exit(-1);
        } else {
            TN_INFO("Main window QML loaded");
            // Keep window hidden initially
            QQuickWindow* window = qobject_cast<QQuickWindow*>(obj);
            if (window) {
                window->setVisible(false);
            }
        }
    }, Qt::QueuedConnection);

    engine.loadFromModule("TypeNoodle", "Main");

    if (engine.rootObjects().isEmpty()) {
        TN_ERROR("No root objects loaded from QML");
        return -1;
    }

    // Get the main window
    QQuickWindow* mainWindow = qobject_cast<QQuickWindow*>(engine.rootObjects().first());

    // Connect scan completion to show main window
    QObject::connect(&fontManager, &TypeNoodle::FontManager::scanCompleted,
                     [splash, mainWindow]() {
        TN_INFO("Font scanning completed, showing main window...");
        
        // Show main window
        if (mainWindow) {
            mainWindow->show();
            mainWindow->raise();
            mainWindow->requestActivate();
        }
        
        // Close splash screen after a short delay
        QTimer::singleShot(300, [splash]() {
            splash->close();
            splash->deleteLater();
        });
    });

    // Start font scanning in background
    TN_INFO("Starting background font scan...");
    splash->showMessage("Initializing...", Qt::AlignHCenter | Qt::AlignBottom, QColor("#cdd6f4"));
    QMetaObject::invokeMethod(&fontManager, &TypeNoodle::FontManager::refreshFonts, Qt::QueuedConnection);

    TN_INFO("TypeNoodle started successfully");

    // Run application
    int result = app.exec();

    // Cleanup
    TN_INFO("TypeNoodle shutting down...");
    TypeNoodle::Config::instance().save();
    TypeNoodle::Logger::shutdown();

    return result;
}
