export function* rangeIt(start, end, step = 1) {
  for (let i = start; i < end; i += step) {
    yield i;
  }
}

export function range(start, end, step = 1) {
  return Array.from(rangeIt(start, end, step));
}

export function repeat(el, n) {
  return new Array(n).fill(el);
}

export function between(val, start, end) {
  return start <= val && val < end;
}
