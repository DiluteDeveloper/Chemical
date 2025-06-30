//#[derive(Debug)]
//struct Rectangle {
//    width: u32,
//    height: u32,
//}
//
//impl Rectangle {
//    fn square(size: u32) -> Self {
//        Self {
//            width: size,
//            height: size,
//        }
//    }
//    fn area(&self) -> u32 {
//        self.width * self.height
//    }
//    fn width(&self) -> bool {
//        self.width > 0
//    }
//    fn can_hold(&self, other: &Rectangle) -> bool {
//        self.width > other.width && self.height > other.height
//    }
//}
//
//pub fn struct_main() {
//    let scale = 2;
//    let rect1 = Rectangle {
//        width: dbg!(30 * scale),
//        height: 50,
//    };
//    let rect2 = Rectangle {
//        width: dbg!(29 * scale),
//        height: 20,
//    };
//    let rect3 = Rectangle::square(5);
//
//    //println!("The area of the rectangle is {rect1:#?} square pixels.");
//    println!("The area of rectangle 1 is {} square pixels.", rect1.area());
//    println!("The area of rectangle 2 is {} square pixels.", rect2.area());
//    println!("The area of rectangle 3 is {} square pixels.", rect3.area());
//    dbg!(&rect1);
//    dbg!(&rect2);
//    println!("Can rect1 hold rect2: {}", rect1.can_hold(&rect2));
//    if rect1.width() {
//        println!("Width!");
//    }
//    //dbg!(&rect1);
//}
//
//fn area(rectangle: &Rectangle) -> u32 {
//    rectangle.width * rectangle.height
//}
