import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import TypeNoodle

Rectangle {
    id: root

    property var collectionModel: null

    signal collectionSelected(string collectionId)
    signal showAllFonts()
    signal createCollectionRequested()

    color: Theme.backgroundAlt
    width: 250

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: Theme.spacingMd
        spacing: Theme.spacingMd

        // All Fonts
        Button {
            Layout.fillWidth: true
            text: "All Fonts"
            flat: true
            checkable: true
            checked: true

            background: Rectangle {
                color: parent.checked ? Theme.cardBackground : (parent.hovered ? Theme.surface0 : "transparent")
                radius: Theme.radiusMd
            }

            contentItem: RowLayout {
                spacing: Theme.spacingSm

                Text {
                    text: "\uf0c7" // Grid icon
                    color: Theme.foreground
                    font.pixelSize: Theme.fontSizeMd
                    font.family: "Font Awesome 6 Free"
                }

                Text {
                    text: "All Fonts"
                    color: Theme.foreground
                    font.pixelSize: Theme.fontSizeMd
                    Layout.fillWidth: true
                }
            }

            onClicked: root.showAllFonts()
        }

        // Collections header
        RowLayout {
            Layout.fillWidth: true
            spacing: Theme.spacingSm

            Text {
                text: "Collections"
                color: Theme.foregroundDim
                font.pixelSize: Theme.fontSizeSm
                font.bold: true
                Layout.fillWidth: true
            }

            Button {
                text: "+"
                flat: true
                implicitWidth: 24
                implicitHeight: 24

                background: Rectangle {
                    color: parent.hovered ? Theme.cardBackground : "transparent"
                    radius: Theme.radiusSm
                }

                contentItem: Text {
                    text: parent.text
                    color: Theme.primary
                    font.pixelSize: Theme.fontSizeLg
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                onClicked: root.createCollectionRequested()
            }
        }

        // Collections list
        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true

            ListView {
                id: collectionsList
                model: root.collectionModel
                spacing: Theme.spacingXs

                delegate: Button {
                    width: ListView.view.width
                    text: model.name
                    flat: true

                    background: Rectangle {
                        color: parent.hovered ? Theme.cardBackground : "transparent"
                        radius: Theme.radiusMd
                    }

                    contentItem: RowLayout {
                        spacing: Theme.spacingSm

                        Text {
                            text: "\uf07b" // Folder icon
                            color: Theme.accent
                            font.pixelSize: Theme.fontSizeMd
                            font.family: "Font Awesome 6 Free"
                        }

                        Text {
                            text: model.name
                            color: Theme.foreground
                            font.pixelSize: Theme.fontSizeMd
                            Layout.fillWidth: true
                        }

                        Text {
                            text: model.fontCount.toString()
                            color: Theme.foregroundDim
                            font.pixelSize: Theme.fontSizeSm
                        }
                    }

                    onClicked: root.collectionSelected(model.collectionId)
                }
            }
        }
    }
}
