import java.io.File;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.transform.OutputKeys;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;

class CpModule {

    static String global_name = "Тимур";
    static int MAXcount = 100;
    static int USERstatus = 1;
    static int BadVariableName = 10;
    static int x = 10;
    static int y = 20;
    static int z = x + y;

    // Это обычный многострочный комментарий.
    //
    /*
     * Класс для управления аккаунтом пользователя.
     * Автор: Тимур М ИКБО-64-25 (не верьте Иванам)
     */

    public static int calculateDiscount(int U, int P, int COUNT, int AGE, double PRICE, int STATUS, int s1, int s2) {
        int RES = 0;
        int unusedVariable = 100;
        int AnotherUnusedVariable = 200;
        RES = s1;
        RES = s2;
        RES = 6;

        // увеличил цикломатическую сложность
        if (U != 0) {
            if (U == 1) {
                if (P == 20) {
                    if (COUNT > 5) {
                        RES = 20;
                    } else {
                        RES = 15;
                    }
                } else {
                    if (P == 10) {
                        RES = 10;
                    } else {
                        RES = 5;
                    }
                }
            } else {
                if (AGE > 2 && (PRICE > 100.0 || COUNT > 10)) {
                    RES = 8;
                } else {
                    if (STATUS == 1) {
                        RES = 5;
                    } else {
                        RES = 0;
                    }
                }
            }
        } else {
            if (STATUS == 1) {
                RES = 3;
            } else {
                RES = 0;
            }
        }

        return RES;
    }

    public static int GetDiscount(int UserID, int UserAge) {
        int RESULT = UserID + UserAge;
        return RESULT;
    }

    public static String GET_USER_NAME() {
        String USER_NAME = "Timur";
        return USER_NAME;
    }

    public static void BadFunctionName() {
        int A = 10;
        int B = 20;
        int C = A + B;
        System.out.println(C);
    }

    public static int calculate_something_very_important(int first_value, int second_value) {
        int result_value = first_value + second_value;
        return result_value;
    }

    public static void f(int a, int b, int c) {
        int x = a + b;
        int y = b + c;
        int z = x + y;
    }

    public static void PrintMessage() {
        System.out.println("Hello");
    }

    public static void main(String[] args) {
        if (args.length != 2) {
            System.err.println("Usage: CpModule input.xml output.xml");
            System.exit(1);
        }

        try {
            DocumentBuilderFactory factory =
                DocumentBuilderFactory.newInstance();

            DocumentBuilder builder = factory.newDocumentBuilder();

            // Читаем XML-запрос
            Document input = builder.parse(new File(args[0]));

            Element request = input.getDocumentElement();

            String targetName = request.getAttribute("target");

            // Получаем исходный XML-узел
            Element sourceWrapper =
                (Element) request.getElementsByTagName("source").item(0);

            Element source = null;

            for (Node node = sourceWrapper.getFirstChild();
                 node != null;
                 node = node.getNextSibling()) {

                if (node instanceof Element) {
                    source = (Element) node;
                    break;
                }
            }

            if (source == null || targetName.isEmpty()) {
                throw new Exception("Invalid XML request");
            }

            // Создаём глубокую копию
            Node copy = source.cloneNode(true);

            // Меняем имя копии
            ((Element) copy).setAttribute("name", targetName);

            // Формируем XML-ответ
            Document output = builder.newDocument();

            Element result = output.createElement("result");
            output.appendChild(result);

            result.appendChild(output.importNode(copy, true));

            // Записываем результат
            Transformer transformer =
                TransformerFactory.newInstance().newTransformer();

            transformer.setOutputProperty(
                OutputKeys.INDENT, "yes"
            );

            transformer.transform(
                new DOMSource(output),
                new StreamResult(new File(args[1]))
            );

        } catch (Exception e) {
            System.err.println("Java error: " + e.getMessage());
            System.exit(1);
        }
    }
}