#ifndef CSV_H
#define CHV_H

#include <stdio.h>
#include <stdlib.h>
#include <vector>

/**
 * @class CSV
 * @brief Clase para guardar datos en un archivo CSV
*/
class CSV
{
public:

	/**
	 * @brief Guarda los datos en un archivo CSV
	 * @param filename Nombre del archivo CSV
	 * @param data Datos a guardar en el archivo CSV (vector de vectores de doubles)
	*/
	CSV(const char* filename, const std::vector<std::vector<double>>& data) {
		FILE* file;
		if (fopen_s(&file, filename, "w") != 0) {
			perror("Error al abrir el archivo");
			return;
		}

		for (size_t i = 0; i < data.size(); ++i) {
			for (size_t j = 0; j < data[i].size(); ++j) {
				fprintf(file, "%.6f", data[i][j]);
				if (j < data[i].size() - 1) {
					fprintf(file, ",");
				}
			}
			fprintf(file, "\n");
		}

		fclose(file);
	}

	CSV(const char* filename, double** data, size_t numRows, size_t numCols) {
		FILE* file;
		if (fopen_s(&file, filename, "w") != 0) {
			perror("Error al abrir el archivo");
			return;
		}

		for (size_t i = 0; i < numRows; ++i) {
			for (size_t j = 0; j < numCols; ++j) {
				fprintf(file, "%.6f", data[i][j]);
				if (j < numCols - 1) {
					fprintf(file, ",");
				}
			}
			fprintf(file, "\n");
		}

		fclose(file);
	}

};

#endif // !CSV_H
