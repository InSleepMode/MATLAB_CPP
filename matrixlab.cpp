#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include "matrixClass.h"

struct Expression {
	std::string element; //числа, переменные или команды
	std::vector<Expression> arguments; //выражения
	int lineNumber;

	bool isSimple() const {
		return arguments.empty();
	}
};

bool isCommand(const std::string& com) {
	static const std::vector<std::string> commands =
	{
		"=", "+", "-", ".*", "./", "*", "/",
		"zeros", "ones", "eye", "linspace", "vertcat", "horzcat",
		"transpose", "det", "inv", "sum", "prod", "min", "max", "disp"
	};

	return std::find(commands.begin(), commands.end(), com) != commands.end();
}

Expression parseExpression(const std::string& str, int& index, int lineNumber) {

	
	if (str[index] == '(') {
		index++;

		std::string token;
		Expression expr;
		expr.lineNumber = lineNumber;

		while (index < str.size() && (str[index] == ' ' || str[index] == '\n')) {
			index++;
		}

		while (index < str.size() && str[index] != ')' && str[index] != '(' && str[index] != ' ' && str[index] != '\n') {
			token += str[index];
			index++;
		}
		expr.element = token;

		while (index < str.size()) {

			while (index < str.size() && (str[index] == ' ' || str[index] == '\n')) {
				index++;
			}

			if (str[index] == ')') {
				index++;
				break;
			}

			expr.arguments.push_back(parseExpression(str, index, lineNumber));
		}

		return expr;
		
	}
	
	std::string symb;
	while (index < str.size() && str[index] != ')' && str[index] != '(' && str[index] != ' ' && str[index] != '\n') {
		symb += str[index];
		index++;
	}

	Expression expr;
	expr.element = symb;
	expr.lineNumber = lineNumber;
	return expr;
	
}

Matrix solveExpression(const Expression& expression, std::map<std::string, Matrix>& variables, std::ofstream& outFile) {

	if (expression.isSimple()) {
		if (variables.find(expression.element) != variables.end()) {
			return variables[expression.element];
		}
		Fraction drob(BigInteger(expression.element), BigInteger(1));
		return Matrix(drob);
	}

	std::string element = expression.element;

	if (element == "=") {
		if (expression.arguments.size() != 2) {
			throw std::invalid_argument("You have less or more than 2 arguments");
		}
		std::string varName = expression.arguments[0].element;
		Matrix value = solveExpression(expression.arguments[1], variables, outFile);
		variables[varName] = value;
		return value;
	}
	if (element == "+") {
		if (expression.arguments.size() != 2) {
			throw std::invalid_argument("You have less or more than 2 arguments");
		}
		Matrix A = solveExpression(expression.arguments[0], variables, outFile);
		Matrix B = solveExpression(expression.arguments[1], variables, outFile);
		return A.add(B);
	}
	if (element == "-") {
		if (expression.arguments.size() != 1 && expression.arguments.size() != 2) {
			throw std::invalid_argument("You can evaluate from 1 to 2 arguments");
		}
		Matrix A = solveExpression(expression.arguments[0], variables, outFile);
		if (expression.arguments.size() == 1) {
			return -A;
		}
		Matrix B = solveExpression(expression.arguments[1], variables, outFile);
		return A.minus(B);
	}
	if (element == ".*") {
		if (expression.arguments.size() != 2) {
			throw std::invalid_argument("You have less or more than 2 arguments");
		}
		Matrix A = solveExpression(expression.arguments[0], variables, outFile);
		Matrix B = solveExpression(expression.arguments[1], variables, outFile);
		return A.elementalMultiply(B);
	}
	if (element == "./") {
		if (expression.arguments.size() != 2) {
			throw std::invalid_argument("You have less or more than 2 arguments");
		}
		Matrix A = solveExpression(expression.arguments[0], variables, outFile);
		Matrix B = solveExpression(expression.arguments[1], variables, outFile);
		return A.elementalDiv(B);
	}
	if (element == "*") {
		if (expression.arguments.size() != 2) {
			throw std::invalid_argument("You have less or more than 2 arguments");
		}
		Matrix A = solveExpression(expression.arguments[0], variables, outFile);
		Matrix B = solveExpression(expression.arguments[1], variables, outFile);
		return A.Multiply(B);
	}
	if (element == "/") {
		if (expression.arguments.size() != 2) {
			throw std::invalid_argument("You have less or more than 2 arguments");
		}
		Matrix A = solveExpression(expression.arguments[0], variables, outFile);
		Matrix B = solveExpression(expression.arguments[1], variables, outFile);
		return A.del(B);
	}
	if (element == "zeros") {
		if (expression.arguments.size() != 2) {
			throw std::invalid_argument("You have less or more 2 arguments");
		}
		Matrix arg1 = solveExpression(expression.arguments[0], variables, outFile);
		Matrix arg2 = solveExpression(expression.arguments[1], variables, outFile);
		Fraction n = arg1.toNumber();
		Fraction m = arg2.toNumber();
		return Matrix::zeros(n, m);
	}
	if (element == "ones") {
		if (expression.arguments.size() != 2) {
			throw std::invalid_argument("You have less or more than 2 arguments");
		}
		Matrix arg1 = solveExpression(expression.arguments[0], variables, outFile);
		Matrix arg2 = solveExpression(expression.arguments[1], variables, outFile);
		Fraction n = arg1.toNumber();
		Fraction m = arg2.toNumber();
		return Matrix::ones(n, m);
	}
	if (element == "linspace") {
		if (expression.arguments.size() != 3) {
			throw std::invalid_argument("You have less or more than 3 arguments");
		}
		Matrix arg1 = solveExpression(expression.arguments[0], variables, outFile);
		Matrix arg2 = solveExpression(expression.arguments[1], variables, outFile);
		Matrix arg3 = solveExpression(expression.arguments[2], variables, outFile);

		Fraction x0 = arg1.toNumber();
		Fraction x1 = arg2.toNumber();
		Fraction n = arg3.toNumber();

		return Matrix::linspace(x0, x1, n);
	}
	if (element == "vertcat") {
		if (expression.arguments.empty()) {
			throw std::invalid_argument("You must have more than 0 arguments");
		}
		std::vector<Matrix> matrix_vec;
		for (const auto& arg : expression.arguments) {
			matrix_vec.push_back(solveExpression(arg, variables, outFile));
		}
		return Matrix::vertcat_all(matrix_vec);
	}
	if (element == "horzcat") {
		if (expression.arguments.empty()) {
			throw std::invalid_argument("You must have more than 0 arguments");
		}
		std::vector<Matrix> matrix_vec;
		for (const auto& arg : expression.arguments) {
			matrix_vec.push_back(solveExpression(arg, variables, outFile));
		}
		return Matrix::horzcat_all(matrix_vec);
	}
	if (element == "transpose") {
		if (expression.arguments.size() != 1) {
			throw std::invalid_argument("You have less or more than 1 argument");
		}
		Matrix A = solveExpression(expression.arguments[0], variables, outFile);
		return A.transpose();
	}
	if (element == "det") {
		if (expression.arguments.size() != 1) {
			throw std::invalid_argument("You have less or more than 1 argument");
		}
		Matrix A = solveExpression(expression.arguments[0], variables, outFile);
		return A.det();
	}
	if (element == "inv") {
		if (expression.arguments.size() != 1) {
			throw std::invalid_argument("You have less or more than 1 argument");
		}
		Matrix A = solveExpression(expression.arguments[0], variables, outFile);
		return A.inv();
	}
	if (element == "eye") {
		if (expression.arguments.size() != 1) {
			throw std::invalid_argument("You have less or more than 1 argument");
		}
		Matrix arg1 = solveExpression(expression.arguments[0], variables, outFile);
		Fraction n = arg1.toNumber();
		return Matrix::eye(n);
	}
	if (element == "sum") {
		if (expression.arguments.size() != 1) {
			throw std::invalid_argument("You have less or more than 1 argument");
		}
		Matrix A = solveExpression(expression.arguments[0], variables, outFile);
		return A.sum();
	}
	if (element == "prod") {
		if (expression.arguments.size() != 1) {
			throw std::invalid_argument("You have less or more than 1 argument");
		}
		Matrix A = solveExpression(expression.arguments[0], variables, outFile);
		return A.prod();
	}
	if (element == "max") {
		if (expression.arguments.size() != 2) {
			throw std::invalid_argument("You have less or more than 2 arguments");
		}
		Matrix A = solveExpression(expression.arguments[0], variables, outFile);
		Matrix B = solveExpression(expression.arguments[1], variables, outFile);
		return A.maximalElementMatrix(B);
	}
	if (element == "min") {
		if (expression.arguments.size() != 2) {
			throw std::invalid_argument("You have less or more than 2 arguments");
		}
		Matrix A = solveExpression(expression.arguments[0], variables, outFile);
		Matrix B = solveExpression(expression.arguments[1], variables, outFile);
		return A.minimalElementMatrix(B);
	}
	if (element == "disp") {
		if (expression.arguments.size() != 1) {
			throw std::invalid_argument("You have less or more than 1 argument");
		}
		if (expression.arguments[0].isSimple() && !isCommand(expression.arguments[0].element) && variables.find(expression.arguments[0].element) != variables.end()) {
			std::string name = expression.arguments[0].element;
			Matrix A = variables[name];
			A.display(name, outFile);
			return A;
		}
		
		Matrix A = solveExpression(expression.arguments[0], variables, outFile);
		A.display("ans", outFile);
		return A;
		
	}
	
	throw std::invalid_argument("Unknown command...");
	
}


int main(int argc, char* argv[]) {
	
	if (argc != 3) {
		std::cerr << "Wrang amount of arguments" << '\n';
		return 1;
	}

	std::ifstream inFile(argv[1]);
	if (!inFile) {
		std::cerr << "Could not open inputFile" << std::endl;
		return 1;
	}

	std::ofstream outFile(argv[2]);
	if (!outFile) {
		std::cerr << "Could not write into outputFile" << std::endl;
		return 1;
	}

	std::string program;
	char ch;
	while (inFile.get(ch)) {
		program += ch;
	}
	

	std::map<std::string, Matrix> variables;

	int index = 0;
	int progLine = 1;

	while (index < program.size()) {
		while (index < program.size() && (program[index] == ' ' || program[index] == '\n')) {
			if (program[index] == '\n') {
				progLine++;
			}
			index++;
		}

		if (index >= program.size()) {
			break;
		}

		int lineStart = progLine;

		try {
			Expression expr = parseExpression(program, index, lineStart);
			solveExpression(expr, variables, outFile);
		}
		catch(...){
			outFile << "Error in line " << lineStart << '\n';
			while (index < program.size() && program[index] != '\n') {
				index++;
			}
		}
	}
	outFile.close();
	return 0;
}

