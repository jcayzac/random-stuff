#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <iterator>

// For any two strings of length n and m, respectively (disregarding any
// common prefix or suffix), the following algorithm:
// - Uses an internal buffer of 2+2*max(n,m) integers
// - Evaluates n*m costs.
unsigned int levenshtein_v1(const std::string& source, const std::string& target) {
    unsigned int source_length(source.size());
    unsigned int target_length(target.size());

    const char* source_c(&source[0]);
    const char* target_c(&target[0]);

    // Ignore common prefix
    while (source_length &&
           target_length &&
           *source_c==*target_c
    ) {
        ++source_c;
        ++target_c;
        --source_length;
        --target_length;
    }

    // Ignore common suffix
    while (source_length &&
           target_length &&
           source_c[source_length-1]==target_c[target_length-1]
    ) {
        --source_length;
        --target_length;
    }

    // make the inner loop the longest one
    if (source_length<target_length) {
        std::swap(source_length, target_length);
        std::swap(source_c, target_c);
    }

    // empty case
    if (!target_length) return source_length;

    // special case
    if (target_length==1) {
        const char* end(source_c+source_length);
        return (source_length-1) + (std::find(source_c,end,*target_c)==end);
    }

	// This version stores two rows of the matrix:
    std::vector<unsigned int> buffer(2*source_length+2);
    unsigned int* back(&buffer[0]);
    unsigned int* front(&buffer[source_length+1]);

    for (unsigned int i(0); i<=source_length; ++i) {
        back[i]=i;
    }

    for (unsigned int i(0); i<target_length; ++i) {
        *front=i+1;
        for (unsigned int j(0); j<source_length; ++j) {
            front[j+1]=(target_c[i]==source_c[j])?back[j]:1+std::min(
                front[j],
                std::min(back[j], back[j+1])
            );
        }
        std::swap(front, back);
    }
    return back[source_length];
}


// For any two strings of length n and m, respectively (disregarding any
// common prefix or suffix), the following algorithm:
// - Uses an internal buffer of 1+max(n,m) integers
// - Evaluates n*m-min²(n,m)/4 costs.
unsigned int levenshtein_v2(const std::string& source, const std::string& target) {
	unsigned int iter=0;
	unsigned int source_length(source.size());
	unsigned int target_length(target.size());
	const char* source_c(&source[0]);
	const char* target_c(&target[0]);
	
	// Ignore common prefix
	while (source_length &&
	       target_length &&
	       *source_c==*target_c
	) {
		--source_length;
		--target_length;
		++source_c;
		++target_c;
	}

	// Ignore common suffix
	while (source_length &&
	       target_length &&
	       source_c[source_length-1]==target_c[target_length-1]
	) {
		--source_length;
		--target_length;
	}
	
	// Make the inner loop the longest one
	if (source_length > target_length) {
		std::swap(source_length, target_length);
		std::swap(source_c, target_c);
	}

	// Empty case
	if (!source_length) return target_length;
	
	// Special case when remaining length of either string is 1
	if (source_length == 1) {
        const char* end(&target_c[target_length]);
        return target_length-(std::find(target_c,end,*source_c)!=end);
	}
	
	// Init
	const unsigned int half((source_length+1) >> 1);
	const unsigned int source_half(source_length-half);
	const unsigned int target_half(target_length-half);

	// Only one row of costs is needed
	std::vector<unsigned int> row(target_length+1);
	row[0] = source_half;
	for (unsigned int i(1); i<=target_half; ++i) {
		row[i] = i;
	}

	for (unsigned int i(0); i<source_length; ++i) {
		const unsigned int in_upper_triangle(i>=source_half);
		const unsigned int start_offset(std::max(i, source_half)-source_half);
		const unsigned int final_offset(std::min(i+target_half, target_length));

		unsigned int offset(start_offset+in_upper_triangle);
		unsigned int C(row[start_offset] + (source_c[i] != target_c[start_offset]));
		unsigned int last(in_upper_triangle?row[offset]:i);

		row[offset] = in_upper_triangle?std::min(1+row[offset],C):row[offset];
		++iter;
		unsigned int next(in_upper_triangle?row[offset]:i+1);
		
		// main
		C=last + (source_c[i] != target_c[offset]);
		while (++offset <= final_offset) {
			++iter;
			last = row[offset];
			next = std::min(C, 1+std::min(next, row[offset]));
			row[offset] = next;
			C = last + (source_c[i] != target_c[offset]);
		}
		// lower triangle sentinel
		row[offset] = (i<half)?std::min(C, 1+next):row[offset];
	}
	
	
/*	std::cerr << "     Sizes: " << source_length << ", " << target_length << "\n";
	std::cerr << "Iterations: " << (iter*100)/(source_length*target_length) << "%\n";
	iter = source_length*(2+row.back());
	std::cerr << "       New: " << (iter*100)/(source_length*target_length) << "%\n";
*/	return row.back();
}


unsigned int levenshtein_v3(const std::string& source, const std::string& target) {
	unsigned int iter=0;
	unsigned int source_length(source.size());
	unsigned int target_length(target.size());
	const char* source_c(&source[0]);
	const char* target_c(&target[0]);
	
	// Ignore common prefix
	while (source_length &&
	       target_length &&
	       *source_c==*target_c
	) {
		--source_length;
		--target_length;
		++source_c;
		++target_c;
	}

	// Ignore common suffix
	while (source_length &&
	       target_length &&
	       source_c[source_length-1]==target_c[target_length-1]
	) {
		--source_length;
		--target_length;
	}
	
	// Make the inner loop the longest one
	if (source_length > target_length) {
		std::swap(source_length, target_length);
		std::swap(source_c, target_c);
	}

	// Empty case
	if (!source_length) return target_length;
	
	std::vector<unsigned int> row(target_length);
	row[0]=(source_c[0]!=target_c[0]);
	
	unsigned maxiter=source_length*(3+target_length-source_length);
	std::cerr << "Iterations: " << maxiter << "/" << source_length*target_length << "\n";

	return 0;
}

// Test program
// Usage: program <source> <target>
int main(const int argc, const char* argv[]) {
    if (argc!=3) return 1;

    std::string s, t;

	std::ifstream sifs(argv[1]);
	if (sifs.good()) {
		s=std::string(std::istreambuf_iterator<char>(sifs), std::istreambuf_iterator<char>());
		sifs.close();
	}
	else s=argv[1];
	
	std::ifstream tifs(argv[2]);
	if (tifs.good()) {
		t=std::string(std::istreambuf_iterator<char>(tifs), std::istreambuf_iterator<char>());
		tifs.close();
	}
	else t=argv[2];

    unsigned int max=std::max(s.size(), t.size());
    unsigned int ld=levenshtein_v1(s,t);
    std::cout << "levenshtein_v1: Distance=" << ld << ", Similarity=" << ((max-ld)*100)/max << "%\n";
    ld=levenshtein_v2(s,t);
    std::cout << "levenshtein_v2: Distance=" << ld << ", Similarity=" << ((max-ld)*100)/max << "%\n";
    ld=levenshtein_v3(s,t);
    std::cout << "levenshtein_v3: Distance=" << ld << ", Similarity=" << ((max-ld)*100)/max << "%\n";
    return 0;
}

