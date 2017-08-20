from PyQt4 import QtGui
import bezier_curve


class MainWindow(QtGui.QWidget):

    def __init__(self):
        super(MainWindow, self).__init__()
        self.init_ui()

    def init_ui(self):
        layout = QtGui.QHBoxLayout()
        bezier_curve_view = bezier_curve.BezierCurve()

        layout.addWidget(bezier_curve_view)

        self.setLayout(layout)
        self.setWindowTitle("Bezier Curve")
        self.resize(800, 600)
