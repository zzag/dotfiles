function Point(x, y) {
  this.x = x;
  this.y = y;
}


Point.randomPoint = function(minX, maxX, minY, maxY) {
  return new Point(
    Math.random() * (maxX - minX) + minX,
    Math.random() * (maxY - minY) + minY);
}


function Box(center, halfWidth, halfHeight) {
  this.center = center;
  this.halfWidth = halfWidth;
  this.halfHeight = halfHeight;
}


Box.prototype.intersect = function(other) {
  var deltaX = Math.abs(this.center.x - other.center.x),
    deltaY = Math.abs(this.center.y - other.center.y);
  return deltaX < this.halfWidth + other.halfWidth,
    deltaY < this.halfHeight + other.halfHeight;
};


Box.prototype.inside = function(point) {
  var deltaX = Math.abs(this.center.x - point.x),
    deltaY = Math.abs(this.center.y - point.y);
  return deltaX <= this.halfWidth && deltaY <= this.halfHeight;
};


function QuadTree(box) {
  this.leaf = true;
  this.box = box;
  this.points = [];

  this.children = [];
}


QuadTree.prototype.insert = function(point) {
  if (!this.box.inside(point)) {
    throw new Error('Given point is outside of the bounding box');
  }

  if (this.leaf && this.points.length < 1) {
    this.points.push(point);
    return;
  }

  if (this.leaf) {
    this.split();
  }

  var quadrant = this.getQuadrant(point);
  quadrant.insert(point);
};


QuadTree.prototype.split = function() {
  var childWidth = this.box.halfWidth / 2,
    childHeight = this.box.halfHeight / 2;
  var center = this.box.center;

  this.children[0] = new QuadTree(
    new Box(new Point(center.x - childWidth, center.y - childHeight),
      childWidth, childHeight));
  this.children[1] = new QuadTree(
    new Box(new Point(center.x + childWidth, center.y - childHeight),
      childWidth, childHeight));
  this.children[2] = new QuadTree(
    new Box(new Point(center.x + childWidth, center.y + childHeight),
      childWidth, childHeight));
  this.children[3] = new QuadTree(
    new Box(new Point(center.x - childWidth, center.y + childHeight),
      childWidth, childHeight));
  this.leaf = false;

  var points = this.points;
  while (points.length > 0) {
    var point = points.shift();
    var quadrant = this.getQuadrant(point);
    quadrant.insert(point);
  }
};


QuadTree.prototype.getQuadrant = function(point) {
  var center = this.box.center;
  var northwest = this.children[0],
    northeast = this.children[1],
    southeast = this.children[2],
    southwest = this.children[3];
  if (point.x < center.x) {
    return point.y < center.y ? northwest : southwest;
  } else {
    return point.y < center.y ? northeast : southeast;
  }
};


QuadTree.prototype.nearest = function(searchPoint, k, metric) {
  var queue = new BoundedQueue(k);

  function search(tree) {
    if (tree.leaf) {
      for (var i = 0; i < tree.points.length; i++) {
        var point = tree.points[i];
        var distance = metric(searchPoint, point);
        queue.put(distance, point);
      }
      return;
    }

    var pointQuadrant = tree.getQuadrant(searchPoint);
    search(pointQuadrant);

    var possibleQuadrants = [];
    if (queue.empty()) {
      // Queue is empty, so we search across all neighbors.
      possibleQuadrants = tree.children
        .filter(function(quadrant) {
          return quadrant != pointQuadrant;
        });
    } else {
      // Some quadrants can be overlapped by search box. If so then
      // search across them too.
      var searchRadius = queue.last().priority;
      var searchBox = new Box(searchPoint, searchRadius, searchRadius);
      possibleQuadrants = tree.children
        .filter(function(quadrant) {
          return quadrant != pointQuadrant &&
            quadrant.box.intersect(searchBox);
        })
    }

    for (var i = 0; i < possibleQuadrants.length; i++) {
      search(possibleQuadrants[i]);
    }
  }

  search(this);
  return queue.elements;
};


function BoundedQueue(capacity) {
  this.capacity = capacity;
  this.elements = [];
}


BoundedQueue.prototype.empty = function() {
  return this.elements.length == 0;
};


BoundedQueue.prototype.full = function() {
  return this.elements.length == this.capacity;
};


BoundedQueue.prototype.last = function() {
  return this.elements[this.elements.length - 1];
};


BoundedQueue.prototype.put = function(priority, value) {
  var elements = this.elements,
    capacity = this.capacity;

  if (elements.length < capacity) {
    // Insert a new value in the queue.
    elements.push({
      priority: priority,
      value: value
    });

    // Fix queue: priority of the new value can be
    //   lower than priority of last value in the queue.
    var i = elements.length - 1;
    while (i > 0 && elements[i - 1].priority > priority) {
      var tmp = elements[i];
      elements[i] = elements[i - 1];
      elements[i - 1] = tmp;
      i--;
    }
    return;
  }

  // If priority of last value is less than or equal to
  // priority of a new value then do nothing.
  var lastValue = elements[elements.length - 1];
  if (lastValue.priority <= priority) {
    return;
  }

  // Find place where to insert the new value.
  var i = elements.length - 1;
  while (i > 0 && elements[i - 1].priority > priority) {
    elements[i] = elements[i - 1];
    i--;
  }

  elements[i] = {
    priority: priority,
    value: value
  };
};
