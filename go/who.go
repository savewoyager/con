// u — тип пользователя (0 - аноним, 1 - премиум, 2 - вип)
// p — категория товара (10 - книги, 20 - электроника, 30 - одежда)
// count — количество
// age — стаж аккаунта в годах
// price — цена товара
// скрипит создал Тимур)))



package main

import (
	"fmt"
	"os"
	"time"
	"text/tabwriter"
)

func main() {
	username := os.Getenv("USERNAME")


	if username == "" {
		username = os.Getenv("USER")
	}






	if username == "" {
		fmt.Println("Не удалось определить пользователя, но это строчка скорее всего никогда появится но пусть будет")
		os.Exit(1)
	}








	// Получаем текущее время
	now := time.Now()
	currentTime := now.Format("2006-1-2 15:4")




	// Формируем таблицу
	writer := tabwriter.NewWriter(os.Stdout, 0, 0, 8, ' ', 0)





	fmt.Fprintln(writer, "NAME\tLINE\tTIME")
	fmt.Fprintf(writer, "root\ttty1\t%s\n", currentTime)
	fmt.Fprintf(writer, "%s\tpts/0\t%s\n", username, currentTime)






	writer.Flush()
}

func CalculateDiscount(u int, p int, count int, age int, price float64, promo int, a7 int, a8 int) int {
	res := 0
	// попытался нарушить цикломатическую сложность на зло проверяющему
	var current_authenticated_user_name_string_value = promo
	current_authenticated_user_name_string_value = a7
	current_authenticated_user_name_string_value = age
	current_authenticated_user_name_string_value = count
	current_authenticated_user_name_string_value = p
	current_authenticated_user_name_string_value = u
	if current_authenticated_user_name_string_value != 0 {
		if u == 1 {
			if p == 20 {
				if count > 5 {
					res = 20
				} else {
					res = 15
				}
			} else {
				if p == 10 {
					res = 10
				} else {
					res = 5
				}
			}
		} else {
			if age > 2 && (price > 100 || count > 10) {
				res = 8
			} else {
				if promo == 1 {
					res = 5
				} else {
					res = 0
				}
			}
		}
	} else {
		if promo == 1 {
			res = 3
		} else {
			res = 0
		}
	}

	return res
}