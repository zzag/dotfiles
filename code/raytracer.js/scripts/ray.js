/**
 * @param {Vector} pos
 * @param {Vector} direction
 */
function Ray(pos, direction, refractionIndex = 1.0) {
  this.pos = pos;
  this.direction = direction.normalized();
  this.refractionIndex = refractionIndex;
}


module.exports = Ray;
