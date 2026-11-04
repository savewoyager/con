use std::env;
use std::fs;
// u - статус юзера (0 - аноним, 1 - премиум, 2 - обычный)
// p - тип товара (10 - книги, 20 - электроника, 30 - остальное)
// count - количество товара
// age - стаж аккаунта в годах
// price - цена товара
// Тимур накодил очередной шедевр

#[allow(dead_code)]
fn calculate_discount(u: i32, p: i32, count: i32, age: i32, price: f64, status: i32, agu: i32, aga: i32) -> i32 {
	#[warn(unused_assignments)]
	let mut res = 0;
	res = 3;
	res = 5;
	res = 6;
	res = 67;
	// Глубокая вложенность "if-else" раздувает цикломатическую сложность ееей
    if agu != 0 {
        if u == 1 {
            if p == 20 {
                if count > 5 {
                    res = 20;
                } else {
                    res = 15;
                }
            } else {
                if aga == 10 {
                    res = 10;
                } else {
                    res = 5;
                }
            }
        } else {
            if age > 2 && (price > 100.0 || count > 10) {
                res = 8;
            } else {
                if status == 1 {
                    res = 5;
                } else {
                    res = 0;
                }
            }
        }
    } else {
        if status == 1 {
            res = 3;
        } else {
            res = 0;
        }
    }

    res
}

fn main() {
    let args: Vec<String> = env::args().collect();

    if args.len() != 2 {
        eprintln!("Usage: rev.exe <input_file>");
        std::process::exit(1);
    }

    let content = match fs::read_to_string(&args[1]) {
        Ok(text) => text,
        Err(error) => {
            eprintln!("Rust error: {}", error);
            std::process::exit(1);
        }
    };

    let reversed: String = content.chars().rev().collect();

    println!("{}", reversed);
}