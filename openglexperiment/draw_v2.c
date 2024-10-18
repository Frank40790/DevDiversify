// gcc draw_v2.c -o draw_v2 -Wno-deprecated-declarations -lglfw -framework OpenGL
// ./draw_v2

#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define POINT_SIZE 20.0
#define LINE_WIDTH 10.0

typedef float type_t;

typedef struct {
  type_t* vector;  // array of float_t
  int vector_len;
} vector_t;

typedef struct {
  vector_t** rows;  // array of vector_t poitner
  int row_len;
} matrix_t;

typedef void* (*matmul_func)(void*, void*);

// creating vector or matrix
vector_t* create_vector(type_t* input, int size);
matrix_t* create_matrix(type_t** input, int row, int col);
matrix_t* transformation_matrix(float input[3][3]);
matrix_t* projection_matrix(float input[2][3]);
matrix_t* rotation_matrix(char direction, float theta);

// print vector or matrix
void print_vector(vector_t* vec);
void print_matrix(matrix_t* mat);

// draw OpenGL
void draw_point(vector_t* coordinate);
void draw_line(vector_t* coordinate_1, vector_t* coordinate_2);
void draw_triangle(vector_t* coordinate_1, vector_t* coordinate_2,
                   vector_t* coordinate_3);

// math
float to_radian(float);
matrix_t* mm(matrix_t* matrix_1,
             matrix_t* matrix_2);  // matrix matrix multiplication
vector_t* mv(matrix_t* matrix,
             vector_t* vector);  // matrix vector multiplication
void* matmul(matrix_t* matrix, void* input, matmul_func func);

// free memory
void free_vector(vector_t* vec);
void free_matrix(matrix_t* matrix);

// utils
void display(float);


int main() {
  if (!glfwInit()) {
    return -1;
  }

  GLFWwindow* window = glfwCreateWindow(640, 480, "OpenGL", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glClearColor(1.0, 1.0, 1.0, 1.0);  // Set background to white

  float degree = 0;
  while (!glfwWindowShouldClose(window)) {
    display(degree);
    glfwSwapBuffers(window);
    glfwPollEvents();
    degree += 0.5;
  }

  glfwTerminate();
  return 0;
}

void display(float degree) {
  glClear(GL_COLOR_BUFFER_BIT);
  glColor3f(1.0, 0.0, 0.0);  // set color to red

  int vector_size = 3;
  // coordinates
  type_t tmpA[] = {0, 0, 1};
  vector_t* A = create_vector(tmpA, vector_size);

  type_t tmpB[] = {1, 0, 0};
  vector_t* B = create_vector(tmpB, vector_size);

  type_t tmpC[] = {0, 1, 0};
  vector_t* C = create_vector(tmpC, vector_size);

  type_t tmpD[] = {-1, 0, 0};
  vector_t* D = create_vector(tmpD, vector_size);

  type_t tmpE[] = {0, -1, 0};
  vector_t* E = create_vector(tmpE, vector_size);

  type_t tmpF[] = {0, 0, -1};
  vector_t* F = create_vector(tmpF, vector_size);

  // transform point
  type_t t[3][3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
  matrix_t* T = transformation_matrix(t);

  // project from 3d to 2d
  type_t p[2][3] = {{1, 0, 0}, {0, 1, 0}};
  matrix_t* P = projection_matrix(p);

  // rotate degree
  matrix_t* Rx = rotation_matrix('x', degree);
  matrix_t* Ry = rotation_matrix('y', degree);
  matrix_t* Rz = rotation_matrix('z', degree);

  vector_t* Ar = matmul(Ry, A, (matmul_func)mv);
  vector_t* Br = matmul(Ry, B, (matmul_func)mv);
  vector_t* Cr = matmul(Ry, C, (matmul_func)mv);
  vector_t* Dr = matmul(Ry, D, (matmul_func)mv);
  vector_t* Er = matmul(Ry, E, (matmul_func)mv);
  vector_t* Fr = matmul(Ry, F, (matmul_func)mv);

  // projection to screen (2D)
  vector_t* Ap = matmul(P, Ar, (matmul_func)mv);
  vector_t* Bp = matmul(P, Br, (matmul_func)mv);
  vector_t* Cp = matmul(P, Cr, (matmul_func)mv);
  vector_t* Dp = matmul(P, Dr, (matmul_func)mv);
  vector_t* Ep = matmul(P, Er, (matmul_func)mv);
  vector_t* Fp = matmul(P, Fr, (matmul_func)mv);

  // draw the points
  draw_point(Ap);
  draw_point(Bp);
  draw_point(Cp);
  draw_point(Dp);
  draw_point(Ep);
  draw_point(Fp);

  // draw line
  // draw_line(Ap, Bp);
  draw_line(Ap, Cp);
  // draw_line(Ap, Dp);
  draw_line(Ap, Ep);

  // draw_line(Fp, Bp);
  draw_line(Fp, Cp);
  // draw_line(Fp, Dp);
  draw_line(Fp, Ep);

  draw_line(Bp, Cp);
  draw_line(Cp, Dp);
  draw_line(Dp, Ep);
  draw_line(Ep, Bp);

  // free memory
  free_vector(A);
  free_vector(B);
  free_vector(C);
  free_vector(D);
  free_vector(E);
  free_vector(F);

  free_vector(Ap);
  free_vector(Bp);
  free_vector(Cp);
  free_vector(Dp);
  free_vector(Ep);
  free_vector(Fp);

  free_vector(Ar);
  free_vector(Br);
  free_vector(Cr);
  free_vector(Dr);
  free_vector(Er);
  free_vector(Fr);

  free_matrix(T);
  free_matrix(P);

  free_matrix(Rx);
  free_matrix(Ry);
  free_matrix(Rz);
}
float to_radian(float degree) { return degree * (M_PI / 180); }

vector_t* create_vector(type_t* values, int size) {
  vector_t* vector = calloc(1, sizeof(vector_t));
  assert(vector);
  vector->vector_len = size;
  vector->vector = calloc(size, sizeof(type_t));
  assert(vector->vector);
  for (int i = 0; i < size; i++) {
    vector->vector[i] = values[i];
  }
  return vector;
}

matrix_t* create_matrix(type_t** input, int row, int col) {
  matrix_t* matrix = calloc(row, sizeof(matrix_t));
  assert(matrix);
  matrix->row_len = row;
  matrix->rows = calloc(matrix->row_len, sizeof(vector_t*));
  assert(matrix->rows);

  // each row is a vector_t
  for (int i = 0; i < row; i++) {
    matrix->rows[i] = create_vector(input[i], col);
  }
  return matrix;
}

matrix_t* transformation_matrix(float input[3][3]) {
  type_t** input_dynamic = calloc(3, sizeof(type_t*));
  assert(input_dynamic);

  for (int i = 0; i < 3; i++) {
    input_dynamic[i] = calloc(3, sizeof(type_t));
    assert(input_dynamic[i]);

    for (int j = 0; j < 3; j++) {
      input_dynamic[i][j] = input[i][j];
    }
  }

  matrix_t* matrix = create_matrix(input_dynamic, 3, 3);

  for (int i = 0; i < 3; i++) {
    free(input_dynamic[i]);
  }
  free(input_dynamic);

  return matrix;
}

matrix_t* projection_matrix(float input[2][3]) {
  type_t** input_dynamic = calloc(2, sizeof(type_t*));
  assert(input_dynamic);

  for (int i = 0; i < 2; i++) {
    input_dynamic[i] = calloc(3, sizeof(type_t));
    assert(input_dynamic[i]);

    for (int j = 0; j < 3; j++) {
      input_dynamic[i][j] = input[i][j];
    }
  }

  matrix_t* matrix = create_matrix(input_dynamic, 2, 3);

  for (int i = 0; i < 2; i++) {
    free(input_dynamic[i]);
  }
  free(input_dynamic);

  return matrix;
}

matrix_t* rotation_matrix(char direction, float theta) {
  theta = to_radian(theta);
  type_t** R = calloc(3, sizeof(type_t*));  // Allocate 3 rows
  assert(R);

  for (int i = 0; i < 3; i++) {
    R[i] = calloc(3, sizeof(type_t));  // Allocate 3 columns for each row
    assert(R[i]);
  }

  if (direction == 'x') {
    R[0][0] = 1.0;
    R[0][1] = 0.0;
    R[0][2] = 0.0;
    R[1][0] = 0.0;
    R[1][1] = cos(theta);
    R[1][2] = sin(theta);
    R[2][0] = 0.0;
    R[2][1] = -sin(theta);
    R[2][2] = cos(theta);
  } else if (direction == 'y') {
    R[0][0] = cos(theta);
    R[0][1] = 0.0;
    R[0][2] = sin(theta);
    R[1][0] = 0.0;
    R[1][1] = 1.0;
    R[1][2] = 0.0;
    R[2][0] = -sin(theta);
    R[2][1] = 0.0;
    R[2][2] = cos(theta);
  } else if (direction == 'z') {
    R[0][0] = cos(theta);
    R[0][1] = -sin(theta);
    R[0][2] = 0.0;
    R[1][0] = sin(theta);
    R[1][1] = cos(theta);
    R[1][2] = 0.0;
    R[2][0] = 0.0;
    R[2][1] = 0.0;
    R[2][2] = 1.0;
  } else {
    free(R);
    return NULL;
  }

  matrix_t* matrix = create_matrix(R, 3, 3);

  for (int i = 0; i < 3; i++) {
    free(R[i]);
  }
  free(R);

  return matrix;
}

void print_vector(vector_t* vec) {
  printf("[ ");
  for (int i = 0; i < vec->vector_len; i++) {
    printf("%f ", vec->vector[i]);
  }
  printf("]\n");
}

void print_matrix(matrix_t* mat) {
  printf("[ \n");
  for (int i = 0; i < mat->row_len; i++) {
    printf("\t");
    print_vector(mat->rows[i]);
  }
  printf("]\n");
}

void draw_point(vector_t* coordinate) {
  glPointSize(POINT_SIZE);
  glBegin(GL_POINTS);
  glVertex2f(coordinate->vector[0], coordinate->vector[1]);
  glEnd();
}

void draw_line(vector_t* coordinate_1, vector_t* coordinate_2) {
  glLineWidth(LINE_WIDTH);
  glBegin(GL_LINES);
  glVertex2f(coordinate_1->vector[0], coordinate_1->vector[1]);  // Start point
  glVertex2f(coordinate_2->vector[0], coordinate_2->vector[1]);  // End point
  glEnd();
}

void draw_triangle(vector_t* coordinate_1, vector_t* coordinate_2,
                   vector_t* coordinate_3) {
  glBegin(GL_TRIANGLES);
  glVertex2f(coordinate_1->vector[0], coordinate_1->vector[1]);  // First point
  glVertex2f(coordinate_2->vector[0], coordinate_2->vector[1]);  // Second point
  glVertex2f(coordinate_3->vector[0], coordinate_3->vector[1]);  // Third point
  glEnd();
}

matrix_t* mm(matrix_t* matrix_1, matrix_t* matrix_2) {
  // Check matrix dimensions
  assert(matrix_1->rows && matrix_2->rows);
  assert(matrix_1->row_len > 0 && matrix_2->row_len > 0);
  assert(matrix_1->rows[0]->vector_len == matrix_2->row_len);

  // Allocate memory for the result matrix
  int matrix_1_row = matrix_1->row_len;
  int matrix_1_col = matrix_1->rows[0]->vector_len;
  int matrix_2_col = matrix_2->rows[0]->vector_len;

  matrix_t* result = calloc(1, sizeof(matrix_t));
  assert(result);
  result->row_len = matrix_1_row;
  result->rows = calloc(matrix_1_row, sizeof(vector_t*));
  assert(result->rows);

  for (int i = 0; i < matrix_1_row; i++) {
    result->rows[i] = calloc(1, sizeof(vector_t));
    assert(result->rows[i]);
    result->rows[i]->vector_len = matrix_2_col;
    result->rows[i]->vector = calloc(matrix_2_col, sizeof(type_t));
    assert(result->rows[i]->vector);
  }

  // Matrix multiplication
  for (int i = 0; i < matrix_1_row; i++) {
    for (int k = 0; k < matrix_2_col; k++) {
      float num = 0.;
      for (int j = 0; j < matrix_1_col; j++) {
        num += matrix_1->rows[i]->vector[j] * matrix_2->rows[j]->vector[k];
      }
      result->rows[i]->vector[k] = num;
    }
  }

  return result;
}

vector_t* mv(matrix_t* matrix, vector_t* vector) {
  assert(matrix != NULL && vector != NULL);
  assert(matrix->rows != NULL);
  assert(matrix->row_len > 0 &&
         matrix->rows[0]->vector_len == vector->vector_len);

  vector_t* result = calloc(1, sizeof(vector_t));
  assert(result);
  result->vector_len = matrix->row_len;
  result->vector = calloc(result->vector_len, sizeof(type_t));
  assert(result->vector);

  for (int i = 0; i < matrix->row_len; i++) {
    type_t sum = 0;
    for (int j = 0; j < vector->vector_len; j++) {
      sum += matrix->rows[i]->vector[j] * vector->vector[j];
    }
    result->vector[i] = sum;
  }

  return result;
}

void* matmul(matrix_t* matrix, void* input, matmul_func func) {
  return func(matrix, input);
}

void free_vector(vector_t* vec) {
  free(vec->vector);
  free(vec);
}
void free_matrix(matrix_t* matrix) {
  for (int i = 0; i < matrix->row_len; i++) {
    if (matrix->rows[i] != NULL) {
      free_vector(matrix->rows[i]);
    }
  }
  free(matrix->rows);
  free(matrix);
}