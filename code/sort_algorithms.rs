fn bubble_sort(list: &Vec<i32>) -> Vec<i32> {
  let mut arr = list.to_vec();

  for i in (0 .. arr.len()).rev() {
    for j in 0 .. i {
      if arr[j] > arr[j + 1] {
        arr.swap(j, j + 1);
      }
    }
  }

  arr
}


fn insertion_sort(list: &Vec<i32>) -> Vec<i32> {
  let mut arr = list.to_vec();

  for i in 1 .. arr.len() {
    let mut j = i;

    while j > 0 && arr[j - 1] > arr[j] {
      arr.swap(j - 1, j);
      j -= 1;
    }
  }

  arr
}


fn cocktail_sort(list: &Vec<i32>) -> Vec<i32> {
  let mut arr = list.to_vec();
  let mut left = 0;
  let mut right = arr.len() - 1;

  while left < right {
    for i in left .. right {
      if arr[i] > arr[i + 1] {
        arr.swap(i, i + 1);
      }
    }

    for i in (left + 1 .. right).rev() {
      if arr[i] < arr[i - 1] {
        arr.swap(i - 1, i);
      }
    }

    left += 1;
    right -= 1;
  }

  arr
}

fn sort_helper(arr: &mut Vec<i32>, left: isize, right: isize) {
  if left >= right {
    return
  }

  let m = (left + right) / 2;
  let pivot = arr[m as usize];
  let mut i = left;
  let mut j = right;

  while i < j {
    while arr[i as usize] < pivot {
      i += 1;
    }

    while arr[j as usize] > pivot {
      j -= 1;
    }


    if i <= j {
      arr.swap(i as usize, j as usize);
      i += 1;
      j -= 1;
    }
  }

  if left < j {
    sort_helper(arr, left, j);
  }

  if i < right {
    sort_helper(arr, i, right);
  }
}

fn quick_sort(list: &Vec<i32>) -> Vec<i32> {
  let mut arr = list.to_vec();
  let n = arr.len();
  sort_helper(&mut arr, 0, n as isize - 1);
  arr
}


fn main () {
  let arr = vec![1, 6, 2, -1, 3, 0, 1];
  let bubble_sorted = bubble_sort(&arr);
  let insert_sorted = insertion_sort(&arr);
  let shaker_sorted = cocktail_sort(&arr);
  let quick_sorted = quick_sort(&arr);
  println!("Bubbl sort: {:?}", bubble_sorted);
  println!("Insertion sort: {:?}", insert_sorted);
  println!("Cocktail sort: {:?}", shaker_sorted);
  println!("Quick sort: {:?}", quick_sorted);
}
