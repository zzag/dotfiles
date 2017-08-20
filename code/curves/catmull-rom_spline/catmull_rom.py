from PyQt4.QtGui import *
import numpy as np


class CatmullRomSpline(QWidget):

    def __init__(self):
        super(CatmullRomSpline, self).__init__()
        self.points = np.array([
            [-300.0, 200.0],
            [-200.0, 50.0],
            [-100.0, -210.0],
            [0.0, 50.0],
            [20.0, 10.0],
            [50.0, 50.0],
            [100.0, 100.0],
            [150.0, 250.0],
            [220.0, 10.0]
        ])

    def paintEvent(self, event):
        qp = QPainter()
        M = np.array([
            [1.0, 0.0, self.width() / 2],
            [0.0, -1.0, self.height() / 2],
            [0.0, 0.0, 1.0]
        ])

        qp.begin(self)

        # init stuff
        qp.setBackground(QBrush(QColor(255, 255, 255)))
        qp.eraseRect(event.rect())
        qp.setRenderHint(QPainter.Antialiasing)
        qp.setRenderHint(QPainter.HighQualityAntialiasing)

        # draw pieces
        for i in range(1, len(self.points) - 2):
            self.draw_piece(M, qp, i)

        # draw points
        pen = QPen(QColor(33, 33, 33), 7)
        qp.setPen(pen)
        for point in self.points:
            p = np.dot(M, np.hstack((point, 1.0)))
            qp.drawPoint(p[0], p[1])

        qp.end()

    def draw_piece(self, M, qp, i):
        pen = QPen(QColor(255, 0, 0), 4)
        qp.setPen(pen)

        for t in np.linspace(0.0, 1.0, 1000):
            p = np.dot(M, np.hstack((self.piece(i, t), 1.0)))
            qp.drawPoint(p[0], p[1])

    def piece(self, i, t):
        a = -t + 2 * t ** 2 - t ** 3
        b = 2 - 5 * t ** 2 + 3 * t ** 3
        c = t + 4 * t ** 2 - 3 * t ** 3
        d = -(t ** 2) + t ** 3
        p = a * self.points[i - 1] + b * self.points[i] + c * self.points[i + 1] + d * self.points[i + 2]
        return 0.5 * p
