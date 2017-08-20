var Ray = require('./ray');
var Vector = require('./vector');
var Utils = require('./utils');


/**
 * FPS camera.
 * NOTE: view plane is placed at z=-1.0
 * @param {Number} pitch
 * @param {Number} yaw
 * @param {Vector} position
 */
function Camera(pitch, yaw, position, options) {
  this.pitch = Math.PI * pitch / 180;
  this.yaw = Math.PI * yaw / 180;
  this.position = position;
  this.viewport = {
    width: 0,
    height: 0
  };
  if (!options) {
    options = {};
  }
  this.fov = Math.tan(Utils.toRad(options.fov || 45.0) / 2.0);
}


/**
 * Update viewport dimensions.
 * @param {Number} width
 * @param {Number} height
 */
Camera.prototype.setViewport = function(width, height) {
  this.viewport.width = width;
  this.viewport.height = height;
};


/**
 * Get coordinates of (x, y) in viewport given
 * coordinates of (x, y) in the screen space.
 * @param {Number} i
 * @param {Number} j
 * @returns {Ray}
 */
Camera.prototype.getRay = function(x, y) {
  var position = this.position;
  var direction = this.transform(this.toViewport(x, y));
  return new Ray(position, direction);
};


/**
 * Convert from screen space to viewport space.
 * @param {Number} screenX
 * @param {Number} screenY
 * @returns {Vector}
 */
Camera.prototype.toViewport = function(screenX, screenY) {
  var width = this.viewport.width,
    height = this.viewport.height,
    aspectRatio = width / height,
    fov = this.fov;
  return new Vector(
    aspectRatio * fov * (2 * screenX / width - 1),
    fov * (1 - 2 * screenY / height),
    -1.0);
};


/**
 * Transform given vector.
 * @param {Vector} v
 * @returns {Vector}
 */
Camera.prototype.transform = function(v) {
  var pitch = this.pitch,
      yaw = this.yaw;
  var xAxis = new Vector(
      Math.cos(yaw),
      0,
      -Math.sin(yaw)),
    yAxis = new Vector(
      Math.sin(yaw) * Math.sin(pitch),
      Math.cos(pitch),
      Math.cos(yaw) * Math.sin(pitch)),
    zAxis = new Vector(
      Math.sin(yaw) * Math.cos(pitch),
      -Math.sin(pitch),
      Math.cos(pitch) * Math.cos(yaw));

  var transformed = new Vector(
    xAxis.x * v.x + yAxis.x * v.y + zAxis.x * v.z,
    xAxis.y * v.x + yAxis.y * v.y + zAxis.y * v.z,
    xAxis.z * v.x + yAxis.z * v.y + zAxis.z * v.z
  );

  return transformed;
};


module.exports = Camera;
