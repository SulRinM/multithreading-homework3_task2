#include <iostream>
#include <algorithm>
#include <iterator>
#include <future>

using namespace std;

template <class Iterator, class Function>
void parallel_for_each(Iterator first, Iterator last, Function func) {
	auto const len = distance(first, last);
	auto const maxChunkSize = 25;
	if (len <= maxChunkSize) {
		for_each(first, last, func);
	}
	else {
		Iterator midPoint = first;
		advance(midPoint, len / 2);
		future<void> firstHalfResult = async(launch::async, parallel_for_each<Iterator, Function>, first, midPoint, func);
		parallel_for_each(midPoint, last, func);
	}
}

void printVector(vector<int>& v) {
	for (const auto& a : v)
		cout << a << " ";
	cout << '\n';
}

int main() {
	vector<int> v(10);
	generate(v.begin(), v.end(), []() {return rand() % 10; });
	printVector(v);
	parallel_for_each(v.begin(), v.end(), [](auto& el) { return el *= el; });
	printVector(v);

	return 0;
}