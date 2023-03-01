#include <iostream>
#include <random>
#include <iomanip>

using namespace std;

// Использование встроенного алгоритма генерации случайных чисел Мерсенна
random_device rd;
mt19937 gen(rd());

// Генерирует случайные целые числа равномерно распределенные на закрытом интервале
int Random(int low, int high) { uniform_int_distribution<> dist(low, high); return dist(gen); }

// Вывод матрицы А
void printM(int n, double** matrA, char matrID) {
	cout << "_________________________" << matrID << "__________________________" << endl;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) cout << setw(5) << matrA[i][j] << " ";
		cout << endl;
	}
	cout << "____________________________________________________" << endl;
}

// Вывод матрицы B
void printB(int n, double* matrB) {
	cout << "_________________________B__________________________" << endl;
	for (int i = 0; i < n; i++) cout << setw(5) << matrB[i] << endl;
	cout << "____________________________________________________" << endl;
}

// Вывод ошибки поиска
void printError() {
	cout << "____________________________________________________" << endl;
	cout << "Такого пункта не существует!" << endl;
	cout << "____________________________________________________" << endl;
}

// Факторизация
void factoring(int n, double** matrA) {
	for (int k = 0; k < n; k++) {
		double king = matrA[k][k]; // Главное число
		for (int kk = k + 1; kk < n; kk++) matrA[kk][k] /= king; // Нурмируем
		for (int i = k + 1; i < n; i++) { // Разложение
			for (int j = k + 1; j < n; j++)
				matrA[i][j] -= matrA[i][k] * matrA[k][j];
		}
	}
}

// Разделение матриц на L и U
void separating(int n, double** matrA, double** matrL, double** matrU) {
	for (int i = 0; i < n; i++) matrL[i] = new double[n]; // Создаем в каждом элемента массива еще по массиву (получим матрицу)
	for (int i = 0; i < n; i++) matrU[i] = new double[n]; // Создаем в каждом элемента массива еще по массиву (получим матрицу)

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (j < i) matrL[j][i] = 0;
			else if (j == i) matrL[i][j] = 1;
			else matrL[j][i] = matrA[j][i];
		}
		for (int j = 0; j < n; j++) {
			if (j < i) matrU[i][j] = 0;
			else matrU[i][j] = matrA[i][j];
		}
	}

	printM(n, matrL, 'L');
	printM(n, matrU, 'U');
}

// Решение СЛАУ
double* solve_lu(double** L, double** U, double* b, int n) {
	// Переменные для хранения промежуточных результатов
	double* y = new double[n];
	double* x = new double[n];

	// Решаем систему Ly = b методом прямой подстановки
	for (int i = 0; i < n; i++) {
		double sum = 0.0;
		for (int j = 0; j < i; j++) sum += L[i][j] * y[j];
		y[i] = b[i] - sum;
	}

	// Решаем систему Ux = y методом обратной подстановки
	for (int i = n - 1; i >= 0; i--) {
		double sum = 0.0;
		for (int j = i + 1; j < n; j++) sum += U[i][j] * x[j];
		x[i] = (y[i] - sum) / U[i][i];
	}

	// Возвращаем решение
	delete[] y;
	return x;
}

// Поиск определителя
void findD(int n, double** matrA, double* D) {
	for (int i = 0; i < n; i++)
		*D *= matrA[i][i];
	cout << "____________________________________________________" << endl;
	cout << "Определитель равен: " << *D << endl;
	cout << "____________________________________________________" << endl;
}

int main()
{
	setlocale(LC_ALL, "Russian");

	cout << "____________________________________________________" << endl;
	cout << "Лабораторная №1. Вариант 6. (-LU) – разложение на основе гауссова исключения по столбцам с выбором главного элемента по активной подматрице." << endl; cout << endl;
	cout << "Автор: Куликов Сергей Андреевич и Якушов Николай Сегреевич, ПРИ-О-21" << endl;

	int what;
	int n = 100;

	double** matrA = new double* [n]; // Создаем динамический массив A
	double* matrB = new double[n]; // Создаем динамический массив B
	double* matrX = new double[n]; // Создаем динамический массив X
	double D = 1; // Определитель

	double** matrL = new double* [n]; // Создаем динамический массив A
	double** matrU = new double* [n]; // Создаем динамический массив A

	while (true) {
		cout << "____________________________________________________" << endl;
		cout << "                      МЕНЮ:" << endl;
		cout << "____________________________________________________" << endl;
		cout << "1) Ввод данных An*n, Bn*1" << endl;
		cout << "2) Факторизация" << endl;
		cout << "3) Решение СЛАУ" << endl;
		cout << "4) Определитель" << endl;
		cout << "5) Обращение I через Ax=E" << endl;
		cout << "6) Обращение II через элементарные преобразования" << endl;
		cout << "7) Эксперимент 1" << endl;
		cout << "8) Эксперимент 2" << endl;
		cout << "9) Эксперимент 3" << endl;
		cout << "10) Выход" << endl;
		cout << "____________________________________________________" << endl;
		cout << "Введите номер действия: "; cin >> what;

		if (what == 1) {
			cout << "____________________________________________________" << endl;
			cout << "Введите размер матрицы: "; cin >> n;

			for (int i = 0; i < n; i++) matrA[i] = new double[n]; // Создаем в каждом элемента массива еще по массиву (получим матрицу)

			cout << "____________________________________________________" << endl;
			cout << "                 Выберите вид ввода:" << endl;
			cout << "____________________________________________________" << endl;
			char mini_what;
			cout << "a) Ручной" << endl;
			cout << "b) Заполнение случайными числами" << endl;
			cout << "____________________________________________________" << endl;
			cout << "Введите букву действия: "; cin >> mini_what;
			if (mini_what == 'a') {
				cout << "____________________________________________________" << endl;
				cout << "Заполните матрицу An*n:" << endl;
				// Заполнение матрицы A
				for (int i = 0; i < n; i++) { for (int j = 0; j < n; j++) cin >> matrA[i][j]; }
				printM(n, matrA, 'A');

				cout << "Заполните матрицу Bn*1:" << endl;
				// Заполнение матрицы B
				for (int i = 0; i < n; i++) cin >> matrB[i];
				printB(n, matrB);
			}
			else if (mini_what == 'b') {
				// Заполнение матрицы A
				for (int i = 0; i < n; i++) { for (int j = 0; j < n; j++) matrA[i][j] = (double)Random(-100, 100); }
				printM(n, matrA, 'A');

				// Заполнение матрицы B - каждый элемент - сумма строки
				for (int i = 0; i < n; i++) {
					double sum = 0;
					for (int j = 0; j < n; j++) sum += matrA[i][j];
					matrB[i] = sum;
				}
				printB(n, matrB);
			}
			else { printError(); }
		}
		else if (what == 2) {
			factoring(n, matrA);
			printM(n, matrA, 'A');
			separating(n, matrA, matrL, matrU);
		}
		else if (what == 3) { 
			matrX = solve_lu(matrL, matrU, matrB, n); 
			cout << "X = ";
			for (int i = 0; i < n; i++) cout << matrX[i] << " ";
			cout << endl;
		}
		else if (what == 4) { findD(n, matrA, &D); }
		else if (what == 5) { /* Тут должно что-то быть */ }
		else if (what == 6) { /* Тут должно что-то быть */ }
		else if (what == 7) { /* Тут должно что-то быть */ }
		else if (what == 8) { /* Тут должно что-то быть */ }
		else if (what == 9) { /* Тут должно что-то быть */ }
		else if (what == 10) { break; }
		else { printError(); }
	}

	system("pause");
	return 0;
}