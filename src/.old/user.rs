//struct User {
//    _active: bool,
//    username: String,
//    email: String,
//    _sign_in_count: u64,
//}
//
//struct _Color(i32, i32, i32);
//struct _Point(i32, i32, i32);
//
//fn build_user(email: String, username: String) -> User {
//    User {
//        _active: true,
//        username,
//        email,
//        _sign_in_count: 1,
//    }
//}
//
//struct _AlwaysEqual;
//
//pub fn _user() {
//    let mut user1 = User {
//        _active: true,
//        username: String::from("someusername123"),
//        email: String::from("someone@example.com"),
//        _sign_in_count: 1,
//    };
//
//    user1.email = String::from("anotheremail@example.com");
//
//    let user2 = build_user(user1.email.clone(), user1.username.clone());
//
//    println!("user1: {}, user2: {}, ", user1.username, user2.username);
//}
