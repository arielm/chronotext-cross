#include "chr/path/Shape.h"

using namespace std;
using namespace chr;
using namespace math;

namespace chr
{
  namespace path
  {
    const string& Shape::getId() const
    {
      return id;
    }

    size_t Shape::size() const
    {
      return paths.size();
    }

    bool Shape::empty() const
    {
      return paths.empty();
    }

    Shape::FillRule Shape::getFillRule() const
    {
      return fillRule;
    }

    Rectf Shape::getBounds() const
    {
      return bounds;
    }

    const vector<Path>& Shape::getPaths() const
    {
      return paths;
    }

    Path& Shape::addPath()
    {
      paths.emplace_back();
      return paths.back();
    }
  }
}
