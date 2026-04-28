#include "analyzer.h"
#include "ui_analyzer.h"
#include "database.h"
#include <algorithm>
#include <unordered_map>
#include <vector>
#include<QProcess>
#include<QLayout>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

Analyzer::Analyzer(int userId, Graph* graph, QWidget *parent)
    : QWidget(parent), ui(new Ui::Analyzer), currentUserId(userId), friendGraph(graph) {
    ui->setupUi(this);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
   // QVBoxLayout *layout = new QVBoxLayout(this);
}


// Simple PageRank
std::unordered_map<int, double> computePageRank(
    const std::unordered_map<int, std::vector<int>>& graph) {

    int n = graph.size();
    std::unordered_map<int, double> pr;

    // Initialize
    for (const auto& node : graph) pr[node.first] = 1.0 / n;

    // 20 iterations is enough
    for (int iter = 0; iter < 20; iter++) {
        std::unordered_map<int, double> newPr;
        for (const auto& node : graph) {
            double sum = 0;
            for (const auto& other : graph) {
                const auto& neighbors = other.second;
                if (std::find(neighbors.begin(), neighbors.end(), node.first) != neighbors.end()) {
                    sum += pr[other.first] / neighbors.size();
                }
            }
            newPr[node.first] = 0.15/n + 0.85 * sum;
        }
        pr = newPr;
    }
    return pr;
}

void Analyzer::on_connectionsButton_clicked() {
    showHighestConnections();
}

void Analyzer::on_influencersButton_clicked() {
    showTopInfluencers();
}

void Analyzer::showHighestConnections() {
    ui->resultsList->clear();

    // Get all users and sort by friend count
    QVector<User*> users = Database::instance().getAllUsers();
    std::vector<std::pair<QString, int>> list; // (username, friendCount)

    for (User* user : users) {
        int friendCount = friendGraph->getConnectionCount(user->getUserId());
        list.push_back({user->getUsername(), friendCount});
        delete user;
    }

    // Sort descending
    std::sort(list.begin(), list.end(),
              [](auto& a, auto& b) { return a.second > b.second; });

    // Show top 10
    for (int i = 0; i < std::min(10, (int)list.size()); i++) {
        ui->resultsList->addItem(QString("%1. @%2 - %3 friends")
                                     .arg(i+1).arg(list[i].first).arg(list[i].second));
    }
}

void Analyzer::showTopInfluencers() {
    ui->resultsList->clear();

    // Build graph for PageRank
    std::unordered_map<int, std::vector<int>> graph;
    QVector<User*> users = Database::instance().getAllUsers();

    for (User* user : users) {
        graph[user->getUserId()] = friendGraph->getNeighbors(user->getUserId());
        delete user;
    }

    // Compute PageRank
    auto ranks = computePageRank(graph);

    // Convert to list for sorting
    std::vector<std::pair<int, double>> rankList(ranks.begin(), ranks.end());
    std::sort(rankList.begin(), rankList.end(),
              [](auto& a, auto& b) { return a.second > b.second; });

    // Show top 10
    for (int i = 0; i < std::min(10, (int)rankList.size()); i++) {
        int userId = rankList[i].first;
        double score = rankList[i].second * 100; // as percentage

        User* user = Database::instance().getUserById(userId);
        if (user) {
            QString marker = (userId == currentUserId) ? " YOU " : "";
            ui->resultsList->addItem(QString("%1. %2@%3 - %4% influence")
                                         .arg(i+1)
                                         .arg(marker)
                                         .arg(user->getUsername())
                                         .arg(score, 0, 'f', 1));
            delete user;
        }
    }
}
Analyzer::~Analyzer()
{
    delete ui;
}

void Analyzer::on_graph_clicked()
{
    createAndUpdateInfluencerTable();
    QProcess *process = new QProcess(this);
    process->start("C:/Users/USER/py310env/Scripts/python.exe",
                   QStringList() << "C:/Users/USER/Documents/social_media_daa/graph_visualizer.py");
}
// Create/Update table in C++ (run this when your app starts or when connections change)
void Analyzer::createAndUpdateInfluencerTable() {

    QSqlQuery query;

    // Create table if not exists
    query.exec(R"(
        CREATE TABLE IF NOT EXISTS influencer_analysis (
            user_id INTEGER PRIMARY KEY,
            username TEXT,
            connection_count INTEGER,
            influencer_percent REAL
        )
    )");

    // Get all users
    QVector<User*> users = Database::instance().getAllUsers();
    int n=users.size();

    // Clear old data
    query.exec("DELETE FROM influencer_analysis");

    // Calculate and insert fresh data
    for (User* user : users) {
        int userId = user->getUserId();
        int connections = friendGraph->getConnectionCount(userId);
        double influencerPercent = calculateInfluencerPercent(userId,n); // Your PageRank or other metric

        query.prepare("INSERT INTO influencer_analysis (user_id, username, connection_count, influencer_percent) "
                      "VALUES (?, ?, ?, ?)");
        query.addBindValue(userId);
        query.addBindValue(user->getUsername());
        query.addBindValue(connections);
        query.addBindValue(influencerPercent);
        query.exec();

        delete user;
    }
}

// Simple influencer calculation (can be PageRank or just connection-based)
double Analyzer::calculateInfluencerPercent(int userId,int n) {
    // Option 1: Simple percentage of max connections
    int connections = friendGraph->getConnectionCount(userId);
    int maxConnections=n;
    if (maxConnections > 0) {
        return (double)connections / maxConnections * 100;
    }

    return 0.0;
}

