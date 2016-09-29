#include <iostream>

int main(int argc, char * argv[])
{

	std::cout << "Arguments are:" << std::endl;
	
	for(int i=0; i<argc; i++){
		std::cout << i << " " << argv[i] << std::endl;
	}

	for(int i=0; i<argc; i++){
		if(i==1) steps = atoi(argv[i]);

	}

	


	return 0;
}
