#ifndef GRAPH
#define GRAPH
#include <utility>
#include <set>
#include <queue>
#include <map>
#include <cmath>

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgproc/types_c.h"

template<typename T, typename Number=int>
struct PriorityQueue {
  typedef std::pair<Number, T> PQElement;
  std::priority_queue<PQElement, std::vector<PQElement>, 
                 std::greater<PQElement> > elements;

  inline bool empty() { return elements.empty(); }

  inline void put(T item, Number priority) {
    elements.push(std::make_pair(priority, item) );
  }

  inline T get() {
    T best_item = elements.top().second;
    elements.pop();
    return best_item;
  }
};

class Graph
{
public:
   typedef std::pair<int, int> Location;
   typedef std::set<Location> Neighbours;
   Graph(){}
   Graph(int w, int h, cv::Mat* lGrad, cv::Mat* pGrad, cv::Mat* lapZ):_width(w), 
                                                                                         _height(h), 
                                                                                         _lGrad(lGrad),
                                                                                         _pGrad(pGrad),
                                                                                         _lapZ(lapZ)
   {}
   std::set<Location> getNeighbours(const Location &l) const
   {
      std::set<Location> neighbours;
      int dir[][2] = {{-1,0},{1,0}, {0,-1}, {0,1}, {-1,-1},{1,-1}, {-1,-1}, {1,1}};
      for ( unsigned i = 0; i<sizeof(dir);++i)
      {
         Location n; n.first = l.first + dir[i][0]; n.second = l.second + dir[i][1];
         if ( n.first > 0 && n.first < _width && 
              n.second > 0 && n.second < _height )
            neighbours.insert(n);
      }
      
      return neighbours;
   }
   
   float cost(const Location&a , const Location& b) const
   {
      const float wz = 0.43; // lap
      const float wg = 0.14; // mag
      const float wd = 0.43; // dir

      
      const float gm = _lGrad->at<float>(b.first, b.second);
      
      const float aa = _pGrad->at<float>(a.first, a.second);
      const float ba = _pGrad->at<float>(b.first, b.second);
      const float ga = 1 / (cos(aa) * cos(ba) + sin(aa) * sin(ba) );
      const float lap = _lapZ->at<float>(b.first, b.second);
      
      return wz * lap + wg * gm + wd * ga;
   }
   
private:
int _width, _height;   
cv::Mat *_lGrad, *_pGrad ,*_lapZ;
};

std::vector<Graph::Location> 
aStar( const Graph& graph, const Graph::Location& start, const Graph::Location& goal );


#endif // GRAPH

