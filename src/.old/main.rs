//#![allow(unused)]
//#![allow(dead_code)]
////use std::cmp::Ordering;
////use std::io;
//
//mod enums;
//
//const THREE_HOURS_IN_SECONDS: u32 = 60 * 60 * 3;
//
//fn _main2() {
//    let mut x = 5;
//    println!("The value of x is: {x}");
//    x = 6;
//    println!("The value of x is: {x}");
//
//    println!("Three hours in seconds: {THREE_HOURS_IN_SECONDS}");
//
//    let x = 5;
//    let mut x = x + 1;
//    {
//        x = x + 1;
//        let x = x * 2;
//        println!("The value of x in the inner scope is: {x}");
//    }
//    println!("The value of x is: {x}");
//}
//
//fn _square(v: f64) -> f64 {
//    return v * v;
//}
//// TODO: figure out how to return a panic
////fn _get_user_float() -> f64 {
////    let mut val: String = String::new();
////    io::stdin()
////        .read_line(&mut val)
////        .expect("Failed to read line");
////    match val.trim().parse() {
////        Ok(num) => return num,
////        Err(_) => return 0.0,
////    };
////}
//
//fn _return_tuple() -> (i64, i32, i8) {
//    return (32, 32, 32);
//}
//
//fn _fn_expression() -> i32 {
//    5
//}
//
//fn _plus_one(x: i32) -> i32 {
//    x + 1
//}
//
//fn _borrowfn(str: &mut String) {
//    str.push_str("Hi");
//    println!("Move str: {str}");
//}
//
//mod bubble;
//
//fn main() {
//    //user::_user();
//    //main2();
//    let mut vec = vec![1, 7, 5, 3, 6];
//    bubble::bubble_sort(&mut vec);
//
//    dbg!(vec);
//
//    //struct_example::struct_main();
//    enums::enums();
//    //    let mut s = String::from("Hello World!");
//    //
//    //    let _hello = &s[0..5];
//    //    let _world = &s[6..11];
//    //
//    //    s.push_str(" Haha!");
//    //
//    //    let mut hi: String = String::from("Hello!");
//    //
//    //    borrowfn(&mut hi);
//    //
//    //    println!("{hi}");
//    //
//    //    let mut tuple: (u32, i64, i8) = (57, -40, -127);
//    //    tuple.2 = 5;
//    //    tuple.0 = 1;
//    //
//    //    let (x, y, z) = tuple;
//    //
//    //    println!("{x}, {y}, {z}");
//    //
//    //    let user_input = get_user_float();
//    //
//    //    println!(
//    //        "User entered {user_input}, square is {}",
//    //        square(user_input)
//    //    );
//    //
//    //    let expression = {
//    //        let x = 5;
//    //        x + 1
//    //    };
//    //
//    //    let number = if expression > 5 { expression } else { 1 };
//    //    println!("Number is {number}");
//    //
//    //    let array = [1, 2, 3, 4, 5];
//    //
//    //    for x in array {
//    //        println!("val: {x}");
//    //    }
//    //    for x in (1..4).rev() {
//    //        println!("val: {x}");
//    //    }
//    //
//    //    let mut _str: &str = "Hi";
//    //
//    //    let _explicit_array: [i32; 5] = [1, 2, 3, 4, 5];
//    //    let _default_value_array: [i32; 5] = [4; 5]; // 5 elements of value 4
//    //
//    //    let _first = _explicit_array[1];
//    //    let _bounds = _explicit_array[4];
//    //
//    //    let mut counter = 0;
//    //    let loop_result = loop {
//    //        counter += 1;
//    //
//    //        if counter == 10 {
//    //            break counter * 2;
//    //        }
//    //    };
//    //    println!("loop result: {loop_result}");
//    //
//    //    let _count = 'counting_up: loop {
//    //        break 'counting_up;
//    //    };
//    //
//    //    println!("Guess the number!");
//    //
//    //    let secret_number = rand::thread_rng().gen_range(1..=100);
//    //
//    //    loop {
//    //        println!("Please input your guess.");
//    //
//    //        let mut guess = String::new();
//    //        //let mut guess = "Hi".to_string();
//    //
//    //        io::stdin()
//    //            .read_line(&mut guess)
//    //            .expect("Failed to read line");
//    //
//    //        println!("You guessed: {guess}");
//    //
//    //        //match guess.as_str() {
//    //        //    "quit" => break,
//    //        //}
//    //
//    //        let guess: u32 = match guess.trim().parse() {
//    //            Ok(num) => num,
//    //            Err(_) => continue,
//    //        };
//    //
//    //        match guess.cmp(&secret_number) {
//    //            Ordering::Less => println!("Too small!"),
//    //            Ordering::Greater => println!("Too big!"),
//    //            Ordering::Equal => {
//    //                println!("You win!");
//    //                break;
//    //            }
//    //        }
//    //    }
//    //    println!("Thankyou for playing!");
//}
