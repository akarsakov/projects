#include "AminoAcids_processing.h"
#include "RNA_processing.h"
#include "string_processing.h"
#include <algorithm>
#include <fstream>
#include <sstream>

using namespace std;

static char int2AA[] = { 'K', 'N', 'K', 'N', 'T', 'T', 'T', 'T', 'R', 'S', 'R', 'S', 'I', 'I', 'M', 'I',
                         'Q', 'H', 'Q', 'H', 'P', 'P', 'P', 'P', 'R', 'R', 'R', 'R', 'L', 'L', 'L', 'L',
                         'E', 'D', 'E', 'D', 'A', 'A', 'A', 'A', 'G', 'G', 'G', 'G', 'V', 'V', 'V', 'V',
                         '*', 'Y', '*', 'Y', 'S', 'S', 'S', 'S', '*', 'C', 'W', 'C', 'L', 'F', 'L', 'F' };

static int moleculeMasses[] = { 71, 0, 103, 115, 129, 147, 57, 137, 113, 0, 128, 113, 131, 114, 0, 97, 128,
                                156, 87, 101, 0, 99, 186, 0, 163, 0 };

static char AminoAcids[] = { 'A', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'K', 'L', 'M', 'N', 'P', 'Q', 'R', 'S', 'T', 'V', 'W', 'Y' };

char bio::codon2AA(string codon)
{
    if (codon.size() != 3 || !isRNA(codon))
        throw exception("invalid codon");

    int index = rnucl2int(codon[0]) * 16 + rnucl2int(codon[1]) * 4 + rnucl2int(codon[2]);
    return int2AA[index];
}

string bio::RNA2AA(string RNA)
{
    string AA_string = "";
    for (size_t i=0; i<RNA.size() - 2; i+=3)
    {
        string codon = RNA.substr(i, 3);
        char AA = codon2AA(codon);

        if (AA != '*')
            AA_string += AA;
        else
            break;
    }
    return AA_string;
}

int bio::getMoleculeMass(char molecule)
{
    int index = molecule - 'A';
    if (index < 0 || index > 25)
        throw exception("invalid molecule");

    int mass = moleculeMasses[index];
    if (mass <= 0)
        throw exception("invalid molecule");

    return mass;
}

int bio::getPeptideMass(string peptide)
{
    int mass = 0;
    for (size_t i=0; i<peptide.size(); i++)
        mass += bio::getMoleculeMass(peptide[i]);

    return mass;
}


namespace bio
{

map<pair<char, char>, int> readScoringMatrix(const string matrix_filename) {
    ifstream matrix(matrix_filename);
    
    if (matrix.is_open()) {
        map<pair<char, char>, int> scoringMatrix;
        vector<char> aminoacids;

        string line;
        getline(matrix, line);

        stringstream ss(line);
        while (!ss.eof()) {
            string aa;
            ss >> aa;
            aminoacids.push_back(aa[0]);
        }

        for (size_t i=0; i<aminoacids.size(); i++) {
            string curAA;
            matrix >> curAA;

            for (size_t j=0; j<aminoacids.size(); j++) {
                int weight;
                matrix >> weight;
                scoringMatrix[make_pair(curAA[0], aminoacids[j])] = weight;
            }
        }
        return scoringMatrix;
    } else {
        throw exception(("Couldn't open file: " + matrix_filename).c_str());
    }
}

map<pair<char, char>, int> generateScoringMatrix(const int sigma) {
    int numAminoAcids = sizeof(AminoAcids) / sizeof(char);
    map<pair<char, char>, int> scoringMatrix;

    for (int i=0; i<numAminoAcids; i++) {
        for (int j=0; j<numAminoAcids; j++) {
            if (i == j)
                scoringMatrix[make_pair(AminoAcids[i], AminoAcids[i])] = sigma;
            else
                scoringMatrix[make_pair(AminoAcids[i], AminoAcids[j])] = -sigma;
        }
    }

    return scoringMatrix;
}


namespace week2
{

vector<int> getMassesVector()
{
    vector<int> masses(moleculeMasses, moleculeMasses + sizeof(moleculeMasses) / sizeof(int));
    auto end = remove_if(masses.begin(), masses.end(), 
                    [](const int& el) -> bool 
                    {
                        return el == 0;
                    });
    masses.erase(end, masses.end());
    sort(masses.begin(), masses.end());
    masses.erase( unique( masses.begin(), masses.end() ), masses.end() );
    return masses;
}

} /* week2 */

namespace week5
{

static void getAlignment_v(const vector<vector<DIRECTIONS>>& backtrack, const string& v, int i, int j, string& aligned_v, bool isFitting = false)
{
    if (isFitting) {
	    if ((i==0) || (j==0)) {
		    return;
	    }
    } else {
	    if ((i==0) && (j==0)) {
		    return;
	    }
    }

	if (backtrack[i][j] == DIRECTIONS::DOWN) {
		getAlignment_v(backtrack, v, i-1, j, aligned_v, isFitting);
		aligned_v += '-';
	} else if (backtrack[i][j] == DIRECTIONS::RIGHT) {
		getAlignment_v(backtrack, v, i, j-1, aligned_v, isFitting);
        aligned_v += v[j-1];
    } else if (backtrack[i][j] == DIRECTIONS::DIAGONAL) {
		getAlignment_v(backtrack, v, i-1, j-1, aligned_v, isFitting);
		aligned_v += v[j-1];
	}
}

static void getAlignment_w(const vector<vector<DIRECTIONS>>& backtrack, const string& w, int i, int j, string& aligned_w, bool isFitting = false)
{
    if (isFitting) {
	    if ((i==0) || (j==0)) {
		    return;
	    }
    } else {
	    if ((i==0) && (j==0)) {
		    return;
	    }
    }

	if (backtrack[i][j] == DIRECTIONS::DOWN) {
        getAlignment_w(backtrack, w, i-1, j, aligned_w, isFitting);
		aligned_w += w[i-1];
	} else if (backtrack[i][j] == DIRECTIONS::RIGHT) {
		getAlignment_w(backtrack, w, i, j-1, aligned_w, isFitting);
		aligned_w += '-';
	} else if (backtrack[i][j] == DIRECTIONS::DIAGONAL) {
		getAlignment_w(backtrack, w, i-1, j-1, aligned_w, isFitting);
		aligned_w += w[i-1];
	}
}

int globalAlignment(string& v, string& w, const map<pair<char, char>, int>& blosum, const int sigma) {
    size_t m = v.size();
	size_t n = w.size();

    vector<vector<int>> s;
	vector<vector<DIRECTIONS>> backtracks;

    s.resize(n+1);
	for (size_t i=0; i<n+1; i++)
		s[i].resize(m+1, INT_MAX);

	backtracks.resize(n+1);
	for (size_t i=0; i<n+1; i++)
		backtracks[i].resize(m+1);
	
	s[0][0] = 0;
	for (size_t i=1; i<n+1; i++) {
		s[i][0] = s[i-1][0] - sigma;
		backtracks[i][0] = DIRECTIONS::DOWN;
	}

	for (size_t j=1; j<m+1; j++) {
		s[0][j] = s[0][j-1] - sigma;
		backtracks[0][j] = DIRECTIONS::RIGHT;
	}

	for (size_t i=1; i<n+1; i++) {
		for (size_t j=1; j<m+1; j++) {
            int diag = s[i-1][j-1] + blosum.at(make_pair(v[j-1], w[i-1]));
			
			int maximum = max(diag, max(s[i-1][j] - sigma, s[i][j-1] - sigma));
			if (maximum == s[i][j-1] - sigma)
				backtracks[i][j] = DIRECTIONS::RIGHT;
			else if (maximum == s[i-1][j] - sigma)
				backtracks[i][j] = DIRECTIONS::DOWN;
			else
                backtracks[i][j] = DIRECTIONS::DIAGONAL;

			s[i][j] = maximum;
		}
	}

    string oldV = v, oldW = w;
    v.clear();
    w.clear();
    getAlignment_v(backtracks, oldV, n, m, v);
    getAlignment_w(backtracks, oldW, n, m, w);
    return s[n][m];
}

int localAlignment(string& v, string& w, const map<pair<char, char>, int>& pam250, const int sigma) {
    vector<vector<int>> s;
    vector<vector<DIRECTIONS>> backtracks;

    size_t m = v.size();
	size_t n = w.size();

    s.resize(n+1);
	for (size_t i=0; i<n+1; i++)
		s[i].resize(m+1, INT_MAX);

	backtracks.resize(n+1);
	for (size_t i=0; i<n+1; i++)
		backtracks[i].resize(m+1);
	
	s[0][0] = 0;
	for (size_t i=1; i<n+1; i++) {
		s[i][0] = s[i-1][0] - sigma;
		backtracks[i][0] = DIRECTIONS::DOWN;
	}

	for (size_t j=1; j<m+1; j++) {
		s[0][j] = s[0][j-1] - sigma;
		backtracks[0][j] = DIRECTIONS::RIGHT;
	}

	for (size_t i=1; i<n+1; i++) {
		for (size_t j=1; j<m+1; j++) {
			int diag = s[i-1][j-1] + pam250.at(make_pair(v[j-1], w[i-1]));
			
			int maximum = max(max(diag, 0), max(s[i-1][j] - sigma, s[i][j-1] - sigma));
			if (maximum == s[i][j-1] - sigma)
				backtracks[i][j] = DIRECTIONS::RIGHT;
			else if (maximum == s[i-1][j] - sigma)
				backtracks[i][j] = DIRECTIONS::DOWN;
			else if (maximum == diag)
				backtracks[i][j] = DIRECTIONS::DIAGONAL;
			else
				backtracks[i][j] = DIRECTIONS::TAXI;

			s[i][j] = maximum;
		}
	}
	int max_s = INT_MIN;
	int max_i, max_j;
	for (size_t i=1; i<n+1; i++) {
		for (size_t j=1; j<m+1; j++) {
			if (s[i][j] > max_s)
			{
				max_s = s[i][j];
				max_i = i;
				max_j = j;
			}
		}
    }

    string oldV = v, oldW = w;
    v.clear();
    w.clear();
    getAlignment_v(backtracks, oldV, max_i, max_j, v);
    getAlignment_w(backtracks, oldW, max_i, max_j, w);
    return s[n][m];
}

int fittingAlignment(string& v, string& w, const int sigma) {
    vector<vector<int>> s;
    vector<vector<DIRECTIONS>> backtracks;

    size_t m = v.size();
	size_t n = w.size();

    s.resize(n+1);
	for (size_t i=0; i<n+1; i++)
		s[i].resize(m+1, INT_MAX);

	backtracks.resize(n+1);
	for (size_t i=0; i<n+1; i++)
		backtracks[i].resize(m+1);
	
	s[0][0] = 0;
	for (size_t i=1; i<n+1; i++) {
		s[i][0] = 0;
		backtracks[i][0] = DIRECTIONS::DOWN;
	}

	for (size_t j=1; j<m+1; j++) {
		s[0][j] = s[0][j-1] - sigma;
		backtracks[0][j] = DIRECTIONS::RIGHT;
	}

	for (size_t i=1; i<n+1; i++) {
		for (size_t j=1; j<m+1; j++) {
			int diag = v[j-1] == w[i-1] ? s[i-1][j-1] + sigma : s[i-1][j-1] - sigma;
			
			int maximum = max(diag, max(s[i-1][j] - sigma, s[i][j-1] - sigma));
			if (maximum == s[i-1][j] - sigma)
				backtracks[i][j] = DIRECTIONS::DOWN;
			else if (maximum == s[i][j-1] - sigma)
				backtracks[i][j] = DIRECTIONS::RIGHT;
			else if (maximum == diag)
				backtracks[i][j] = DIRECTIONS::DIAGONAL;

			s[i][j] = maximum;
		}
	}
	int max_s = INT_MIN;
	int max_i;
	for (size_t i=0; i<n+1; i++) {
		if (s[i][m] > max_s)
		{
			max_s = s[i][m];
			max_i = i;
		}
	}

    string oldV = v, oldW = w;
    v.clear();
    w.clear();
    getAlignment_v(backtracks, oldV, max_i, m, v, true);
    getAlignment_w(backtracks, oldW, max_i, m, w, true);
    return max_s;
}

int overlapAlignment(string& v, string& w, const int sigma) {
    vector<vector<int>> s;
    vector<vector<DIRECTIONS>> backtracks;

    size_t m = v.size();
	size_t n = w.size();

    s.resize(n+1);
	for (size_t i=0; i<n+1; i++)
		s[i].resize(m+1, INT_MAX);

	backtracks.resize(n+1);
	for (size_t i=0; i<n+1; i++)
		backtracks[i].resize(m+1);
	
	s[0][0] = 0;
	for (size_t i=1; i<n+1; i++) {
		s[i][0] = 0;
		backtracks[i][0] = DIRECTIONS::DOWN;
	}

	for (size_t j=1; j<m+1; j++) {
		s[0][j] = s[0][j-1] - sigma;
		backtracks[0][j] = DIRECTIONS::RIGHT;
	}

	for (size_t i=1; i<n+1; i++) {
		for (size_t j=1; j<m+1; j++) {
			int diag = v[j-1] == w[i-1] ? s[i-1][j-1] + 1 : s[i-1][j-1] - sigma;
			
			int maximum = max(diag, max(s[i-1][j] - sigma, s[i][j-1] - sigma));
			if (maximum == s[i-1][j] - sigma)
				backtracks[i][j] = DIRECTIONS::DOWN;
			else if (maximum == s[i][j-1] - sigma)
				backtracks[i][j] = DIRECTIONS::RIGHT;
			else if (maximum == diag)
				backtracks[i][j] = DIRECTIONS::DIAGONAL;

			s[i][j] = maximum;
		}
	}

	int max_s = INT_MIN;
	int max_j;
	for (size_t j=0; j<m+1; j++) {
		if (s[n][j] > max_s)
		{
			max_s = s[n][j];
			max_j = j;
		}
	}

    string oldV = v, oldW = w;
    v.clear();
    w.clear();
    getAlignment_v(backtracks, oldV, n, max_j, v, true);
    getAlignment_w(backtracks, oldW, n, max_j, w, true);
    return max_s;
}

static enum ALIGNMENT_LEVEL { MIDDLE = 0, UPPER, LOWER };

static void getAffineGapAlignment_v(const vector<vector<DIRECTIONS>>& backtrack, const vector<vector<DIRECTIONS>>& l_backtrack, const vector<vector<DIRECTIONS>>& u_backtrack, 
                                    const string& v, int i, int j, ALIGNMENT_LEVEL level, string& aligned_v) {
	if (i==0 && j==0)
		return;

	if (level == MIDDLE) {
		if (backtrack[i][j] == DIRECTIONS::DOWN) {
			getAffineGapAlignment_v(backtrack, l_backtrack, u_backtrack, v, i-1, j, LOWER, aligned_v);
            aligned_v += '-';
		} else if (backtrack[i][j] == DIRECTIONS::RIGHT) {
			getAffineGapAlignment_v(backtrack, l_backtrack, u_backtrack, v, i, j-1, UPPER, aligned_v);
            aligned_v += v[j-1];
		} else {
			getAffineGapAlignment_v(backtrack, l_backtrack, u_backtrack, v, i-1, j-1, MIDDLE, aligned_v);
            aligned_v += v[j-1];
		}
	} else if (level == UPPER) {
        if (u_backtrack[i][j] == DIRECTIONS::EXTENSION) {
			getAffineGapAlignment_v(backtrack, l_backtrack, u_backtrack, v, i, j-1, UPPER, aligned_v);
            aligned_v += v[j-1];
		} else {
			getAffineGapAlignment_v(backtrack, l_backtrack, u_backtrack, v, i, j-1, MIDDLE, aligned_v);
            aligned_v += v[j-1];
		}
	} else {
		if (l_backtrack[i][j] == DIRECTIONS::EXTENSION) {
			getAffineGapAlignment_v(backtrack, l_backtrack, u_backtrack, v, i-1, j, LOWER, aligned_v);
            aligned_v += '-';
		} else {
			getAffineGapAlignment_v(backtrack, l_backtrack, u_backtrack, v, i-1, j, MIDDLE, aligned_v);
            aligned_v += '-';
		}
	}
}

void getAffineGapAlignment_w(const vector<vector<DIRECTIONS>>& backtrack, const vector<vector<DIRECTIONS>>& l_backtrack, const vector<vector<DIRECTIONS>>& u_backtrack, 
                             const string& w, int i, int j, ALIGNMENT_LEVEL level, string& aligned_w) {
	if (i==0 && j==0)
		return;

	if (level == MIDDLE) {
		if (backtrack[i][j] == DIRECTIONS::DOWN) {
			getAffineGapAlignment_w(backtrack, l_backtrack, u_backtrack, w, i-1, j, LOWER, aligned_w);
            aligned_w += w[i-1];
		} else if (backtrack[i][j] == DIRECTIONS::RIGHT) {
			getAffineGapAlignment_w(backtrack, l_backtrack, u_backtrack, w, i, j-1, UPPER, aligned_w);
            aligned_w += '-';
		} else {
			getAffineGapAlignment_w(backtrack, l_backtrack, u_backtrack, w, i-1, j-1, MIDDLE, aligned_w);
            aligned_w += w[i-1];
		}
    } else if (level == UPPER) {
		if (u_backtrack[i][j] == DIRECTIONS::EXTENSION) {
			getAffineGapAlignment_w(backtrack, l_backtrack, u_backtrack, w, i, j-1, UPPER, aligned_w);
            aligned_w += '-';
		} else {
			getAffineGapAlignment_w(backtrack, l_backtrack, u_backtrack, w, i, j-1, MIDDLE, aligned_w);
            aligned_w += '-';
		}
	} else {
		if (l_backtrack[i][j] == DIRECTIONS::EXTENSION) {
			getAffineGapAlignment_w(backtrack, l_backtrack, u_backtrack, w, i-1, j, LOWER, aligned_w);
            aligned_w += w[i-1];
		} else {
			getAffineGapAlignment_w(backtrack, l_backtrack, u_backtrack, w, i-1, j, MIDDLE, aligned_w);
            aligned_w += w[i-1];
		}
	}
}

int alignmentWithAffineGapPenalties(string& v, string& w, const map<pair<char, char>, int>& blosum_map, 
                                    const int gap_open, const int gap_extension) {
	size_t m = v.size();
	size_t n = w.size();

    vector<vector<int>> middle, lower, upper;
	vector<vector<DIRECTIONS>> m_back, l_back, u_back;

	middle.resize(n+1);
	upper.resize(n+1);
	lower.resize(n+1);
	for (size_t i=0; i<n+1; i++) {
		middle[i].resize(m+1, INT_MIN);
		upper[i].resize(m+1, INT_MIN);
		lower[i].resize(m+1, INT_MIN);
	}

	m_back.resize(n+1);
	l_back.resize(n+1);
	u_back.resize(n+1);
	for (size_t i=0; i<n+1; i++) {
		m_back[i].resize(m+1);
		l_back[i].resize(m+1);
		u_back[i].resize(m+1);
	}

	middle[0][0] = 0;
	upper[0][0] = -gap_open;
	lower[0][0] = -gap_open;
	for (size_t i=1; i<n+1; i++) {
		upper[i][0] = upper[i-1][0] - gap_extension;
		middle[i][0] = upper[i][0];
		m_back[i][0] = DIRECTIONS::DOWN;
		u_back[i][0] = DIRECTIONS::OPEN;
	}

	for (size_t j=1; j<m+1; j++) {
		lower[0][j] = lower[0][j-1] - gap_extension;
		middle[0][j] = lower[0][j];
		m_back[0][j] = DIRECTIONS::RIGHT;
		l_back[0][j] = DIRECTIONS::OPEN;
	}

	for (size_t i=1; i<n+1; i++) {
		for (size_t j=1; j<m+1; j++) {
			int maximum = max(lower[i-1][j] - gap_extension, middle[i-1][j] - gap_open);
			if (maximum == lower[i-1][j] - gap_extension)
                l_back[i][j] = DIRECTIONS::EXTENSION;
			else
				l_back[i][j] = DIRECTIONS::OPEN;
			lower[i][j] = maximum;

			maximum = max(upper[i][j-1] - gap_extension, middle[i][j-1] - gap_open);
			if (maximum == upper[i][j-1] - gap_extension)
				u_back[i][j] = DIRECTIONS::EXTENSION;
			else
				u_back[i][j] = DIRECTIONS::OPEN;
			upper[i][j] = maximum;

			int diag = middle[i-1][j-1] + blosum_map.at(make_pair(v[j-1], w[i-1]));
			maximum = max(diag, max(lower[i][j], upper[i][j]));
			if (maximum == upper[i][j]) {
				m_back[i][j] = DIRECTIONS::RIGHT;
			}
			else if (maximum == lower[i][j]) {
				m_back[i][j] = DIRECTIONS::DOWN;
			}
			else
                m_back[i][j] = DIRECTIONS::DIAGONAL;

			middle[i][j] = maximum;
		}
	}

    string oldV = v, oldW = w;
    v.clear();
    w.clear();
    getAffineGapAlignment_v(m_back, l_back, u_back, oldV, n, m, MIDDLE, v);
    getAffineGapAlignment_w(m_back, l_back, u_back, oldW, n, m, MIDDLE, w);
    return middle[n][m];
}

void alignmentMiddleEdge(const string& v, const string& w, const int sigma, const map<pair<char, char>, int>& blosum_map,
                         pair<int, int>& start, pair<int, int>& end) {
    size_t m = v.size();
	size_t n = w.size();

    vector<vector<int>> s(m+1);
	vector<vector<DIRECTIONS>> backtracks(m+1);

	for (size_t i=0; i<m+1; i++)
		s[i].resize(n+1, INT_MAX);

	for (size_t i=0; i<m+1; i++)
		backtracks[i].resize(n+1);
	
	size_t middle = n/2;

	// from source
	s[0][0] = 0;
	for (size_t i=1; i<m+1; i++) {
		s[i][0] = s[i-1][0] - sigma;
		backtracks[i][0] = DIRECTIONS::DOWN;
	}

	for (size_t j=1; j<n+1; j++) {
		s[0][j] = s[0][j-1] - sigma;
		backtracks[0][j] = DIRECTIONS::RIGHT;
	}
	
	for (size_t j=1; j<=middle; j++) {
		for (size_t i=1; i<m+1; i++) {
			int diag = s[i-1][j-1] + blosum_map.at(make_pair(v[i-1], w[j-1]));
			
			int maximum = max(diag, max(s[i-1][j] - sigma, s[i][j-1] - sigma));
			if (maximum == s[i][j-1] - sigma)
				backtracks[i][j] = DIRECTIONS::RIGHT;
			else if (maximum == s[i-1][j] - sigma)
				backtracks[i][j] = DIRECTIONS::DOWN;
			else
				backtracks[i][j] = DIRECTIONS::DIAGONAL;

			s[i][j] = maximum;
		}
	}

	vector<int> mid(m+1);
	for (size_t i=0; i<m+1; i++)
		mid[i] = s[i][middle];

	// to sink
	s[m][n] = 0;
	for (int i=m-1; i>=0; i--) {
		s[i][n] = s[i+1][n] - sigma;
		backtracks[i+1][n] = DIRECTIONS::DOWN;
	}

	for (int j=n-1; j>=0; j--) {
		s[m][j] = s[m][j+1] - sigma;
		backtracks[m][j] = DIRECTIONS::RIGHT;
	}

	for (size_t j=n-1; j>=middle; j--) {
		for (int i=m-1; i>=0; i--) {
			int diag = s[i+1][j+1] + blosum_map.at(make_pair(v[i], w[j]));
			
			int maximum = max(diag, max(s[i+1][j] - sigma, s[i][j+1] - sigma));
			if (maximum == s[i][j+1] - sigma)
				backtracks[i][j] = DIRECTIONS::RIGHT;
			else if (maximum == s[i+1][j] - sigma)
				backtracks[i][j] = DIRECTIONS::DOWN;
			else
                backtracks[i][j] = DIRECTIONS::DIAGONAL;
			s[i][j] = maximum;
		}
	}

	int max_s = INT_MIN;
	int max_i = 0;
	for (size_t i=0; i<m+1; i++) 
	{
		if (mid[i] + s[i][middle] > max_s)
		{
			max_s = mid[i] + s[i][middle];
			max_i = i;
		}
	}

    start = make_pair(max_i, middle);
    end = backtracks[max_i][middle] == DIRECTIONS::RIGHT ? make_pair(max_i, middle+1) :
          backtracks[max_i][middle] == DIRECTIONS::DOWN ? make_pair(max_i+1, middle) :
                                                          make_pair(max_i+1, middle+1);
}

int linearSpaceAlignment(string& v, string& w, const map<pair<char, char>, int>& blosum_map, const int sigma) {



}

} /* week5 */

} /* bio */