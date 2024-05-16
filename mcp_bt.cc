// Ilyas Umalatov X7278165E
#include <iostream>
#include <fstream>
#include <vector>
#include <climits>

using namespace std;

void mcp_bt(const vector<vector<int>> &matrix, int rows, int cols, int x, int y, int &best_v, int current_v)
{
    if (x == rows - 1 && y == cols - 1)
    {
        best_v = max(best_v, current_v);
    }
    else
    {
        for (int i = -1; i <= 1; i++)
        {
            for (int j = -1; j <= 1; j++)
            {
                if (x + i >= 0 && x + i < rows && y + j >= 0 && y + j < cols)
                {
                    mcp_bt(matrix, rows, cols, x + i, y + j, best_v, current_v + matrix[x + i][y + j]);
                }
            }
        }
    }
}

int mcp_bt(const vector<vector<int>> &matrix, int rows, int cols)
{
    int x = 0;
    int y = 0;
    int best_v = INT_MIN;
    int current_v = matrix[x][y];
    mcp_bt(matrix, rows, cols, x, y, best_v, current_v);
    return best_v;
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

    int best_v = mcp_bt(matrix, rows, cols);
    cout << best_v << endl;

    if (p2D)
    {
        // imprimir
    }
    file.close();
    return 0;
}