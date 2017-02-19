#include <iostream>
#include <iomanip>

using namespace std;

#define CREATE_matrix \
matrix = new float*[height];\
 for (int i = 0; i < height; ++i)\
	matrix[i] = new float[width];

#define CLEAR_matrix \
for (int i = 0; i < height; ++i)\
	delete[] matrix[i];	\
		delete[] matrix;


class RealMatrix
{
	float** matrix;
	int height = 0;
	int width = 0;

public:
	friend ostream& operator<<(ostream& oss, const RealMatrix& obj); 

	RealMatrix(int height_, int width_)
		:height{ height_ }, width{ width_ }
	{
		CREATE_matrix
	}

	RealMatrix()
	{
		while (height < 1 || width < 1) {
			cout << "\nEnter the height of the matrix: ";
			cin >> height;
			cout << "Enter the width of the matrix: ";
			cin >> width;
		}
		CREATE_matrix
	}

	RealMatrix(const RealMatrix& obj)
		:height{ obj.height }, width{ obj.width }
	{
		CREATE_matrix
			for (int i = 0; i < height; ++i)
				for (int j = 0; j < width; ++j)
					matrix[i][j] = obj.matrix[i][j];
	}

	~RealMatrix()
	{
		CLEAR_matrix
	}

	void setTheMatrixWithOneNumber(float realNumber)
	{
		for (int i = 0; i < height; ++i)
			for (int j = 0; j < width; ++j)
				matrix[i][j] = realNumber;
	}

	void setTheMatrix()
	{
		int answerInt = 0;
		while (answerInt != 1 && answerInt != 2) {
			cout << "Choose the action:\n"
				"1. Fill the matrix manually.\n"
				"2. Fill the matrix with single number.\n";
			cin >> answerInt;
		}
		if (answerInt == 1) {
			cout << "Enter a real numbers to fill the matrix: \n";
			for (int i = 0; i < height; ++i)
				for (int j = 0; j < width; ++j)
					cin >> (float)matrix[i][j];
		}
		else {
			float realNumber;
			cout << "Enter a real number to fill the matrix: \n";
			cin >> (float)realNumber;
			setTheMatrixWithOneNumber(realNumber);
		}
	}

	int getHeight()	const
	{
		return height;
	}

	int getWidth() const
	{
		return width;
	}

	bool square() const
	{
		return height == width;
	}

	float** getMatrix() {
		return matrix;
	}					 

	const RealMatrix& operator+=(const RealMatrix& obj)
	{
		if (height != obj.getHeight() || width != obj.getWidth()) {
			cout << "Error! Matrices of different orders can't be summed.\n";
		}
		else
			for (int i = 0; i < height; ++i)
				for (int j = 0; j < width; ++j)
					matrix[i][j] += obj.matrix[i][j];
		return *this;
	}

	RealMatrix operator+(const RealMatrix& obj)
	{
		RealMatrix resultMatrix = *this;
		resultMatrix += obj;
		return resultMatrix;
	}

	const RealMatrix& operator=(const RealMatrix& obj)
	{
		if (height != obj.getHeight() || width != obj.getWidth()) {
			CLEAR_matrix
				height = obj.getHeight();
			width = obj.getWidth();
			CREATE_matrix
		}
		for (int i = 0; i < height; ++i)
			for (int j = 0; j < width; ++j)
				matrix[i][j] = obj.matrix[i][j];
		return *this;
	}
	const RealMatrix& operator-=(const RealMatrix& obj)
	{
		if (getHeight() != obj.getHeight() || getWidth() != obj.getWidth()) {
			cout << "Error! Matrices of different orders can't be subtracted.\n";
		}
		else
			for (int i = 0; i < height; ++i)
				for (int j = 0; j < width; ++j)
					matrix[i][j] -= obj.matrix[i][j];
		return *this;
	}

	RealMatrix operator-(const RealMatrix& obj)
	{
		RealMatrix resultMatrix = *this;
		resultMatrix -= obj;
		return resultMatrix;
	}  

	const RealMatrix& operator*=(const RealMatrix& obj)
	{
		if (getWidth() != obj.getHeight()) {
			cout << "Error! The quantity of columns of the main matrix differs from\n"
				"the quantity of strings of another matrix, so they can't be multiplied.\n";
		}
		else {
			int order = getWidth();
			int resultHeight = getHeight();
			int resultWidth = obj.getWidth();
			RealMatrix resultMatrix(resultHeight, resultWidth);
			resultMatrix.setTheMatrixWithOneNumber(0);
			for (int i = 0; i < resultHeight; ++i)
				for (int j = 0; j < resultWidth; ++j)
					for (int k = 0; k < order; ++k)
						resultMatrix.index(i, j) += index(i, k) * obj.index(k, j);
			for (int i = 0; i < height; ++i)
				delete[] matrix[i];
			delete[] matrix;
			height = resultHeight;
			width = resultWidth;
			matrix = new float*[height];
			for (int i = 0; i < height; ++i)
				matrix[i] = new float[width];
			*this = resultMatrix;
		}
		return *this;
	}

	RealMatrix operator*(const RealMatrix& obj)
	{
		RealMatrix resultMatrix = *this;
		resultMatrix *= obj;
		return resultMatrix;
	}

	const RealMatrix& operator*=(float value)
	{
		for (int i = 0; i < height; ++i)
			for (int j = 0; j < width; ++j)
				matrix[i][j] *= value;
		return *this;
	}

	const RealMatrix& operator/=(RealMatrix& obj) {
		*this = obj.invertedMatrix(obj) *= *this;
		return *this;
	}

	RealMatrix operator/(RealMatrix& obj)
	{
		RealMatrix resultMatrix = *this;
		resultMatrix /= obj;
		return resultMatrix;
	}  

	const RealMatrix& operator/=(float value)
	{
		value = 1.f / value;
		for (int i = 0; i < height; ++i)
			for (int j = 0; j < width; ++j)
				matrix[i][j] *= value;
		return *this;
	}

	RealMatrix operator*(float value)
	{
		RealMatrix resultMatrix = *this;
		resultMatrix *= value;
		return resultMatrix;
	}

	RealMatrix operator/(float value)
	{
		value = 1.f / value;
		RealMatrix resultMatrix = *this;
		resultMatrix *= value;
		return resultMatrix;
	}	  

	bool operator==(const RealMatrix& obj)
	{
		if (height != obj.getHeight() || width != obj.getWidth())
			return false;
		for (int i = 0; i < height; ++i)
			for (int j = 0; j < width; ++j)
				if (matrix[i][j] != obj.matrix[i][j])
					return false;
		return true;
	}

	bool operator!=(const RealMatrix& obj)
	{
		return !operator==(obj);
	}

	float determinant() {
		float det = 0;
		if (height == 1)
			det = matrix[0][0];
		else if (height == 2)
			det = matrix[0][0] * matrix[1][1] - matrix[1][0] * matrix[0][1];
		else if (height == 3)
			det = matrix[0][0] * matrix[1][1] * matrix[2][2] + matrix[0][1] * matrix[1][2] * matrix[2][0] +
			matrix[1][0] * matrix[2][1] * matrix[0][2] - matrix[2][0] * matrix[1][1] * matrix[0][2] -
			matrix[1][0] * matrix[0][1] * matrix[2][2] - matrix[0][0] * matrix[2][1] * matrix[1][2];
		return det;
	}	

	RealMatrix minorOfElement(int i_, int j_) const
	{
		RealMatrix minor(height - 1, width - 1);
		for (int i = 0, g = 0; i < height - 1; ++i, ++g) {
			if (i == i_)
				++g;
			for (int j = 0, h = 0; j < width - 1; ++j, ++h) {
				if (j == j_)
					++h;
				minor.matrix[i][j] = matrix[g][h];
			}
		}
		return minor;
	}

	RealMatrix transponnedMatrix() const
	{
		RealMatrix transponned(width, height);
		for (int i = 0; i < width; ++i)
			for (int j = 0; j < height; ++j)
				transponned.matrix[i][j] = matrix[j][i];
		return transponned;
	}

	float algebraicAddition(int i_, int j_) const
	{
		return minorOfElement(i_, j_).determinant() * pow((-1), (i_ + j_));
	}

	RealMatrix invertedMatrix(const RealMatrix& obj)
	{
		RealMatrix inverted(height, width);
		for (int i = 0; i < height; ++i)
			for (int j = 0; j < width; ++j)
				inverted.matrix[i][j] = obj.algebraicAddition(i, j);
		inverted /= determinant();
		return inverted;
	}

	float infinityNorm() const
	{
		float* sums = new float[height];
		for (int i = 0; i < height; ++i)
			sums[i] = 0;
		for (int i = 0; i < height; ++i)
			for (int j = 0; j < width; ++j)
				sums[i] += abs(matrix[i][j]);
		float iNorm = sums[0];
		for (int i = 1; i < height; ++i)
			if (iNorm < sums[i])
				iNorm = sums[i];
		delete[] sums;
		return iNorm;
	}

	float firstNorm() const
	{
		float* sums = new float[width];
		for (int i = 0; i < width; ++i)
			sums[i] = 0;
		for (int i = 0; i < width; ++i) {
			for (int j = 0; j < height; ++j)
				sums[i] += abs(matrix[j][i]);
			cout << "sums[" << i << "] = " << sums[i] << endl;
		}
		float fNorm = sums[0];
		for (int i = 1; i < width; ++i)
			if (fNorm < sums[i])
				fNorm = sums[i];
		delete[] sums;
		return fNorm;
	}

	float euclid_sNorm() const
	{
		float eNorm = 0;
		for (int i = 0; i < height; ++i)
			for (int j = 0; j < width; ++j)
				eNorm += pow(matrix[i][j], 2);
		eNorm = sqrt(eNorm);
		return eNorm;
	}

	RealMatrix power(int power)	const
	{
		RealMatrix resultMatrix(height, width);
		resultMatrix = *this;
		while (power-- > 1)
			resultMatrix *= *this;
		return resultMatrix;
	}

	bool diagonal()	const
	{
		for (int i = 0; i < height; ++i) 
			for (int j = 0; j < width; ++j) {
				if (j == i)
					continue;
				if (matrix[i][j] != 0)
					return false;
			}
		return true;
	}

	bool null()	const
	{
		for (int i = 0; i < height; ++i)
			for (int j = 0; j < width; ++j) {
				if (matrix[i][j] != 0)
					return false;
			}
		return true;
	}

	bool singular() const
	{	for (int i = 0; i < height; ++i)
			if (matrix[i][i] != 1)
				return false;
		return true;
	}

	bool symmetric() const
	{
		for (int i = 0; i < height; ++i)
			for (int j = 0; j < width; ++j) {
				if (matrix[i][j] != matrix[j][i])
					return false;
			}
		return true;
	}

	bool upperTriangle()
	{
		for (int i = 1; i < height; ++i)
			for (int j = 0; j < i; ++j) {
				if (matrix[i][j] != 0)
					return false;
			}
		return true; 
	}

	bool lowerTriangle()
	{
		for (int i = 0; i < height - 1; ++i)
			for (int j = i + 1; j < width; ++j) {
				if (matrix[i][j] != 0)
					return false;
			}
		return true;
	} 

	float& index(int i, int j) const
	{
		while (i >= getHeight() || j >= getWidth()) {
			cout << "Error! Indices you put is not correct! Enter again: \n";
			cin >> i;
			cin >> j;
		} 
		return matrix[i][j];
	} 
};

int menu()
{
	int answer = 0;
	while (answer < 1 || answer > 22) {
		cin.get();
		cin.get();
		system("cls");
		cout << "\nChoose the action:\n"
			"1. Summation of matrices (method with overloaded operator\"+\".\n"
			"2. Summation of matrices (method with overloaded operator\"+=\".\n"
			"3. Subtraction of matrices (method with overloaded operator\"-\".\n"
			"4. Subtraction of matrices (method with overloaded operator\"-=\".\n"
			"5. Equation of matrix (method with overloaded operator \"=\".\n"
			"6. Multiplication of matrices (method with overloaded operator \"*\".\n"
			"7. Multiplication of matrices (method with overloaded operator \"*=\".\n"
			"8. Multiplication of matrix and number (method with overloaded operator \"*\".\n"
			"9. Multiplication of matrix and number (method with overloaded operator \"*=\".\n"
			"10. Division of matrices (method with overloaded operator \"/\".\n"
			"11. Division of matrices (method with overloaded operator \"/=\".\n"
			"12. Division of matrix and number (method with overloaded operator \"/\".\n"
			"13. Division of matrix and number (method with overloaded operator \"=/\".\n"
			"14. Define if matrices are equal (method with overloaded operator \"==\".\n"
			"15. Define if matrices are not equal (method with overloaded operator \"!=\".\n"
			"16. Compute the determinant of the matrix (not bigger than third order).\n"
			"17. Compute the inverse matrix.\n"
			"18. Compute the transponented matrix.\n"
			"19. Compute the norms of matrix.\n"
			"20. Compute the power of matrix.\n"
			"21. Show the properties of matrix.\n"
			"22. Exit the program.\n";
		cin >> answer;
		if (answer == 22)
			exit(0);
	}
	return answer;
}

ostream& operator<<(ostream& oss, const RealMatrix& obj)
{
	int height = obj.getHeight();
	int width = obj.getWidth();
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j)
			oss << setw(5) << obj.index(i, j) << " ";
		oss << endl;
	}
	return oss;
}

istream& operator >> (istream& iss, RealMatrix& obj)
{
	int height = obj.getHeight();
	int width = obj.getWidth();
	for (int i = 0; i < height; ++i)
		for (int j = 0; j < width; ++j)
			iss >> obj.index(i, j);
	return iss;
}  

int main()
{
	cout << "\t\t-= Class \"RealMatrix\" =-\n\n"
		"Creare the main matrix:\n";
	RealMatrix mainMatrix;
	mainMatrix.setTheMatrix();
	cout << "\nMain matrix is: \n" << mainMatrix << "\nPress any key to continue: ";
	int answerInt = 0;
	while (answerInt != 20) {
		answerInt = menu();
		switch (answerInt) {
		case 1: //overloading operator "+"
		{
			cout << "The main matrix will be equated to the sum of another two matrices.\n"
				"Create the first matrix:\n";
			RealMatrix matrix1;
			matrix1.setTheMatrix();
			cout << "The first matrix for summation is: \n" << matrix1 <<
				"\nCreate the second matrix:\n";
			RealMatrix matrix2;
			matrix2.setTheMatrix();
			cout << "The second matrix for summation is: \n" << matrix2 << endl;
			mainMatrix = matrix1 + matrix2;
			cout << "\nMain matrix is: \n" << mainMatrix << "\nPress any key to continue: ";
			break;
		}
		case 2: //overloading operator "+="
		{
			cout << "The main matrix will be summed with another matrix.\n"
				"Create the matrix for summation:\n";
			RealMatrix matrix1;
			matrix1.setTheMatrix();
			cout << "The matrix for summation is: \n" << matrix1 << endl;
			mainMatrix += matrix1;
			cout << "\nMain matrix is: \n" << mainMatrix << "\nPress any key to continue: ";
			break;
		}
		case 3: //overloading operator "-"
		{
			cout << "The main matrix will be equated to the difference of another two matrices.\n"
				"Create the first matrix:\n";
			RealMatrix matrix1;
			matrix1.setTheMatrix();
			cout << "The first matrix for subtraction is: \n" << matrix1 <<
				"\nCreate the second matrix:\n";
			RealMatrix matrix2;
			matrix2.setTheMatrix();
			cout << "The second matrix for subtraction is: \n" << matrix2 << endl;
			mainMatrix = matrix1 + mainMatrix;
			cout << "\nMain matrix is: \n" << mainMatrix << "\nPress any key to continue: ";
			break;
		}
		case 4: //overloading operator "-="
		{
			cout << "A matrix will be the subtracted from the main matrix.\n"
				"Create the matrix tfor subtraction:\n";
			RealMatrix matrix1;
			matrix1.setTheMatrix();
			cout << "The matrix tfor subtraction is: \n" << matrix1 << endl;
			mainMatrix -= matrix1;
			cout << "\nMain matrix is: \n" << mainMatrix << "\nPress any key to continue: ";
			break;
		}
		case 5: //overloading operator "="
		{
			cout << "Main matrix will be equated to another matrix.\n"
				"Create the matrix to equate:\n";
			RealMatrix matrix1;
			matrix1.setTheMatrix();
			cout << "The matrix to equate is: \n" << matrix1 << endl;
			mainMatrix = matrix1;
			cout << "\nMain matrix is: \n" << mainMatrix << "\nPress any key to continue: ";
			break;
		}
		case 6: {  //overloading of operator "*"
			cout << "The main matrix will be equated to the composition of another two matrices.\n"
				"Create the first matrix:\n";
			RealMatrix matrix1;
			matrix1.setTheMatrix();
			cout << "First matrix for composition is: \n" << matrix1 << "Create the The second matrix:\n";
			RealMatrix matrix2;
			matrix2.setTheMatrix();
			cout << "The second matrix for composition is: \n" << matrix2 << endl;
			mainMatrix = matrix1 * matrix2;
			cout << "\nMain matrix is: \n" << mainMatrix << "\nPress any key to continue: ";
			break;
		}
		case 7: {  //overloading of operator "*="
			cout << "Main matrix will be composed on another matrix.\n"
				"Create the matrix for composition:\n";
			RealMatrix matrix1;
			matrix1.setTheMatrix();
			cout << "Matrix for composition is: \n" << matrix1 << endl;
			mainMatrix *= matrix1;
			cout << "\nMain matrix is: \n" << mainMatrix << "\nPress any key to continue: ";
			break;
		}
		case 8: {	//overloading of operator "*" for number
			float realNumber;
			cout << "Main matrix will equated to the composition of another matrix and a number.\n"
				"Enter a real number: ";
			cin >> realNumber;
			cout << "Create the matrix for composition:\n";
			RealMatrix matrix1;
			matrix1.setTheMatrix();
			cout << "Matrix for composition is: \n" << matrix1 << endl;
			mainMatrix = matrix1 * realNumber;
			cout << "\nMain matrix is: \n" << mainMatrix << "\nPress any key to continue: ";
			break;
		}
		case 9: {	//overloading of operator "*=" for number
			float realNumber;
			cout << "Main matrix will be composed on a number.\n"
				"Enter a real number: ";
			cin >> realNumber;
			mainMatrix *= realNumber;
			cout << "\nMain matrix is: \n" << mainMatrix << "\nPress any key to continue: ";
			break;
		}
		case 10: {  //overloading of operator "/"
			cout << "The main matrix will be equated to the product of division of another two matrices.\n"
				"Create the first matrix:\n";
			RealMatrix matrix1;
			matrix1.setTheMatrix();
			cout << "First matrix for division is: \n" << matrix1 << "Create the The second matrix:\n";
			RealMatrix matrix2;
			matrix2.setTheMatrix();
			cout << "The second matrix for division is: \n" << matrix2 << endl;
			mainMatrix = matrix1 / matrix2;
			cout << "\nMain matrix is: \n" << mainMatrix << "\nPress any key to continue: ";
			break;
		}
		case 11: {	//overloading of operator "/="
			cout << "Main matrix will be divided on another matrix.\n"
				"Create the matrix for division:\n";
			RealMatrix matrix1;
			matrix1.setTheMatrix();
			cout << "Matrix for division is: \n" << matrix1 << endl;
			mainMatrix /= matrix1;
			cout << "\nMain matrix is: \n" << mainMatrix << "\nPress any key to continue: ";
			break;
		}
		case 12: {	//overloading of operator "/" for number
			float realNumber;
			cout << "Main matrix will equated to the product of division of another matrix and a number.\n"
				"Enter a real number: ";
			cin >> realNumber;
			cout << "Create the matrix for divdision:\n";
			RealMatrix matrix1;
			matrix1.setTheMatrix();
			cout << "Matrix for divdision is: \n" << matrix1 << endl;
			mainMatrix = matrix1 / realNumber;
			cout << "\nMain matrix is: \n" << mainMatrix << "\nPress any key to continue: ";
			break;
		}
		case 13: {	//overloading of operator "/=" for number
			float realNumber;
			cout << "Main matrix will be divided on a number.\n"
				"Enter a real number: ";
			cin >> realNumber;
			mainMatrix /= realNumber;
			cout << "\nMain matrix is: \n" << mainMatrix << "\nPress any key to continue: ";
			break;
		}
		case 14: {
			cout << "Create a matrix to define if it's equal to the main matrix:\n";
			RealMatrix matrix1;
			matrix1.setTheMatrix();
			cout << "The matrix to compare with the main matrix:\n" << matrix1 << endl;
			if (mainMatrix == matrix1)
				cout << "Matrixes are equal.\n";
			else
				cout << "Matrixes are not equal.\n";
			break;
		}

		case 15: {
			cout << "Create a matrix to define if it isn't equal to the main matrix:\n";
			RealMatrix matrix1;
			matrix1.setTheMatrix();
			cout << "The matrix to compare with the main matrix:\n" << matrix1 << endl;
			if (mainMatrix != matrix1)
				cout << "Matrixes are not equal.\n";
			else
				cout << "Matrixes are equal.\n";
			break;
		}
		case 16: {
			cout << "The main matrix:\n" << mainMatrix << endl;
			if (mainMatrix.getHeight() != mainMatrix.getWidth()) {
				cout << "The matrix is not square, so it doesn't have a determinant.\n";
				break;
			}
			else if (mainMatrix.getHeight() > 3) {
				cout << "The order of matrix is too big, too hard to calculate the determinant.\n";
				break;
			}
			else
				cout << "The determinant of the matrix is " << mainMatrix.determinant() << endl;
			break;
		}
		case 17:
			cout << "The main matrix:\n" << mainMatrix << endl;
			if (mainMatrix.getHeight() > 1 && mainMatrix.square() && mainMatrix.determinant()) {
				cout << "Matrix, that is inverted to main matrix is\n"
					<< mainMatrix.invertedMatrix(mainMatrix.transponnedMatrix()) << endl;
				break;
			}
			else if (!mainMatrix.square()) {
				cout << "The main matrix isn't squate, so the inverted matrix doesn't exist.\n";
				break;
			}
			else if (mainMatrix.getHeight() == 1) {
				cout << "The matrix with order = 1 hasn't invere matrix";
			}
			else {
				cout << "The main matrix's determinant = 0, so the inverted matrix doesn't exist.\n";
				break;
			}
		case 18: {
			cout << "The main matrix:\n" << mainMatrix <<
				"\nTransponned main matrix:\n" << mainMatrix.transponnedMatrix() << endl;
			break;
		}
		case 19: {
			cout << "The main matrix:\n" << mainMatrix <<
				"\nThe infinity norm is " << mainMatrix.infinityNorm() <<
				"\nThe first norm is " << mainMatrix.firstNorm() <<
				"\nThe euclid's norm is " << mainMatrix.euclid_sNorm() << endl;
			break;
		}
		case 20: {
			if (!mainMatrix.square()) {
				cout << "The main matrix isn't square, so it can't be powered.\n";
				break;
			}
			int power;
			cout << "The main matrix:\n" << mainMatrix <<
				"\nEnter the power (integer type): ";
			cin >> power;
			cout << "The result is: \n" << mainMatrix.power(power) << endl;
			break;
		}
		case 21: {
			cout << "The main matrix:\n" << mainMatrix << "\nIt is:\n";
			if (mainMatrix.square()) {
				cout << "- square;\n";

				if (mainMatrix.diagonal()) {
					cout << "- diagonal;\n";
					if (mainMatrix.singular())
						cout << "- singular;\n";
					else
						cout << "- not singular;\n";
				}
				else
					cout << "- not diagonal;\n";

				if (mainMatrix.symmetric())
					cout << "- symmetric;\n";
				else
					cout << "- not symmetric;\n";

				if (mainMatrix.upperTriangle())
					cout << "- upper triangle;\n";
				else
					cout << "- not upper triangle;\n";

				if (mainMatrix.lowerTriangle())
					cout << "- lower triangle;\n";
				else
					cout << "- not lower triangle;\n";
			}
			else
				cout << "- not square;\n";

			if (mainMatrix.null())
				cout << "- null.\n";
			else
				cout << "- not null.\n";
			break;
		}
		}
	}	 
	return 0;
}