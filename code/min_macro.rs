use std::cmp;

macro_rules! min {
    ($e:expr) => ($e);
    ($e:expr, $($t:expr),*) => (cmp::min($e, min!($($t),*)));
}
