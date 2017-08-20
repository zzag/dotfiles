function Scene() {
  this.objects = [];
  this.lights = [];
  this.camera = null;
}


/**
 * Find closest object that intersects the ray.
 * @param {Ray} ray
 * @param {SceneObject} excluded
 * @param {Number} maxDistance
 */
Scene.prototype.intersect = function (ray, excluded=null, maxDistance=Infinity) {
  var intersectObj = {
    distance: Infinity,
    object: null,
    info: null
  };

  for (var i = 0; i < this.objects.length; i++) {
    if (this.objects[i] == excluded) continue;

    var info = this.objects[i].hit(ray);
    if (!info) continue;
    var distance = info.position.distanceTo(ray.pos);
    if (distance < intersectObj.distance && distance < maxDistance) {
      intersectObj.distance = distance;
      intersectObj.object = this.objects[i];
      intersectObj.info = info;
    }
  }

  return intersectObj;
};


/**
 * Get camera
 * @returns Camera
 */
Scene.prototype.getCamera = function () {
  return this.camera;
};


/**
 * Set camera.
 * @param {Camera} camera
 */
Scene.prototype.setCamera = function (camera) {
  this.camera = camera;
};


module.exports = Scene;
