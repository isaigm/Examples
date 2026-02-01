use bitvec::macros::internal::funty::Fundamental;
use bitvec::prelude::*;

fn create_bit_vec_from_number(n: u32) -> BitVec<u8>
{
    let mut bit_vec : BitVec<u8> = BitVec::new();
    let mut k = n;
    while k > 0
    {
        bit_vec.push(k % 2 == 1);

        k >>= 1;
    }
    bit_vec
}
fn to_number(bits: &BitVec<u8>) -> u32
{
    let mut result : u32 = 0;
    for (idx, bit) in bits.iter().enumerate()
    {
        result |= (bit.as_u8() << idx) as u32;
    }
    result
}
fn process_by_four(bcd: &mut BitVec<u8>)
{
    let digits = bcd.len() / 4;
    for i in 0..digits
    {
        let idx = i + 1;
        let block = &mut bcd[(idx - 1) * 4..idx * 4];
        let mut number = to_number(block.to_bitvec().as_ref());
        if number >= 5
        {
            number += 3;
            let mut new_block = create_bit_vec_from_number(number);
            while new_block.len() < 4
            {
                new_block.push(false);
            }
            block.copy_from_bitslice(&new_block);
        }
    }
}
fn double_dabble(n: u32)
{
    let mut bits = create_bit_vec_from_number(n);
    let mut bcd: BitVec<u8>  = BitVec::new();
    let n_bits = bits.len();
    bcd.resize( (4.0 * (n_bits as f32 / 3.0).ceil()) as usize, false);
    while bits.len() > 0
    {
        match bits.last()
        {
            Some(bit) => {
                process_by_four(&mut bcd);
                bcd.shift_right(1);
                bcd.set(0, *bit);
            },
            None => {}
        }
        bits.pop();
    }
    println!("{}", bcd);
}

fn main() {
    double_dabble(1234);
}

