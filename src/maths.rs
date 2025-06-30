pub fn get_divisors(x: u32) -> Vec<u32> {
    let mut vec: Vec<u32> = Vec::new();

    for i in 1..=(x / 2) {
        if x % i == 0 {
            vec.push(x / i);
        }
    }
    vec.push(1);
    vec
}
