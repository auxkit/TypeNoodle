import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import TypeNoodle

ApplicationWindow {
    id: window
    visible: true
    width: 1280
    height: 800
    title: "TypeNoodle - Font Manager"

    color: Theme.background

    Component.onCompleted: {
        // Initial font refresh
        if (FontManager) {
            FontManager.refreshFonts()
        }
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        // Top bar
        TopBar {
            Layout.fillWidth: true
            onRefreshRequested: {
                if (FontManager) {
                    FontManager.refreshFonts()
                }
            }
            onSettingsRequested: {
                stackView.push(settingsPage)
            }
        }

        // Main content area
        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 0

            // Sidebar
            Sidebar {
                id: sidebar
                Layout.fillHeight: true
                collectionModel: collectionModel

                onShowAllFonts: {
                    if (fontListModel) {
                        fontListModel.filterCollection = ""
                    }
                }

                onCollectionSelected: function(collectionId) {
                    if (fontListModel) {
                        fontListModel.filterCollection = collectionId
                    }
                }

                onCreateCollectionRequested: {
                    createCollectionDialog.open()
                }
            }

            // Main content
            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: Theme.background

                StackView {
                    id: stackView
                    anchors.fill: parent
                    initialItem: libraryPage
                }
            }
        }
    }

    // Pages
    Component {
        id: libraryPage
        LibraryPage {
            fontListModel: fontListModel
        }
    }

    Component {
        id: settingsPage
        SettingsPage {
            onCloseRequested: stackView.pop()
        }
    }

    // Create collection dialog
    Dialog {
        id: createCollectionDialog
        anchors.centerIn: parent
        width: 400
        title: "Create Collection"
        modal: true

        background: Rectangle {
            color: Theme.cardBackground
            radius: Theme.radiusLg
        }

        header: Rectangle {
            height: 50
            color: Theme.backgroundAlt
            radius: Theme.radiusLg

            Text {
                anchors.centerIn: parent
                text: "Create Collection"
                color: Theme.foreground
                font.pixelSize: Theme.fontSizeLg
                font.bold: true
            }
        }

        ColumnLayout {
            anchors.fill: parent
            spacing: Theme.spacingMd

            Text {
                text: "Collection Name"
                color: Theme.foreground
                font.pixelSize: Theme.fontSizeMd
            }

            TextField {
                id: collectionNameField
                Layout.fillWidth: true
                placeholderText: "My Collection"

                background: Rectangle {
                    color: Theme.surface0
                    radius: Theme.radiusMd
                }

                color: Theme.foreground
                font.pixelSize: Theme.fontSizeMd
                padding: Theme.spacingSm
            }

            Text {
                text: "Description (optional)"
                color: Theme.foreground
                font.pixelSize: Theme.fontSizeMd
            }

            TextField {
                id: collectionDescField
                Layout.fillWidth: true
                placeholderText: "Description..."

                background: Rectangle {
                    color: Theme.surface0
                    radius: Theme.radiusMd
                }

                color: Theme.foreground
                font.pixelSize: Theme.fontSizeMd
                padding: Theme.spacingSm
            }
        }

        footer: DialogButtonBox {
            background: Rectangle {
                color: Theme.backgroundAlt
                radius: Theme.radiusLg
            }

            Button {
                text: "Create"
                DialogButtonBox.buttonRole: DialogButtonBox.AcceptRole

                background: Rectangle {
                    color: parent.hovered ? Theme.primaryHover : Theme.primary
                    radius: Theme.radiusMd
                }

                contentItem: Text {
                    text: parent.text
                    color: Theme.background
                    font.pixelSize: Theme.fontSizeMd
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }

            Button {
                text: "Cancel"
                DialogButtonBox.buttonRole: DialogButtonBox.RejectRole

                background: Rectangle {
                    color: parent.hovered ? Theme.surface1 : Theme.surface0
                    radius: Theme.radiusMd
                }

                contentItem: Text {
                    text: parent.text
                    color: Theme.foreground
                    font.pixelSize: Theme.fontSizeMd
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }

            onAccepted: {
                if (collectionNameField.text.trim() !== "" && collectionModel) {
                    collectionModel.createCollection(
                        collectionNameField.text,
                        collectionDescField.text
                    )
                    collectionNameField.text = ""
                    collectionDescField.text = ""
                    createCollectionDialog.close()
                }
            }

            onRejected: {
                collectionNameField.text = ""
                collectionDescField.text = ""
                createCollectionDialog.close()
            }
        }
    }
}
