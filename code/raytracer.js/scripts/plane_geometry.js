var Utils = require('./utils');


/**
 * Plane geometry.
 * @param {Vector} origin
 * @param {Vector} normal
 * @param {Number} width
 * @param {Number} height
 */
function PlaneGeometry(origin, normal, width=1, height=1) {
  this.origin = origin;
  this.normal = normal.normalized();
  this.extents = {
    width: width,
    height: height
  };
}


/**
 * Intersect plane
 * @param {Ray} ray
 */
PlaneGeometry.prototype.hit = function (ray) {
  var t = intersectPlane(this, ray);
  if (t < 0) {
    return null;
  }

  var position = ray.pos.add(ray.direction.mul(t));

  return {
    position: position,
    normal: this.normal,
    reflection: ray.direction.reflect(this.normal)
  };
};


function intersectPlane(plane, ray) {
  var d = -plane.normal.dot(ray.direction);
  if (d <= 0) return -1.0;
  var n = ray.pos.sub(plane.origin).dot(plane.normal);
  var t = n / d;
  return t;
}


module.exports = PlaneGeometry;
