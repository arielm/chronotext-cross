#include "MeshCreator.h"

int main ()
{
  Polyhedron P1(8, 24, 12);
  Polyhedron P2(8, 24, 12);

  std::vector<double> vertices1 = { 0, 0, 0,
                                    2, 0, 0,
                                    0, 2, 0,
                                    2, 2, 0,
                                    0, 0, 2,
                                    2, 0, 2,
                                    0, 2, 2,
                                    2, 2, 2 };

  std::vector<double> vertices2 = { 1, 1, 1,
                                    3, 1, 1,
                                    1, 3, 1,
                                    3, 3, 1,
                                    1, 1, 3,
                                    3, 1, 3,
                                    1, 3, 3,
                                    3, 3, 3 };

  std::vector<int> triangles = { 0, 1, 2,
                                 1, 3, 2,
                                 0, 2, 4,
                                 4, 2, 5,
                                 4, 5, 7,
                                 5, 6, 7,
                                 3, 1, 7,
                                 7, 6, 3,
                                 2, 3, 6,
                                 6, 5, 2,
                                 1, 0, 7,
                                 0, 4, 7 };

  MeshCreator<HalfedgeDS> meshCreator1(vertices1, triangles, 24);
  MeshCreator<HalfedgeDS> meshCreator2(vertices2, triangles, 24);
  P1.delegate(meshCreator1);
  P2.delegate(meshCreator2);

  bool isClosed    = P1.is_closed()        && (P2.is_closed());
  bool isValid     = P1.is_valid()         && (P2.is_valid());
  bool isTriangles = P1.is_pure_triangle() && (P2.is_pure_triangle());

  Nef_polyhedron nef1(P1);
  Nef_polyhedron nef2(P2);

  Nef_polyhedron nef3 = nef1 + nef2;
  Polyhedron result;
  nef3.convert_to_Polyhedron(result);

  return 0;
}
