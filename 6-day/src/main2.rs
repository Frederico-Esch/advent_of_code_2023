fn main() {
    let input = include_str!("../input.txt");

    let lines : Vec<&str> = input.lines().collect();

    let time :u64 = lines[0].split(":")
        .skip(1).next().unwrap()
        .trim().chars().filter(|c| c.is_numeric()).collect::<String>()
        .parse().unwrap();

    let record :u64 = lines[1].split(":")
        .skip(1).next().unwrap()
        .trim().chars().filter(|c| c.is_numeric()).collect::<String>()
        .parse().unwrap();

    let mut sol = 0;
    let mut win = false;
    for t in 1..time {
        let result = t*(time-t);
        if result > record { sol += 1; win = true; }
        else { if win { break; } }
    }

    println!("{}", sol);
}
