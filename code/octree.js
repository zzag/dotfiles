/**
 * Axis-aligned bounded box.
 *
 * @param origin Origin of an AABB
 * @param extents Size of the AABB
 */
function AABB(origin, extents) {
  this.origin = origin;
  this.extents = extents;
}


AABB.fromTriangle = function(a, b, c) {
  var lower = {
    x: Math.min(a.x, Math.min(b.x, c.x)),
    y: Math.min(a.y, Math.min(b.y, c.y)),
    z: Math.min(a.z, Math.min(b.z, c.z))
  };
  var upper = {
    x: Math.max(a.x, Math.max(b.x, c.x)),
    y: Math.max(a.y, Math.max(b.y, c.y)),
    z: Math.max(a.z, Math.max(b.z, c.z))
  };
  return new AABB({
    x: (upper.x + lower.x) / 2,
    y: (upper.y + lower.y) / 2,
    z: (upper.z + lower.z) / 2
  }, {
    x: upper.x - lower.x,
    y: upper.y - lower.y,
    z: upper.z - lower.z
  });
}


/**
 * Check whether two AABBs are intersecting.
 *
 * @param other Another AABB
 */
AABB.prototype.intersect = function(other) {
  var deltaX = Math.abs(this.origin.x - other.origin.x),
    deltaY = Math.abs(this.origin.y - other.origin.y),
    deltaZ = Math.abs(this.origin.z - other.origin.z);
  return deltaX <= (this.extents.x + other.extents.x) / 2 &&
    deltaY <= (this.extents.y + other.extents.y) / 2 &&
    deltaZ <= (this.extents.z + other.extents.z) / 2;
};


function Octree(aabb, depth = 0) {
  this.leaf = true;
  this.aabb = aabb;
  this.depth = depth;
  this.objects = [];
  this.children = [];
}


Octree.prototype.maxDepth = 15;
Octree.prototype.maxNodeCapacity = 3;


Octree.prototype.insert = function(object) {
  if (!this.aabb.intersect(object)) {
    return;
  }

  if (this.depth > this.maxDepth) {
    this.objects.push(object);
    return;
  }

  if (this.leaf && this.objects.length < this.maxNodeCapacity) {
    this.objects.push(object);
    return;
  }

  if (this.leaf) {
    this.split();
  }

  this.insertToChild(object);
};


Octree.prototype.insertToChild = function(object) {
  var isSeparating = false;
  var child = null;

  for (var i = 0; i < 8; i++) {
    if (this.children[i].aabb.intersect(object)) {
      if (child != null) {
        isSeparating = true;
      }
      child = this.children[i];
    }
  }

  if (isSeparating) {
    this.objects.push(object);
  } else {
    child.insert(object);
  }
};


Octree.prototype.split = function() {
  var extents = this.aabb.extents;
  var origin = this.aabb.origin;
  var childExtents = {
    x: extents.x / 2,
    y: extents.y / 2,
    z: extents.z / 2
  };
  var deltaOrigin = {
    x: childExtents.x / 2,
    y: childExtents.y / 2,
    z: childExtents.z / 2
  };

  this.leaf = false;

  // split AABB
  this.children = new Array(8);
  this.children[0] = new Octree(
    new AABB({
      x: origin.x - deltaOrigin.x,
      y: origin.y - deltaOrigin.y,
      z: origin.z - deltaOrigin.z
    }, childExtents), this.depth + 1);
  this.children[1] = new Octree(
    new AABB({
      x: origin.x + deltaOrigin.x,
      y: origin.y - deltaOrigin.y,
      z: origin.z - deltaOrigin.z
    }, childExtents), this.depth + 1);
  this.children[2] = new Octree(
    new AABB({
      x: origin.x - deltaOrigin.x,
      y: origin.y + deltaOrigin.y,
      z: origin.z - deltaOrigin.z
    }, childExtents), this.depth + 1);
  this.children[3] = new Octree(
    new AABB({
      x: origin.x + deltaOrigin.x,
      y: origin.y + deltaOrigin.y,
      z: origin.z - deltaOrigin.z
    }, childExtents), this.depth + 1);
  this.children[4] = new Octree(
    new AABB({
      x: origin.x - deltaOrigin.x,
      y: origin.y - deltaOrigin.y,
      z: origin.z + deltaOrigin.z
    }, childExtents), this.depth + 1);
  this.children[5] = new Octree(
    new AABB({
      x: origin.x + deltaOrigin.x,
      y: origin.y - deltaOrigin.y,
      z: origin.z + deltaOrigin.z
    }, childExtents), this.depth + 1);
  this.children[6] = new Octree(
    new AABB({
      x: origin.x - deltaOrigin.x,
      y: origin.y + deltaOrigin.y,
      z: origin.z + deltaOrigin.z
    }, childExtents), this.depth + 1);
  this.children[7] = new Octree(
    new AABB({
      x: origin.x + deltaOrigin.x,
      y: origin.y + deltaOrigin.y,
      z: origin.z + deltaOrigin.z
    }, childExtents), this.depth + 1);

  // split objects
  var objects = this.objects;
  this.objects = [];

  while (objects.length > 0) {
    var object = objects.pop();
    this.insertToChild(object);
  }
};
