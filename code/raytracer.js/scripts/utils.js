/**
 * Convert degrees to radians.
 * @param {Number} degree
 * @returns {Number}
 */
function toRad(degree) {
  return degree * Math.PI / 180.0;
}


/**
 * Convert radians to degrees.
 * @param {Number} radians
 * @returns {Number}
 */
function toDegree(radians) {
  return radians * 180.0 / Math.PI;
}


/**
 * Get euclidean distance between two vectors.
 * @param {Vector} v1
 * @param {Vector} v2
 * @returns {Number}
 */
function euclideanDistance(v1, v2) {
  return v1.sub(v2).length;
}


/**
 * Get Manhattan distance between two vectors.
 * @param {Vector} v1
 * @param {Vector} v2
 * @returns {Number}
 */
function manhattanDistance(v1, v2) {
  var diff = v1.sub(v2);
  return Math.abs(diff.x) + Math.abs(diff.y) + Math.abs(diff.z);
}


module.exports = {
  toRad: toRad,
  toDegree: toDegree,
  euclideanDistance: euclideanDistance,
  manhattanDistance: manhattanDistance
};
