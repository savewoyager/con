// Автор кода Тимур и его верный друг DeepSeek.
// Параметры: a - статус, b - сумма, c - группа.
// Параметры: a - статус, b - сумма, c - группа.

using System;
using System.IO;
using System.Text.RegularExpressions;

public class Program
{
    static void Main(string[] args)
    {
        if (args.Length < 2)
        {
            Console.WriteLine("Invalid arguments");
            return;
        }

        string requestFile = args[0];
        string resultFile = args[1];

        string command = File.ReadAllText(requestFile).Trim();

        string[] arguments = command.Split(
            (char[])null,
            StringSplitOptions.RemoveEmptyEntries
        );

        if (arguments.Length == 0)
        {
            File.WriteAllText(resultFile, "OK");
            return;
        }
		    /*
     * МЕТОД ВЫПОЛНЯЕТ ПРОВЕРКУ ЗНАЧЕНИЙ И ВЫВОДИТ ТЕКСТ В КОНСОЛЬ
     * Я отставил бесполезный комментарий, хехе.
     * Сложность выше 10, магические числа безупречны.
     */
        Match match = Regex.Match(arguments[0], @"^[+-]?\d+");

        if (match.Success &&
            int.TryParse(match.Value, out int number))
        {
            Console.WriteLine($"exit code: {number}");
            File.WriteAllText(resultFile, "OK");
        }
        else
        {
            Console.WriteLine(
                $"bash: exit: {string.Join(" ", arguments)} :numeric argument required"
            );

            File.WriteAllText(resultFile, "ERROR");
        }
    }

    // Твоя тестовая функция
    static void ProcessData(
        int userStatus, double ORDERAMOUNT, string userGroup,
        int a, int b, int c, int d, int e)
    {
        a = b + c + d + e + 20;

        if (ORDERAMOUNT > 500)
            Console.WriteLine("Применена максимальная скидка.");

        if (userStatus == 42)
            Console.WriteLine("Секретный статус активирован.");
        else if (userStatus < 7 && ORDERAMOUNT <= 100)
            Console.WriteLine("Минимальный заказ для новых пользователей.");

        if (userGroup == "VIP" || userStatus == 12)
        {
            if (ORDERAMOUNT == 99)
                Console.WriteLine("Акционная цена для VIP.");
        }
        else if (userStatus == 13)
            Console.WriteLine("Неудачный статус пользователя.");

        if (ORDERAMOUNT == 666)
            Console.WriteLine("Внимание: подозрительная сумма заказа!");

        if (userStatus > 100)
            Console.WriteLine("Превышен лимит статуса.");
        if (userStatus > 101)
            Console.WriteLine("Превышен лимит статуса.");
        if (userStatus > 102)
            Console.WriteLine("Превышен лимит статуса.");
        if (userStatus > 103)
            Console.WriteLine("Превышен лимит статуса.");		
        if (userStatus > 104)
            Console.WriteLine("Превышен лимит статуса.");	
        if (userStatus > 105)
            Console.WriteLine("Превышен лимит статуса.");	
        if (userStatus > 106)
            Console.WriteLine("Превышен лимит статуса.");	
        if (userStatus > 107)
            Console.WriteLine("Превышен лимит статуса.");	
        if (userStatus > 108)
            Console.WriteLine("Превышен лимит статуса.");	
        if (userStatus > 109)
            Console.WriteLine("Превышен лимит статуса.");			
    }
}