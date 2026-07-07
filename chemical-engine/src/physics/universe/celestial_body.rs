use cgmath::Point3;
use cgmath::Vector3;

const ORBIT_TRAIL_RESOLUTION: u8 = 100;

pub struct CelestialBody {
    pub velocity: Vector3<f32>,
    pub position: Point3<f32>,
    pub mass: f32,
    pub trail: Vec<Point3<f32>>,
}

impl CelestialBody {
    pub fn tick(&mut self, tick_count: u64) {
        self.position += self.velocity;
        if tick_count % ORBIT_TRAIL_RESOLUTION as u64 == 0 {
            self.trail.push(self.position);
        }
    }
}
