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
            let mut reductions = vec![nums.clone()];
            let mut last = zip(&nums[..], &nums[1..]).map(|(&a, &b)| b-a).collect::<Vec<i64>>();
            while !last.iter().all(|&v| v == 0) {
                reductions.push(last.clone());
                last = zip(&last[..], &last[1..]).map(|(&a, &b)| b-a).collect::<Vec<i64>>();
            }
            reductions.iter().rfold(0, |acc, v| v.first().unwrap() - acc)
         })
         .sum();

    println!("{:?}", result);
}
