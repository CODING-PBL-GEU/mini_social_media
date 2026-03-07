#include "Graph.h"

void Graph::addUser(int userId)
{
    adjacencyList[userId]=std::vector<int>();
}

void Graph::addFriendship(int user1, int user2)
{
    adjacencyList[user1].push_back(user2);
    adjacencyList[user2].push_back(user1);
}
