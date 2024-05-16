//Ilyas Umalatov X7278165E
#include <iostream>
#include <fstream>
#include <vector>
#include <climits>
#include <chrono>
#include <iomanip>

using namespace std;

int nvisit = 0;
int nexplored = 0;
int nleaf = 0;
int nunfeasible = 0;
int nnot_promising = 0;
vector<pair<int, int>> currentPath;
vector<pair<int, int>> minPath;
int currentCost = 0;
int minCostTotal = INT_MAX;

void mcp_btaux(const vector<vector<int>> &matrix, vector<vector<int>> &minCost, vector<vector<bool>> &visited, int rows, int cols, int x, int y, int current_v)
{
    nvisit++;

    if (current_v >= minCost[x][y])
    {
        nnot_promising++;
        return;
    }

    minCost[x][y] = current_v;

    currentPath.push_back({x, y});
    currentCost += matrix[x][y];

    if (x == rows - 1 && y == cols - 1)
    {
        nleaf++;

        if (currentCost < minCostTotal)
        {
            minPath = currentPath;
            minCostTotal = currentCost;
        }
    }
    else
    {
        visited[x][y] = true;

        vector<pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {1, 1}, {-1, 1}, {1, -1}};
        for (auto &dir : directions)
        {
            int newX = x + dir.first;
            int newY = y + dir.second;

            if (newX >= 0 && newX < rows && newY >= 0 && newY < cols && !visited[newX][newY])
            {
                nexplored++;
                mcp_btaux(matrix, minCost, visited, rows, cols, newX, newY, current_v + matrix[newX][newY]);
            }
            else
            {
                nunfeasible++;
            }
        }

        visited[x][y] = false;
    }

    currentPath.pop_back();
    currentCost -= matrix[x][y];
}

int mcp_bt(const vector<vector<int>> &matrix, int rows, int cols)
{
    vector<vector<bool>> visited(rows, vector<bool>(cols, false));
    vector<vector<int>> minCost(rows, vector<int>(cols, INT_MAX));
    int x = 0;
    int y = 0;
    int current_v = matrix[x][y];
    mcp_btaux(matrix, minCost, visited, rows, cols, x, y, current_v);
    return minCost[rows - 1][cols - 1];
}

int main(int argc, char *argv[])
{
    bool p2D = false;
    bool p = false;
    if (argc < 3)
    {
        cerr << "ERROR: missing filename.\nUsage:\nmcp [--p2D] [-p] -f file" << endl;
        return 1;
    }

    ifstream file;
    for (int i = 1; i < argc; ++i)
    {
        string arg = argv[i]; // Convert to std::string

        if (arg == "--p2D")
        {
            p2D = true;
        }
        else if (arg == "-p")
        {
            p = true;
        }
        else if (arg == "-f")
        {
            file.open(argv[i + 1]);
            i++;
        }
        else
        {
            cerr << "ERROR: unknown option " << arg << ".\nUsage:\nmcp [--p2D] [-p] -f file" << endl;
            return 1;
        }
    }

    if (!file)
    {
        cerr << "No se pudo abrir el archivo.\n";
        return 1;
    }

    int rows, cols;
    file >> rows >> cols;

    vector<vector<int>> matrix(rows, vector<int>(cols));
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            file >> matrix[i][j];
        }
    }

    auto start = chrono::high_resolution_clock::now();
    int best_v = mcp_bt(matrix, rows, cols);
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
    double duration_ms = duration.count() / 1000.0;
    cout << best_v << endl;
    cout << nvisit << " " << nexplored << " " << nleaf << " " << nunfeasible << " " << nnot_promising << endl;
    cout << fixed << setprecision(3) << duration_ms << endl;

    if (p2D)
    {
        vector<vector<char>> pathMatrix(rows, vector<char>(cols, '.'));
        int pathDifficulty = 0;
        for (const auto &p : minPath)
        {
            pathMatrix[p.first][p.second] = 'x';
            pathDifficulty += matrix[p.first][p.second];
        }

        for (const auto &row : pathMatrix)
        {
            for (const auto &cell : row)
            {
                cout << cell;
            }
            cout << endl;
        }

        cout << pathDifficulty << endl;
    }

    if (p)
    {
        if (minPath.size() == 1)
        {
            cout << "<>" << endl;
        }
        else
        {
            string path = "<";
            for (size_t i = 0; i < minPath.size() - 1; ++i)
            {
                int dx = minPath[i + 1].first - minPath[i].first;
                int dy = minPath[i + 1].second - minPath[i].second;
                if (dx == -1 && dy == 0)
                    path += '1';
                else if (dx == -1 && dy == 1)
                    path += '2';
                else if (dx == 0 && dy == 1)
                    path += '3';
                else if (dx == 1 && dy == 1)
                    path += '4';
                else if (dx == 1 && dy == 0)
                    path += '5';
                else if (dx == 1 && dy == -1)
                    path += '6';
                else if (dx == 0 && dy == -1)
                    path += '7';
                else if (dx == -1 && dy == -1)
                    path += '8';
            }
            path += '>';
            cout << path << endl;
        }
    }

    file.close();
    return 0;
}