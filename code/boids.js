function Boids(container, totalBoids, opts) {
  opts = opts || {};

  this.container = container;
  this.ctx = container.getContext("2d");

  this.cohesionDistance = Math.pow(opts.cohesionDistance || 40, 2);
  this.separationDistance = Math.pow(opts.separationDistance || 90, 2);
  this.alignmentDistance = Math.pow(opts.alignmentDistance || 40, 2);
  this.cohesionForce = opts.cohesionForce || 0.05;
  this.separationForce = opts.separationForce || 0.1;
  this.alignmentForce = opts.alignmentForce || 0.25;
  this.accelerationLimit = Math.pow(opts.accelerationLimit || 1, 2);
  this.speedLimit = Math.pow(opts.speedLimit || 1.414, 2);
  this.boidRadius = opts.boidRadius || 2;
  this.padding = opts.padding || 0;
  this.pushForce = opts.pushForce || 20;
  this.boidColor = opts.boidColor || "rgba(200, 20, 20, 0.8)";
  this.bgColor = opts.bgColor || "rgba(255, 255, 255, 0.2)";
  this.boids = [];

  for (var i = 0; i < totalBoids; i++) {
    var boid = {
      pos: { 
        x: this.container.width * Math.random(),
        y: this.container.height * Math.random()
      },
      velocity: { 
        x: Math.random()*2 - 1,
        y: Math.random()*2 - 1
      },
      acceleration: { x: 0, y: 0 }
    };
    this.boids.push(boid);
  }
}


Boids.prototype.update = function () {
  var ctx = this.ctx,
      width = this.container.width,
      height = this.container.height,
      i = 0,
      j = 0,
      boids = this.boids,
      boid = null,
      n = this.boids.length,
      twoPi = 2 * Math.PI,
      norm = 0;

  // render boids
  ctx.fillStyle = this.bgColor;
  ctx.fillRect(0, 0, width, height);
  for (i = 0; i < n; i++) {
    ctx.beginPath();
    ctx.fillStyle = this.boidColor;
    ctx.arc(boids[i].pos.x, boids[i].pos.y, this.boidRadius, 0, twoPi);
    ctx.fill();
    ctx.closePath();
  }

  // apply some forces
  for (i = 0; i < n; i++) {
    var current = boids[i];
    var separation = { x: 0, y: 0 };
    var cohesion = { x: 0, y: 0 };
    var alignment = { x: 0, y: 0 };
    var separationCount = 0;
    var cohesionCount = 0;
    var alignmentCount = 0;

    for (j = 0; j < n; j++) {
      if (i == j) continue;
      boid = boids[j];
      var dx = boid.pos.x - current.pos.x;
      var dy = boid.pos.y - current.pos.y;
      var d = dx*dx + dy*dy;

      if (d < this.separationDistance) {
        separation.x += -dx;
        separation.y += -dy;
        separationCount++;
      }
      if (d < this.cohesionDistance) {
        cohesion.x += dx;
        cohesion.y += dy;
        cohesionCount++;
      }
      if (d < this.alignmentDistance) {
        alignment.x += boid.velocity.x;
        alignment.y += boid.velocity.y;
        alignmentCount++;
      }
    }

    separation.x = separation.x / separationCount || 0;
    separation.y = separation.y / separationCount || 0;
    cohesion.x = cohesion.x / cohesionCount || 0;
    cohesion.y = cohesion.y / cohesionCount || 0;
    alignment.x = alignment.x / alignmentCount || 0;
    alignment.y = alignment.y / alignmentCount || 0;

    norm = Math.sqrt(separation.x*separation.x + separation.y*separation.y);
    current.acceleration.x += (this.separationForce * separation.x / norm) || 0;
    current.acceleration.y += (this.separationForce * separation.y / norm) || 0;

    norm = Math.sqrt(cohesion.x*cohesion.x + cohesion.y*cohesion.y);
    current.acceleration.x += (this.cohesionForce * cohesion.x / norm) || 0;
    current.acceleration.y += (this.cohesionForce * cohesion.y / norm) || 0;

    norm = Math.sqrt(alignment.x*alignment.x + alignment.y*alignment.y);
    current.acceleration.x += (this.alignmentForce * alignment.x / norm) || 0;
    current.acceleration.y += (this.alignmentForce * alignment.y / norm) || 0;
  }

  for (i = 0; i < n; i++) {
    boid = boids[i];

    if (boid.pos.x < this.padding) boid.acceleration.x = this.pushForce;
    else if (boid.pos.x > width - this.padding) boid.acceleration.x = -this.pushForce;
    if (boid.pos.y < this.padding) boid.acceleration.y = this.pushForce;
    else if (boid.pos.y > height - this.padding) boid.acceleration.y = -this.pushForce;

    norm = boid.acceleration.x*boid.acceleration.x + boid.acceleration.y*boid.acceleration.y;
    if (norm > this.accelerationLimit) {
      norm = Math.sqrt(norm);
      boid.acceleration.x = boid.acceleration.x * this.accelerationLimit / norm;
      boid.acceleration.y = boid.acceleration.y * this.accelerationLimit / norm;
    }
    boid.velocity.x += boid.acceleration.x;
    boid.velocity.y += boid.acceleration.y;

    norm = boid.velocity.x*boid.velocity.x + boid.velocity.y*boid.velocity.y;
    if (norm > this.speedLimit) {
      norm = Math.sqrt(norm);
      boid.velocity.x = boid.velocity.x * this.speedLimit / norm;
      boid.velocity.y = boid.velocity.y * this.speedLimit / norm;
    }
    boid.pos.x += boid.velocity.x;
    boid.pos.y += boid.velocity.y;
  }
}
