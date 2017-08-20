var Vector = require('./vector');
var Ray = require('./ray');
var Utils = require('./utils');


const WORLD_COLOR = new Vector(33 / 255, 33 / 255, 33 / 255);


function Renderer(drawable, scene, options) {
  this.drawable = drawable;
  this.scene = scene;
  this.ctx = drawable.getContext('2d');
  if (!options) {
    options = {};
  }
  this.pathDepth = options.pathDepth || 7;
  this.numSamples = options.numSamples || 1;
  this.numShadowSamples = options.numShadowSamples || 1;
  this.shadowRadius = options.shadowRadius || 0.8;
  this.renderTarget = null;
}


/**
 * Render given scene.
 * @param {Scene} scene
 */
Renderer.prototype.render = function() {
  this.initializeRenderTarget();
  this.initializeCamera();

  for (var y = 0; y < this.renderTarget.height; y++) {
    for (var x = 0; x < this.renderTarget.width; x++) {
      var color = this.samplePixel(x, y);
      this.putPixel(x, y, color);
    }
  }

  this.present();
};


/**
 * Sample given pixel.
 * @param {Scene} scene
 * @param {Number} x
 * @param {Number} y
 * @returns {Number}
 */
Renderer.prototype.samplePixel = function(x, y) {
  var resultColor = new Vector(0, 0, 0),
    deltaX = 0.5,
    deltaY = 0.5;

  for (var i = 0; i < this.numSamples; i++) {
    var ray = this.scene.camera.getRay(
      x + Math.random() - deltaX,
      y + Math.random() - deltaY);
    var color = this.traceRay(ray, this.pathDepth);
    resultColor.x += color.x;
    resultColor.y += color.y;
    resultColor.z += color.z;
  }

  resultColor.x = Math.min(255, 255 * resultColor.x / this.numSamples);
  resultColor.y = Math.min(255, 255 * resultColor.y / this.numSamples);
  resultColor.z = Math.min(255, 255 * resultColor.z / this.numSamples);

  return resultColor;
};


/**
 * Put pixel
 * @param {Number} x
 * @param {Number} y
 * @param {Vector} color
 */
Renderer.prototype.putPixel = function(x, y, color) {
  var index = (y * this.renderTarget.width + x) * 4;
  this.renderTarget.buffer.data[index + 0] = color.x;
  this.renderTarget.buffer.data[index + 1] = color.y;
  this.renderTarget.buffer.data[index + 2] = color.z;
  this.renderTarget.buffer.data[index + 3] = 255;
};


/**
 * Initialize render target.
 */
Renderer.prototype.initializeRenderTarget = function() {
  // create data buffer
  var buffer = this.ctx.createImageData(
    this.drawable.width,
    this.drawable.height);

  this.renderTarget = {
    buffer: buffer,
    width: this.drawable.width,
    height: this.drawable.height
  };
};


/**
 * Initialize camera.
 */
Renderer.prototype.initializeCamera = function() {
  var width = this.renderTarget.width;
  var height = this.renderTarget.height;
  this.scene.camera.setViewport(width, height);
};


/**
 * Present rendered scene
 */
Renderer.prototype.present = function() {
  var buffer = this.renderTarget.buffer;
  this.ctx.clearRect(0, 0, this.drawable.width, this.drawable.height);
  this.ctx.putImageData(buffer, 0, 0);
};


/**
 * Shading
 */

Renderer.prototype.shadowWeight = function (light, meta) {
  var shadowWeight = 0;
  for (var k = 0; k < this.numShadowSamples; k++) {
    var lightPos = new Vector(
      light.position.x + (2 * Math.random() - 1) * this.shadowRadius,
      light.position.y + (2 * Math.random() - 1) * this.shadowRadius,
      light.position.z + (2 * Math.random() - 1) * this.shadowRadius
    );
    var toLight = lightPos.sub(meta.info.position);
    var deltaPos = meta.info.position.add(toLight.mul(0.001));
    var occlusion = this.scene.intersect(
      new Ray(deltaPos, toLight),
      null,
      toLight.length);
    var delta = 1;
    if (occlusion.object && occlusion.object.material.dialectric) {
      delta = 0.8;
    } else if (occlusion.object) {
      delta = 0;
    }
    shadowWeight += delta;
  }
  return shadowWeight / this.numShadowSamples;
};

Renderer.prototype.shadow = function(ray, depth, meta) {
  var result = new Vector(0, 0, 0);
  var hit = meta.info;
  var object = meta.object;
  var totalWeight = 0;

  for (var i = 0; i < this.scene.lights.length; i++) {
    var light = this.scene.lights[i];
    var w = this.shadowWeight(light, meta);
    totalWeight += w;
    if (w == 0) continue;

    var lightContrib = light.shade(object, hit, this.scene.camera);
    result.x += w * lightContrib.x;
    result.y += w * lightContrib.y;
    result.z += w * lightContrib.z;
  }
  return result;
};


/**
 * Reflections and refractions
 */
Renderer.prototype.reflectRefract = function(ray, depth, meta) {
  var position = meta.info.position;
  var normal = meta.info.normal;
  var rayDir = ray.direction;
  var reflectDir = rayDir.reflect(normal);

  var Ct;
  var cos;
  var n;
  var t;

  // reflect ray
  var Cr = this.traceRay(
    new Ray(position, reflectDir, ray.refractionIndex),
    depth - 1,
    meta.object);

  // refract ray

  if (rayDir.dot(normal) < 0) {
    n = meta.object.material.refractionIndex;
    if (rayDir.isRefracted(1, n, normal)) {
      t = rayDir.refract(1, n, normal);
      Ct = this.traceRay(
        new Ray(position.add(t.mul(0.001)), t, n),
        depth - 1,
        null);
      cos = -rayDir.dot(normal);
    } else {
      return Cr;
    }
  } else {
    // outgoing
    n = ray.refractionIndex;
    var invNormal = normal.mul(-1.0);
    if (rayDir.isRefracted(n, 1, invNormal)) {
      t = rayDir.refract(n, 1, invNormal);
      Ct = this.traceRay(
        new Ray(position.add(t.mul(0.001)), t, 1),
        depth - 1,
        null);
      cos = t.dot(normal);
    } else {
      return Cr;
    }
  }

  var R0 = Math.pow(n - 1, 2) / Math.pow(n + 1, 2);
  var R = R0 + (1 - R0) * Math.pow(1 - cos, 5);
  return new Vector(
    R * Cr.x + (1 - R) * Ct.x,
    R * Cr.y + (1 - R) * Ct.y,
    R * Cr.z + (1 - R) * Ct.z
  );
};


Renderer.prototype.traceRay = function traceRay(ray, depth, excluded = null) {
  // find closest object that intersects the ray
  var meta = this.scene.intersect(ray, excluded);
  if (!meta.object) {
    return WORLD_COLOR;
  }

  var material = meta.object.material;
  var color = this.shadow(ray, depth, meta);

  color.x *= material.color.x;
  color.y *= material.color.y;
  color.z *= material.color.z;

  // reflection and refraction
  if (material.dialectric && depth > 1) {
    var reflected = this.reflectRefract(ray, depth, meta);
    color.x += reflected.x;
    color.y += reflected.y;
    color.z += reflected.z;
  }

  return color;
}


module.exports = Renderer;
