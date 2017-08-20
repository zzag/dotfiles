from PyQt4 import QtGui
import hermite_curve


class MainWindow(QtGui.QWidget):

    def __init__(self):
        super(MainWindow, self).__init__()
        self.init_ui()

    def init_ui(self):
        layout = QtGui.QHBoxLayout()
        hermit_curve_view = hermite_curve.HermiteCurve()

        layout.addWidget(hermit_curve_view)

        self.setLayout(layout)
        self.setWindowTitle("Hermite Curve")
        self.resize(800, 600)
