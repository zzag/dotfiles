/**
 * Scene object.
 * @param {Geometry} geometry
 * @param {Material} material
 */
function SceneObject(geometry, material) {
  this.geometry = geometry;
  this.material = material;
}


/**
 * Hit an object with the ray.
 * @param {Ray} ray
 */
SceneObject.prototype.hit = function(ray) {
  return this.geometry.hit(ray);
};


module.exports = SceneObject;
