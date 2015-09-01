#include "graph.h"

void Graph::setStart( Graph::Location aStart )
{
   start = aStart;
   cost_so_far.clear();
   came_from.clear();
   frontier.clear();
   
   frontier.put(start, 0);
   came_from[start] = start;
   cost_so_far[start] = 0;   
}

std::vector<Graph::Location> 
Graph::aStar( const Graph::Location& goal )
{   
   while (!frontier.empty())
   {
      Graph::Location current = frontier.get();
      if ( current == goal )
      {
         break;
      }
      else
      {
         frontier.pop();         
      }
      
      Graph::Neighbours n = getNeighbours( current );
      for ( Graph::Neighbours::const_iterator it = n.begin();
            it!= n.end(); ++it )
      {
         Graph::Location next = *it;
         float new_cost = cost_so_far[current] + cost(current, next);
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

