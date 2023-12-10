//use std::collections::binary_heap;

use std::collections::{HashMap, BinaryHeap};


#[allow(dead_code)]
#[derive(Debug, PartialEq, Eq, Hash, Clone, Copy)]
enum Card {
    A, K, Q, J, T,
    Value(u8)
}

#[derive(Debug, Eq, PartialEq)]
#[allow(dead_code)]
enum HandType {
    FiveOfAKind,
    FourOfAKind,
    FullHouse,
    ThreeOfAKind,
    TwoPair,
    OnePair,
    HighCard
}

#[allow(dead_code)]
#[derive(Debug, PartialEq, Eq)]
struct Hand {
    cards: Vec<Card>,
    bid: u32,
}

impl Ord for Hand {
    fn cmp(&self, other: &Self) -> std::cmp::Ordering {
        if self.hand_type() != other.hand_type() {
            self.hand_type().to_number().cmp(&other.hand_type().to_number())
        }
        else {
            for (m, o) in std::iter::zip(&self.cards, &other.cards) {
                if m != o { return m.to_number().cmp(&o.to_number()); }
            }
            std::cmp::Ordering::Equal
        }
    }
}

impl PartialOrd for Hand {
    fn partial_cmp(&self, other: &Self) -> Option<std::cmp::Ordering> { Some(self.cmp(other)) }
}

impl Hand {
    fn from_str(cards_str: &str, bid_str: &str) -> Hand {
        Hand { cards: cards_str.chars().map(Card::from_char).collect(), bid: bid_str.parse().unwrap() }
    }

    fn hand_type(&self) -> HandType {
        let mut cards: HashMap<Card, i32> = HashMap::new();
        for c in &self.cards {
            if let Some (val) = cards.get_mut(&c) {
                 *val += 1;
            }
            else {
                cards.insert(*c, 1);
            }
        }

        let mut hand_type = None;
        for &a in cards.values() {
            if a == 5 {
                hand_type = Some(HandType::FiveOfAKind);
                break;
            }
            else if a == 4 {
                hand_type = Some(HandType::FourOfAKind);
                break;
            }
            else if a == 3 {
                if hand_type.is_some() {
                    hand_type = Some(HandType::FullHouse);
                    break;
                }
                else {
                    hand_type = Some(HandType::ThreeOfAKind);
                }
            } else if a == 2 {
                if let Some(ht) = hand_type {
                    hand_type = if ht == HandType::OnePair {
                         Some(HandType::TwoPair)
                    }
                    else {
                        Some(HandType::FullHouse)
                    };
                    break;
                }
                else {
                    hand_type = Some(HandType::OnePair); //dont break because it could be two_pair or fullhouse
                }
            }
        }

        if hand_type.is_none() {
            hand_type = Some(HandType::HighCard);
        }

        return hand_type.unwrap();
    }
}

impl Card {
    fn from_char(v: char) -> Card {
        match v {
            'A' => Card::A,
            'K' => Card::K,
            'Q' => Card::Q,
            'J' => Card::J,
            'T' => Card::T,
            _ => Card::Value(v.to_digit(10).unwrap() as u8)
        }
    }

    fn to_number(&self) -> i32 {
        match self {
            Card::A => 20,
            Card::K => 19,
            Card::Q => 18,
            Card::J => 17,
            Card::T => 16,
            Card::Value(v) => *v as i32
        }
    }
}

impl HandType {
    fn to_number(&self) -> u32 {
        match self {
            HandType::FiveOfAKind => 6,
            HandType::FourOfAKind => 5,
            HandType::FullHouse => 4,
            HandType::ThreeOfAKind => 3,
            HandType::TwoPair => 2,
            HandType::OnePair => 1,
            HandType::HighCard => 0
        }
    }
}

fn main() {

    let input = include_str!("../input.txt");

    let maos = input
        .lines()
        .map(|l| {
            let strs = l.split_whitespace().collect::<Vec<&str>>();
            Hand::from_str(strs[0], strs[1])
        })
        .collect::<BinaryHeap<Hand>>().into_sorted_vec();

    for (i, m) in std::iter::zip(0..maos.len(), &maos) {
        println!("{:?} -> bid {} -> type {:?} -> rank {}", m.cards, m.bid, m.hand_type(), i as u32 + 1);
    }

    let result: u32 = std::iter::zip(0..maos.len(), &maos)
        .map(|(i, m)| (i as u32 + 1)*m.bid)
        .sum();
    println!("{result}");
}
