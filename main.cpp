#include <algorithm>
#include <iostream>
#include <vector>
#include <random>

int randSampling(int num);

int main() {
    //this a guessing game
    std:: cout << "Hello This is a Guessing game\n Select a number between 1 and 3000 and I will guess it" <<std::endl;
    int num;
    std::cin >> num;
    while (num>3000 || num<0) {
        std::cout << "Please enter a number between 1 and 3000" <<std::endl;
        std::cin >> num;
    }
    randSampling(num);
    return 0;
}
int randSampling(int num) {
   std::vector<int> numbers;
    int len=3000;
    for (int i = 0; i < len; i++) {
        numbers.push_back(i);
    }
   auto rng = std::default_random_engine{};
    std::shuffle(numbers.begin(), numbers.end(), rng);
    int guesses;
    for (int k:numbers) {
        if (k == num) {
            std::cout << "Found! Your number was " << k << "." << std::endl;
            std::cout << "Using Random Sampling (no repeats), it took " << guesses << " guesses." << std::endl;
            break;
        }
        guesses++;
    }
    return 0;
}