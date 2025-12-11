pragma Singleton
import QtQuick

QtObject {
    // TypeNoodle color palette
    // Base backgrounds
    readonly property color base: "#0a0a0b"
    readonly property color mantle: "#111113"
    readonly property color crust: "#050506"

    readonly property color surface0: "#18181b"
    readonly property color surface1: "#27272a"
    readonly property color surface2: "#3f3f46"

    readonly property color overlay0: "#52525b"
    readonly property color overlay1: "#71717a"
    readonly property color overlay2: "#a1a1aa"

    readonly property color text: "#fafafa"
    readonly property color subtext0: "#a1a1aa"
    readonly property color subtext1: "#d4d4d8"

    // Accent colors - Warm Orange
    readonly property color orange: "#ff6b35"
    readonly property color orangeLight: "#ff8f66"
    readonly property color orangeDark: "#e55a2b"

    // Supporting colors
    readonly property color blue: "#3b82f6"
    readonly property color green: "#10b981"
    readonly property color yellow: "#f59e0b"
    readonly property color red: "#ef4444"
    readonly property color purple: "#8b5cf6"
    readonly property color pink: "#ec4899"
    readonly property color teal: "#14b8a6"
    readonly property color indigo: "#6366f1"

    // Semantic colors
    readonly property color background: base
    readonly property color backgroundAlt: mantle
    readonly property color backgroundDark: crust

    readonly property color foreground: text
    readonly property color foregroundDim: subtext0
    readonly property color foregroundBright: subtext1

    readonly property color primary: orange
    readonly property color primaryHover: orangeLight
    readonly property color primaryDark: orangeDark
    readonly property color accent: orange
    readonly property color success: green
    readonly property color warning: yellow
    readonly property color error: red

    readonly property color cardBackground: surface0
    readonly property color cardBackgroundHover: surface1
    readonly property color border: surface1

    // Spacing
    readonly property int spacingXs: 4
    readonly property int spacingSm: 8
    readonly property int spacingMd: 16
    readonly property int spacingLg: 24
    readonly property int spacingXl: 32

    // Border radius
    readonly property int radiusSm: 4
    readonly property int radiusMd: 8
    readonly property int radiusLg: 12
    readonly property int radiusXl: 16
    readonly property int radiusXxl: 20
    readonly property int radiusFull: 9999

    // Typography
    readonly property int fontSizeXs: 10
    readonly property int fontSizeSm: 12
    readonly property int fontSizeMd: 14
    readonly property int fontSizeLg: 16
    readonly property int fontSizeXl: 20
    readonly property int fontSizeXxl: 24
    readonly property int fontSizeHero: 48

    readonly property string fontFamily: "Inter"
    readonly property string fontFamilyMono: "JetBrains Mono"

    // Font weights
    readonly property int fontWeightLight: 300
    readonly property int fontWeightRegular: 400
    readonly property int fontWeightMedium: 500
    readonly property int fontWeightSemibold: 600
    readonly property int fontWeightBold: 700
    readonly property int fontWeightExtrabold: 800

    // Transitions
    readonly property int transitionFast: 100
    readonly property int transitionMedium: 200
    readonly property int transitionSlow: 300

    // Shadows (as opacity values for dark theme)
    readonly property real shadowSm: 0.2
    readonly property real shadowMd: 0.3
    readonly property real shadowLg: 0.4
}
