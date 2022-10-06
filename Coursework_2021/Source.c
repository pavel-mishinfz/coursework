#include <stdio.h> // printf_s()
#include <conio.h>	// _getch()
#include <string.h>	//  strlen()
#include <ctype.h> // isdigit()
#include <stdlib.h> // exit()
#include <Windows.h> // SetConsoleTitle()

wchar_t title[] = L"Мишин Павел Александрович";

#define MAX 10000 // максимальное количество символов в числе

int sign_one, sign_two; // знак числа
int correctly = 0; // проверка для корректного сложения/вычитания
int BASE = 10; // основание системы счисления

FILE* fp = NULL;

// Прототипы функций 
void FunTransform(char*, int*, int);
int FunSumBASE(int*, int*, int*, int);
int FunDiffBASE(int*, int*, int*, int);
int FunMultiBASE(int*, int*, int*, int, int);
int FunPowerBASE(int*, int*, int*, int, int, int);
void FunSum(int*, int*, int*, int);
void FunDiff(int*, int*, int*, int);
void FunMulti(int*, int*, int*, int, int);
void FunPower(int*, int*, int*, int, int, int, int);
void FunQuotient(int*, int*, int, int);
void FunPrintFile(int, int*, int);
int FunControl(char*);


int main(void) {
	SetConsoleTitle(title);
	int er = 0; // индикатор ошибки
	int chunks_numOne[MAX] = { 0 }; // массив первого числа
	int chunks_numTwo[MAX] = { 0 }; // массив второго числа
	int res[MAX] = { 0 }; // массив результата 
	char str_numOne[MAX]; // строка первого числа
	char str_numTwo[MAX]; // строка второго числа

	printf_s("\n\n --- Arithmetic of large numbers --- \n");

	fopen_s(&fp, "Result.txt", "w"); // открытие файла
	if (!fp) {
		printf_s("\n\t Error. Press any key to exit: ");
		int g = _getch();
		exit(1);
	}

	// Ввод первого числа
	do {
		printf_s("\n Enter the first number: ");
		gets_s(str_numOne, MAX);
		er = FunControl(str_numOne);
	} while (!er);

	// Определение знака числа
	if (str_numOne[0] == '-') {
		sign_one = -1;
	}
	else {
		sign_one = 1;
	}

	FunTransform(str_numOne, chunks_numOne, sign_one);

	int size_strOne = strlen(str_numOne); //размер первого числа

	// Вывод первого числа на консоль
	fprintf_s(fp, "\n First number:  ");
	FunPrintFile(size_strOne, chunks_numOne, sign_one);


	// Ввод второго числа
	do {
		printf_s("\n Enter the second number: ");
		gets_s(str_numTwo, MAX);
		er = FunControl(str_numTwo);
	} while (!er);

	// Определение знака числа
	if (str_numTwo[0] == '-') {
		sign_two = -1;
	}
	else {
		sign_two = 1;
	}
	FunTransform(str_numTwo, chunks_numTwo, sign_two);

	int size_strTwo = strlen(str_numTwo); // размер второго числа

	// Вывод второго числа на консоль
	fprintf_s(fp, "\n\n Second number: ");
	FunPrintFile(size_strTwo, chunks_numTwo, sign_two);


	// Выбор операции над числами
	char p;
	printf_s("\n Select an operation (+  -  *  /  ^): ");
	scanf_s("%c", &p, 1);
	int size_str = size_strOne > size_strTwo ? size_strOne : size_strTwo; // выбираем размер самой большой строки
	switch (p) {
	case '+': {
		printf_s("\n The result is written to the text file \"Result\". ");
		fprintf_s(fp, "\n\n Sum of numbers: ");
		FunSum(chunks_numOne, chunks_numTwo, res, size_str);
		break;
	}
	case '-': {
		printf_s("\n The result is written to the text file \"Result\". ");
		fprintf_s(fp, "\n\n Difference of numbers: ");
		FunDiff(chunks_numOne, chunks_numTwo, res, size_str);
		break;
	}
	case '*': {
		printf_s("\n The result is written to the text file \"Result\". ");
		fprintf_s(fp, "\n\n Product of numbers: ");
		FunMulti(chunks_numOne, chunks_numTwo, res, size_strOne, size_strTwo);
		break;
	}
	case '/': {
		printf_s("\n The result is written to the text file \"Result\". ");
		fprintf_s(fp, "\n\n Quotient of numbers: ");
		FunQuotient(chunks_numOne, chunks_numTwo, size_strOne, size_strTwo);
		break;
	}
	case '^': {
		int n = 0;
		printf_s("\n Set the numbers to a power of N: ");
		scanf_s("%d", &n);
		printf_s("\n The result is written to the text file \"Result\". ");
		if (!n) {
			fprintf_s(fp, "\n\n The first number to the power of %d:  %d", n, n + 1);
			fprintf_s(fp, "\n\n The second number to the power of %d: %d", n, n + 1);
		}
		else {
			fprintf_s(fp, "\n\n The first number to the power of %d:  ", n);
			FunPower(chunks_numOne, chunks_numOne, res, size_strOne, size_strOne, n, sign_one);
			fprintf_s(fp, "\n\n The second number to the power of %d: ", n);
			FunPower(chunks_numTwo, chunks_numTwo, res, size_strTwo, size_strTwo, n, sign_two);
		}
		break;
	}
	default:
		printf_s("\n Error. Invalid input! "); // Вывод ошибки при неверном выборе операции
		break;
	}

	if (fclose(fp)) {
		printf_s("\n\t Error. Press any key to exit: ");
		int g = _getch();
		exit(1);
	}

	printf_s("\n\n Press any key to exit: ");
	int ge = _getch();
}

// Функция перевода строки в "большое" число
void FunTransform(char* str, int* chunks, int sign) {
	if (sign == -1) {
		// Считываем цифры из строки 
		for (int i = (int)strlen(str) - 1, j = 0; i > 0; --i) {
			chunks[j++] = str[i] - '0';
		}
	}
	else {
		for (int i = (int)strlen(str) - 1, j = 0; i >= 0; --i) {
			chunks[j++] = str[i] - '0';
		}
	}
}

//Функция сложения 
int FunSumBASE(int* chunks_numOne, int* chunks_numTwo, int* res, int size_str) {
	// ------ Входные параметры ------
	// chunks_numOne - массив первого числа
	// chunks_numTwo - массив второго числа
	// res - результат сложения
	// size_str - «длина» максимального по разрядам числа
	int w = 0;
	int carry = 0;
	for (int i = 0; i <= size_str; ++i) {
		// BASE = 10 - основание системы числения
		w = chunks_numOne[i] + chunks_numTwo[i] + carry; // результат сложения цифр соответствующего разряда
		carry = w / BASE; // единица переноса
		res[i] = w % BASE; // результат суммы
	}
	// Цикл для очищения незначащих нулей впереди числа
	while (size_str && !res[size_str]) {
		size_str--;
	}
	size_str++;
	return(size_str); // размер суммы
}

// Функция правильного сложения 
void FunSum(int* chunks_numOne, int* chunks_numTwo, int* res, int size_str) {
	int size_sum = 0;
	if (sign_one == 1 && sign_two == 1) {
		size_sum = FunSumBASE(chunks_numOne, chunks_numTwo, res, size_str);
		FunPrintFile(size_sum, res, 1);
	}
	else if (sign_one == -1 && sign_two == -1) {
		size_sum = FunSumBASE(chunks_numOne, chunks_numTwo, res, size_str);
		FunPrintFile(size_sum + 1, res, -1);
	}
	else {
		correctly = 1;
		FunDiff(chunks_numOne, chunks_numTwo, res, size_str);
	}
}

// Функция вычитания 
int FunDiffBASE(int* chunks_numOne, int* chunks_numTwo, int* res, int size_str) {
	// ------ Входные параметры ------
	// chunks_numOne - массив первого числа
	// chunks_numTwo - массив второго числа
	// res - результат вычитания
	// size_str - «длина» максимального по разрядам числа
	for (int i = 0; i < MAX; i++) { // MAX - максимальное количество символов в числе 
		res[i] = chunks_numOne[i];
	}
	for (int i = 0; i < size_str; ++i) {
		res[i] -= chunks_numTwo[i]; // вычитаем поразрядно из первого числа второе
		if (res[i] < 0) {
			res[i] += BASE; // прибавляем 10, если в разряде первого числа значение больше, чем в разряде второго 
			res[i + 1]--; // вычитаем 1 у следующего разряда
		}
	}
	// Цикл для очищения незначащих нулей впереди числа
	while (size_str && !res[size_str]) {
		size_str--;
	}
	size_str++;
	return(size_str); // размер разности
}

// Функция правильного вычитания 
void FunDiff(int* chunks_numOne, int* chunks_numTwo, int* res, int size_str) {
	int size_diff = 0;
	int i = size_str - 1;
	while (chunks_numOne[i] == chunks_numTwo[i]) {
		--i;
	}
	if (chunks_numOne[i] > chunks_numTwo[i]) {
		if (sign_one == 1 && sign_two == -1 && correctly == 0) {
			size_diff = FunSumBASE(chunks_numOne, chunks_numTwo, res, size_str);
			FunPrintFile(size_diff, res, 1);
		}
		else if (sign_one == -1 && sign_two == -1 && correctly == 0) {
			size_diff = FunDiffBASE(chunks_numOne, chunks_numTwo, res, size_str);
			FunPrintFile(size_diff + 1, res, -1);
		}
		else {
			size_diff = FunDiffBASE(chunks_numOne, chunks_numTwo, res, size_str);
			if (sign_one == -1) {
				FunPrintFile(size_diff + 1, res, -1);
			}
			else if (sign_two == -1) {
				FunPrintFile(size_diff, res, 1);
			}
			else {
				FunPrintFile(size_diff, res, 1);
			}
		}
	}
	else {
		if (sign_one == 1 && sign_two == -1 && correctly == 0) {
			size_diff = FunSumBASE(chunks_numOne, chunks_numTwo, res, size_str);
			FunPrintFile(size_diff, res, 1);
		}
		else if (sign_one == -1 && sign_two == -1 && correctly == 0) {
			size_diff = FunDiffBASE(chunks_numTwo, chunks_numOne, res, size_str);
			FunPrintFile(size_diff, res, 1);
		}
		else {
			size_diff = FunDiffBASE(chunks_numTwo, chunks_numOne, res, size_str);
			if (sign_one == -1) {
				FunPrintFile(size_diff, res, 1);
			}
			else if (sign_two == -1) {
				FunPrintFile(size_diff + 1, res, -1);
			}
			else {
				res[size_diff - 1] = -res[size_diff - 1];
				FunPrintFile(size_diff, res, 1);
			}
		}
	}
}

// Функция умножения 
int FunMultiBASE(int* chunks_numOne, int* chunks_numTwo, int* res, int size_strOne, int size_strTwo) {
	// ------ Входные параметры ------
	// chunks_numOne - массив первого числа
	// chunks_numTwo - массив второго числа
	// res - результат умножения
	// size_strOne - «длина» первого числа
	// size_strTwo - «длина» второго числа
	int size_multi = 0; // размер произведения
	for (int i = 0; i < size_strTwo; i++) {
		int carry = 0; // единица переноса
		for (int j = 0; j <= size_strOne; ++j) {
			// BASE = 10 - основание системы числения
			res[i + j] += chunks_numOne[j] * chunks_numTwo[i] + carry; // поразрядное умножение
			carry = res[i + j] / BASE; // если результат поразрядного перемножения больше либо равен 10
			res[i + j] -= carry * BASE; // результат умножения
		}
	}
	size_multi = size_strOne + size_strTwo; // размер произведения
	// Цикл для очищения незначащих нулей впереди числа
	while (size_multi && !res[size_multi]) {
		size_multi--;
	}
	size_multi++;
	return(size_multi);
}

// Функция правильного умножения 
void FunMulti(int* chunks_numOne, int* chunks_numTwo, int* res, int size_strOne, int size_strTwo) {
	int size_multi = 0;
	size_multi = FunMultiBASE(chunks_numOne, chunks_numTwo, res, size_strOne, size_strTwo);
	if (sign_one == sign_two) {
		FunPrintFile(size_multi, res, 1);
	}
	else {
		FunPrintFile(size_multi + 1, res, -1);
	}
}

// Функция деления 
void FunQuotient(int* chunks_numOne, int* chunks_numTwo, int size_strOne, int size_strTwo) {
	// ------ Входные параметры ------
	// chunks_numOne - массив первого числа
	// chunks_numTwo - массив второго числа
	// size_strOne - «длина» первого числа
	// size_strTwo - «длина» второго числа
	int i = 0;
	// sign_two - глобальная переменная, отвечающая за знак делителя
	if (sign_two == -1) {
		++i;
	}
	if (!chunks_numTwo[size_strTwo - (i + 1)]) { // деление на ноль
		fprintf_s(fp, " Error. It is forbidden to divide by zero!");
		printf_s("\n\n Press any key to exit : ");
		int g = _getch();
		exit(1);
	}
	if (size_strOne < size_strTwo) { // если делитель больше делимого
		fprintf_s(fp, " 0 \n\n Remains: ");
		FunPrintFile(size_strOne, chunks_numOne, 1);
	}
	else {
		int Q[MAX] = { 0 }; // частное
		int U[MAX] = { 0 }; // остаток
		int u = size_strOne;
		int n = 0, m = 0; // размер числа и остатка соотвественно
		for (i = 0; i < size_strOne; i++) {
			U[i] = chunks_numOne[i];
		}
		if (sign_two == -1) {
			n = size_strTwo - 1, m = u - (size_strTwo - 1);
		}
		else {
			n = size_strTwo, m = u - size_strTwo;
		}

		// bShift_cur - текущий сдвиг B относительно U, используемый при вычитании,
		// по совместительству - индекс очередной цифры частного.
		// uID_cur – индекс текущей цифры U
		int uID_cur, bShift_cur;
		int temp, temp1, temp2;

		unsigned short qGuess, r;  // догадка для частного и соответствующий остаток
		short borrow, carry;  // переносы


		// Главный цикл шагов деления. Каждая итерация дает очередную цифру частного.
		for (bShift_cur = m, uID_cur = n + bShift_cur; bShift_cur >= 0; --bShift_cur, --uID_cur)
		{
			qGuess = (U[uID_cur] * BASE + U[uID_cur - 1]) / chunks_numTwo[n - 1];
			r = (U[uID_cur] * BASE + U[uID_cur - 1]) % chunks_numTwo[n - 1];

			// Пока не будут выполнены условия уменьшать частное
			while (r < BASE) {
				if (n == 1) {
					temp2 = chunks_numTwo[n] * qGuess;
					temp1 = r * BASE + U[uID_cur];
				}
				else {
					temp2 = chunks_numTwo[n - 2] * qGuess;
					temp1 = r * BASE + U[uID_cur - 2];
				}

				if ((temp2 > temp1) || (qGuess == BASE)) {
					// условия не выполнены, уменьшить qGuess
					// и досчитать новый остаток
					--qGuess;
					r += chunks_numTwo[n - 1];
				}
				else {
					break;
				}
			}

			// Теперь qGuess - правильное частное или на единицу больше q
			// Вычесть делитель B, умноженный на qGuess из делимого U,
			// начиная с позиции bShift_cur + i

			carry = 0; borrow = 0;
			int* uShift = U + bShift_cur;

			for (i = 0; i < n; i++) {
				// получить в temp цифру произведения B*qGuess
				temp1 = chunks_numTwo[i] * qGuess + carry;
				carry = temp1 / BASE;
				temp1 -= carry * BASE;

				// Сразу же вычесть из U
				temp2 = uShift[i] - temp1 + borrow;
				if (temp2 < 0) {
					uShift[i] = temp2 + BASE;
					borrow = -1;
				}
				else {
					uShift[i] = temp2;
					borrow = 0;
				}
			}

			// Возможно, умноженое на qGuess число B удлинилось.
			// Если это так, то после умножения остался
			// неиспользованный перенос carry. Вычесть и его тоже.
			temp2 = uShift[i] - carry + borrow;
			if (temp2 < 0) {
				uShift[i] = temp2 + BASE;
				borrow = -1;
			}
			else {
				uShift[i] = temp2;
				borrow = 0;
			}

			// Прошло ли вычитание нормально?
			if (borrow == 0) { // да, частное угадано правильно
				Q[bShift_cur] = qGuess;
			}
			else { // Нет, последний перенос при вычитании borrow = -1,
					// значит, qGuess на единицу больше истинного частного
				Q[bShift_cur] = qGuess - 1;

				// добавить одно, вычтенное сверх необходимого B к U
				carry = 0;
				for (i = 0; i < n; i++) {
					temp = uShift[i] + chunks_numTwo[i] + carry;
					if (temp >= BASE) {
						uShift[i] = temp - BASE;
						carry = 1;
					}
					else {
						uShift[i] = temp;
						carry = 0;
					}
				}
				uShift[i] = uShift[i] + carry - BASE;
			}

			// Обновим размер U, который после вычитания мог уменьшиться
			i = u - 1;
			while (i && !U[i]) {
				i--;
			}
			u = i + 1;
		}
		// Размер частного равен m + 1, но, возможно, первая цифра - ноль.
		while (m && !Q[m]) {
			m--;
		}
		int q = m + 1;

		if (sign_one == sign_two) {
			FunPrintFile(q, Q, 1);
		}
		else {
			FunPrintFile(q + 1, Q, -1);
		}
		fprintf_s(fp, "\n\n Remains: ");
		FunPrintFile(u, U, 1);

	}
}

// Функция возведения в степень
int FunPowerBASE(int* temp, int* chunks, int* res, int size_temp, int size_chunks, int n) {
	// ------ Входные параметры ------
	// temp и chunks - массив числа, которое будем возводить в степень n
	// res - результат умножения
	// size_temp - «длина» результата
	// size_chunks - «длина» числа, которое будем возводить в степень
	int clone_chunks[MAX] = { 0 }; // дублирующий массив
	int k = 1;
	for (int i = 0; i < MAX; ++i) {
		clone_chunks[i] = chunks[i];
	}
	while (k < n) {
		size_temp = FunMultiBASE(temp, clone_chunks, res, size_temp, size_chunks);

		for (int i = 0; i < size_temp; ++i) {
			temp[i] = res[i]; // сохраняем результат умножения на каждой итерации while в temp, для последующего умножения
							  // temp - результат возведения числа в степень n 
		}
		++k;
		// Обнуляем массив, для корректного умножения на следующей итерации
		for (int i = 0; i < MAX; ++i) {
			res[i] = 0;
		}
	}
	return(size_temp); // размер числа после возведения в степень n 
}

// Функция правильного возведения в степень
void FunPower(int* temp, int* chunks, int* res, int size_temp, int size_chunks, int n, int sign) {
	int size_power = 0;
	if (n == 1) {
		FunPrintFile(size_chunks, chunks, 1);
	}
	else {
		if (!(n % 2)) {
			size_power = FunPowerBASE(temp, chunks, res, size_temp, size_chunks, n);
			FunPrintFile(size_power, temp, 1);
		}
		else if (n % 2 && sign < 0) {
			size_power = FunPowerBASE(temp, chunks, res, size_temp, size_chunks, n);
			FunPrintFile(size_power + 1, temp, sign);
		}
		else {
			size_power = FunPowerBASE(temp, chunks, res, size_temp, size_chunks, n);
			FunPrintFile(size_power, temp, sign);
		}
	}

}

// Функция записи числа в файл
void FunPrintFile(int str_size, int* chunks, int sign) {
	if (sign == -1) {
		if (chunks[str_size - 2]) {
			fprintf_s(fp, "-");
		}
		for (int i = str_size - 2; i >= 0; --i) {
			fprintf_s(fp, "%d", chunks[i]);
		}
	}
	else {
		for (int i = str_size - 1; i >= 0; --i) {
			fprintf_s(fp, "%d", chunks[i]);
		}
	}
}

// Функция проверки ввода пользователя
int FunControl(char* str) {
	int count = 0;
	if (str[0] == '-') {
		count = 1;
	}
	for (int i = count; i < (int)strlen(str); ++i) {
		if (isdigit(str[i])) {
			count = 1;
		}
		else {
			count = 0;
			break;
		}
	}
	return(count);
}
