#include "BigInt.h"

int main()
{
	auto start = std::chrono::high_resolution_clock::now();

	BigInt a("3832275581675228818048208302955800321621160585142877401584938618809809258305453638823390051719977379069769509412952627026909820889257761090455634561350780834178636905476855567306554584790972545747987403873470379509105247809997994687948677378186205774203758545724735385552");
	BigInt b("1264454021966901262335128019840324326065584710107148622148167077443003056639670680572126701");

	std::cout << "a = " << a << std::endl;

	std::cout << "b = " << b << std::endl;

	std::cout << "a + b = " << a + b << std::endl;
	std::cout << "a - b = " << a - b << std::endl;
	std::cout << "a * b = " << a * b << std::endl;
	std::cout << "a / b = " << a / b << std::endl;
	std::cout << "a % b = " << a % b << std::endl;
	// std::cout << a % b << std::endl;
	std::cout << "--------------" << std::endl;

	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

	std::cout << "Time taken by function: " << duration.count() << " microseconds" << std::endl;

	return 0;
}