use chemical_api::input::KeyCode as ChemicalKeyCode;
use winit::keyboard::KeyCode as WinitKeyCode;

pub fn winit_keycode_to_chemical(key_code: WinitKeyCode) -> Option<ChemicalKeyCode> {
    match key_code {
        WinitKeyCode::KeyW => Some(ChemicalKeyCode::KeyW),
        WinitKeyCode::KeyA => Some(ChemicalKeyCode::KeyA),
        WinitKeyCode::KeyS => Some(ChemicalKeyCode::KeyS),
        WinitKeyCode::KeyD => Some(ChemicalKeyCode::KeyD),
        WinitKeyCode::Space => Some(ChemicalKeyCode::Space),
        WinitKeyCode::ShiftLeft => Some(ChemicalKeyCode::ShiftLeft),
        WinitKeyCode::ArrowLeft => Some(ChemicalKeyCode::ArrowLeft),
        WinitKeyCode::ArrowRight => Some(ChemicalKeyCode::ArrowRight),
        WinitKeyCode::ArrowUp => Some(ChemicalKeyCode::ArrowUp),
        WinitKeyCode::ArrowDown => Some(ChemicalKeyCode::ArrowDown),
        _ => None,
    }
}
