function Vector(x, y, z) {
  this.x = x;
  this.y = y;
  this.z = z;
  this.length = Math.sqrt(x * x + y * y + z * z);
}


/**
 * Get dot product.
 * @param {Vector} that
 * @returns {Number}
 */
Vector.prototype.dot = function(that) {
  return this.x * that.x + this.y * that.y + this.z * that.z;
};


/**
 * Get cross product.
 * @param  {Vector} that
 * @returns {Vector}
 */
Vector.prototype.cross = function(that) {
  return new Vector(
    this.y * that.z - this.z * that.y,
    this.x * that.z - this.z * that.x,
    this.x * that.y - this.y * that.x
  );
};


/**
 * Add two vectors.
 * @param {Number|Vector} that
 * @returns {Vector}
 */
Vector.prototype.add = function(that) {
  if (that instanceof Vector) {
    return new Vector(this.x + that.x, this.y + that.y, this.z + that.z);
  }
  return new Vector(this.x + that, this.y + that, this.z + that);
};


/**
 * Subtract two vectors.
 * @param {Number|Vector} that
 * @returns {Vector}
 */
Vector.prototype.sub = function(that) {
  if (that instanceof Vector) {
    return new Vector(this.x - that.x, this.y - that.y, this.z - that.z);
  }
  return new Vector(this.x - that, this.y - that, this.z - that);
};


/**
 * Multiply two vectors.
 * @param {Number|Vector} that
 * @returns {Vector}
 */
Vector.prototype.mul = function(that) {
  if (that instanceof Vector) {
    return new Vector(this.x * that.x, this.y * that.y, this.z * that.z);
  }
  return new Vector(this.x * that, this.y * that, this.z * that);
};


/**
 * Divide two vectors.
 * @param {Number|Vector} that
 * @returns {Vector}
 */
Vector.prototype.div = function(that) {
  if (that instanceof Vector) {
    return new Vector(this.x / that.x, this.y / that.y, this.z / that.z);
  }
  return new Vector(this.x / that, this.y / that, this.z / that);
};


/**
 * Get normalized vector.
 * @returns {Vector} Normalized vector
 */
Vector.prototype.normalized = function() {
  return new Vector(
    this.x / this.length,
    this.y / this.length,
    this.z / this.length);
};


/**
 * Reflect the vector given the normal.
 * @param {Vector} normal
 * @returns {Vector} Reflected vector.
 */
Vector.prototype.reflect = function(normal) {
  var angle = -this.dot(normal) / Math.pow(normal.length, 2);
  var reflected = new Vector(
    this.x + 2.0 * angle * normal.x,
    this.y + 2.0 * angle * normal.y,
    this.z + 2.0 * angle * normal.z);
  return reflected;
};


/**
 * Is the vector refracted.
 * @param {Number} n1
 * @param {Number} n2
 * @param {Vector} normal
 * @returns {Boolean}
 */
Vector.prototype.isRefracted = function(n1, n2, normal) {
  if (n1 < n2) {
    return true;
  }

  var criticalAngle = Math.asin(n2 / n1);
  var angle = Math.acos(-this.dot(normal) / (this.length * normal.length));
  return angle < criticalAngle;
};


/**
 * Refract the vector.
 * @param {Number} n1
 * @param {Number} n2
 * @param {Number} normal
 * @returns {Vector}
 */
Vector.prototype.refract = function(n1, n2, normal) {
  var k = n1 / n2;
  var dn = this.dot(normal);
  var tSq = Math.pow(this.length, 2);
  var nSq = Math.pow(normal.length, 2);
  var m = k*dn/nSq + Math.sqrt(tSq - k*k*(tSq-dn*dn/nSq))/normal.length;
  var refracted = new Vector(
    k * this.x - m * normal.x,
    k * this.y - m * normal.y,
    k * this.z - m * normal.z
  );
  return refracted;
};


/**
 * Get distance between two points
 * @param {Vector} that
 * @returns {Number}
 */
Vector.prototype.distanceTo = function(that) {
  return Math.sqrt(
    Math.pow(this.x - that.x, 2) +
    Math.pow(this.y - that.y, 2) +
    Math.pow(this.z - that.z, 2));
};


Vector.prototype.toString = function() {
  return "Vector(" + this.x + ", " + this.y + ", " + this.z + ")";
};


module.exports = Vector;
