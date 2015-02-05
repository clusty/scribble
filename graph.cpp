#include "graph.h"

inline int heuristic(const Graph::Location& a, const Graph::Location& b) 
{
  return abs(a.first - b.first) + abs(a.second - b.second);
}

std::vector<Graph::Location> 
aStar( const Graph& graph, const Graph::Location& start, const Graph::Location& goal )
{
   
   std::map<Graph::Location, float> cost_so_far;
   std::map<Graph::Location, Graph::Location> came_from;
   
   PriorityQueue<Graph::Location, float> frontier;
   frontier.put(start, 0);
   came_from[start] = start;
   cost_so_far[start] = 0;
   
   while (!frontier.empty())
   {
      Graph::Location current = frontier.get();
      if ( current == goal )
      {
         break;
      }
      
      Graph::Neighbours n = graph.getNeighbours( current );
      for ( Graph::Neighbours::const_iterator it = n.begin();
            it!= n.end(); ++it )
      {
         Graph::Location next = *it;
         float new_cost = cost_so_far[current] + graph.cost(current, next);
         if (!cost_so_far.count(next) || new_cost < cost_so_far[next]) 
         {
            cost_so_far[next] = new_cost;
            float priority = new_cost + heuristic(next, goal);
            frontier.put(next, priority);
            came_from[next] = current;
         }
      } 
   }
   
   std::vector<Graph::Location> path;
   Graph::Location current = goal;
   path.push_back(current);
   while (current != start) 
   {
     current = came_from[current];
     path.push_back(current);
   }
   return path;
}

