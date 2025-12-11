import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import TypeNoodle

Window {
    id: loadingWindow
    width: 600
    height: 400
    title: "TypeNoodle"
    flags: Qt.SplashScreen | Qt.FramelessWindowHint
    modality: Qt.ApplicationModal
    
    property int progress: 0
    property string statusText: "Initializing..."
    
    signal loadingComplete()
    
    // Connect to FontManager signals
    Connections {
        target: FontManager
        function onScanProgressChanged(prog) {
            loadingWindow.progress = prog;
        }
        function onScanStatusChanged(status) {
            loadingWindow.statusText = status;
        }
    }
    
    Rectangle {
        anchors.fill: parent
        color: Theme.background
        
        ColumnLayout {
            anchors.centerIn: parent
            width: parent.width * 0.8
            spacing: Theme.spacingXl
            
            // Logo/Icon area
            Item {
                Layout.alignment: Qt.AlignHCenter
                Layout.preferredWidth: 120
                Layout.preferredHeight: 120
                
                Rectangle {
                    anchors.fill: parent
                    color: Theme.primary
                    radius: Theme.radiusXl
                    
                    Text {
                        anchors.centerIn: parent
                        text: "TN"
                        color: Theme.background
                        font.pixelSize: 48
                        font.bold: true
                    }
                }
            }
            
            // App name
            Text {
                Layout.alignment: Qt.AlignHCenter
                text: "TypeNoodle"
                color: Theme.foreground
                font.pixelSize: Theme.fontSizeXxl
                font.bold: true
            }
            
            // Spacer
            Item {
                Layout.preferredHeight: Theme.spacingLg
            }
            
            // Status text
            Text {
                id: statusLabel
                Layout.alignment: Qt.AlignHCenter
                text: loadingWindow.statusText
                color: Theme.foregroundDim
                font.pixelSize: Theme.fontSizeMd
            }
            
            // Progress bar
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 8
                color: Theme.surface0
                radius: 4
                
                Rectangle {
                    width: parent.width * (loadingWindow.progress / 100)
                    height: parent.height
                    color: Theme.primary
                    radius: 4
                    
                    Behavior on width {
                        NumberAnimation {
                            duration: Theme.transitionMedium
                            easing.type: Easing.OutCubic
                        }
                    }
                }
            }
            
            // Progress percentage
            Text {
                Layout.alignment: Qt.AlignHCenter
                text: Math.round(loadingWindow.progress) + "%"
                color: Theme.foregroundDim
                font.pixelSize: Theme.fontSizeSm
                font.family: "monospace"
            }
            
            // Version info
            Text {
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: Theme.spacingLg
                text: "v1.0.0"
                color: Theme.foregroundDim
                font.pixelSize: Theme.fontSizeSm
                opacity: 0.5
            }
        }
        
        // Subtle background pattern (optional)
        Canvas {
            anchors.fill: parent
            opacity: 0.03
            
            onPaint: {
                var ctx = getContext("2d");
                ctx.strokeStyle = Theme.foreground;
                ctx.lineWidth = 1;
                
                var spacing = 40;
                for (var x = 0; x < width; x += spacing) {
                    ctx.beginPath();
                    ctx.moveTo(x, 0);
                    ctx.lineTo(x, height);
                    ctx.stroke();
                }
                
                for (var y = 0; y < height; y += spacing) {
                    ctx.beginPath();
                    ctx.moveTo(0, y);
                    ctx.lineTo(width, y);
                    ctx.stroke();
                }
            }
        }
    }
    
    // Auto-close when loading is complete
    Connections {
        target: loadingWindow
        function onLoadingComplete() {
            loadingWindow.close();
        }
    }
}
