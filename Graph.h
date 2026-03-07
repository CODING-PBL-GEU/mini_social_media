#ifndef GRAPH_H
#define GRAPH_H
#include<map>
#include<vector>

class Graph{
private:
    std::map<int,std::vector<int>>adjacencyList;
public:
    void addUser(int userId);
    void addFriendship(int user1,int user2);
    std::vector<int>getFriends(int userId);
};

#endif // GRAPH_H
