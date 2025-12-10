import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import TypeNoodle

Item {
    id: root

    property var fontListModel: null

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        // Search and filters
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 80
            color: Theme.background

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: Theme.spacingMd
                spacing: Theme.spacingSm

                SearchBar {
                    Layout.fillWidth: true
                    onTextChanged: {
                        if (root.fontListModel) {
                            root.fontListModel.filterQuery = text
                        }
                    }
                }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: Theme.spacingSm

                    Text {
                        text: root.fontListModel ? root.fontListModel.count + " fonts" : "0 fonts"
                        color: Theme.foregroundDim
                        font.pixelSize: Theme.fontSizeSm
                    }

                    Item { Layout.fillWidth: true }

                    CheckBox {
                        text: "Active only"
                        checked: false
                        onCheckedChanged: {
                            if (root.fontListModel) {
                                root.fontListModel.showActiveOnly = checked
                            }
                        }

                        indicator: Rectangle {
                            width: 18
                            height: 18
                            radius: 3
                            border.color: Theme.primary
                            border.width: 2
                            color: parent.checked ? Theme.primary : "transparent"

                            Text {
                                anchors.centerIn: parent
                                text: "\u2713" // Check mark
                                color: Theme.background
                                font.pixelSize: 12
                                visible: parent.parent.checked
                            }
                        }

                        contentItem: Text {
                            text: parent.text
                            color: Theme.foreground
                            font.pixelSize: Theme.fontSizeSm
                            leftPadding: parent.indicator.width + Theme.spacingSm
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                }
            }
        }

        // Font grid
        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true

            GridView {
                id: fontGrid
                model: root.fontListModel
                cellWidth: 320
                cellHeight: 220

                delegate: Item {
                    width: fontGrid.cellWidth
                    height: fontGrid.cellHeight

                    FontCard {
                        anchors.centerIn: parent
                        fontFamily: model.family
                        fontStyle: model.style
                        displayName: model.displayName
                        format: model.format
                        active: model.active

                        onClicked: {
                            // Show preview pane
                            previewPane.fontFamily = model.family
                            previewPane.fontStyle = model.style
                            previewPane.displayName = model.displayName
                            previewPane.format = model.format
                            previewPane.filePath = model.filePath
                            previewPane.visible = true
                        }

                        onToggleActivation: {
                            if (FontManager) {
                                if (model.active) {
                                    FontManager.deactivateFont(model.fontId)
                                } else {
                                    FontManager.activateFont(model.fontId)
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // Preview pane overlay
    FontPreviewPane {
        id: previewPane
        anchors.fill: parent
        visible: false
    }
}
