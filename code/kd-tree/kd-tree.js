/**
 * kd-tree
 */
function KDTree() {
  this.queue = [];
  this.root = null;
}

KDTree.axes = ['x', 'y'];


/**
 * Insert new point.
 */
KDTree.prototype.insert = function(point) {
  this.queue.push(point);
};


/**
 * Balance kd-tree.
 */
KDTree.prototype.balance = function() {
  this.root = new KDNode(this.queue, 0);
  this.queue = [];
};


/**
 * Find k nearest points.
 */
KDTree.prototype.nearest = function(point, k, metric) {
  var neighbors = [];

  function put(point, distance) {
    neighbors.push({ point: point, distance: distance });
    neighbors = neighbors.sort(function(a, b) {
      return a.distance - b.distance;
    });
    if (neighbors.length > k) {
      neighbors.pop();
    }
  }

  function search(tree) {
    var axis = tree.axis;
    var current = tree.point;

    if (!current) {
      return;
    }

    put(current, metric(current, point));
    if (tree.leaf) {
      return;
    }

    var otherSubtree;
    if (point[axis] < current[axis]) {
      search(tree.left);
      otherSubtree = tree.right;
    } else {
      search(tree.right);
      otherSubtree = tree.left;
    }

    // search the other subtree
    var farthest = neighbors[neighbors.length - 1].distance;
    if (neighbors.length < k
        || Math.abs(current[axis] - point[axis]) < farthest) {
      search(otherSubtree);
    }
  }

  search(this.root);
  return neighbors;
};


/**
 * kd-tree node.
 *
 * @param {Array} data List of data points.
 * @param {Number} axis Axis in which divide the data points.
 */
function KDNode(data, axis = 0) {
  var axisName = axis ? KDTree.axes[axis] : KDTree.axes[0];
  this.axis = axisName;
  if (data.length == 0) {
    this.leaf = true;
    this.point = null;
  } else if (data.length == 1) {
    this.leaf = true;
    this.point = data[0];
  } else {
    var sorted = data.sort(function(a, b) {
      return a[axisName] - b[axisName];
    });
    var median = Math.floor(data.length / 2);
    var nextAxis = (axis + 1) % KDTree.axes.length;
    this.leaf = false;
    this.left = new KDNode(data.slice(0, median), nextAxis);
    this.right = new KDNode(data.slice(median + 1, data.length), nextAxis);
    this.point = data[median];
  }
}
