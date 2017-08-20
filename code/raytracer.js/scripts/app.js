var Vector = require('./vector');
var Ray = require('./ray');
var SphereGeometry = require('./sphere_geometry');
var PlaneGeometry = require('./plane_geometry');
var SceneObject = require('./object');
var Material = require('./material');
var Renderer = require('./renderer');
var Camera = require('./camera');
var Scene = require('./scene');
var PointLight = require('./point_light');


function buildScene() {
  var scene = new Scene();
  var geometry;
  var material;

  // ground
  geometry = new PlaneGeometry(
    new Vector(0.0, -0.5, 0.0),
    new Vector(0, 1, 0),
    2, 1);
  material = new Material({
    color: new Vector(0.7, 0.7, 0.7),
    dialectric: false
  });
  scene.objects.push(new SceneObject(geometry, material));

  // back wall
  geometry = new PlaneGeometry(
    new Vector(0.0, 0.0, -10.0),
    new Vector(0, 0, 1),
    2, 1);
  material = new Material({
    color: new Vector(0.3, 0.3, 0.3),
    dialectric: false
  });
  scene.objects.push(new SceneObject(geometry, material));

  // side wall
  geometry = new PlaneGeometry(
    new Vector(-5.0, 0.0, 0.0),
    new Vector(1, 0, 0),
    2, 1);
  material = new Material({
    color: new Vector(0.7, 0.0, 0.0),
    dialectric: false
  });
  scene.objects.push(new SceneObject(geometry, material));

  geometry = new SphereGeometry(
    new Vector(0.0, 1.0, -2.0),
    1.5);
  material = new Material({
    color: new Vector(0.0, 0.0, 0.01),
    dialectric: true,
    refractionIndex: 1.33
  });
  scene.objects.push(new SceneObject(geometry, material));

  geometry = new SphereGeometry(
    new Vector(3.0, 1.0, -4.0),
    1.0);
  material = new Material({
    color: new Vector(0.07, 0.0, 0.0),
    dialectric: true,
    refractionIndex: 2.42
  });
  scene.objects.push(new SceneObject(geometry, material));

  geometry = new SphereGeometry(
    new Vector(5.0, 0.0, -1.0),
    0.5);
  material = new Material({
    color: new Vector(0.5, 0.0, 0.0),
    dialectric: false,
    refractionIndex: 2.42
  });
  scene.objects.push(new SceneObject(geometry, material));

  geometry = new SphereGeometry(
    new Vector(5.4, 0.3, 1.0),
    0.5);
  material = new Material({
    color: new Vector(0.0, 0.0, 0.0),
    dialectric: true,
    refractionIndex: 1.33
  });
  scene.objects.push(new SceneObject(geometry, material));

  scene.lights.push(new PointLight(new Vector(4, 7, 1), {
    ka: 0.4,
    kd: 2.0,
    ks: 0.3,
    specular: 60,
    a: 0.01,
    b: 0.4,
    color: new Vector(1, 1, 1)
  }));

  scene.lights.push(new PointLight(new Vector(0, 7, 2), {
    ka: 0.4,
    kd: 2.0,
    ks: 3.0,
    specular: 30,
    a: 0.01,
    b: 0.4,
    color: new Vector(1, 1, 1)
  }));

  // camera
  scene.setCamera(new Camera(cameraPitch, cameraYaw, cameraPos));

  return scene;
}

var numSamples = 16;
var numShadowSamples = 8;
var pathDepth = 5;
var cameraPitch = -10;
var cameraYaw = 10;
var cameraPos = new Vector(4.0, 2.0, 7.0);
var scene = buildScene();
var renderer = new Renderer(document.getElementById('canvas'), scene, {
  numSamples: numSamples,
  numShadowSamples: numShadowSamples,
  pathDepth: pathDepth,
  shadowRadius: 1.5
});
renderer.render();


console.log('Num. objects: ' + scene.objects.length);
console.log('Num. lights: ' + scene.lights.length);
console.log('Num. samples: ' + numSamples);
console.log('Num. shadow samples: ' + numShadowSamples);
console.log('Path depth: ' + pathDepth);
console.log('Camera pitch: ' + cameraPitch);
console.log('Camera yaw: ' + cameraYaw);
console.log('Camera pos: ' + cameraPos);
