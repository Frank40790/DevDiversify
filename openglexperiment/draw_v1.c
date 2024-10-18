// gcc draw_v1.c -o draw_v1 -Wno-deprecated-declarations -lglfw -framework OpenGL
// ./draw_v1

#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define POINT_SIZE 20.0
#define LINE_WIDTH 10.0

float** create_vector(float values[3]);
void draw_point(float**);
void draw_line(float**, float**);
void draw_triangle(float**, float**, float**);
void display(float);
float to_radian(float);
float** rotation_matrix(char, float);
float** transformation_matrix(float input[3][3]);
float** projection_matrix(float input[2][3]);
float** matmul(float**, float**, int, int, int);
float** point_to_heap(float arr[3]);

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

  // coordinates
  float tmpA[3] = {0., 0., 1.};
  float** A = create_vector(tmpA);

  float tmpB[3] = {1., 0., 0.};
  float** B = create_vector(tmpB);

  float tmpC[3] = {0., 1., 0.};
  float** C = create_vector(tmpC);

  float tmpD[3] = {-1., 0., 0.};
  float** D = create_vector(tmpD);

  float tmpE[3] = {0., -1., 0.};
  float** E = create_vector(tmpE);

  float tmpF[3] = {0., 0., -1.};
  float** F = create_vector(tmpF);

  // transform point
  float t[3][3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
  float** T = transformation_matrix(t);

  A = matmul(T, A, 3, 3, 1);

  // rotate degree
  float** Rx = rotation_matrix('x', degree);
  float** Ry = rotation_matrix('y', degree);
  float** Rz = rotation_matrix('z', degree);

  A = matmul(Ry, A, 3, 3, 1);
  B = matmul(Ry, B, 3, 3, 1);
  C = matmul(Ry, C, 3, 3, 1);
  D = matmul(Ry, D, 3, 3, 1);
  E = matmul(Ry, E, 3, 3, 1);
  F = matmul(Ry, F, 3, 3, 1);

  // project from 3d to 2d
  float p[2][3] = {{1, 0, 0}, {0, 1, 0}};
  float** P = projection_matrix(p);

  float** Ap = matmul(P, A, 2, 3, 1);
  float** Bp = matmul(P, B, 2, 3, 1);
  float** Cp = matmul(P, C, 2, 3, 1);
  float** Dp = matmul(P, D, 2, 3, 1);
  float** Ep = matmul(P, E, 2, 3, 1);
  float** Fp = matmul(P, F, 2, 3, 1);

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
}

float** create_vector(float values[3]) {
  float** vector = malloc(3 * sizeof(float*));
  for (int i = 0; i < 3; i++) {
    vector[i] = malloc(1 * sizeof(float));
    vector[i][0] = values[i];
  }
  return vector;
}

void free_vector(float** vector) {
  for (int i = 0; i < 3; i++) {
    free(vector[i]);
  }
  free(vector);
}

float** matmul(float** matrix_1, float** matrix_2, int matrix_1_row,
               int matrix_1_col, int matrix_2_col) {
  // allocate memory for the result matrix
  float** mul = calloc(matrix_1_row, sizeof(float*));
  assert(mul);
  for (int i = 0; i < matrix_1_row; i++) {
    mul[i] = calloc(matrix_2_col, sizeof(float));
    assert(mul[i]);
  }

  // matrix multiplication
  for (int i = 0; i < matrix_1_row; i++) {
    for (int k = 0; k < matrix_2_col; k++) {
      float num = 0.;
      for (int j = 0; j < matrix_1_col; j++) {
        num += matrix_1[i][j] * matrix_2[j][k];
      }
      mul[i][k] = num;
    }
  }

  return mul;
}

void draw_point(float** coordinate) {
  glPointSize(POINT_SIZE);
  glBegin(GL_POINTS);
  glVertex2f(coordinate[0][0], coordinate[1][0]);
  glEnd();
}

void draw_line(float** coordinate_1, float** coordinate_2) {
  glLineWidth(LINE_WIDTH);
  glBegin(GL_LINES);
  glVertex2f(coordinate_1[0][0], coordinate_1[1][0]);  // Start point
  glVertex2f(coordinate_2[0][0], coordinate_2[1][0]);  // End point
  glEnd();
}

void draw_triangle(float** coordinate_1, float** coordinate_2,
                   float** coordinate_3) {
  glBegin(GL_TRIANGLES);
  glVertex2f(coordinate_1[0][0], coordinate_1[1][0]);  // First point
  glVertex2f(coordinate_2[0][0], coordinate_2[1][0]);  // Second point
  glVertex2f(coordinate_3[0][0], coordinate_3[1][0]);  // Third point
  glEnd();
}

float** rotation_matrix(char direction, float theta) {
  theta = to_radian(theta);
  float** R = calloc(3, sizeof(float*));  // allocate 3 rows
  assert(R);
  R[0] = calloc(3, sizeof(float));
  assert(R[0]);
  R[1] = calloc(3, sizeof(float));
  assert(R[1]);
  R[2] = calloc(3, sizeof(float));
  assert(R[2]);

  if (direction == 'x') {
    // first row
    R[0][0] = 1.;
    R[0][1] = 0.;
    R[0][2] = 0.;
    // second row
    R[1][0] = 0.;
    R[1][1] = cos(theta);
    R[1][2] = sin(theta);
    // third row
    R[2][0] = 0.;
    R[2][1] = sin(theta);
    R[2][2] = cos(theta);
  }
  if (direction == 'y') {
    // first row
    R[0][0] = cos(theta);
    R[0][1] = 0.;
    R[0][2] = sin(theta);
    // second row
    R[1][0] = 0.;
    R[1][1] = 1.;
    R[1][2] = 0.;
    // third row
    R[2][0] = -sin(theta);
    R[2][1] = 0.;
    R[2][2] = cos(theta);
  }
  if (direction == 'z') {
    // first row
    R[0][0] = cos(theta);
    R[0][1] = -sin(theta);
    R[0][2] = 0;
    // second row
    R[1][0] = sin(theta);
    R[1][1] = cos(theta);
    R[1][2] = 0.;
    // third row
    R[2][0] = 0.;
    R[2][1] = 0.;
    R[2][2] = 1.;
  }
  return R;
}

float** transformation_matrix(float input[3][3]) {
  float** matrix = calloc(3, sizeof(float*));
  assert(matrix);

  for (int i = 0; i < 3; i++) {
    matrix[i] = calloc(3, sizeof(float));
    assert(matrix[i]);
  }

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      matrix[i][j] = input[i][j];
    }
  }

  return matrix;
}

float** projection_matrix(float input[2][3]) {
  float** matrix = calloc(2, sizeof(float*));
  assert(matrix);

  for (int i = 0; i < 2; i++) {
    matrix[i] = calloc(3, sizeof(float));
    assert(matrix[i]);
  }

  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 3; j++) {
      matrix[i][j] = input[i][j];
    }
  }

  return matrix;
}

float to_radian(float degree) { return degree * (M_PI / 180); }
