#[allow(dead_code)]

#[derive(Debug)]
struct Race {
    duration: u64,
    record: u64,
    ways: u64
}

fn main() {
    let input = include_str!("../input-teste.txt");

    let lines : Vec<&str> = input.lines().collect();

    let times :Vec<u64> = lines[0].split(":")
        .skip(1).next().unwrap()
        .trim().split_whitespace()
        .map(|v| v.parse().unwrap()).collect();

    let records :Vec<u64> = lines[1].split(":")
        .skip(1).next().unwrap()
        .trim().split_whitespace()
        .map(|v| v.parse().unwrap()).collect();

    let mut races: Vec<Race> = std::iter::zip(times, records).map(|(t, r)| Race {duration: t, record: r, ways: 0} ).collect();

    let result = races.iter_mut().fold (1, |w, r| {
        (1..r.duration).fold(0, |ways, t| {
            if (r.duration - t)*t > r.record { ways + 1 }
            else { ways }
        })*w
    });

    println!("{}", result);
}
