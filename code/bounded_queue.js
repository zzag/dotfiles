function BoundedQueue(capacity) {
  this.capacity = capacity;
  this.elements = [];
}


BoundedQueue.prototype.put = function(priority, value) {
  var capacity = this.elements,
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
