#ifndef GRAPH
#define GRAPH
#include <utility>
#include <set>
#include <queue>
#include <map>
#include <cmath>
#include <QImage>

#include<cassert>

template<typename T, typename Number=int>
struct PriorityQueue {
  typedef std::pair<Number, T> PQElement;
  std::priority_queue<PQElement, std::vector<PQElement>, 
                 std::greater<PQElement> > elements;

  inline bool empty() { return elements.empty(); }

  inline void put(T item, Number priority) {
    elements.push(std::make_pair(priority, item) );
  }
  
  void clear()
  {
    elements = std::priority_queue<PQElement, std::vector<PQElement>, 
          std::greater<PQElement> >();     
  }

  inline T get() {
    T best_item = elements.top().second;
    //elements.pop();
    return best_item;
  }
  
  inline void pop()
  {
      elements.pop();     
  }
};

class Graph
{
public:
   typedef std::pair<int, int> Location;
   typedef std::set<Location> Neighbours;
   Graph(){}
   Graph(int w, int h, const QImage* cost):_width(w), _height(h), _cost(cost){};
   void setStart( Location ); 
   std::vector<Graph::Location> 
   aStar( const Graph::Location& goal );
private:
   
   std::set<Location> getNeighbours(const Location &l) const
   {
      std::set<Location> neighbours;
      int dir[][2] = {{-1,0},{1,0}, {0,-1}, {0,1}/*, {-1,-1},{1,-1}, {-1,-1}, {1,1}*/};
      for ( unsigned i = 0; i<sizeof(dir);++i)
      {
         Location n; n.first = l.first + dir[i][0]; n.second = l.second + dir[i][1];
         if ( n.first > 0 && n.first < _width && 
              n.second > 0 && n.second < _height )
            neighbours.insert(n);
      }
      
      return neighbours;
   }
   
   inline
   
   inline float heuristic(const Graph::Location& a, const Graph::Location& b) 
   {
     return abs(a.first - b.first) + abs(a.second - b.second) ;
   }
   
   float cost(const Location&a , const Location& b) const
   {
      QPoint x(a.first, a.second);
      QPoint y(b.first, b.second);
      float c = qRed(_cost->pixel(y)) / 255.0;
      assert(c <= 1);
      return c;
   }
   
   
int _width, _height;   
const QImage *_cost;
Location start;
std::map<Graph::Location, float> cost_so_far;
std::map<Graph::Location, Graph::Location> came_from;
PriorityQueue<Graph::Location, float> frontier;
};




#endif // GRAPH

