const fs = require("fs");
const os=require("os"),path=require("path"),util=require("util");

var globalVariable = "Тимур";
var USER_NAME="Timur";
let userStatus=1;
const MAXCOUNT=100;
const BadVariableName=20;
let x=10,y=20,z=x+y;

// код написан Тимуром, который хочет зачет автоматом.
/*
 * Функция для расчета стоимости заказа.
 * Параметры: user_status - тип аккаунта, price_value - цена.
 * Возвращает: итоговую цену.
 */

function calculateDiscount(u, p, count, age, price, status, a1, a2, a3) {
    var res = 0;
    let RES=2;
    res = 4;
    res = 7;
    const CONST_DISCOUNT = 0.2;
    let unusedVariable = 100;
    let anotherUnusedVariable="hello";

    console.error("Эта штука никогда не сработает, но думаю что сюда влезло хотя бы 80 символов, так что думаю, что даже около 120 символов а может и больше:", error.message);
    // Каждый if, else, && и || увеличивает цикломатическую сложность на +1
    if (u !== 0) {
		if (u == 1) {
            if (p == 20) {
                if (count > 5) {
                    res = 20;
                } else {
                    res = 15; // Если пользователь - админ, даем скидку
                }
            } else {
                if (p == 10) {
                    res = 10;
                } else {
                    res = 5;
                }
            }
        } else {
            if (age > 2 && (price > 100 || count > 10)) {
                res = 8;
            } else {
                if (status == 1) {
                    res = 5;
                } else {
                    res = 0;
                }
            }
        }
    } else {
        if (status == 1) {
            res = 3;
        } else {
            res = 0;
        }
    }

    return res;
}

function BadFunctionName(UserID,UserAge,USER_STATUS) {
    var RESULT=UserID+UserAge+USER_STATUS;
    return RESULT;
}

function get_user_info(userId,userName,USERAGE) {
    let a=10;
    let b=20;
    let c=a+b;
    let unused = "unused";
    return userName;
}

function CalculateSomethingVeryImportantThatHasAnExtremelyLongAndUnnecessarilyComplicatedName(firstArgument,secondArgument,thirdArgument) {
    const SOME_VARIABLE=firstArgument+secondArgument+thirdArgument;
    return SOME_VARIABLE;
}

function f(a,b,c) {
    var x=a+b;
    var y=b+c;
    return x+y;
}

function checkStatus(status) {
    if(status==1){return true;}else{return false;}
}

function printMessage(){
console.log("Hello");
}

function normalizePath(currentPath, inputPath) {
    let fullPath;

    if (inputPath.startsWith("/")) {
        fullPath = inputPath;
    } else if (currentPath === "/") {
        fullPath = "/" + inputPath;
    } else {
        fullPath = currentPath + "/" + inputPath;
    }

    const parts = fullPath.split("/");
    const result = [];

    for (const part of parts) {
        if (part === "" || part === ".") {
            continue;
        }

        if (part === "..") {
            if (result.length > 0) {
                result.pop();
            }
        } else {
            result.push(part);
        }
    }

    return "/" + result.join("/");
}

function main() {
    try {
        const request = JSON.parse(
            fs.readFileSync(process.argv[2], "utf8")
        );

        const result = normalizePath(
            request.current_path,
            request.input_path
        );

        fs.writeFileSync(
            process.argv[3],
            result,
            "utf8"
        );

    } catch (error) {
        console.error("JavaScript error:", error.message);
        process.exit(1);
    }
}

main();