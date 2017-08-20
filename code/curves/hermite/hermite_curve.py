from PyQt4.QtGui import *
import numpy as np


class HermiteCurve(QWidget):

    def __init__(self):
        super(HermiteCurve, self).__init__()
        self.p1 = np.array([-100.0, 0.0, 1.0])
        self.p2 = np.array([200.0, 0.0, 1.0])
        self.t1 = np.array([400.0, -200.0, 1.0])
        self.t2 = np.array([-500.0, 250.0, 1.0])
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

        # draw endpoints
        pen = QPen(QColor(0, 0, 0), 4.4)
        qp.setPen(pen)

        s = np.dot(M, self.p1)
        e = np.dot(M, self.p1 + self.t1 - np.array([0, 0, 1]))
        self.draw_endpoint(qp, s, e)

        s = np.dot(M, self.p2)
        e = np.dot(M, self.p2 + self.t2 - np.array([0, 0, 1]))
        self.draw_endpoint(qp, s, e)

        qp.end()

    def draw_endpoint(self, qp, s, e):
        qp.drawEllipse(s[0] - self.endpoint_size / 2,
                       s[1] - self.endpoint_size / 2,
                       self.endpoint_size,
                       self.endpoint_size)
        qp.drawEllipse(e[0] - self.endpoint_size / 2,
                       e[1] - self.endpoint_size / 2,
                       self.endpoint_size,
                       self.endpoint_size)
        qp.drawLine(s[0], s[1], e[0], e[1])

    def h(self, t):
        a = 1.0 - 3 * t * t + 2 * t * t * t
        b = t * t * (3 - 2 * t)
        c = t * (t - 1) * (t - 1)
        d = t * t * (t - 1)
        ret = a * self.p1 + b * self.p2 + c * self.t1 + d * self.t2
        ret[2] = 1.0
        return ret
