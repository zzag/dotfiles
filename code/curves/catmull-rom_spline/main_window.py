from PyQt4.QtGui import QWidget, QHBoxLayout
from catmull_rom import CatmullRomSpline


class MainWindow(QWidget):

    def __init__(self):
        super(MainWindow, self).__init__()

        self.init_ui()

    def init_ui(self):
        layout = QHBoxLayout()
        render = CatmullRomSpline()
        layout.addWidget(render)
        self.setLayout(layout)

        self.resize(800, 600)
        self.setWindowTitle("Catmull-Rom spline")
