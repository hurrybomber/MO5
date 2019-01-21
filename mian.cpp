#include <iostream>
#include <cmath>
#include <array>
#include <fstream>
#include <random>
#include <algorithm>
#include <vector>
#include <chrono>

double function(const double &x) {
	return ((1-x)*(1-x)+exp(x));
}

double multimodal_function(const double &x) {
	return (sin(5*x))*((1 - x)*(1 - x) + exp(x));
}

//Функция возвращает номер элемента массива с которым самое малое расстояние до точки x
//нужна во время поиска погрешностей 
size_t x_extra(std::vector<double > x, double k) {
	for (auto& it : x) {
		it = abs(it - k);
	}
	auto result = std::min_element(x.begin(), x.end());
	return std::distance(x.begin(), result);
}

int main() {
	const double Big_num = 1000000;
	int a = -4, b = 3, D = b - a;
	double P1 = 0.9, q1 = 0.005, Pn, ebs, x;
	int N;
	std::vector<std::pair<size_t, double >> error;
	std::mt19937 gen(time(0));
	std::ofstream file_out("results");
	file_out << "(1-x)*(1-x)+exp(x)" << "\n";
	for (; P1 <= 0.99;) {
		q1 = 0.005;
		for (; q1 < 0.105;) {
			double result, result_x;
			ebs = D * q1;
			// Обратная вероятность 
			Pn = 1 - q1;
			// нахождение числа N
			N = log(1 - P1) / log(Pn);
			file_out << "| P=" << P1 << "| ebs=" << ebs << "| N=" << N;
			for (std::size_t i = 0; i < N; ++i) {
				std::uniform_int_distribution<int> uid(a*Big_num, b*Big_num);
				//число из промежутка [a ; b]
				x = uid(gen) / Big_num;
				if (i == 0) {
					result = function(x);
					result_x = x;
				}
				else {
					//Если новая точка ближе к минимуму, то запоминаем
					if (result > function(x)) {
						result = function(x);
						result_x = x;
					}
				}
			}
			file_out << "| Result=" << result << "| X=" << result_x << "|" << "\n";
			// Нахождение погрешности (разница между x и аналитической точкой минимума)
			error.push_back(std::pair<size_t, double >(N, result_x - 0.315));
			q1 += 0.005;
		}
		P1 += 0.01;
	}
	file_out << "\n\n\n";

	//Сортировка вектора погрешностей 
	std::sort(error.begin(), error.end(), [](auto &left, auto &right) {
		return left.first < right.first;
	});

	//Вывод погрешностей для каждого N
	for (auto it : error) {
		file_out << "(" << it.first << " ; " << it.second << ") ";
	}
	error.clear();

	file_out << "\n" << "(sin(5*x))*((1-x)*(1-x)+exp(x)))" << "\n";
	P1 = 0.91;
	// точки локальных экстремумов
	std::vector<double > x_extr{ -2.827584, -1.560563, -0.326486, 0.935639, 2.199657 };
	size_t  K;
	for (; P1 <= 0.99;) {
		q1 = 0.005;
		for (; q1 < 0.105;) {
			double result, result_x;
			ebs = D * q1;
			// Обратная вероятность 
			Pn = 1 - q1;
			// нахождение числа N
			N = log(1 - P1) / log(Pn);
			file_out << "|P=" << P1 << "| ebs=" << ebs << "| N=" << N;
			for (std::size_t i = 0; i < N; ++i) {
				std::uniform_int_distribution<int> uid(a*Big_num, b*Big_num);
				//число из промежутка [a ; b]
				x = uid(gen) / Big_num;
				if (i == 0) {
					result = multimodal_function(x);
					result_x = x;
					// нахождения к какой аналитической точке мы ближе всего 
					K = x_extra(x_extr, x);
				}
				else {
					//Если новая точка ближе к минимуму, то запоминаем
					if (result > multimodal_function(x)) {
						result = multimodal_function(x);
						result_x = x;
					}
				}
			}
			file_out << "| Result=" << result << "| X=" << result_x << "|" << "\n";
			// Нахождение погрешности (разница между x и аналитической точкой минимума)
			error.push_back(std::pair<size_t, double >(N, result_x - x_extr.at(K)));
			q1 += 0.005;
		}
		P1 += 0.01;
	}
	file_out << "\n\n\n";

	std::sort(error.begin(), error.end(), [](auto &left, auto &right) {
		return left.first < right.first;
	});

	for (auto it : error) {
		file_out << "(" << it.first << " ; " << it.second << ") ";
	}
	file_out << "\n\n\n";
	file_out.close();
	system("pause");

	return 0;
	
}
