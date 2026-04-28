#ifndef ANALYZER_H
#define ANALYZER_H

#include <QWidget>
#include "graph.h"
namespace Ui {
class Analyzer;
}

class Analyzer : public QWidget
{
    Q_OBJECT
private:
    Ui::Analyzer *ui;
    int currentUserId;
    Graph* friendGraph;

    void showHighestConnections();
    void showTopInfluencers();
    void createAndUpdateInfluencerTable();
    double calculateInfluencerPercent(int userId,int n);

public:
    explicit  Analyzer(int userId, Graph* graph, QWidget *parent = nullptr);
    ~Analyzer();
private slots:
    void on_connectionsButton_clicked();
    void on_influencersButton_clicked();


    void on_graph_clicked();
};

#endif // ANALYZER_H
