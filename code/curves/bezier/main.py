import sys
import main_window
from PyQt4 import QtGui


if __name__ == '__main__':
    app = QtGui.QApplication(sys.argv)
    window = main_window.MainWindow()
    window.show()
    sys.exit(app.exec_())
