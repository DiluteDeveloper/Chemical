pub struct Quaternion {
    pub w: f32,
    pub xi: f32,
    pub yj: f32,
    pub zk: f32,
}

impl Quaternion {
    pub fn unit() -> Self {
        Self {
            w: 1.0,
            xi: 0.0,
            yj: 0.0,
            zk: 0.0,
        }
    }
    pub fn norm(&self) -> Self {
        let mag = (self.w * self.w + self.xi * self.xi + self.yj * self.yj + self.zk * self.zk).sqrt();
        Self {
            w: self.w / mag,
            xi: self.xi / mag,
            yj: self.yj / mag,
            zk: self.zk / mag,
        }
    }

    pub fn mul(&self, b: &Self) -> Self {
        Quaternion {
            w: self.w * b.w - self.xi * b.xi - self.yj * b.yj - self.zk * b.zk, // 1
            xi: self.w * b.xi + self.xi * b.w + self.yj * b.zk - self.zk * b.yj, // i
            yj: self.w * b.yj - self.xi * b.zk + self.yj * b.w + self.zk * b.xi, // j
            zk: self.w * b.zk + self.xi * b.yj - self.yj * b.xi + self.zk * b.w,
        } // k
    }
    pub fn as_vector(&self) -> [f32; 3] {
        (self.xi, self.yj, self.zk).into()
    }
    pub fn conjugate(&self) -> Self {
        Quaternion {
            w: self.w,
            xi: -self.xi,
            yj: -self.yj,
            zk: -self.zk,
        }
    }
}
