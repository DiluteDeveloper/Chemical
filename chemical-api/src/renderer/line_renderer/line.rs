use anyhow::{self};

// Makes the memory layout of the struct C-like, rust normally optimizes struct memory layout
#[repr(C)]
// Automatically has traits Copy Clone and Debug
#[derive(Debug, Copy, Clone, bytemuck::Pod, bytemuck::Zeroable)]
pub(super) struct LineSegment {
    position_a: [f32; 3],
    position_a_norm: [f32; 3],
    position_b: [f32; 3],
    position_b_norm: [f32; 3],
    colour: [f32; 4],
    width: f32,
}

pub struct LineDescriptor {
    pub data: Option<Vec<glam::Vec3>>,
    pub width: f32,
    pub colour: wgpu::Color,
}

#[derive(Debug)]
pub struct Line {
    pub(super) segments: Vec<LineSegment>,
    raw: Vec<glam::Vec3>,
    colour: [f32; 4],
    width: f32,
}

impl Line {
    pub(super) fn new(descriptor: &LineDescriptor) -> Self {
        // let buffer = device.create_buffer(&wgpu::BufferDescriptor {
        //     label: Some("line_buffer"),
        //     size: size_of::<[[f32; 16]; MAX_LINE_SEGMENTS]>() as u64,
        //     usage: wgpu::BufferUsages::VERTEX | wgpu::BufferUsages::COPY_DST,
        //     mapped_at_creation: false,
        // });

        let colour_arr = [
            descriptor.colour.r as f32,
            descriptor.colour.g as f32,
            descriptor.colour.b as f32,
            descriptor.colour.a as f32,
        ];

        /*if let Some(data) = &descriptor.data {
            if data.len() <= 1 {
                bail!("Line must contain atleast two points!");
            }

            {
                let start_normal = (data[1] - data[0]).normalize().into();
                segments.push(LineSegment {
                    position_a: data[0].into(),
                    position_a_norm: start_normal,
                    position_b: data[1].into(),
                    position_b_norm: start_normal,
                    colour: colour_arr,
                    width: descriptor.width,
                });
            }

            for (i, pos) in data.iter().enumerate().skip(2) {
                segments.push(LineSegment {
                    position_a: data[i - 1].into(),
                    position_a_norm: (data[i - 1] - data[i - 2]).normalize().into(),
                    position_b: (*pos).into(),
                    position_b_norm: (pos - data[i - 1]).normalize().into(),
                    colour: colour_arr,
                    width: descriptor.width,
                });
            }
        }*/

        let mut l = Line {
            raw: Vec::new(),
            segments: Vec::new(),
            colour: colour_arr,
            width: descriptor.width,
        };

        if let Some(data) = &descriptor.data {
            l.push(&data);
        }

        l
    }

    pub fn push(&mut self, points: &[glam::Vec3]) {
        for p in points.iter() {
            self.raw.push(*p);
            match self.raw.len() {
                1 => {}
                2 => {
                    let start_normal = (self.raw[1] - self.raw[0]).normalize().into();
                    self.segments.push(LineSegment {
                        position_a: self.raw[0].into(),
                        position_a_norm: start_normal,
                        position_b: self.raw[1].into(),
                        position_b_norm: start_normal,
                        colour: self.colour,
                        width: self.width,
                    });
                }
                _ => {
                    let idx = self.raw.len() - 1;
                    self.segments.push(LineSegment {
                        position_a: self.raw[idx - 1].into(),
                        position_a_norm: self.segments[idx - 2].position_b_norm,
                        position_b: self.raw[idx].into(),
                        position_b_norm: (self.raw[idx] - self.raw[idx - 1]).normalize().into(),
                        colour: self.colour,
                        width: self.width,
                    });
                }
            }
        }
    }

    pub fn get_num_points(&self) -> usize {
        self.raw.len()
    }
}
