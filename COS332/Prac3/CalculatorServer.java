import java.io.*;
import java.net.*;
import java.net.URLEncoder;

public class CalculatorServer {
    private static final int PORT = 55555;
    private static String currentState = "0";

    public static void main(String[] args) {
        try (ServerSocket serverSocket = new ServerSocket(PORT)) {
            System.out.println("Calculator server running on port " + PORT);

            while (true) {
                Socket clientSocket = serverSocket.accept();
                handleRequest(clientSocket);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private static void handleRequest(Socket clientSocket) {
        try (BufferedReader in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
             PrintWriter out = new PrintWriter(clientSocket.getOutputStream(), true)) {

            String requestLine = in.readLine();
            if (requestLine == null || !requestLine.startsWith("GET")) {
                sendErrorResponse(out, 400, "Bad Request");
                return;
            }

            String path = requestLine.split(" ")[1].substring(1);
            if(path.equals("a"))
            {
              path = ".";
            }
            if(path.equals("b"))
            {
              path = "/";
            }
            switch (path) {
                case "":
                    sendCalculatorResponse(out);
                    break;
                case "=":
                    evaluateExpression();
                    sendCalculatorResponse(out);
                    break;
                case "AC":
                    currentState = "0";
                    sendCalculatorResponse(out);
                    break;
                case "DEL":
                    clearLastCharacter();
                    sendCalculatorResponse(out);
                    break;
                default:
                    if (isValidInput(path)) {
                        updateCalculatorState(path);
                        sendCalculatorResponse(out);
                    } else {
                        sendErrorResponse(out, 404, "Not Found");
                    }
                    break;
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private static boolean isValidInput(String input) {
        return input.matches("[0-9+\\-*/.()]");
    }

    private static void updateCalculatorState(String input) {
        if (currentState.equals("0") && input.matches("[0-9]")) {
            currentState = input;
        } else {
            currentState += input;
        }
    }

    private static void evaluateExpression() {
        try {
            double result = evaluate(currentState);
            currentState = String.valueOf(result);
        } catch (ArithmeticException e) {
            currentState = "Error: Division by zero";
        } catch (Exception e) {
            currentState = "Error";
        }
    }

    private static void clearLastCharacter() {
        if (currentState.length() > 1) {
            currentState = currentState.substring(0, currentState.length() - 1);
        } else {
            currentState = "0";
        }
    }

    private static double evaluate(String expression)
    {
        return new Object() {
            int pos = -1, ch;

            void nextChar() {
                ch = (++pos < expression.length()) ? expression.charAt(pos) : -1;
            }

            boolean eat(int charToEat) {
                while (ch == ' ') nextChar();
                if (ch == charToEat) {
                    nextChar();
                    return true;
                }
                return false;
            }

            double parse() {
                nextChar();
                double x = parseExpression();
                if (pos < expression.length()) throw new RuntimeException("Unexpected: " + (char) ch);
                return x;
            }

            double parseExpression() {
                double x = parseTerm();
                for (;;) {
                    if (eat('+')) x += parseTerm();
                    else if (eat('-')) x -= parseTerm();
                    else return x;
                }
            }

            double parseTerm() {
                double x = parseFactor();
                for (;;) {
                    if (eat('*')) {
                        double factor = parseFactor();
                        x *= factor;
                    } else if (eat('/')) {
                        double divisor = parseFactor();
                        if (divisor == 0) {
                            throw new ArithmeticException("Division by zero");
                        }
                        x /= divisor;
                    } else return x;
                }
            }

            double parseFactor() {
                if (eat('+')) return parseFactor();
                if (eat('-')) return -parseFactor();

                double x;
                int startPos = this.pos;
                if (eat('(')) {
                    x = parseExpression();
                    eat(')');
                } else if ((ch >= '0' && ch <= '9') || ch == '.') {
                    while ((ch >= '0' && ch <= '9') || ch == '.') nextChar();
                    x = Double.parseDouble(expression.substring(startPos, this.pos));
                } else {
                    throw new RuntimeException("Unexpected: " + (char) ch);
                }
                return x;
            }
        }.parse();
    }

    private static void sendCalculatorResponse(PrintWriter out) {
        String htmlResponse = "<html><head>" +
                "<style>" +
                "body { font-family: Arial, sans-serif; background-color: #f4f4f4; display: flex; justify-content: center; align-items: center; height: 100vh; margin: 0; }" +
                ".calculator { background-color: #222; padding: 20px; border-radius: 10px; box-shadow: 0 0 20px rgba(0, 0, 0, 0.5); width: 350px; text-align: center; }" +
                ".display { background-color: #333; color: #fff; font-size: 2em; text-align: right; padding: 10px; border-radius: 5px; margin-bottom: 10px; }" +
                ".buttons { display: grid; gap: 10px; }" +
                ".row-1, .row-2, .row-3, .row-4 { grid-template-columns: repeat(5, minmax(50px,2fr)); }" +
                ".row { display: grid; gap: 10px; margin-bottom: 10px; }" +
                ".buttons a { background-color: #444; color: #fff; text-decoration: none; padding: 20px; text-align: center; border-radius: 5px; font-size: 1.5em; display: block; }" +
                ".buttons a:hover { background-color: #666; }" +
                "</style>" +
                "</head><body>" +
                "<div class=\"calculator\">" +
                "<div class=\"display\">" + currentState + "</div>" +
                "<div class=\"buttons\">" +
                "<div class=\"row row-1\">" +
                generateLink("7") + generateLink("8") + generateLink("9") + generateLink("DEL") + generateLink("AC") +
                "</div>" +
                "<div class=\"row row-2\">" +
                generateLink("4") + generateLink("5") + generateLink("6") + generateLink("*") + generateLink("b","/") +
                "</div>" +
                "<div class=\"row row-3\">" +
                generateLink("1") + generateLink("2") + generateLink("3") + generateLink("+") + generateLink("-") +
                "</div>" +
                "<div class=\"row row-4\">" +
                generateLink("0") + generateLink("a",".") + generateLink("=") +
                "</div>" +
                "</div>" +
                "</div>" +
                "</body></html>";

        out.println("HTTP/1.1 200 OK");
        out.println("Content-Type: text/html");
        out.println("Content-Length: " + htmlResponse.length());
        out.println();
        out.println(htmlResponse);
    }

    private static String generateLink(String value, String display) 
    {
      return "<a href=\"" + value + "\">" + display + "</a>";
    }

    private static String generateLink(String value) {
        return generateLink(value, value);
    }

    private static void sendErrorResponse(PrintWriter out, int statusCode, String statusMessage) {
        String response = "<html><body><h1>Error " + statusCode + ": " + statusMessage + "</h1></body></html>";
        out.println("HTTP/1.1 " + statusCode + " " + statusMessage);
        out.println("Content-Type: text/html");
        out.println("Content-Length: " + response.length());
        out.println();
        out.println(response);
    }
}