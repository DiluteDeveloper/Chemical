use log::info;
use std::time::{Duration, Instant};

pub struct FPSCounter {
    last_frame: Instant,
    frame_count: i32,
    fps_timer: Instant,
}

impl FPSCounter {
    pub fn new() -> Self {
        let now = Instant::now();

        Self {
            last_frame: now,
            frame_count: 0,
            fps_timer: now,
        }
    }

    pub fn update(&mut self) {
        self.frame_count += 1;

        let now = Instant::now();
        let dt = now - self.last_frame;
        self.last_frame = now;

        if self.fps_timer.elapsed() >= Duration::from_secs(1) {
            info!("FPS: {} | DT: {}", self.frame_count, dt.as_secs_f64());
            self.frame_count = 0;
            self.fps_timer = Instant::now();
        }
    }
}
