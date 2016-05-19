#pragma once

#include <CGAL/Polyhedron_3.h>
#include <CGAL/Nef_polyhedron_3.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel Kernel;
typedef CGAL::Nef_polyhedron_3<Kernel> Nef_polyhedron;

//typedef Kernel::Vector_3                                     Vector;
//typedef CGAL::Polyhedron_3<Kernel>                           Polyhedron;
//typedef Polyhedron::Vertex                                   Vertex;
//typedef Polyhedron::Vertex_iterator                          Vertex_iterator;
//typedef Polyhedron::Halfedge_handle                          Halfedge_handle;
//typedef Polyhedron::Edge_iterator                            Edge_iterator;
//typedef Polyhedron::Facet_iterator                           Facet_iterator;
//typedef Polyhedron::Halfedge_around_vertex_const_circulator  HV_circulator;
//typedef Polyhedron::Halfedge_around_facet_circulator         HF_circulator;
//typedef Polyhedron::HalfedgeDS HalfedgeDS;
//typedef Polyhedron::size_type size_type;
//typedef Polyhedron::Halfedge_const_handle Halfedge_const_handle;
//typedef Polyhedron::Facet_const_iterator Facet_const_iterator;
//typedef Polyhedron::Facet::Halfedge_around_facet_circulator Halfedge_around_facet_circulator;
