fn quick_sort<T : Ord>(v: &mut Vec<T>, left: usize, right: usize)
{
  if right <= left {
    return;
  }

  let pivot_index = partition(v, left, right);

  if left < pivot_index {
    quick_sort(v, left, pivot_index);
  }
  if pivot_index + 1 < right {
    quick_sort(v, pivot_index + 1, right);
  }
}

fn partition<T : Ord>(v: &mut Vec<T>, left: usize, right: usize) -> usize {
  let pivot_index = left + (right - left) / 2;
  v.swap(pivot_index, right);

  let mut index = left;
  for i in left..right {
    if &v[i] < &v[right] {
      v.swap(i, index);
      index += 1;
    }
  }

  v.swap(index, right);
  index
}

fn main() {
  let mut v = vec![-100, 98, 78, 45, 23, -45, 5, 10, 37, 27];
  quick_sort(&mut v, 0, 9);
  println!("{:?}", v);
}
