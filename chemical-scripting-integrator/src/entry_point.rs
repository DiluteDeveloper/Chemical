use chemical_engine::scene::SceneContainer;

#[cfg(feature = "chemical-scripting")]
use chemical_scripting::entry_point;
#[cfg(feature = "chemical-scripting")]
pub fn start(scene: &mut SceneContainer) {
    entry_point::start(scene);
}
#[cfg(not(feature = "chemical-scripting"))]
pub fn start(scene: &mut SceneContainer) {}

#[cfg(feature = "chemical-scripting")]
pub fn update(scene: &mut SceneContainer) {
    entry_point::update(scene);
}
#[cfg(not(feature = "chemical-scripting"))]
pub fn update(scene: &mut SceneContainer) {}
