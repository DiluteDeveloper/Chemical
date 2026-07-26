use anyhow::anyhow;
use chemical_api::{
    geometry::{model, sphere},
    scene::types::{DirectionalLight, StaticMesh, Transform},
};

pub fn start(world: &mut hecs::World) {
    let (sphere_vertices, sphere_indices) = sphere::generate_index_sphere(200)
        .map_err(|e| anyhow!("Failed to generate index sphere: {}", e))
        .expect("Tried to create invalid index sphere for celestial body mesh!");
    // let cube_vertices = cube::generate_vertex_cube((1.0, 1.0, 1.0).into());

    // let sphere_mesh_data = Mesh::new(&sphere_vertices, Some(&sphere_indices), true);
    let light_mesh_data = StaticMesh::new(&sphere_vertices, Some(&sphere_indices), false);
    // let model_mesh_data =
    //     model::gltf_load("res/models/stanford_dragon.gltf", true).expect("Failed to load model!");
    let slum_mesh_data =
        model::gltf_load("res/models/slums.gltf", true).expect("Failed to load model!");
    // let cube_mesh_data = Mesh::new(&cube_vertices, None, true);

    // let under_cube_transform = Transform {
    //     position: (0.0, -2.0, 0.0).into(),
    //     orientation: glam::Quat::from_xyzw(0.0, 0.0, 0.0, -1.0),
    //     scale: (5.0, 1.0, 5.0).into(),
    // };
    // let cube_transform = Transform {
    //     position: (3.0, 0.0, 3.0).into(),
    //     orientation: glam::Quat::from_xyzw(0.0, 0.0, 0.0, -1.0),
    //     scale: (1.0, 1.0, 1.0).into(),
    // };
    // let model_transform = Transform {
    //     position: (-2.0, 0.68, 2.0).into(),
    //     orientation: glam::Quat::from_xyzw(0.0, 0.0, 0.0, -1.0),
    //     scale: (3.0, 3.0, 3.0).into(),
    // };
    // let sphere_transform = Transform {
    //     position: (-3.0, 0.0, -3.0).into(),
    //     orientation: glam::Quat::from_xyzw(0.0, 0.0, 0.0, -1.0),
    //     scale: (1.0, 1.0, 1.0).into(),
    // };
    let mut light_model_transform = Transform::default();
    light_model_transform.scale = (0.2, 0.2, 0.2).into();

    let directional_light = DirectionalLight {
        orientation: glam::Quat::from_euler(glam::EulerRot::ZYX, -1.00, 0.70, -0.5),
        strength: 0.1,
        colour: (1.0, 0.8, 0.8).into(),
    };
    let directional_light_2 = DirectionalLight {
        orientation: glam::Quat::from_euler(glam::EulerRot::ZYX, -0.50, 0.70, -1.00),
        strength: 0.1,
        colour: (1.0, 0.8, 0.8).into(),
    };
    let directional_light_3 = DirectionalLight {
        orientation: glam::Quat::from_euler(glam::EulerRot::ZYX, -0.80, 0.40, -0.60),
        strength: 0.1,
        colour: (1.0, 0.8, 0.8).into(),
    };
    let directional_light_4 = DirectionalLight {
        orientation: glam::Quat::from_euler(glam::EulerRot::ZYX, 0.80, 0.40, 0.60),
        strength: 0.1,
        colour: (1.0, 0.8, 0.8).into(),
    };
    // let directional_light_2 = DirectionalLight {
    //     orientation: glam::Quat::from_euler(glam::EulerRot::XYZ, 1.0, 1.0, 0.0),
    //     strength: 0.1,
    //     colour: (1.0, 0.5, 0.0).into(),
    // };

    /*world
        .transform_handler
        .insert_transform(sphere_transform, 0);
    world.mesh_handler.insert_mesh(sphere_mesh_data.clone(), 0);

    world.transform_handler.insert_transform(cube_transform, 1);
    world.mesh_handler.insert_mesh(cube_mesh_data.clone(), 1);

    world
        .transform_handler
        .insert_transform(under_cube_transform, 2);
    world.mesh_handler.insert_mesh(cube_mesh_data, 2);*/
    light_model_transform.position = directional_light.orientation.to_axis_angle().0 * 100.0;
    world.spawn((light_model_transform, light_mesh_data.clone()));
    light_model_transform.position = directional_light_2.orientation.to_axis_angle().0 * 100.0;
    world.spawn((light_model_transform, light_mesh_data.clone()));
    // world
    //     .transform_handler
    //     .insert_transform(light_model_transform, 3);
    // world.mesh_handler.insert_mesh(light_mesh_data.clone(), 3);

    /*world.transform_handler.insert_transform(model_transform, 5);
    world
        .mesh_handler
        .insert_mesh(model_mesh_data.get(0).unwrap().clone(), 5);*/

    // world
    //     .transform_handler
    //     .insert_transform(light_model_transform, 4);
    // world.mesh_handler.insert_mesh(light_mesh_data, 4);

    for mesh in slum_mesh_data.iter() {
        world.spawn((Transform::default(), mesh.clone()));
        // world
        //     .transform_handler
        //     .insert_transform(Transform::default(), 6 + i as u32);
        // world.mesh_handler.insert_mesh(mesh.clone(), 6 + i as u32);
    }
    world.spawn((directional_light,));
    world.spawn((directional_light_2,));
    world.spawn((directional_light_3,));
    world.spawn((directional_light_4,));
    // world
    //     .light_handler
    //     .insert_directional_light(directional_light, 0);
    // world
    //     .light_handler
    //     .insert_directional_light(directional_light_2, 1);
}
pub fn update(world: &mut hecs::World, seconds_elapsed: f64) {
    // for t in world.query_mut::<&mut DirectionalLight>() {
    //     t.orientation =
    //         glam::Quat::from_euler(glam::EulerRot::XYZ, seconds_elapsed as f32 + 0.01, 1.0, 0.0);
    // }

    // let dir = world
    //     .light_handler
    //     .get_directional_light(0)
    //     .unwrap()
    //     .orientation
    //     .to_axis_angle()
    //     .0;
    // world
    //     .transform_handler
    //     .modify(3, move |t| {
    //         t.position = dir * 100.0;
    //     })
    //     .unwrap();
}
