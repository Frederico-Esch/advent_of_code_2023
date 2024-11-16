use std::iter::zip;

fn main() {

    let data : Vec<Vec<i64>> = include_str!("../input.txt")
        .lines()
        .map(|c| c.split_whitespace()
                  .map(|num_str| num_str.parse::<i64>().unwrap())
                  .collect::<Vec<i64>>()
        )
        .collect();

    let result : i64 = data
        .iter()
        .map(|nums|{
            let mut value = *nums.last().unwrap_or(&0);

            let mut reductions = zip(&nums[..], &nums[1..]).map(|(&a, &b)| b-a).collect::<Vec<i64>>();
            while !reductions.iter().all(|&v| v == 0) {
                value += *reductions.last().unwrap();
                reductions = zip(&reductions[..], &reductions[1..]).map(|(&a, &b)| b-a).collect::<Vec<i64>>();
            }
            value
         })
         .sum();

    println!("{:?}", result);
}
