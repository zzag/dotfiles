import QtQuick 2.0
import QtQuick.Layouts 1.1
import org.kde.plasma.components 2.0 as PlasmaComponents
import org.kde.plasma.core 2.0 as PlasmaCore

Item {
    Layout.preferredWidth: timeLabel.width + 25

    PlasmaCore.DataSource {
        id: timeSource
        engine: 'time'
        connectedSources: ['Local']
        interval: 1000
    }

    PlasmaComponents.Label {
        id: timeLabel
        anchors.centerIn: parent
        font.pointSize: 12
        text: Qt.formatDateTime(timeSource.data.Local.DateTime, "h:mm AP")
    }
}
