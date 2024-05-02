#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <sstream>
using namespace std;
struct Point {
    double x, y;
};

class DBSCAN {
private:
    vector<Point> points;
    vector<vector<int>> clusters;
    double eps;
    int minPts;

public:
    DBSCAN(vector<Point> points, double eps, int minPts) : points(points), eps(eps), minPts(minPts) {}

    double distance(const Point& a, const Point& b) {
        return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
    }

    vector<int> rangeQuery(int pointIndex) {
        vector<int> neighbors;
        for (int i = 0; i < points.size(); ++i) {
            if (distance(points[pointIndex], points[i]) <= eps) {
                neighbors.push_back(i);
            }
        }
        return neighbors;
    }

    void dbscan() {
        vector<bool> visited(points.size(), false);
        vector<bool> clustered(points.size(), false);

        for (int i = 0; i < points.size(); ++i) {
            if (!visited[i]) {
                visited[i] = true;
                vector<int> neighbors = rangeQuery(i);

                if (neighbors.size() < minPts) {
                    continue;
                }

                vector<int> cluster;
                cluster.push_back(i);
                clustered[i] = true;

                for (int j = 0; j < neighbors.size(); ++j) {
                    int neighborIdx = neighbors[j];
                    if (!visited[neighborIdx]) {
                        visited[neighborIdx] = true;
                        vector<int> neighborNeighbors = rangeQuery(neighborIdx);
                        if (neighborNeighbors.size() >= minPts) {
                            neighbors.insert(neighbors.end(), neighborNeighbors.begin(), neighborNeighbors.end());
                        }
                    }
                    if (!clustered[neighborIdx]) {
                        cluster.push_back(neighborIdx);
                        clustered[neighborIdx] = true;
                    }
                }

                clusters.push_back(cluster);
            }
        }
    }

    void printClusters() {
        for (int i = 0; i < clusters.size(); ++i) {
            cout << endl;
            cout << "Cluster " << i << ": " << "{";
            for (int j = 0; j < clusters[i].size(); ++j) {
                cout << clusters[i][j] << " , ";
            }
            cout << "}" << endl;
        }
    }
};
//Read data from file
vector<Point> readCSV(const std::string& filename) {
    vector<Point> points;
    ifstream file(filename);
    string line;

    //Read line by line in file
    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        Point point;

        // Read comma-separated values from the line
        getline(ss, token, ',');
        point.x = stod(token);
        getline(ss, token, ',');
        point.y = stod(token);

        // Add the point to the vector
        points.push_back(point);
    }

    return points;
}



//This is main function
int main() {
    vector<Point> points = readCSV("test.csv");

    double eps = 1.3;
    int minPts = 5;

    DBSCAN dbscan(points, eps, minPts);
    dbscan.dbscan();
    dbscan.printClusters();

    return 0;
}