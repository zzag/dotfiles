from PyQt4.QtGui import *
from PyQt4.QtCore import Qt
import numpy as np


class BezierCurve(QWidget):

    def __init__(self):
        super(BezierCurve, self).__init__()
        self.p0 = np.array([-100.0, 80.0, 1.0])
        self.p1 = np.array([10.0, 250.0, 1.0])
        self.p2 = np.array([100.0, -200.0, 1.0])
        self.p3 = np.array([100.0, 0.0, 1.0])
        self.endpoint_size = 8

    def paintEvent(self, event):
        qp = QPainter()
        w = self.width()
        h = self.height()
        M = np.array([
            [1.0, 0.0, w/2],
            [0.0, -1.0, h/2],
            [0.0, 0.0, 1.0]
        ])

        qp.begin(self)
        # anti-aliasing
        qp.setRenderHint(QPainter.HighQualityAntialiasing)
        qp.setRenderHint(QPainter.Antialiasing)

        # draw curve
        pen = QPen(QColor(255, 0, 0), 3.0)
        qp.setPen(pen)
        for t in np.linspace(0.0, 1.0, 1000):
            p = np.dot(M, self.h(t))
            qp.drawPoint(p[0], p[1])

        # draw control points
        pen = QPen(QColor(0, 0, 0), 14)
        qp.setPen(pen)
        p0 = np.dot(M, self.p0)
        p1 = np.dot(M, self.p1)
        p2 = np.dot(M, self.p2)
        p3 = np.dot(M, self.p3)
        qp.drawPoint(p0[0], p0[1])
        qp.drawPoint(p1[0], p1[1])
        qp.drawPoint(p2[0], p2[1])
        qp.drawPoint(p3[0], p3[1])

        pen = QPen(QColor(33, 33, 33), 4, Qt.DashDotLine)
        qp.setPen(pen)
        qp.drawLine(p0[0], p0[1], p1[0], p1[1])
        qp.drawLine(p1[0], p1[1], p2[0], p2[1])
        qp.drawLine(p2[0], p2[1], p3[0], p3[1])

        qp.end()

    def h(self, t):
        a = (1 - t) * (1 - t) * (1 - t)
        b = 3 * t * (1 - t) * (1 - t)
        c = 3 * t * t * (1 - t)
        d = t * t * t
        ret = a * self.p0 + b * self.p1 + c * self.p2 + d * self.p3
        ret[2] = 1.0
        return ret
