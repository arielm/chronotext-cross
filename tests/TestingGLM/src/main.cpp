
#include <gtest/gtest.h>

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/constants.hpp> // glm::pi
#include <glm/gtc/epsilon.hpp>

using namespace std;

glm::mat4 camera(float Translate, glm::vec2 const & Rotate)
{
  glm::mat4 Projection = glm::perspective(glm::pi<float>() * 0.25f, 4.0f / 3.0f, 0.1f, 100.f);
  glm::mat4 View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -Translate));
  View = glm::rotate(View, Rotate.y, glm::vec3(-1.0f, 0.0f, 0.0f));
  View = glm::rotate(View, Rotate.x, glm::vec3(0.0f, 1.0f, 0.0f));
  glm::mat4 Model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
  return Projection * View * Model;
}

template <typename genType>
void print(genType const & Mat0)
{
  printf("mat4(\n");
  printf("\tvec4(%2.9f, %2.9f, %2.9f, %2.9f)\n", Mat0[0][0], Mat0[0][1], Mat0[0][2], Mat0[0][3]);
  printf("\tvec4(%2.9f, %2.9f, %2.9f, %2.9f)\n", Mat0[1][0], Mat0[1][1], Mat0[1][2], Mat0[1][3]);
  printf("\tvec4(%2.9f, %2.9f, %2.9f, %2.9f)\n", Mat0[2][0], Mat0[2][1], Mat0[2][2], Mat0[2][3]);
  printf("\tvec4(%2.9f, %2.9f, %2.9f, %2.9f))\n\n", Mat0[3][0], Mat0[3][1], Mat0[3][2], Mat0[3][3]);
}

TEST(TestGLM, test_vec2_operators)
{
  int Error = 0;

  {
    glm::vec2 A(1.0f);
    glm::vec2 B(1.0f);
    Error += A != B ? 1 : 0;
    Error += A == B ? 0 : 1;
  }

  {
    glm::vec2 A(1.0f);
    glm::vec2 C = A + 1.0f;
    A += 1.0f;
    Error += A.x == 2.0f && A.y == 2.0f ? 0 : 1;
    Error += A.x == C.x && A.y == C.y ? 0 : 1;
  }

  {
    glm::vec2 A(1.0f);
    glm::vec2 B(2.0f,-1.0f);
    glm::vec2 C = A + B;
    A += B;
    Error += A.x == 3.0f && A.y == 0.0f ? 0 : 1;
    Error += A.x == C.x && A.y == C.y ? 0 : 1;
  }

  {
    glm::vec2 A(1.0f);
    glm::vec2 C = A - 1.0f;
    A -= 1.0f;
    Error += A.x == 0.0f && A.y == 0.0f ? 0 : 1;
    Error += A.x == C.x && A.y == C.y ? 0 : 1;
  }

  {
    glm::vec2 A(1.0f);
    glm::vec2 B(2.0f,-1.0f);
    glm::vec2 C = A - B;
    A -= B;
    Error += A.x == -1.0f && A.y == 2.0f ? 0 : 1;
    Error += A.x == C.x && A.y == C.y ? 0 : 1;
  }

  {
    glm::vec2 A(1.0f);
    glm::vec2 C = A * 2.0f;
    A *= 2.0f;
    Error += A.x == 2.0f && A.y == 2.0f ? 0 : 1;
    Error += A.x == C.x && A.y == C.y ? 0 : 1;
  }

  {
    glm::vec2 A(2.0f);
    glm::vec2 B(2.0f);
    glm::vec2 C = A / B;
    A /= B;
    Error += A.x == 1.0f && A.y == 1.0f ? 0 : 1;
    Error += A.x == C.x && A.y == C.y ? 0 : 1;
  }

  {
    glm::vec2 A(1.0f, 2.0f);
    glm::vec2 B(4.0f, 5.0f);

    glm::vec2 C = A + B;
    Error += C == glm::vec2(5, 7) ? 0 : 1;

    glm::vec2 D = B - A;
    Error += D == glm::vec2(3, 3) ? 0 : 1;

    glm::vec2 E = A * B;
    Error += E == glm::vec2(4, 10) ? 0 : 1;

    glm::vec2 F = B / A;
    Error += F == glm::vec2(4, 2.5) ? 0 : 1;

    glm::vec2 G = A + 1.0f;
    Error += G == glm::vec2(2, 3) ? 0 : 1;

    glm::vec2 H = B - 1.0f;
    Error += H == glm::vec2(3, 4) ? 0 : 1;

    glm::vec2 I = A * 2.0f;
    Error += I == glm::vec2(2, 4) ? 0 : 1;

    glm::vec2 J = B / 2.0f;
    Error += J == glm::vec2(2, 2.5) ? 0 : 1;

    glm::vec2 K = 1.0f + A;
    Error += K == glm::vec2(2, 3) ? 0 : 1;

    glm::vec2 L = 1.0f - B;
    Error += L == glm::vec2(-3, -4) ? 0 : 1;

    glm::vec2 M = 2.0f * A;
    Error += M == glm::vec2(2, 4) ? 0 : 1;

    glm::vec2 N = 2.0f / B;
    Error += N == glm::vec2(0.5, 2.0 / 5.0) ? 0 : 1;
  }

  {
    glm::vec2 A(1.0f, 2.0f);
    glm::vec2 B(4.0f, 5.0f);

    A += B;
    Error += A == glm::vec2(5, 7) ? 0 : 1;

    A += 1.0f;
    Error += A == glm::vec2(6, 8) ? 0 : 1;
  }
  {
    glm::vec2 A(1.0f, 2.0f);
    glm::vec2 B(4.0f, 5.0f);

    B -= A;
    Error += B == glm::vec2(3, 3) ? 0 : 1;

    B -= 1.0f;
    Error += B == glm::vec2(2, 2) ? 0 : 1;
  }
  {
    glm::vec2 A(1.0f, 2.0f);
    glm::vec2 B(4.0f, 5.0f);

    A *= B;
    Error += A == glm::vec2(4, 10) ? 0 : 1;

    A *= 2.0f;
    Error += A == glm::vec2(8, 20) ? 0 : 1;
  }
  {
    glm::vec2 A(1.0f, 2.0f);
    glm::vec2 B(4.0f, 5.0f);

    B /= A;
    Error += B == glm::vec2(4, 2.5) ? 0 : 1;

    B /= 2.0f;
    Error += B == glm::vec2(2, 1.25) ? 0 : 1;
  }
  {
    glm::vec2 B(2.0f);

    B /= B.y;
    Error += B == glm::vec2(1.0f) ? 0 : 1;
  }

  {
    glm::vec2 A(1.0f, 2.0f);
    glm::vec2 B = -A;
    Error += B == glm::vec2(-1.0f, -2.0f) ? 0 : 1;
  }

  {
    glm::vec2 A(1.0f, 2.0f);
    glm::vec2 B = --A;
    Error += B == glm::vec2(0.0f, 1.0f) ? 0 : 1;
  }

  {
    glm::vec2 A(1.0f, 2.0f);
    glm::vec2 B = A--;
    Error += B == glm::vec2(1.0f, 2.0f) ? 0 : 1;
    Error += A == glm::vec2(0.0f, 1.0f) ? 0 : 1;
  }

  {
    glm::vec2 A(1.0f, 2.0f);
    glm::vec2 B = ++A;
    Error += B == glm::vec2(2.0f, 3.0f) ? 0 : 1;
  }

  {
    glm::vec2 A(1.0f, 2.0f);
    glm::vec2 B = A++;
    Error += B == glm::vec2(1.0f, 2.0f) ? 0 : 1;
    Error += A == glm::vec2(2.0f, 3.0f) ? 0 : 1;
  }

  EXPECT_FALSE(Error);
}

TEST(TestGLM, test_vec3_operators)
{
  int Error = 0;
  
  {
    glm::vec3 A(1.0f);
    glm::vec3 B(1.0f);
    bool R = A != B;
    bool S = A == B;

    Error += (S && !R) ? 0 : 1;
  }

  {
    glm::vec3 A(1.0f, 2.0f, 3.0f);
    glm::vec3 B(4.0f, 5.0f, 6.0f);

    glm::vec3 C = A + B;
    Error += C == glm::vec3(5, 7, 9) ? 0 : 1;

    glm::vec3 D = B - A;
    Error += D == glm::vec3(3, 3, 3) ? 0 : 1;

    glm::vec3 E = A * B;
    Error += E == glm::vec3(4, 10, 18) ? 0 : 1;

    glm::vec3 F = B / A;
    Error += F == glm::vec3(4, 2.5, 2) ? 0 : 1;

    glm::vec3 G = A + 1.0f;
    Error += G == glm::vec3(2, 3, 4) ? 0 : 1;

    glm::vec3 H = B - 1.0f;
    Error += H == glm::vec3(3, 4, 5) ? 0 : 1;

    glm::vec3 I = A * 2.0f;
    Error += I == glm::vec3(2, 4, 6) ? 0 : 1;

    glm::vec3 J = B / 2.0f;
    Error += J == glm::vec3(2, 2.5, 3) ? 0 : 1;

    glm::vec3 K = 1.0f + A;
    Error += K == glm::vec3(2, 3, 4) ? 0 : 1;

    glm::vec3 L = 1.0f - B;
    Error += L == glm::vec3(-3, -4, -5) ? 0 : 1;

    glm::vec3 M = 2.0f * A;
    Error += M == glm::vec3(2, 4, 6) ? 0 : 1;

    glm::vec3 N = 2.0f / B;
    Error += N == glm::vec3(0.5, 2.0 / 5.0, 2.0 / 6.0) ? 0 : 1;
  }

  {
    glm::vec3 A(1.0f, 2.0f, 3.0f);
    glm::vec3 B(4.0f, 5.0f, 6.0f);

    A += B;
    Error += A == glm::vec3(5, 7, 9) ? 0 : 1;

    A += 1.0f;
    Error += A == glm::vec3(6, 8, 10) ? 0 : 1;
  }
  {
    glm::vec3 A(1.0f, 2.0f, 3.0f);
    glm::vec3 B(4.0f, 5.0f, 6.0f);

    B -= A;
    Error += B == glm::vec3(3, 3, 3) ? 0 : 1;

    B -= 1.0f;
    Error += B == glm::vec3(2, 2, 2) ? 0 : 1;
  }
  {
    glm::vec3 A(1.0f, 2.0f, 3.0f);
    glm::vec3 B(4.0f, 5.0f, 6.0f);

    A *= B;
    Error += A == glm::vec3(4, 10, 18) ? 0 : 1;

    A *= 2.0f;
    Error += A == glm::vec3(8, 20, 36) ? 0 : 1;
  }
  {
    glm::vec3 A(1.0f, 2.0f, 3.0f);
    glm::vec3 B(4.0f, 5.0f, 6.0f);

    B /= A;
    Error += B == glm::vec3(4, 2.5, 2) ? 0 : 1;

    B /= 2.0f;
    Error += B == glm::vec3(2, 1.25, 1) ? 0 : 1;
  }
  {
    glm::vec3 B(2.0f);

    B /= B.y;
    Error += B == glm::vec3(1.0f) ? 0 : 1;
  }

  {
    glm::vec3 A(1.0f, 2.0f, 3.0f);
    glm::vec3 B = -A;
    Error += B == glm::vec3(-1.0f, -2.0f, -3.0f) ? 0 : 1;
  }

  {
    glm::vec3 A(1.0f, 2.0f, 3.0f);
    glm::vec3 B = --A;
    Error += B == glm::vec3(0.0f, 1.0f, 2.0f) ? 0 : 1;
  }

  {
    glm::vec3 A(1.0f, 2.0f, 3.0f);
    glm::vec3 B = A--;
    Error += B == glm::vec3(1.0f, 2.0f, 3.0f) ? 0 : 1;
    Error += A == glm::vec3(0.0f, 1.0f, 2.0f) ? 0 : 1;
  }

  {
    glm::vec3 A(1.0f, 2.0f, 3.0f);
    glm::vec3 B = ++A;
    Error += B == glm::vec3(2.0f, 3.0f, 4.0f) ? 0 : 1;
  }

  {
    glm::vec3 A(1.0f, 2.0f, 3.0f);
    glm::vec3 B = A++;
    Error += B == glm::vec3(1.0f, 2.0f, 3.0f) ? 0 : 1;
    Error += A == glm::vec3(2.0f, 3.0f, 4.0f) ? 0 : 1;
  }

  EXPECT_FALSE(Error);
}

TEST(TestGLM, test_inverse)
{
  int Error(0);

  {
    glm::mat4 const Matrix(
      glm::vec4(0.6f, 0.2f, 0.3f, 0.4f), 
      glm::vec4(0.2f, 0.7f, 0.5f, 0.3f), 
      glm::vec4(0.3f, 0.5f, 0.7f, 0.2f), 
      glm::vec4(0.4f, 0.3f, 0.2f, 0.6f));
    glm::mat4 const Inverse = glm::inverse(Matrix);
    glm::mat4 const Identity = Matrix * Inverse;

    print(Matrix);
    print(Inverse);
    print(Identity);

    Error += glm::all(glm::epsilonEqual(Identity[0], glm::vec4(1.0f, 0.0f, 0.0f, 0.0f), glm::vec4(0.01f))) ? 0 : 1;
    Error += glm::all(glm::epsilonEqual(Identity[1], glm::vec4(0.0f, 1.0f, 0.0f, 0.0f), glm::vec4(0.01f))) ? 0 : 1;
    Error += glm::all(glm::epsilonEqual(Identity[2], glm::vec4(0.0f, 0.0f, 1.0f, 0.0f), glm::vec4(0.01f))) ? 0 : 1;
    Error += glm::all(glm::epsilonEqual(Identity[3], glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec4(0.01f))) ? 0 : 1;
  }

  {
    glm::highp_mat4 const Matrix(
      glm::highp_vec4(0.6f, 0.2f, 0.3f, 0.4f), 
      glm::highp_vec4(0.2f, 0.7f, 0.5f, 0.3f), 
      glm::highp_vec4(0.3f, 0.5f, 0.7f, 0.2f), 
      glm::highp_vec4(0.4f, 0.3f, 0.2f, 0.6f));
    glm::highp_mat4 const Inverse = glm::inverse(Matrix);
    glm::highp_mat4 const Identity = Matrix * Inverse;

    printf("highp_mat4 inverse\n");
    print(Matrix);
    print(Inverse);
    print(Identity);

    Error += glm::all(glm::epsilonEqual(Identity[0], glm::highp_vec4(1.0f, 0.0f, 0.0f, 0.0f), glm::highp_vec4(0.01f))) ? 0 : 1;
    Error += glm::all(glm::epsilonEqual(Identity[1], glm::highp_vec4(0.0f, 1.0f, 0.0f, 0.0f), glm::highp_vec4(0.01f))) ? 0 : 1;
    Error += glm::all(glm::epsilonEqual(Identity[2], glm::highp_vec4(0.0f, 0.0f, 1.0f, 0.0f), glm::highp_vec4(0.01f))) ? 0 : 1;
    Error += glm::all(glm::epsilonEqual(Identity[3], glm::highp_vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::highp_vec4(0.01f))) ? 0 : 1;
  }

  {
    glm::mediump_mat4 const Matrix(
      glm::mediump_vec4(0.6f, 0.2f, 0.3f, 0.4f), 
      glm::mediump_vec4(0.2f, 0.7f, 0.5f, 0.3f), 
      glm::mediump_vec4(0.3f, 0.5f, 0.7f, 0.2f), 
      glm::mediump_vec4(0.4f, 0.3f, 0.2f, 0.6f));
    glm::mediump_mat4 const Inverse = glm::inverse(Matrix);
    glm::mediump_mat4 const Identity = Matrix * Inverse;

    printf("mediump_mat4 inverse\n");
    print(Matrix);
    print(Inverse);
    print(Identity);

    Error += glm::all(glm::epsilonEqual(Identity[0], glm::mediump_vec4(1.0f, 0.0f, 0.0f, 0.0f), glm::mediump_vec4(0.01f))) ? 0 : 1;
    Error += glm::all(glm::epsilonEqual(Identity[1], glm::mediump_vec4(0.0f, 1.0f, 0.0f, 0.0f), glm::mediump_vec4(0.01f))) ? 0 : 1;
    Error += glm::all(glm::epsilonEqual(Identity[2], glm::mediump_vec4(0.0f, 0.0f, 1.0f, 0.0f), glm::mediump_vec4(0.01f))) ? 0 : 1;
    Error += glm::all(glm::epsilonEqual(Identity[3], glm::mediump_vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::mediump_vec4(0.01f))) ? 0 : 1;
  }

  {
    glm::lowp_mat4 const Matrix(
      glm::lowp_vec4(0.6f, 0.2f, 0.3f, 0.4f), 
      glm::lowp_vec4(0.2f, 0.7f, 0.5f, 0.3f), 
      glm::lowp_vec4(0.3f, 0.5f, 0.7f, 0.2f), 
      glm::lowp_vec4(0.4f, 0.3f, 0.2f, 0.6f));
    glm::lowp_mat4 const Inverse = glm::inverse(Matrix);
    glm::lowp_mat4 const Identity = Matrix * Inverse;

    printf("lowp_mat4 inverse\n");
    print(Matrix);
    print(Inverse);
    print(Identity);

    Error += glm::all(glm::epsilonEqual(Identity[0], glm::lowp_vec4(1.0f, 0.0f, 0.0f, 0.0f), glm::lowp_vec4(0.01f))) ? 0 : 1;
    Error += glm::all(glm::epsilonEqual(Identity[1], glm::lowp_vec4(0.0f, 1.0f, 0.0f, 0.0f), glm::lowp_vec4(0.01f))) ? 0 : 1;
    Error += glm::all(glm::epsilonEqual(Identity[2], glm::lowp_vec4(0.0f, 0.0f, 1.0f, 0.0f), glm::lowp_vec4(0.01f))) ? 0 : 1;
    Error += glm::all(glm::epsilonEqual(Identity[3], glm::lowp_vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::lowp_vec4(0.01f))) ? 0 : 1;
  }

  {
    glm::mat4 const Matrix(
      glm::vec4(0.6f, 0.2f, 0.3f, 0.4f), 
      glm::vec4(0.2f, 0.7f, 0.5f, 0.3f), 
      glm::vec4(0.3f, 0.5f, 0.7f, 0.2f), 
      glm::vec4(0.4f, 0.3f, 0.2f, 0.6f));
    glm::mat4 const Identity = Matrix / Matrix;

    Error += glm::all(glm::epsilonEqual(Identity[0], glm::vec4(1.0f, 0.0f, 0.0f, 0.0f), glm::vec4(0.01f))) ? 0 : 1;
    Error += glm::all(glm::epsilonEqual(Identity[1], glm::vec4(0.0f, 1.0f, 0.0f, 0.0f), glm::vec4(0.01f))) ? 0 : 1;
    Error += glm::all(glm::epsilonEqual(Identity[2], glm::vec4(0.0f, 0.0f, 1.0f, 0.0f), glm::vec4(0.01f))) ? 0 : 1;
    Error += glm::all(glm::epsilonEqual(Identity[3], glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec4(0.01f))) ? 0 : 1;
  }

  EXPECT_FALSE(Error);
}
