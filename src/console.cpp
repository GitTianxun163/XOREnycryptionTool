#include "enycpt.h"
#include <fstream>
using namespace std;

int main(int argc,char* argv[]) {
	if (argc < 3 || argc > 4) {
		printf("Usage: %s <Plian file> <key> [Cipher file]",argv[0]);
	}
	else {
		if (argc == 3) {
			enycptFile(argv[1],argv[2],"tempfile");
			fstream srcf = fstream("tempfile",ios_base::binary|ios_base::in);
			fstream detf = fstream(argv[1],ios_base::binary|ios_base::out|ios_base::trunc);
			detf << srcf.rdbuf();
			remove("tempfile");
		}
		else {
			enycptFile(argv[1],argv[2],argv[3]);
		}
	}
	return 0;
}
