use anyhow::anyhow;
use chemical_engine::{
    geometry::{cube, model, sphere},
    scene::{
        SceneContainer,
        types::{DirectionalLight, Mesh, Transform},
    },
};

pub fn start(scene: &mut SceneContainer) {
    let (sphere_vertices, sphere_indices) = sphere::generate_index_sphere(200)
        .map_err(|e| anyhow!("Failed to generate index sphere: {}", e))
        .expect("Tried to create invalid index sphere for celestial body mesh!");
    let cube_vertices = cube::generate_vertex_cube((1.0, 1.0, 1.0).into());

    let sphere_mesh_data = Mesh::new(&sphere_vertices, Some(&sphere_indices), true);
    let light_mesh_data = Mesh::new(&sphere_vertices, Some(&sphere_indices), false);
    let model_mesh_data =
        model::gltf_load("res/models/stanford_dragon.gltf", true).expect("Failed to load model!");
    let slum_mesh_data =
        model::gltf_load("res/models/slums.gltf", true).expect("Failed to load model!");
    let cube_mesh_data = Mesh::new(&cube_vertices, None, true);

    let under_cube_transform = Transform {
        position: (0.0, -2.0, 0.0).into(),
        orientation: glam::Quat::from_xyzw(0.0, 0.0, 0.0, -1.0),
        scale: (5.0, 1.0, 5.0).into(),
    };
    let cube_transform = Transform {
        position: (3.0, 0.0, 3.0).into(),
        orientation: glam::Quat::from_xyzw(0.0, 0.0, 0.0, -1.0),
        scale: (1.0, 1.0, 1.0).into(),
    };
    let model_transform = Transform {
        position: (-2.0, 0.68, 2.0).into(),
        orientation: glam::Quat::from_xyzw(0.0, 0.0, 0.0, -1.0),
        scale: (3.0, 3.0, 3.0).into(),
    };
    let sphere_transform = Transform {
        position: (-3.0, 0.0, -3.0).into(),
        orientation: glam::Quat::from_xyzw(0.0, 0.0, 0.0, -1.0),
        scale: (1.0, 1.0, 1.0).into(),
    };
    let mut light_model_transform = Transform::default();
    light_model_transform.scale = (0.2, 0.2, 0.2).into();

    let directional_light = DirectionalLight {
        orientation: glam::Quat::from_euler(glam::EulerRot::ZYX, -1.00, 0.70, -0.5),
        strength: 0.1,
        colour: (0.0, 0.5, 1.0).into(),
    };
    let directional_light_2 = DirectionalLight {
        orientation: glam::Quat::from_euler(glam::EulerRot::XYZ, 1.0, 1.0, 0.0),
        strength: 0.1,
        colour: (1.0, 0.5, 0.0).into(),
    };

    /*scene
        .transform_handler
        .insert_transform(sphere_transform, 0);
    scene.mesh_handler.insert_mesh(sphere_mesh_data.clone(), 0);

    scene.transform_handler.insert_transform(cube_transform, 1);
    scene.mesh_handler.insert_mesh(cube_mesh_data.clone(), 1);

    scene
        .transform_handler
        .insert_transform(under_cube_transform, 2);
    scene.mesh_handler.insert_mesh(cube_mesh_data, 2);*/

    scene
        .transform_handler
        .insert_transform(light_model_transform, 3);
    scene.mesh_handler.insert_mesh(light_mesh_data.clone(), 3);

    /*scene.transform_handler.insert_transform(model_transform, 5);
    scene
        .mesh_handler
        .insert_mesh(model_mesh_data.get(0).unwrap().clone(), 5);*/

    // scene
    //     .transform_handler
    //     .insert_transform(light_model_transform, 4);
    // scene.mesh_handler.insert_mesh(light_mesh_data, 4);

    for (i, mesh) in slum_mesh_data.iter().enumerate() {
        scene
            .transform_handler
            .insert_transform(Transform::default(), 6 + i as u32);
        scene.mesh_handler.insert_mesh(mesh.clone(), 6 + i as u32);
    }
    scene
        .light_handler
        .insert_directional_light(directional_light, 0);
    // scene
    //     .light_handler
    //     .insert_directional_light(directional_light_2, 1);
}
pub fn update(scene: &mut SceneContainer) {
    let seconds_elapsed = 0.0;

    /*self.scene
    .light_handler
    .modify_directional_light(0, move |t| {
        t.orientation = glam::Quat::from_euler(
            glam::EulerRot::XYZ,
            seconds_elapsed as f32 + 0.01,
            1.0,
            0.0,
        );
    })
    .unwrap();*/
    let dir = scene
        .light_handler
        .get_directional_light(0)
        .unwrap()
        .orientation
        .to_axis_angle()
        .0;
    scene
        .transform_handler
        .modify(3, move |t| {
            t.position = dir * 100.0;
        })
        .unwrap();
}
