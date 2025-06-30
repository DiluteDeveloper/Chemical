//enum IpAddr {
//    // can be structs in enum aswell
//    V4(u8, u8, u8, u8),
//    V6(String),
//}
//enum Message {
//    Move { x: i32, y: i32 },
//}
//
//impl Message {
//    fn call(&self) {}
//}
//
//pub fn enums() {
//    let home = IpAddr::V4(127, 0, 0, 1);
//
//    let loopback = IpAddr::V6(String::from("::1"));
//
//    let message = Message::Move { x: 14, y: 16 };
//
//    let dice_roll = Some(2);
//    match dice_roll {
//        Some(other) => match other {
//            8 => println!("You rolled 8!"),
//            other => {
//                if other >= 3 && other <= 7 {
//                    println!("you rolled {other}!");
//                } else {
//                    println!("You rolled other value!");
//                }
//            }
//        },
//        None => println!("Dice roll is null!"),
//    }
//
//    let config_max = Some(30u8);
//    let config_max: Option<u8> = None;
//
//    //let cfg = config_max else {
//    //    panic!("Null!");
//    //};
//
//    if let Some(max) = config_max {
//        println!("The maximum is configured to be {max}!");
//    } else {
//        println!("None!");
//    }
//}
