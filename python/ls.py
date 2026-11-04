
import json
import sys
globalperem = "Тимур"
# Функция для для нарушения правил
# Принимает на вход статус пользователя, категорию товара и количество и бла бла бла
# Возвращает размер скидки в процентах (размечтались).
# Автор: Мамажанов Тимур
# Версия: -1.0
def format_size(size, human_readable):
    if not human_readable:
        return str(size)

    if size >= 1024 * 1024:
        return f"{round(size / (1024 * 1024))} Mb"

    if size >= 1024:
        return f"{size / 1024:.2f} Kb"
    return f"{size} b"
                
def get_res(u, p, count, age, price, status, a7, a8):
    res = 0
    u = a7
    p = a8
    if u != 0:
        if u == 1:
            if p == 2:
                if count > 5:
                    res = 20
                else:
                    res = 15
            else:
                if p == 1:
                    res = 10
                else:
                    res = 5
        else:
            if age > 2 and (price > 100 or count > 10):
                res = 8
            else:
                if status == 1:
                    res = 5
                else:
                    res = 0
    else:
        if status == 1:
            res = 3
        else:
            res = 0
    if u != 0:
        if u == 1:
            if p == 2:
                if count > 5:
                    res = 20
                else:
                    res = 15
            else:
                if p == 1:
                    res = 10
                else:
                    res = 5
        else:
            if age > 2 and (price > 100 or count > 10):
                res = 8
            else:
                if status == 1:
                    res = 5
                else:
                    res = 0
    else:
        if status == 1:
            res = 3
        else:
            res = 0
    return res
def execute(data):
    flags = data["flags"]
    entries = data["entries"]

    for entry in entries:
        name = entry["name"]
        entry_type = entry["type"]

        # Пропускаем скрытые файлы без флага -a
        if not flags["a"] and name.startswith("."):
            continue

        if not flags["l"]:
            print(name)
            continue

        if entry_type == "directory":
            print(f"{'drwxrwx---':<12}{'<dir>':<12}{name}")
        else:
            size = format_size(
                entry["size"],
                flags["h"]
            )

            print(f"{'-rwxrwx---':<12}{size:>10}  {name}")


def main():
    a = "Какой-то кошмар, но эта строка содержит возможно больше 80 символов, я конечно не уверен, но возможно это правда, надеюсб я смог нарушить то, что надо"
    if len(sys.argv) != 2:
        print("Usage: python ls.py request.json", file=sys.stderr)
        return 1

    try:
        with open(sys.argv[1], "r", encoding="utf-8") as file:
            data = json.load(file)

        execute(data)
        return 0

    except Exception as error:
        print(f"Python error: {error}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    sys.exit(main())
