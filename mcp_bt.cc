// Ilyas Umalatov X7278165E
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

void mcp_bt(const vector<vector<int>> &matrix, vector<vector<int>> &minCost, vector<vector<bool>> &visited, int rows, int cols, int x, int y, int current_v)
{
    nvisit++;

    if (current_v >= minCost[x][y])
    {
        nnot_promising++;
        return;
    }

    minCost[x][y] = current_v;

    if (x == rows - 1 && y == cols - 1)
    {
        nleaf++;
        return;
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
                mcp_bt(matrix, minCost, visited, rows, cols, newX, newY, current_v + matrix[newX][newY]);
            }
            else
            {
                nunfeasible++;
            }
        }

        visited[x][y] = false;
    }
}

int mcp_bt(const vector<vector<int>> &matrix, int rows, int cols)
{
    vector<vector<bool>> visited(rows, vector<bool>(cols, false));
    vector<vector<int>> minCost(rows, vector<int>(cols, INT_MAX));
    int x = 0;
    int y = 0;
    int current_v = matrix[x][y];
    mcp_bt(matrix, minCost, visited, rows, cols, x, y, current_v);
    return minCost[rows - 1][cols - 1];
}

int main(int argc, char *argv[])
{
    bool p2D = false;
    if (argc < 3)
    {
        cerr << "ERROR: missing filename.\nUsage:\nmcp [--p2D] -f file" << endl;
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
        else if (arg == "-f")
        {
            file.open(argv[i + 1]);
            i++;
        }
        else
        {
            cerr << "ERROR: unknown option " << arg << ".\nUsage:\nmcp [--p2D] -f file" << endl;
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
        // imprimir
    }
    file.close();
    return 0;
}