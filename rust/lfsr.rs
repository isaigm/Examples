use num::traits::{PrimInt, Unsigned};
struct Lfsr<T: PrimInt + Unsigned> {
    state: T,
    seed: T,
    taps: Vec<usize>
}

impl<T: PrimInt + Unsigned> Lfsr<T> {
    fn new(seed: T, taps: Vec<usize>) -> Self {
        Self {
            state: seed,
            seed,
            taps,
        }
    }
    fn next(&mut self) -> T {
        let mut new_bit: T = T::zero();
        for idx in self.taps.iter() {
            let shifted_one: T = T::one() << *idx;
            let bit = if self.state & shifted_one == T::zero() {T::zero()}  else {T::one()};
            new_bit = new_bit ^ bit;
        }
        let output_bit = self.state & T::one();
        self.state = self.state >> 1;
        let n_bits = size_of::<T>() * 8;
        self.state = self.state | new_bit << (n_bits - 1);
        output_bit
    }
}


fn main() {
    let seed: u16 = 0xACE1;
    let taps: Vec<usize> = vec![15, 14, 13, 11];
    let mut lfsr = Lfsr::new(seed, taps);
    for _ in 0..100
    {
        let next = lfsr.next();
        print!("{}", next);
    }
}
