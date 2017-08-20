function randInt(a, b) {
  return Math.floor(Math.random() * (b - a) + a);
}


function distanceBetween(points, point, metric) {
  return points
    .map(function(anotherPoint) {
      return {
        point: anotherPoint,
        distance: metric(point, anotherPoint)
      };
    });
}


function VPNode(points, metric) {
  if (points.length <= 1) {
    this.leaf = true;
    this.vantagePoint = points.length == 1 ? points[0] : null;
    this.radius = 0.0;
  } else {
    var vantagePoint = points[randInt(0, points.length)];
    var distances = distanceBetween(points, vantagePoint, metric)
      .filter(function(info) {
        return info.point != vantagePoint;
      })
      .sort(function(a, b) {
        return a.distance - b.distance;
      });
    var median = Math.floor(distances.length / 2);
    var leftPoints = distances
      .slice(0, median)
      .map(function(info) {
        return info.point;
      });
    var rightPoints = distances
      .slice(median, distances.length)
      .map(function(info) {
        return info.point;
      });
    this.leaf = false;
    this.vantagePoint = vantagePoint;
    this.radius = distances[median].distance;
    this.leftChild = new VPNode(leftPoints, metric);
    this.rightChild = new VPNode(rightPoints, metric);
  }
}


function VPTree(points, metric) {
  this.metric = metric;
  this.root = new VPNode(points, metric);
}


VPTree.prototype.nearest = function(searchPoint, k) {
  var points = new BoundedQueue(k),
    metric = this.metric;

  function search(tree) {
    if (!tree.vantagePoint) {
      return;
    }

    var d = metric(tree.vantagePoint, searchPoint);
    points.put(d, tree.vantagePoint);
    if (tree.leaf) {
      return;
    }

    var anotherChild;
    if (d < tree.radius) {
      search(tree.leftChild);
      anotherChild = tree.rightChild;
    } else {
      search(tree.rightChild);
      anotherChild = tree.leftChild;
    }

    var farthestDistance = points.last().weight;
    if (points.data.length < k ||
      tree.radius < d + farthestDistance) {
      search(anotherChild);
    }
  }

  search(this.root);
  return points.data;
};


function BoundedQueue(k) {
  this.k = k;
  this.data = [];
}


BoundedQueue.prototype.put = function(weight, item) {
  var data = this.data,
    k = this.k,
    i = 0;

  if (data.length < k) {
    data.push({
      weight: weight,
      item: item
    });
    i = data.length - 1;
    while (i > 0 && data[i - 1].weight > weight) {
      swap(data, i, i - 1);
      i--;
    }
    return;
  }

  if (this.last().weight <= weight) {
    return;
  }

  i = data.length - 1;
  while (i > 0 && data[i - 1].weight > weight) {
    data[i] = data[i - 1];
    i--;
  }

  data[i] = {
    weight: weight,
    item: item
  };
};


BoundedQueue.prototype.last = function() {
  return this.data[this.data.length - 1];
};


function swap(arr, i, j) {
  var tmp = arr[i];
  arr[i] = arr[j];
  arr[j] = tmp;
}
