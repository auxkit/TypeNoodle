pragma Singleton
import QtQuick

QtObject {
    // Catppuccin Mocha color palette
    readonly property color base: "#1e1e2e"
    readonly property color mantle: "#181825"
    readonly property color crust: "#11111b"

    readonly property color surface0: "#313244"
    readonly property color surface1: "#45475a"
    readonly property color surface2: "#585b70"

    readonly property color overlay0: "#6c7086"
    readonly property color overlay1: "#7f849c"
    readonly property color overlay2: "#9399b2"

    readonly property color text: "#cdd6f4"
    readonly property color subtext0: "#a6adc8"
    readonly property color subtext1: "#bac2de"

    readonly property color lavender: "#b4befe"
    readonly property color blue: "#89b4fa"
    readonly property color sapphire: "#74c7ec"
    readonly property color sky: "#89dceb"
    readonly property color teal: "#94e2d5"
    readonly property color green: "#a6e3a1"
    readonly property color yellow: "#f9e2af"
    readonly property color peach: "#fab387"
    readonly property color maroon: "#eba0ac"
    readonly property color red: "#f38ba8"
    readonly property color mauve: "#cba6f7"
    readonly property color pink: "#f5c2e7"
    readonly property color flamingo: "#f2cdcd"
    readonly property color rosewater: "#f5e0dc"

    // Semantic colors
    readonly property color background: base
    readonly property color backgroundAlt: mantle
    readonly property color backgroundDark: crust

    readonly property color foreground: text
    readonly property color foregroundDim: subtext0
    readonly property color foregroundBright: subtext1

    readonly property color primary: blue
    readonly property color primaryHover: sapphire
    readonly property color accent: mauve
    readonly property color success: green
    readonly property color warning: yellow
    readonly property color error: red

    readonly property color cardBackground: surface0
    readonly property color cardBackgroundHover: surface1
    readonly property color border: surface2

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

    // Typography
    readonly property int fontSizeXs: 10
    readonly property int fontSizeSm: 12
    readonly property int fontSizeMd: 14
    readonly property int fontSizeLg: 16
    readonly property int fontSizeXl: 20
    readonly property int fontSizeXxl: 24

    readonly property string fontFamily: "Inter"
    readonly property string fontFamilyMono: "JetBrains Mono"

    // Transitions
    readonly property int transitionFast: 100
    readonly property int transitionMedium: 200
    readonly property int transitionSlow: 300
}
