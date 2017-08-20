var Vector = require('./vector');

/**
 * Sphere geometry.
 * @param {Vector} center
 * @param {Number} radius
 */
function SphereGeometry(center, radius) {
  this.center = center;
  this.radius = radius;
}


/**
 * Hit the sphere with a ray.
 * @param {Ray} ray
 */
SphereGeometry.prototype.hit = function (ray) {
  var dist = ray.pos.sub(this.center);
  var a = ray.direction.dot(ray.direction),
      b = 2 * ray.direction.dot(dist),
      c = dist.dot(dist) - Math.pow(this.radius, 2);
  var D = Math.pow(b, 2) - 4 * a * c;
  var de = ray.direction.sub(new Vector(0, 0, -1));
  if (D < 0) return null;

  var t1 = (-b - Math.sqrt(D)) / (2 * a),
      t2 = (-b + Math.sqrt(D)) / (2 * a);
  if (t1 < 0 && t2 < 0) return null;
  var t = Math.min(t1, t2);
  if (t < 0) t = Math.max(t1, t2);

  var pos = ray.pos.add(ray.direction.mul(t));
  var normal = pos.sub(this.center).normalized();
  return {
    position: pos,
    normal: normal,
    reflection: ray.direction.reflect(normal)
  };
};


module.exports = SphereGeometry;
