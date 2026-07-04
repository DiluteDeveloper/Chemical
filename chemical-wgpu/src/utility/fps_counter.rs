use std::time::{Duration, Instant};

pub struct FPSCounter {
    incremental_fps: u32,

    // Only updates every second so not valid for first second
    pub fps: Option<i32>,

    pub delta: f64,

    last_frame: Instant,
    fps_timer: Instant,
}

impl FPSCounter {
    pub fn new() -> Self {
        let now = Instant::now();

        Self {
            last_frame: now,
            incremental_fps: 0,
            fps: None,
            delta: 0.0,
            fps_timer: now,
        }
    }

    pub fn update(&mut self) {
        self.incremental_fps += 1;

        let now = Instant::now();
        self.delta = (now - self.last_frame).as_secs_f64();
        self.last_frame = now;

        if self.fps_timer.elapsed() >= Duration::from_secs(1) {
            self.fps = Some(self.incremental_fps as i32);
            self.incremental_fps = 0;
            self.fps_timer = Instant::now();
        }
    }
}
