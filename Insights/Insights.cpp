``#include <array>
#include <tuple>
#include <vector>
#include <functional>
#include <utility>

using int_array_t = std::array<int, 4>;
using int_tuple_t = std::tuple<int, int, int>;
using int_pair_t = std::pair<int, int>;

constexpr auto lxSumPair = [](auto first, auto second) { return first + second; };

constexpr auto lxSumVariadic = [](auto ...args) { return (... + args); };
constexpr auto lxProduct = [](auto ...args) { return (... * args); };

template<typename TR, typename T1, typename T2>
constexpr TR calculate(T1 lambda, const T2 & value)
{
	return std::apply(lambda, value); // TR assumes that you are providing the correct type, apply returns decltype(auto)
}

template<typename T>
bool filter(const T & filters, int val)
{
	return std::all_of(filters.begin(), filters.end(),
		[val](const auto & filter) { return filter(val); });
}

#include <experimental/vector>

int main()
{
	////////////////////////////////////////
	using FilterContainer = std::vector<std::function<bool(int)>>;
	FilterContainer lxFilters;
	lxFilters.emplace_back([&](int val) { return val % 5 == 0; });
	lxFilters.emplace_back([&](int val) { return val >= 10; });
	lxFilters.emplace_back([&](int val) { return val <= 100; });

	using FilterResults = std::vector<bool>;
	FilterResults filteredValues;
	filteredValues.push_back(filter(lxFilters, 5)); // False
	filteredValues.push_back(filter(lxFilters, 10)); // True
	filteredValues.push_back(filter(lxFilters, 50)); // True
	filteredValues.push_back(filter(lxFilters, 100)); // True
	filteredValues.push_back(filter(lxFilters, 42)); // False

	////////////////////////////////////////
	constexpr auto lxFilter = [](int val) -> bool { return val % 5 == 0; };
	filteredValues.push_back(lxFilter(10)); // Adds true
	filteredValues.push_back(lxFilter(5)); // Adds true
	filteredValues.push_back(lxFilter(7)); // Adds false

	////////////////////////////////////////
	auto sumPairFloat = std::apply(lxSumPair, std::make_pair(1.f, 2.f));
	auto sumPairLong = std::apply(lxSumPair, std::make_pair(2L, 3L));

	auto sumVariadicPairFloat = std::apply(lxSumVariadic, std::make_pair(1.f, 2.f));
	auto sumVariadicPairLong = std::apply(lxSumVariadic, std::make_pair(2L, 3L));

	auto sumVariadicTupleFloat = std::apply(lxSumVariadic, std::make_tuple(1.f, 2.f, 3.f));
	auto sumVariadicTupleLong = std::apply(lxSumVariadic, std::make_tuple(2L, 3L, 4L, 5L));

	auto sumVariadicArrayT = std::apply(lxSumVariadic, int_array_t{ 2, 3, 4, 5 }); // Note: std::experimental::make_array
	auto sumVariadicTupleT = std::apply(lxSumVariadic, int_tuple_t{ 1, 2, 3 });
	auto sumVariadicPairT = std::apply(lxSumVariadic, int_pair_t{ 2, 3 });

	////////////////////////////////////////
	int_array_t int_array{ 2, 3, 4, 5 };
	int_tuple_t int_tuple{ 2, 3, 4 };
	int_pair_t int_pair{ 2, 3 };

	auto sumof_array = calculate<int>(lxSumVariadic, int_array);
	auto productof_array = calculate<int>(lxProduct, int_array);

	auto sumof_tuple = calculate<int>(lxSumVariadic, int_tuple);
	auto productof_tuple = calculate<int>(lxProduct, int_tuple);

	auto sumof_pair1 = calculate<int>(lxSumPair, int_pair);
	auto sumof_pair2 = calculate<int>(lxSumVariadic, int_pair);

	auto sumof_makepair1 = calculate<int>(lxSumVariadic, std::make_pair(2, 3));
	auto sumof_makepair2 = calculate<float>(lxSumVariadic, std::make_pair(2.3f, 3.4f)); // = 5.7 Loses a great deal of precision here...

	auto sumof_maketuple1 = calculate<int>(lxSumVariadic, std::make_tuple(2, 3, 4));
	auto sumof_maketuple2 = calculate<float>(lxSumVariadic, std::make_tuple(2.3f, 3.4f, 4.5f, 5.6f, 6.7f, 7.8f)); // = 30.3

	////////////////////////////////////////
	std::vector<int> vCollection;
	vCollection.push_back(1);
	vCollection.push_back(-2);
	vCollection.push_back(15);
	vCollection.push_back(7);
	vCollection.push_back(10);
	vCollection.push_back(100);
	vCollection.push_back(110);
	vCollection.push_back(111);

	auto ret1 = std::find_if(vCollection.begin(), vCollection.end(),
		[](auto val) { return val > 0 && val < 10; }); // Returns first element "1"
	auto ret2 = std::all_of(vCollection.begin(), vCollection.end(),
		[&](const auto & val) { return val > -10 && val < 120; }); // Returns true
	auto ret3 = std::all_of(begin(vCollection), end(vCollection),
		[](auto val) { return val > 0 && val < 10; }); // Returns false
	auto ret4 = std::find_if(begin(vCollection), end(vCollection),
		[&](const auto & val) { return val > -10 && val < 0; }); // Returns second element "-2"

	namespace exp = std::experimental;
	std::vector<int> ints{ 10, 11, 12, 13, 14, 15, 16 };
	ints.erase(std::remove(ints.begin(), ints.end(), 15), ints.end());
	ints.erase(std::remove(ints.begin(), ints.end(), std::find_if(ints.begin(), ints.end(), [](auto val) { return val == 14; })), ints.end());
	ints.erase(std::remove_if(ints.begin(), ints.end(), [](auto val) { return val == 13; }));
	exp::erase(ints, 12);
	exp::erase_if(ints, [](auto val) { return val == 11; });

	return 0;
}
