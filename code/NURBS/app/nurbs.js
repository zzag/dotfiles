import { range, repeat, between } from './common';

export default class NURBS {

  constructor(options = {}) {
    this.points = options.points;
    this.degree = options.degree || 2;
    this.knots = options.knots ||
      range(0, this.points.length + this.degree + 1);
    this.weights = options.weights ||
      repeat(1.0, this.points.length);
    this.domain = {
      min: this.degree,
      max: this.points.length,
    };
  }

  computeBasis(t, i, d) {
    const knots = this.knots;

    if (d === 0) {
      return between(t, knots[i], knots[i + 1]) ? 1 : 0;
    }

    const w1 = (t - knots[i]) / (knots[i + d] - knots[i]);
    const w2 = (knots[i + d + 1] - t) / (knots[i + d + 1] - knots[i + 1]);
    const res = (w1 * this.computeBasis(t, i, d - 1)) +
      (w2 * this.computeBasis(t, i + 1, d - 1));

    return res;
  }

  mapToDomain(t) {
    const shift = this.domain.min;
    const scale = this.domain.max - this.domain.min;
    const x = (t * scale) + shift;
    return x;
  }

  findInterval(x) {
    const knots = this.knots;
    let l = this.domain.min;
    let r = this.domain.max;

    while ((r - l) > 1) {
      const m = Math.floor((l + r) / 2);
      if (knots[m] > x) { r = m; } else { l = m; }
    }

    return l;
  }

  interpolate(t) {
    const res = { x: 0, y: 0 };
    const x = this.mapToDomain(t);
    const l = this.findInterval(x);
    let weights = 0.0;

    for (let i = l - this.degree; i <= l; i += 1) {
      const p = this.points[i];
      const w = this.computeBasis(x, i, this.degree) * this.weights[i];
      res.x += w * p.x;
      res.y += w * p.y;
      weights += w;
    }

    res.x /= weights;
    res.y /= weights;

    return res;
  }
}
