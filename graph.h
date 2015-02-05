#ifndef GRAPH
#define GRAPH
#include <utility>
#include <set>
#include <queue>
#include <map>

#include <QImage>

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
   Graph(int w, int h, const QImage* cost):_width(w), _height(h), _cost(cost){}
   std::set<Location> getNeighbours(const Location &l) const
   {
      std::set<Location> neighbours;
      int dir[][2] = {{-1,0},{1,0}, {0,-1}, {0,1}};
      for ( unsigned i = 0; i<sizeof(dir);++i)
      {
         Location n; n.first = l.first + dir[i][0]; n.second = l.second + dir[i][1];
         if ( n.first > 0 && n.first < _width && 
              n.second > 0 && n.second < _height )
            neighbours.insert(n);
      }
      
      return neighbours;
   }
   
   float cost(const Location& , const Location& b) const
   {

      QPoint y(b.first, b.second);
      QRgb c = _cost->pixel(y);
      return qRed(c);
   }
   
private:
int _width, _height;   
const QImage *_cost;
};

std::vector<Graph::Location> 
aStar( const Graph& graph, const Graph::Location& start, const Graph::Location& goal );


#endif // GRAPH

