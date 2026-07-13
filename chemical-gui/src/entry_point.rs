use std::process::{Command, Stdio};

pub fn build_game() {
    let game = Command::new("cargo")
        .args([
            "build",
            "--bin",
            "ProjectGamma",
            "--features",
            "chemical-scripting",
        ])
        .current_dir(r"/mnt/storage/Chemical/chemical_rs")
        .output()
        .expect("Failed to launch build command!");
    println!("stdout: {}", String::from_utf8_lossy(&game.stdout));
    println!("stderr: {}", String::from_utf8_lossy(&game.stderr));
    println!("success: {}", game.status.success());
}
pub fn launch_game() {
    let mut child = Command::new(r"./target/debug/ProjectGamma")
        .current_dir(r"/mnt/storage/Chemical/chemical_rs")
        .stdout(Stdio::piped()) // or Stdio::piped() to capture
        .spawn(); // returns immediately with a handle, doesn't block
}
