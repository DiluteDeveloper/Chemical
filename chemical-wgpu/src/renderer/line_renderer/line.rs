use cgmath::Point3;

pub struct Line {
    buffer: wgpu::Buffer,
    length: u32,
}

impl Line {
    pub fn new(length: u32, device: &wgpu::Device) -> Self {
        Line {
            buffer: device.create_buffer(&wgpu::BufferDescriptor {
                label: Some("Line Buffer"),
                size: size_of::<[[f32; 12]; 100000]>() as u64,
                usage: wgpu::BufferUsages::VERTEX | wgpu::BufferUsages::COPY_DST,
                mapped_at_creation: false,
            }),
            length: length,
        }
    }
    pub fn fill(&mut self, queue: &wgpu::Queue, line: &Vec<Point3<f32>>) {
        let mut buflen: u64 = 0;

        let mut b0: [f32; 3] = line[0].into();

        queue.write_buffer(&self.buffer, buflen, bytemuck::bytes_of(&b0));
        buflen += size_of::<[f32; 3]>() as u64;

        b0 = (line[1] - line[0]).normalize().into();

        queue.write_buffer(&self.buffer, buflen, bytemuck::bytes_of(&b0));
        buflen += size_of::<[f32; 3]>() as u64;

        b0 = line[1].into();

        queue.write_buffer(&self.buffer, buflen, bytemuck::bytes_of(&b0));
        buflen += size_of::<[f32; 3]>() as u64;

        b0 = (line[1] - line[0]).normalize().into();

        queue.write_buffer(&self.buffer, buflen, bytemuck::bytes_of(&b0));
        buflen += size_of::<[f32; 3]>() as u64;

        for i in 1..line.len() - 1 {
            b0 = line[i].into();

            queue.write_buffer(&self.buffer, buflen, bytemuck::bytes_of(&b0));
            buflen += size_of::<[f32; 3]>() as u64;

            b0 = (line[i] - line[i - 1]).normalize().into();

            queue.write_buffer(&self.buffer, buflen, bytemuck::bytes_of(&b0));
            buflen += size_of::<[f32; 3]>() as u64;

            b0 = line[i + 1].into();

            queue.write_buffer(&self.buffer, buflen, bytemuck::bytes_of(&b0));
            buflen += size_of::<[f32; 3]>() as u64;

            b0 = (line[i + 1] - line[i]).normalize().into();
            queue.write_buffer(&self.buffer, buflen, bytemuck::bytes_of(&b0));
            buflen += size_of::<[f32; 3]>() as u64;
        }
    }
}
