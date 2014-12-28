#include <iostream>
#include <vector> 
#include <limits>
#include <fstream>
#include <string>

using namespace std;

int main()
{
    string input_filename = "input.txt";
    ifstream input(input_filename);

    if (input.is_open())
    {
	    int m, n;
        input >> n;
        input >> m;

        int temp;
        vector<vector<int>> down, right, s;
        down.resize(n);
        for (int i=0; i<n; i++) {
            down[i].resize(m+1);
            for (int j=0; j<m+1; j++) {
                input >> temp;
                down[i][j] = temp;
            }
        }

        // read '-' separator
        {
            string sep;
            input >> sep;
        }

        right.resize(n+1);
        for (int i=0; i<n+1; i++) {
		    right[i].resize(m, 0);
		    for (int j=0; j<m; j++) {
			    input >> temp;
			    right[i][j] = temp;
		    }
	    }

        s.resize(n+1);
	    for (int i=0; i<n+1; i++)
		    s[i].resize(m+1, INT_MAX);

        s[0][0] = 0;
	    for (int i=1; i<n+1; i++) {
		    s[i][0] = s[i-1][0] + down[i-1][0];
	    }

	    for (int j=1; j<m+1; j++) {
		    s[0][j] = s[0][j-1] + right[0][j-1];
	    }

	    for (int i=1; i<n+1; i++) {
		    for (int j=1; j<m+1; j++) {
			    s[i][j] = max(s[i-1][j] + down[i-1][j], s[i][j-1] + right[i][j-1]);
		    }
	    }

        cout << "Result: " << s[n][m] << endl;
    } else
    {
        cout << "Couldn't find input file: \"" << input_filename << "\"" << endl;
    }
	return 0;
}