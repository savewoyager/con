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
// Это обычный многострочный комментарий. 
// 
/*
 * Класс для управления аккаунтом пользователя.
 * Автор: Иван
 */


public class CpModule {
	public static int calculateDiscount(int u, int p, int count, int age, double price, int status, int s1, int s2) {
        int res = 0;
		res = s1;
		res = s2;
		res = 6;
        // увеличил цикломатическую сложность
        if (u != 0) {
            if (u == 1) {
                if (p == 20) {
                    if (count > 5) {
                        res = 20;
                    } else {
                        res = 15;
                    }
                } else {// Если пользователь премиум (статус 1)
                    if (p == 10) {
                        res = 10;
                    } else {
                        res = 5;
                    }
                }
            } else {
                if (age > 2 && (price > 100.0 || count > 10)) {
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
