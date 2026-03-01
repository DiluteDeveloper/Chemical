use std::ops::{Add, Mul, Sub};

#[repr(C)]
#[derive(Copy, Clone, Debug)]
pub struct Vector3<T>
where
    T: PartialOrd + Copy + Clone + Mul<Output = T> + Sub<Output = T> + Add<Output = T>,
{
    x: T,
    y: T,
    z: T,
}

impl<T> Vector3<T>
where
    T: PartialOrd
        + Mul<Output = T>
        + Sub<Output = T>
        + Copy
        + Clone
        + Add<Output = T>
        + Div<Output = T>,
{
    pub fn new(x: T, y: T, z: T) -> Self {
        Self { x, y, z }
    }

    pub fn cross(self, other: Self) -> Self {
        Self::new(
            self.y * other.z - self.z * other.y,
            self.z * other.x - self.x * other.z,
            self.x * other.y - self.y * other.x,
        )
    }
    pub fn norm(self) -> Self {
        let mag = self.x * self.x + self.y * self.y + self.z * self.z;
        Self::new(self.x / mag, self.y / mag, self.z / mag)
    }
}

impl<T> From<&Vector3<T>> for [T; 3]
where
    T: PartialOrd + Mul<Output = T> + Sub<Output = T> + Copy + Clone + Add<Output = T>,
{
    fn from(v: &Vector3<T>) -> Self {
        [v.x, v.y, v.z]
    }
}

impl<T> Sub for Vector3<T>
where
    T: PartialOrd + Mul<Output = T> + Sub<Output = T> + Copy + Clone + Add<Output = T>,
{
    type Output = Self;

    fn sub(self, other: Self) -> Self::Output {
        Self {
            x: self.x - other.x,
            y: self.y - other.y,
            z: self.z - other.z,
        }
    }
}
impl<T> Add for Vector3<T>
where
    T: PartialOrd + Mul<Output = T> + Sub<Output = T> + Copy + Clone + Add<Output = T>,
{
    type Output = Self;

    fn add(self, other: Self) -> Self::Output {
        Self {
            x: self.x + other.x,
            y: self.y + other.y,
            z: self.z + other.z,
        }
    }
}
