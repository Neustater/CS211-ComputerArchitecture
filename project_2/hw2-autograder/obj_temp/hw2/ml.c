/* 
 * 
 * This code calculates the house price of a house by learing from
 * training data. It uses pseudo inverse of a given matrix to find the 
 * weight of different features.
 * 
 * Predicted Price : Y = W0 + W1*x1 + W2*X2 + W3*X3 + W4*X4
 * Weight Matrix : W = pseudoInv(X)*Y
 * pseudoInv(X) = inverse(transpose(X)*X) * transpose(X)  
 * 
 * weight(w) = pseudoInv(X) * Y
 * 			where	X = Input data matrix
 * 					Y = Target vector
 * 
 */
 
#include<stdio.h>
#include<stdlib.h>

// all methods declarations
double** multiplyMatrix(double **matA, double **matB, int r1, int c1, int r2, int c2);
double** transposeMatrix(double** mat, int row, int col);
double** inverseMatrix(double **matA, int dimension);

void freeMatrix(double **matrix, int R)
{

for(int i = 0; i < R; i++){
        free(matrix[i]);
}
free(matrix);
}

int main(int argc, char** argv){

     FILE *input;

input = fopen(argv[1],"r");
//input = fopen("/Users/michaelneustater/Users/michaelneustater/CS211/project_2/hw2-autograder/wip/trainA.txt","r");

     if(input == NULL){ //check for Null input
        printf("error\n");
        exit(0);
    }
    int coords = 0; //hold read values

    fscanf(input, "%d\n", &coords);
    int xCol = coords;
    fscanf(input, "%d\n", &coords);
    int xRow = coords;

    double** x = malloc(xRow*sizeof(double*)); //malloc the x matrix
    for(int i = 0; i < xRow; i++){
        x[i] = malloc((xCol+1)*sizeof(double*));
            //for(int j = 0; j< xRow; j++)
               //x[i][j] = 0;
    }

    double** y = malloc(xRow*sizeof(double*)); //malloc the y matrix
    for(int i = 0; i < xRow; i++){
         y[i] = malloc(1*sizeof(double*));
         y[i][0] = 0;
    }
    
    double val = 0; //value to hold input

     for(int i = 0; i < xRow; i++){ //fill x array
         for(int j = 0; j < xCol+1; j++){
             if(j==0)
                x[i][j] = 1;
            else{
            fscanf(input, "%lf,", &val);
            x[i][j] = val; 
            }
         }
         fscanf(input, "%lf\n", &val);
         y[i][0] = val;
     }

    //input = fopen("/Users/michaelneustater/Users/michaelneustater/CS211/project_2/hw2-autograder/wip/testA.txt","r");
    input = fopen(argv[2],"r"); //read test values

    int numInputs = 0; 
    fscanf(input, "%d\n", &numInputs);
    double** tests = malloc(numInputs*sizeof(double*)); //malloc test matrix
    for(int i = 0; i < numInputs; i++){
        tests[i] = malloc((xCol+1)*sizeof(double*));
    }

     for(int i = 0; i < numInputs; i++){    //fill test matrix
         for(int j = 0; j < xCol+1; j++){
             if(j==0)
                tests[i][j] = 1;
            else{
            fscanf(input, "%lf,", &val);
            tests[i][j] = val; 
            }
         }
     }

     /* FOR DEBUGGING PURPOSES: PRINTS MATRIX */
    /*
     printf("x\n");
     for(int i = 0; i < xRow; i++){
         for(int j = 0; j < xCol+1; j++)
    {
        printf("%lf\t", x[i][j]);
    }
    printf("\n");
    }
    printf("\n");
    */
 
    double** xT = transposeMatrix(x, xRow, xCol+1); //value for xT
    double** xQ = multiplyMatrix(xT, x, xCol+1, xRow, xRow, xCol+1); //xT * T
    double** xI = inverseMatrix(xQ,xCol+1); //inverse of xT * T

    double** xS = multiplyMatrix(xI, xT, xCol+1, xCol+1,xCol+1,xRow); //result * xT

    double** prod = multiplyMatrix(xS, y, xCol+1, xRow, xRow, 1); //result * y


    double** prices = multiplyMatrix(tests, prod, numInputs, xCol+1, xCol+1, 1); //test * w

    for(int i = 0; i < numInputs; i++)
    {
        printf("%0.0lf\n", prices[i][0]);
    }

    freeMatrix(x, xRow);
    freeMatrix(y, xRow);
    freeMatrix(xT, xCol+1);
    freeMatrix(xQ, xCol+1);
    freeMatrix(xI, xCol+1);
    freeMatrix(xS, xCol+1);
    freeMatrix(prod, xCol+1);
    freeMatrix(prices, numInputs);
     

    fclose(input);
    return 0;

}

double** multiplyMatrix(double **matA, double **matB, int r1, int c1, int r2, int c2)
{
    double** result=malloc(r1*sizeof(double*)); //initilize matrix
    for(int i = 0; i < r1; i++){
        result[i] = malloc(c2*sizeof(double*));
            for(int j = 0; j<c2; j++)
                result[i][j] = 0;
    }
    
    for(int aRow = 0; aRow < r1; aRow++){
        for(int bCol = 0; bCol < c2; bCol++){
            for(int shared = 0; shared < r2; shared++){
                 result[aRow][bCol] += matA[aRow][shared] * matB[shared][bCol];
            }
        }
    }
    
    /* FOR DEBUGGING PURPOSES: PRINTS MATRIX */
    /*
     printf("Array Multiplication\n");
     for(int i = 0; i < r1; i++){
         for(int j = 0; j < c2; j++)
    {
        printf("%lf\t", result[i][j]);
    }
    printf("\n");
    }
    printf("\n");
    */
    
    return result;
}


double** transposeMatrix(double** mat, int row, int col)
{
  
	double** matTran=malloc(col*sizeof(double*)); //initilize matrix
    for(int i = 0; i < col; i++){
        matTran[i] = malloc(row*sizeof(double*));
            for(int j = 0; j<row; j++)
                matTran[i][j] = 0;
    }
    for(int curRow = 0; curRow < row; curRow++){
        for(int curCol = 0; curCol < col; curCol++){
            matTran[curCol][curRow] = mat[curRow][curCol];
        }
    }
    
    return matTran;        
}


double** inverseMatrix(double **matA, int dimension)
{

    double** matI=malloc(dimension*sizeof(double*)); 
    for(int i = 0; i < dimension; i++){
        matI[i] = malloc(dimension*sizeof(double*));
    }

    for(int i = 0; i < dimension; i++){ //creates identitiy matrix
        for(int j = 0; j < dimension; j++){
            if(i == j)
                matI[i][j] = 1.0;
            else
                matI[i][j] = 0.0;
        }
    }

    double f = 1.0;
    for(int p = 0; p < dimension; p++){ //matrix inverted using given algo (I think this is where inaccuracy is)
        f = matA[p][p];
        for(int q = 0; q < dimension; q++){
            double a = (matA[p][q]/f);
            double I = (matI[p][q]/f);
            matA[p][q] = a;
            matI[p][q] = I;
        }
            for(int i = p+1; i < dimension; i++){
                f = matA[i][p];
                    for(int z = 0; z < dimension; z++){
                        double a = matA[i][z] - (matA[p][z] * f);
                        double I = matI[i][z] - (matI[p][z] * f);
                        matA[i][z] = a;
                        matI[i][z] = I;
                    }
            }     
    }
    for(int p = (dimension - 1); p >= 0; p--){
        for(int i = p-1; i >=0; i--){
            f = matA[i][p];
                for(int q = 0; q < dimension; q++){
                    double a = matA[i][q] - (matA[p][q] * f);
                    double I = matI[i][q] - (matI[p][q] * f);
                        matA[i][q] = a;
                        matI[i][q] = I;
                    }
        }
    }
/* FOR DEBUGGING PURPOSES: PRINTS MATRIX */
/*
    printf("Inverse\n");
     for(int i = 0; i < dimension; i++){
         for(int j = 0; j < dimension; j++)
    {
        printf("%lf\t", matI[i][j]);
    }
    printf("\n");
    }
    printf("\n");
*/
	return matI;
}

