// Lamport clock
//
// - increment, when a process does work
// - increment, when a process sends a message
// - set 'max(local time, received time) + 1', if a process
//      got a message

function LamportClock() {
    this.value = 0;
}

LamportClock.prototype.increment = function () {
    this.value++;
}

LamportClock.prototype.merge = function (other) {
    this.value = Math.max(this.value, other.value) + 1;
}
