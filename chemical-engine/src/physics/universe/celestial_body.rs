pub struct CelestialBody {
    pub velocity: glam::Vec3,
    pub position: glam::Vec3,
    pub mass: f32,
}

impl CelestialBody {
    pub fn tick(&mut self) {
        self.position += self.velocity;
    }
}
