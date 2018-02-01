/*!
\ingroup PkgBGLConcepts
\cgalConcept

The concept `FaceGraph` refines the concept `HalfedgeGraph`. 
It adds the requirements for a graph to explicitly
maintain faces described by halfedges, to provide access from a face to
an incident halfedge, and to provide access from a halfedge to its incident
face. 

\cgalAssociatedTypesBegin

\cgalAssociatedTypeBegin{boost::graph_traits<FaceGraph>::%face_descriptor} A face descriptor corresponds to a unique face in an abstract graph instance.
A face descriptor must be `DefaultConstructible`, `Assignable`, `EqualityComparable`, and `Hashable`. 
\cgalAssociatedTypeEnd

\cgalAssociatedTypesEnd

\cgalRefines `HalfedgeGraph`
\cgalHasModel `CGAL::Polyhedron_3`
\cgalHasModel `CGAL::Surface_mesh`
\cgalHasModel `CGAL::Linear_cell_complex_for_combinatorial_map`

*/
class FaceGraph {
  /// Returns a special `boost::graph_traits<HalfedgeGraph>::face_descriptor` object which
  /// does not refer to any face of graph object which type is `FaceGraph`.
  static boost::graph_traits<HalfedgeGraph>::halfedge_descriptor null_face();
};

/*! \relates FaceGraph
returns the face incident to halfedge `h`.
 */
template <typename FaceGraph>
boost::graph_traits<FaceGraph>::face_descriptor
face(boost::graph_traits<FaceGraph>::halfedge_descriptor h, const FaceGraph& g);

/*! \relates FaceGraph
returns the halfedge incident to face `f`.
 */
template <typename FaceGraph>
boost::graph_traits<FaceGraph>::halfedge_descriptor
halfedge(boost::graph_traits<FaceGraph>::face_descriptor f, const FaceGraph& g);

/*! \relates FaceGraph
returns the number of halfedges incident to face `f`.
 */
template <typename FaceGraph>
boost::graph_traits<FaceGraph>::degree_size_type
degree(boost::graph_traits<FaceGraph>::face_descriptor f, const FaceGraph& g);

