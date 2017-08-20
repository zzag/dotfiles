import './styles.css';

import 'babel-polyfill';
import interpolateBezier from './bezier';


const CONTROL_POINT_COLOR = '#90CAF9';
const CURVE_COLOR = '#0D47A1';

const canvas = document.getElementById('drawing-area');
const ctx = canvas.getContext('2d');

const p0 = {
    x: 10,
    y: 100
};
const p1 = {
    x: 400,
    y: 500
};
const p2 = {
    x: 400,
    y: 400
};
const p2_ = {
    x: 400,
    y: 400
};
const p3 = {
    x: 400,
    y: 300
};
const p4 = {
    x: 500,
    y: 500
};


function drawControlPoint(ctx, p) {
    ctx.arc(p.x, p.y, 3, 0, 2 * Math.PI);
    ctx.closePath();
    ctx.fillStyle = CONTROL_POINT_COLOR;
    ctx.fill();
}

function drawControlPoints(ctx, ...points) {
    for (const p of points) {
        drawControlPoint(ctx, p);
    }

    ctx.beginPath();
    points.reduce((prev, curr) => {
        if (prev) {
            ctx.moveTo(prev.x, prev.y);
            ctx.lineTo(curr.x, curr.y);
        }
        return curr;
    }, null);
    ctx.closePath();
    ctx.strokeStyle = CONTROL_POINT_COLOR;
    ctx.stroke();
}

function* range(begin, end, step=1) {
    for (let i = begin; i < end; i += step) {
        yield i;
    }
}

function drawBezierCurvePiece(ctx, [ p0, p1, p2 ], step=1e-3) {
    Array.from(range(0, 1, step)).reduce((prev, t) => {
        const curr = interpolateBezier(p0, p1, p2, t);
        if (prev) {
            ctx.moveTo(prev.x, prev.y);
            ctx.lineTo(curr.x, curr.y);
        }
        return curr;
    })
}

function drawBezierCurve(ctx, ...points) {
    ctx.beginPath();
    for (const i of range(0, points.length, 3)) {
        drawBezierCurvePiece(ctx, points.slice(i, i + 3));
    }
    ctx.closePath();
    ctx.strokeStyle = CURVE_COLOR;
    ctx.stroke();
}


drawBezierCurve(ctx, p0, p1, p2, p2_, p3, p4);
drawControlPoints(ctx, p0, p1, p2, p2_, p3, p4);