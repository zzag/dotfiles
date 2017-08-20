var Vector = require('./vector');


function PointLight(position, options) {
  this.position = position;
  if (!options) {
    options = {};
  }
  this.energy = options.energy || 1.0;
  this.color = options.color || new Vector(1.0, 1.0, 1.0);
  this.ka = options.ka || 0.1;
  this.kd = options.kd || 0.1;
  this.ks = options.ks || 0.1;
  this.specular = options.specular || 10;
  this.a = options.a || 1;
  this.b = options.b || 1;
  this.c = options.c || 1;
}


PointLight.prototype.shade = function (object, hit, eye) {
  var normal = hit.normal,
      eyePos = eye.position;
  var toLight = this.position.sub(hit.position);
  var lightVector = toLight.normalized();
  var viewVector = eyePos.sub(hit.position).normalized();
  var halfVector = viewVector.add(lightVector).normalized();

  var diffuse = this.kd * Math.max(0, normal.dot(lightVector));
  var specular = this.ks * Math.pow(Math.max(0, normal.dot(halfVector)), this.specular);
  var d = toLight.length;
  return this.color.mul((diffuse + specular)/(this.c + this.b * d + this.a * d * d));
};


module.exports = PointLight;
