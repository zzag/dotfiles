#[derive(Debug)]
struct GcdResult {
    a: i32,
    s: i32,
    b: i32,
    t: i32,
    gcd: i32,
}

fn gcd(a: i32, b: i32) -> GcdResult {
    let (mut prev_r, mut r) = (a, b);
    let (mut prev_s, mut s) = (1, 0);
    let (mut prev_t, mut t) = (0, 1);

    while r > 0 {
        let q = prev_r / r;
        let new_r = prev_r - r * q;
        let new_s = prev_s - s * q;
        let new_t = prev_t - t * q;

        prev_r = r; r = new_r;
        prev_s = s; s = new_s;
        prev_t = t; t = new_t;
    }

    GcdResult { a: a, s: prev_s, b: b, t: prev_t, gcd: prev_r }
}

fn main() {
    let a = 240;
    let b = 46;

    println!("{:?}", gcd(a, b));
}
