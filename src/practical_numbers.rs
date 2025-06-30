use crate::maths;
use std::cmp::Ordering;

#[cfg(test)]
mod tests {
    #[test]
    fn test_is_practical_number_0() {
        assert_eq!(super::is_practical_number(0), false);
    }
    #[test]
    fn test_is_practical_number_1() {
        assert_eq!(super::is_practical_number(1), true);
    }
    #[test]
    fn test_is_practical_number_2() {
        assert_eq!(super::is_practical_number(2), true);
    }
    #[test]
    fn test_is_practical_number_3() {
        assert_eq!(super::is_practical_number(3), false);
    }
    #[test]
    fn test_is_practical_number_10() {
        assert_eq!(super::is_practical_number(10), false);
    }
    #[test]
    fn test_is_practical_number_12() {
        assert_eq!(super::is_practical_number(12), true);
    }
}

pub fn is_practical_number(x: u32) -> bool {
    if x == 0 {
        return false;
    };

    let divisors = maths::get_divisors(x);

    'sums: for n in 2..x {
        let mut incrementor: u32 = 0;

        for divisor in &divisors {
            match n.cmp(&(incrementor + divisor)) {
                Ordering::Less => (),
                Ordering::Greater => {
                    incrementor += divisor;
                }
                Ordering::Equal => continue 'sums,
            }
        }
        return false;
    }
    true
}
