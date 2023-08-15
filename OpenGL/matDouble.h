#ifndef MAT_DOUBLE_H
#define MAT_DOUBLE_H

class MatDouble
{
public:
	double** M;
	int n;
	int m;

	MatDouble() {
		M = nullptr;
		n = 0;
		m = 0;
	}

	MatDouble(int i, int j) {
		n = i;
		m = j;
		M = new double* [n];

		for (int k = 0; k < n; k++) {
			M[k] = new double[m];
		}
	}
};


#endif //!MAT_DOUBLE_H