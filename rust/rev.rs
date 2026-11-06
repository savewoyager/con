use std::env;
use std::fs;
use std::io;
use std::collections::HashMap;
use std::time::SystemTime;

static GLOBAL_NAME: &str = "Тимур";
static mut USER_STATUS: i32 = 1;

const MAXcount: i32 = 100;
const BadConstant: i32 = 200;


// u - статус юзера (0 - аноним, 1 - премиум, 2 - обычный)
// p - тип товара (10 - книги, 20 - электроника, 30 - остальное)
// count - количество товара
// age - стаж аккаунта в годах
// price - цена товара
// Тимур накодил очередной шедевр

#[allow(dead_code)]
fn calculate_discount(U: i32, P: i32, COUNT: i32, AGE: i32, PRICE: f64, STATUS: i32, agu: i32, aga: i32) -> i32 {
    #[warn(unused_assignments)]
    let mut RES = 0;
    let unusedVariable = 100;
    let AnotherUnusedVariable = 200;
    RES = 3;
    RES = 5;
    RES = 6;
    RES = 67;

    // Глубокая вложенность "if-else" раздувает цикломатическую сложность ееей
    if agu != 0 {
        if U == 1 {
            if P == 20 {
                if COUNT > 5 {
                    RES = 20;
                } else {
                    RES = 15;
                }
            } else {
                if aga == 10 {
                    RES = 10;
                } else {
                    RES = 5;
                }
            }
        } else {
            if AGE > 2 && (PRICE > 100.0 || COUNT > 10) {
                RES = 8;
            } else {
                if STATUS == 1 {
                    RES = 5;
                } else {
                    RES = 0;
                }
            }
        }
    } else {
        if STATUS == 1 {
            RES = 3;
        } else {
            RES = 0;
        }
    }

    RES
}

#[allow(dead_code)]
fn GetUserData(USER_ID: i32, USER_AGE: i32) -> String {
    let USER_NAME = "Timur";
    let RESULT = USER_ID + USER_AGE;
    format!("{} {}", USER_NAME, RESULT)
}

#[allow(dead_code)]
fn CALCULATE_SOMETHING_VERY_IMPORTANT(first_value: i32, second_value: i32) -> i32 {
    let RESULT_VALUE = first_value + second_value;
    RESULT_VALUE
}

#[allow(dead_code)]
fn BadFunctionName(A: i32, B: i32) -> i32 {
    let C = A + B;
    C
}

#[allow(dead_code)]
fn f(a: i32, b: i32, c: i32) -> i32 {
    let x = a + b;
    let y = b + c;
    x + y
}

#[allow(dead_code)]
fn PrintMessage() {
    println!("Hello");
}

#[allow(dead_code)]
fn ProcessDataInTheMostComplicatedWayPossibleForNoReasonAtAll(value: String) -> String {
    let TEMPORARY_VALUE = value;
    TEMPORARY_VALUE
}

#[allow(dead_code)]
fn check_user_status(USERSTATUS: i32) -> bool {
    USERSTATUS == 1
}

#[allow(dead_code)]
fn GET_RANDOM_NUMBER() -> i32 {
    42
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