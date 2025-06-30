//use std::mem;
//pub fn bubble_sort(vec: &mut Vec<i32>) {
//    let mut swapped = true;
//    while swapped {
//        swapped = false;
//        for i in 1..vec.len() {
//            let [a, b] = vec.get_disjoint_mut([i - 1, i]).unwrap();
//
//            if a > b {
//                mem::swap(a, b);
//                swapped = true;
//            }
//        }
//    }
//}
