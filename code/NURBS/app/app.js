import './styles.css';
import NURBS from './nurbs';
import { range } from './common';

function drawControlPoints(ctx, points, color = '#64B5F6') {
  points.forEach((p) => {
    ctx.beginPath();
    ctx.arc(p.x, p.y, 3, 0, 2 * Math.PI);
    ctx.closePath();
    ctx.fillStyle = color;
    ctx.fill();
  });
}

function drawSpline(ctx, spline, color = '#0D47A1') {
  ctx.beginPath();
  range(0, 1, 1e-3).reduce((prev, t) => {
    const curr = spline.interpolate(t);
    if (prev) {
      ctx.moveTo(prev.x, prev.y);
      ctx.lineTo(curr.x, curr.y);
    }
    return curr;
  });
  ctx.closePath();
  ctx.strokeStyle = color;
  ctx.stroke();
}

function run() {
  const drawingArea = document.getElementById('drawing-area');
  const ctx = drawingArea.getContext('2d');

  const points = [
    { x: 100, y: 100 },
    { x: 150, y: 200 },
    { x: 400, y: 50 },
    { x: 650, y: 150 },
    { x: 450, y: 400 },
    { x: 300, y: 400 },
    { x: 100, y: 400 },
    { x: 100, y: 550 },
    { x: 300, y: 550 },
  ];

  const spline = new NURBS({
    points,
    degree: 2,
  });

  drawSpline(ctx, spline);
  drawControlPoints(ctx, points);
}


run();
