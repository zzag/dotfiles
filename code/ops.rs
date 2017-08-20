use std::fmt;
use std::ops;


struct Point {
    x: i32,
    y: i32,
}


impl fmt::Display for Point {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "({}, {})", self.x, self.y)
    }
}


impl<'a, 'b> ops::Add<&'b Point> for &'a Point {
    type Output = Point;

    fn add(self, rhs: &'b Point) -> Point {
        Point {
            x: self.x + rhs.x,
            y: self.y + rhs.y,
        }
    }
}


impl<'a, 'b> ops::Sub<&'b Point> for &'a Point {
    type Output = Point;

    fn sub(self, rhs: &'b Point) -> Point {
        Point {
            x: self.x - rhs.x,
            y: self.y - rhs.y,
        }
    }
}


fn main() {
    let a = Point { x: 2, y: 5 };
    let b = Point { x: 5, y: 1 };
    println!("{} + {} = {}", a, b, &a + &b);
    println!("{} - {} = {}", a, b, &a - &b);
}
