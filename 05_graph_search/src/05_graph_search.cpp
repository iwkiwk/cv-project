#include <iostream>
#include <vector>
#include <queue>

using namespace std;

const int ROW = 9;
const int COL = 10;

int maze[ROW][COL] =
{
	{ 1, 0, 1, 1, 1, 1, 0, 1, 1, 1 },
	{ 1, 0, 1, 0, 1, 1, 1, 0, 1, 1 },
	{ 1, 1, 1, 0, 1, 1, 0, 1, 0, 1 },
	{ 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 },
	{ 1, 1, 1, 0, 1, 1, 1, 0, 1, 0 },
	{ 1, 0, 1, 1, 1, 1, 0, 1, 0, 0 },
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 1 },
	{ 1, 0, 1, 1, 1, 1, 0, 1, 1, 1 },
	{ 1, 1, 0, 0, 0, 0, 1, 0, 0, 1 }
};

typedef struct _Point {
	int row;
	int col;
	int dis;
	vector<int> path;
} Point;

bool isPositionValid(int row, int col)
{
	return (row > -1) && (row < ROW) && (col > -1) && (col < COL);
}

int BFS(const Point & src, const Point & dest, int mat[ROW][COL])
{
	if (!isPositionValid(src.row, src.col) || !isPositionValid(dest.row, dest.col))
		return -1;
	if (mat[src.row][src.col] == 0 || mat[dest.row][dest.col] == 0)
		return -1;

	queue<Point> searchQueue;
	searchQueue.push(src);

	int dcol[] = { 1,0,-1,0 };
	int drow[] = { 0,1,0,-1 };

	bool visited[ROW][COL];
	memset(visited, 0, sizeof(visited));

	while (!searchQueue.empty())
	{
		Point current = searchQueue.front();
		searchQueue.pop();

		if (current.row == dest.row && current.col == dest.col)
		{
			cout << "Path:\n";
			for (auto & p : current.path)
				cout << " -> (" << p / COL << ", " << p % COL << ")";
			cout << endl;
			return current.dis;
		}

		for (int i = 0; i < 4; i++)
		{
			int newRow = current.row + drow[i];
			int newCol = current.col + dcol[i];

			if (!isPositionValid(newRow, newCol))
				continue;

			if (mat[newRow][newCol] == 1 && visited[newRow][newCol] == 0)
			{
				Point newPoint({ newRow, newCol, current.dis + 1, current.path });
				newPoint.path.push_back(newRow * COL + newCol);

				searchQueue.push(newPoint);
				visited[newRow][newCol] = true;
			}
		}

		// A trick to release vector memory
		vector<int>().swap(current.path);
	}

	return -1;
}

int main()
{
	Point source = { 8, 1, 0, {} };
	Point dest   = { 1, 6, 0, {} };

	cout << "steps: " << BFS(source, dest, maze);

	return 0;
}