import sys
from main_window import MainWindow
from PyQt4.QtGui import QApplication


if __name__ == '__main__':
    app = QApplication(sys.argv)
    window = MainWindow()
    window.show()
    sys.exit(app.exec_())
