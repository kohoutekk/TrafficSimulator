#ifndef PATHFINDING_HPP
#define PATHFINDING_HPP

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>

namespace pathfinding
{
template <typename Node, typename Edge, typename GetEdges, typename GetToNode>
std::vector<Edge> bfs(Node* start, Node *end, GetEdges &&getEdges, GetToNode &&getTo) {
    std::queue<Node *> q;
    std::unordered_map<Node*, std::pair<Node*, Edge>> prev;
    std::unordered_set<Node *> visited;

    q.push(start);
    visited.insert(start);

    while (!q.empty()) {
        Node* current = q.front(); q.pop();
        if (current == end) break;

        for (Edge edge : getEdges(current)) {
            Node* next = getTo(edge);
            if (visited.count(next)) continue;
            visited.insert(next);
            prev[next] = {current, edge};   // store parent
            q.push(next);
        }
    }

    // Reconstruct path
    std::vector<Edge> path;
    path.reserve(prev.size());
    Node* current = end;

    while (current != start) {
        auto it = prev.find(current);
        if (it == prev.end()) return {};
        auto [parent, edge] = it->second;
        path.push_back(edge);
        current = parent;
    }
    std::reverse(path.begin(), path.end());
    return path;
};
} // pathfinding

#endif // PATHFINDING_HPP
