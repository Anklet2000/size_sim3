#include<cstdlib>
#include<math.h>
#include<ctime>
#include<fstream>
#include<string>
#include"size_sim3.h"

#define R 4.0/*大粒子の半径*/
#define r 0.50/*小粒子の半径*/
#define N 1/*大粒子の数*/
#define n 54/*小粒子の数*/
#define M 1.0/*大粒子の質量*/
#define m 1.0/*小粒子の質量*/
#define g 9.8/*重力加速度*/
#define s 3.0/*持ち上げ距離*/

double X[N], Y[N], x[n], y[n], X2[N], Y2[N], x2[n], y2[n];

/*粒子の初期位置決定*/
void particle_init() {
	int i;
	bool flag;
	/*大粒子の初期位置決定*/
	X[0] = 0.0;
	Y[0] = R - 6.0;
	particle(X[0], Y[0], R, 1);
	/*小粒子の初期位置決定*/
	i = 0;
	while (i < n) {
		flag = 1;
		x[i] = rand() % 12 + rand() / (double)RAND_MAX - 6.0;
		y[i] = rand() % 12 + rand() / (double)RAND_MAX - 6.0;
		if (x[i] < r - 6.0 || x[i] > 6.0 - r || y[i] < r - 6.0 || y[i] > 6.0 - r) {
			flag = 0;
		}
		if (flag == 1) {
			for (int j = 0; j < N; j++) {
				double d = sqrt((x[i] - X[j]) * (x[i] - X[j]) + (y[i] - Y[j]) * (y[i] - Y[j]));
				if (d < R + r) {
					flag = 0;
					break;
				}
			}
		}
		if (flag == 1) {
			for (int j = 0; j < i; j++) {
				double d = sqrt((x[i] - x[j]) * (x[i] - x[j]) + (y[i] - y[j]) * (y[i] - y[j]));
				if (d < r + r) {
					flag = 0;
					break;
				}
			}
		}
		if (flag == 0) {
			i--;
		}
		else if (flag == 1) {
			particle(x[i], y[i], r, 0);
		}
		i++;
	}
}

/*落下*/
void down(int cn) {
	int i;
	bool flag;
	double E1, E1R, E1r, E2, E2R, E2r, E;
	std::ofstream writing_file;
	std::string filename = "sim3data_XX.txt";
	writing_file.open(filename, std::ios::app);
	for (int k = 0; k < 10000; k++) {
		E1 = E1R = E1r = E2 = E2R = E2r = 0.0;
		for (i = 0; i < N; i++) {
			X2[i] = X[i];
			Y2[i] = Y[i];
		}
		i = 0;
		while (i < N) {
			if (cn == 0) {
				break;
			}
			flag = 1;
			X2[i] = X[i] + rand() % 2 + rand() / (double)RAND_MAX - 1.0;
			Y2[i] = Y[i] + rand() % 2 + rand() / (double)RAND_MAX - 1.0;
			if (X2[i] < R - 6.0 || X2[i] > 6.0 - R || Y2[i] < R - 6.0 || Y2[i] > 6.0 - R) {
				flag = 0;
			}
			if (flag == 1) {
				for (int j = 0; j < n; j++) {
					double d = sqrt((x[j] - X2[i]) * (x[j] - X2[i]) + (y[j] - Y2[i]) * (y[j] - Y2[i]));
					if (d < R + r) {
						flag = 0;
						break;
					}
				}
			}
			if (flag == 1) {
				for (int j = 0; j < N; j++) {
					double d = sqrt((X2[j] - X2[i]) * (X2[j] - X2[i]) + (Y2[j] - Y2[i]) * (Y2[j] - Y2[i]));
					if (i != j && d < R + R) {
						flag = 0;
						break;
					}
				}
			}
			if (flag == 0) {
				X2[i] = X[i];
				Y2[i] = Y[i];
			}
			E1R += M * (Y[i] + 6.0) * g;
			E2R += M * (Y2[i] + 6.0) * g;
			i++;
		}
		for (i = 0; i < n; i++) {
			x2[i] = x[i];
			y2[i] = y[i];
		}
		i = 0;
		while (i < n) {
			flag = 1;
			x2[i] = x[i] + rand() % 2 + rand() / (double)RAND_MAX - 1.0;
			y2[i] = y[i] + rand() % 2 + rand() / (double)RAND_MAX - 1.0;
			if (x2[i] < r - 6.0 || x2[i] > 6.0 - r || y2[i] < r - 6.0 || y2[i] > 6.0 - r) {
				flag = 0;
			}
			if (flag == 1) {
				for (int j = 0; j < N; j++) {
					double d = sqrt((X2[j] - x2[i]) * (X2[j] - x2[i]) + (Y2[j] - y2[i]) * (Y2[j] - y2[i]));
					if (d < R + r) {
						flag = 0;
						break;
					}
				}
			}
			if (flag == 1) {
				for (int j = 0; j < n; j++) {
					double d = sqrt((x2[j] - x2[i]) * (x2[j] - x2[i]) + (y2[j] - y2[i]) * (y2[j] - y2[i]));
					if (i != j && d < r + r) {
						flag = 0;
						break;
					}
				}
			}
			if (flag == 0) {
				x2[i] = x[i];
				y2[i] = y[i];
			}
			E1r += m * (y[i] + 6.0) * g;
			E2r += m * (y2[i] + 6.0) * g;
			i++;
		}
		E1 = E1R + E1r;
		E2 = E2R + E2r;
		if (E1 > E2) {
			E = E2;
			for (i = 0; i < N; i++) {
				X[i] = X2[i];
				Y[i] = Y2[i];
			}
			for (i = 0; i < n; i++) {
				x[i] = x2[i];
				y[i] = y2[i];
			}
		}
		else {
			E = E1;
		}
	}
	for (i = 0; i < N; i++) {
		particle(X[i], Y[i], R, 1);
		writing_file << cn << " " << Y[i] + 6.0 - R << std::endl;
	}
	writing_file.close();
	for (i = 0; i < n; i++) {
		particle(x[i], y[i], r, 0);
	}
}

/*持ち上げ*/
void up() {
	int i;
	for (i = 0; i < N; i++) {
		Y[i] += s;
		particle(X[i], Y[i], R, 1);
	}
	for (i = 0; i < n; i++) {
		y[i] += s;
		particle(x[i], y[i], r, 0);
	}
}

/*粒子の移動*/
void move(int cn){
	int i;
	bool flag;
	std::ofstream writing_file;
	std::string filename = "sim3data_XXX.txt";
	writing_file.open(filename, std::ios::app);
	for (int k = 0; k < 10000; k++) {
		if (cn == 0) {
			break;
		}
		for (i = 0; i < N; i++) {
			X2[i] = X[i];
			Y2[i] = Y[i];
		}
		i = 0;
		while (i < N) {
			flag = 1;
			X2[i] = X[i] + rand() % 2 + rand() / (double)RAND_MAX - 1.0;
			Y2[i] = Y[i] + rand() % 2 + rand() / (double)RAND_MAX - 1.0;
			if (X2[i] < R - 6.0 || X2[i] > 6.0 - R || Y2[i] < R - 6.0 || Y2[i] > 6.0 - R) {
				flag = 0;
			}
			if (flag == 1) {
				for (int j = 0; j < n; j++) {
					double d = sqrt((x[j] - X2[i]) * (x[j] - X2[i]) + (y[j] - Y2[i]) * (y[j] - Y2[i]));
					if (d < R + r) {
						flag = 0;
						break;
					}
				}
			}
			if (flag == 1) {
				for (int j = 0; j < N; j++) {
					double d = sqrt((X2[j] - X2[i]) * (X2[j] - X2[i]) + (Y2[j] - Y2[i]) * (Y2[j] - Y2[i]));
					if (i != j && d < R + R) {
						flag = 0;
						break;
					}
				}
			}
			if (flag == 0) {
				X2[i] = X[i];
				Y2[i] = Y[i];
			}
			i++;
		}
		for (i = 0; i < n; i++) {
			x2[i] = x[i];
			y2[i] = y[i];
		}
		i = 0;
		while (i < n) {
			flag = 1;
			x2[i] = x[i] + rand() % 2 + rand() / (double)RAND_MAX - 1.0;
			y2[i] = y[i] + rand() % 2 + rand() / (double)RAND_MAX - 1.0;
			if (x2[i] < r - 6.0 || x2[i] > 6.0 - r || y2[i] < r - 6.0 || y2[i] > 6.0 - r) {
				flag = 0;
			}
			if (flag == 1) {
				for (int j = 0; j < N; j++) {
					double d = sqrt((X2[j] - x2[i]) * (X2[j] - x2[i]) + (Y2[j] - y2[i]) * (Y2[j] - y2[i]));
					if (d < R + r) {
						flag = 0;
						break;
					}
				}
			}
			if (flag == 1) {
				for (int j = 0; j < n; j++) {
					double d = sqrt((x2[j] - x2[i]) * (x2[j] - x2[i]) + (y2[j] - y2[i]) * (y2[j] - y2[i]));
					if (i != j && d < r + r) {
						flag = 0;
						break;
					}
				}
			}
			if (flag == 0) {
				x2[i] = x[i];
				y2[i] = y[i];
			}
			i++;
		}
		for (i = 0; i < N; i++) {
			X[i] = X2[i];
			Y[i] = Y2[i];
		}
		for (i = 0; i < n; i++) {
			x[i] = x2[i];
			y[i] = y2[i];
		}
	}
	for (i = 0; i < N; i++) {
		particle(X[i], Y[i], R, 1);
		writing_file << cn << " " << Y[i] + 6.0 - R << std::endl;
	}
	for (i = 0; i < n; i++) {
		particle(x[i], y[i], r, 0);
	}
}