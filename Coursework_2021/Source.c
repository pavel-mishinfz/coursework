#include <stdio.h> // printf_s()
#include <conio.h>	// _getch()
#include <string.h>	//  strlen()
#include <ctype.h> // isdigit()
#include <stdlib.h> // exit()
#include <Windows.h> // SetConsoleTitle()

wchar_t title[] = L"����� ����� �������������";

#define MAX 10000 // ������������ ���������� �������� � �����

int sign_one, sign_two; // ���� �����
int correctly = 0; // �������� ��� ����������� ��������/���������
int BASE = 10; // ��������� ������� ���������

FILE* fp = NULL;

// ��������� ������� 
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
	int er = 0; // ��������� ������
	int chunks_numOne[MAX] = { 0 }; // ������ ������� �����
	int chunks_numTwo[MAX] = { 0 }; // ������ ������� �����
	int res[MAX] = { 0 }; // ������ ���������� 
	char str_numOne[MAX]; // ������ ������� �����
	char str_numTwo[MAX]; // ������ ������� �����

	printf_s("\n\n --- Arithmetic of large numbers --- \n");

	fopen_s(&fp, "Result.txt", "w"); // �������� �����
	if (!fp) {
		printf_s("\n\t Error. Press any key to exit: ");
		int g = _getch();
		exit(1);
	}

	// ���� ������� �����
	do {
		printf_s("\n Enter the first number: ");
		gets_s(str_numOne, MAX);
		er = FunControl(str_numOne);
	} while (!er);

	// ����������� ����� �����
	if (str_numOne[0] == '-') {
		sign_one = -1;
	}
	else {
		sign_one = 1;
	}

	FunTransform(str_numOne, chunks_numOne, sign_one);

	int size_strOne = strlen(str_numOne); //������ ������� �����

	// ����� ������� ����� �� �������
	fprintf_s(fp, "\n First number:  ");
	FunPrintFile(size_strOne, chunks_numOne, sign_one);


	// ���� ������� �����
	do {
		printf_s("\n Enter the second number: ");
		gets_s(str_numTwo, MAX);
		er = FunControl(str_numTwo);
	} while (!er);

	// ����������� ����� �����
	if (str_numTwo[0] == '-') {
		sign_two = -1;
	}
	else {
		sign_two = 1;
	}
	FunTransform(str_numTwo, chunks_numTwo, sign_two);

	int size_strTwo = strlen(str_numTwo); // ������ ������� �����

	// ����� ������� ����� �� �������
	fprintf_s(fp, "\n\n Second number: ");
	FunPrintFile(size_strTwo, chunks_numTwo, sign_two);


	// ����� �������� ��� �������
	char p;
	printf_s("\n Select an operation (+  -  *  /  ^): ");
	scanf_s("%c", &p, 1);
	int size_str = size_strOne > size_strTwo ? size_strOne : size_strTwo; // �������� ������ ����� ������� ������
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
		printf_s("\n Error. Invalid input! "); // ����� ������ ��� �������� ������ ��������
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

// ������� �������� ������ � "�������" �����
void FunTransform(char* str, int* chunks, int sign) {
	if (sign == -1) {
		// ��������� ����� �� ������ 
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

//������� �������� 
int FunSumBASE(int* chunks_numOne, int* chunks_numTwo, int* res, int size_str) {
	// ------ ������� ��������� ------
	// chunks_numOne - ������ ������� �����
	// chunks_numTwo - ������ ������� �����
	// res - ��������� ��������
	// size_str - ������ ������������� �� �������� �����
	int w = 0;
	int carry = 0;
	for (int i = 0; i <= size_str; ++i) {
		// BASE = 10 - ��������� ������� ��������
		w = chunks_numOne[i] + chunks_numTwo[i] + carry; // ��������� �������� ���� ���������������� �������
		carry = w / BASE; // ������� ��������
		res[i] = w % BASE; // ��������� �����
	}
	// ���� ��� �������� ���������� ����� ������� �����
	while (size_str && !res[size_str]) {
		size_str--;
	}
	size_str++;
	return(size_str); // ������ �����
}

// ������� ����������� �������� 
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

// ������� ��������� 
int FunDiffBASE(int* chunks_numOne, int* chunks_numTwo, int* res, int size_str) {
	// ------ ������� ��������� ------
	// chunks_numOne - ������ ������� �����
	// chunks_numTwo - ������ ������� �����
	// res - ��������� ���������
	// size_str - ������ ������������� �� �������� �����
	for (int i = 0; i < MAX; i++) { // MAX - ������������ ���������� �������� � ����� 
		res[i] = chunks_numOne[i];
	}
	for (int i = 0; i < size_str; ++i) {
		res[i] -= chunks_numTwo[i]; // �������� ���������� �� ������� ����� ������
		if (res[i] < 0) {
			res[i] += BASE; // ���������� 10, ���� � ������� ������� ����� �������� ������, ��� � ������� ������� 
			res[i + 1]--; // �������� 1 � ���������� �������
		}
	}
	// ���� ��� �������� ���������� ����� ������� �����
	while (size_str && !res[size_str]) {
		size_str--;
	}
	size_str++;
	return(size_str); // ������ ��������
}

// ������� ����������� ��������� 
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

// ������� ��������� 
int FunMultiBASE(int* chunks_numOne, int* chunks_numTwo, int* res, int size_strOne, int size_strTwo) {
	// ------ ������� ��������� ------
	// chunks_numOne - ������ ������� �����
	// chunks_numTwo - ������ ������� �����
	// res - ��������� ���������
	// size_strOne - ������ ������� �����
	// size_strTwo - ������ ������� �����
	int size_multi = 0; // ������ ������������
	for (int i = 0; i < size_strTwo; i++) {
		int carry = 0; // ������� ��������
		for (int j = 0; j <= size_strOne; ++j) {
			// BASE = 10 - ��������� ������� ��������
			res[i + j] += chunks_numOne[j] * chunks_numTwo[i] + carry; // ����������� ���������
			carry = res[i + j] / BASE; // ���� ��������� ������������ ������������ ������ ���� ����� 10
			res[i + j] -= carry * BASE; // ��������� ���������
		}
	}
	size_multi = size_strOne + size_strTwo; // ������ ������������
	// ���� ��� �������� ���������� ����� ������� �����
	while (size_multi && !res[size_multi]) {
		size_multi--;
	}
	size_multi++;
	return(size_multi);
}

// ������� ����������� ��������� 
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

// ������� ������� 
void FunQuotient(int* chunks_numOne, int* chunks_numTwo, int size_strOne, int size_strTwo) {
	// ------ ������� ��������� ------
	// chunks_numOne - ������ ������� �����
	// chunks_numTwo - ������ ������� �����
	// size_strOne - ������ ������� �����
	// size_strTwo - ������ ������� �����
	int i = 0;
	// sign_two - ���������� ����������, ���������� �� ���� ��������
	if (sign_two == -1) {
		++i;
	}
	if (!chunks_numTwo[size_strTwo - (i + 1)]) { // ������� �� ����
		fprintf_s(fp, " Error. It is forbidden to divide by zero!");
		printf_s("\n\n Press any key to exit : ");
		int g = _getch();
		exit(1);
	}
	if (size_strOne < size_strTwo) { // ���� �������� ������ ��������
		fprintf_s(fp, " 0 \n\n Remains: ");
		FunPrintFile(size_strOne, chunks_numOne, 1);
	}
	else {
		int Q[MAX] = { 0 }; // �������
		int U[MAX] = { 0 }; // �������
		int u = size_strOne;
		int n = 0, m = 0; // ������ ����� � ������� �������������
		for (i = 0; i < size_strOne; i++) {
			U[i] = chunks_numOne[i];
		}
		if (sign_two == -1) {
			n = size_strTwo - 1, m = u - (size_strTwo - 1);
		}
		else {
			n = size_strTwo, m = u - size_strTwo;
		}

		// bShift_cur - ������� ����� B ������������ U, ������������ ��� ���������,
		// �� ���������������� - ������ ��������� ����� ��������.
		// uID_cur � ������ ������� ����� U
		int uID_cur, bShift_cur;
		int temp, temp1, temp2;

		unsigned short qGuess, r;  // ������� ��� �������� � ��������������� �������
		short borrow, carry;  // ��������


		// ������� ���� ����� �������. ������ �������� ���� ��������� ����� ��������.
		for (bShift_cur = m, uID_cur = n + bShift_cur; bShift_cur >= 0; --bShift_cur, --uID_cur)
		{
			qGuess = (U[uID_cur] * BASE + U[uID_cur - 1]) / chunks_numTwo[n - 1];
			r = (U[uID_cur] * BASE + U[uID_cur - 1]) % chunks_numTwo[n - 1];

			// ���� �� ����� ��������� ������� ��������� �������
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
					// ������� �� ���������, ��������� qGuess
					// � ��������� ����� �������
					--qGuess;
					r += chunks_numTwo[n - 1];
				}
				else {
					break;
				}
			}

			// ������ qGuess - ���������� ������� ��� �� ������� ������ q
			// ������� �������� B, ���������� �� qGuess �� �������� U,
			// ������� � ������� bShift_cur + i

			carry = 0; borrow = 0;
			int* uShift = U + bShift_cur;

			for (i = 0; i < n; i++) {
				// �������� � temp ����� ������������ B*qGuess
				temp1 = chunks_numTwo[i] * qGuess + carry;
				carry = temp1 / BASE;
				temp1 -= carry * BASE;

				// ����� �� ������� �� U
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

			// ��������, ��������� �� qGuess ����� B ����������.
			// ���� ��� ���, �� ����� ��������� �������
			// ���������������� ������� carry. ������� � ��� ����.
			temp2 = uShift[i] - carry + borrow;
			if (temp2 < 0) {
				uShift[i] = temp2 + BASE;
				borrow = -1;
			}
			else {
				uShift[i] = temp2;
				borrow = 0;
			}

			// ������ �� ��������� ���������?
			if (borrow == 0) { // ��, ������� ������� ���������
				Q[bShift_cur] = qGuess;
			}
			else { // ���, ��������� ������� ��� ��������� borrow = -1,
					// ������, qGuess �� ������� ������ ��������� ��������
				Q[bShift_cur] = qGuess - 1;

				// �������� ����, ��������� ����� ������������ B � U
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

			// ������� ������ U, ������� ����� ��������� ��� �����������
			i = u - 1;
			while (i && !U[i]) {
				i--;
			}
			u = i + 1;
		}
		// ������ �������� ����� m + 1, ��, ��������, ������ ����� - ����.
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

// ������� ���������� � �������
int FunPowerBASE(int* temp, int* chunks, int* res, int size_temp, int size_chunks, int n) {
	// ------ ������� ��������� ------
	// temp � chunks - ������ �����, ������� ����� ��������� � ������� n
	// res - ��������� ���������
	// size_temp - ������ ����������
	// size_chunks - ������ �����, ������� ����� ��������� � �������
	int clone_chunks[MAX] = { 0 }; // ����������� ������
	int k = 1;
	for (int i = 0; i < MAX; ++i) {
		clone_chunks[i] = chunks[i];
	}
	while (k < n) {
		size_temp = FunMultiBASE(temp, clone_chunks, res, size_temp, size_chunks);

		for (int i = 0; i < size_temp; ++i) {
			temp[i] = res[i]; // ��������� ��������� ��������� �� ������ �������� while � temp, ��� ������������ ���������
							  // temp - ��������� ���������� ����� � ������� n 
		}
		++k;
		// �������� ������, ��� ����������� ��������� �� ��������� ��������
		for (int i = 0; i < MAX; ++i) {
			res[i] = 0;
		}
	}
	return(size_temp); // ������ ����� ����� ���������� � ������� n 
}

// ������� ����������� ���������� � �������
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

// ������� ������ ����� � ����
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

// ������� �������� ����� ������������
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
