#include <iostream>
#include <iomanip>
#include <string>
#include <ostream>
#include <chrono>
#include <random>
#include <memory>
#include <array>
#include <vector>
#include <list>
#include <map>
#include <numeric>
#include <filesystem>
#include <fstream>
#include <utility>
#include <exception>
#include <future>
#include <thread>
#include <functional>
#include <algorithm>

//#define NDEBUG
#include <cassert>

#include <WinSDKVer.h>
#define _WIN32_WINNT 0x0A00
#include <SDKDDKVer.h>

#define _CRT_SECURE_NO_WARNINGS

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define NODRAWTEXT
#define NOGDI
#define NOBITMAP
#define NOMCX
#define NOSERVICE
#define NOHELP

#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <MSWSock.h>

//#include <websocket.h>

//#define _ASCII
#if defined _ASCII
using byte = unsigned char;
constexpr size_t c_buffer_size = (80ULL * 25ULL);

template <class T>
inline std::ostream& operator<<(std::ostream& os, std::vector<T>& v)
{
	std::for_each(v.begin(), v.end(), [&](auto& element) { os << element; });
	return os;
}
#endif

//#define _ARRAY
#if defined _ARRAY
class index_out_of_range_exception : std::exception {};

template <typename T>
class t_array
{
	T* block_pointer = { nullptr };
	size_t block_size = { 0 };

	bool is_valid_index(size_t index) const
	{
		return (index >= 0) && (index < block_size);
	}

public:
	t_array() = default;
	explicit t_array(size_t size)
	{
		assert(size >= 0);
		if (size != 0)
		{
			block_pointer = new T[size]{};
			block_size = size;
		}
	}

	t_array(const t_array& rhs)
	{
		if (!rhs.empty())
		{
			block_size = rhs.block_size;
			block_pointer = new T[block_size]{};
			for (size_t itr = 0; itr < block_size; ++itr)
			{
				block_pointer[itr] = rhs.block_pointer[itr];
			}
		}
	}
	t_array& operator=(t_array rhs)
	{
		swap(*this, rhs);
		return *this;
	}
	t_array(t_array&& lhs)
		: block_pointer{ lhs.block_pointer }
		, block_size{ lhs.block_size }
	{
		lhs.block_pointer = nullptr;
		lhs.block_size = 0;
	}
	~t_array()
	{
		delete[] block_pointer;
	}

	friend void swap(t_array& lhs, t_array& rhs) noexcept
	{
		using std::swap;

		swap(lhs.block_pointer, rhs.block_pointer);
		swap(lhs.block_size, rhs.block_size);
	}

	T& operator[](size_t index)
	{
		if (!is_valid_index(index))
		{
			throw index_out_of_range_exception{};
		}

		return block_pointer[index];
	}

	T operator[](size_t index) const
	{
		if (!is_valid_index(index))
		{
			throw index_out_of_range_exception{};
		}

		return block_pointer[index];
	}

	int size() const
	{
		return block_size;
	}

	bool empty() const
	{
		return (block_size == 0);
	}
};

template <typename T>
inline std::ostream& operator<<(std::ostream& os, const t_array<T>& a) {
	os << "[ ";
	for (size_t itr = 0; itr < a.size(); ++itr)
	{
		os << a[itr] << ' ';
	}
	os << ']';

	return os;
}
#endif

//#define _TREE
#if defined _TREE
template<typename T> // , class Alloc = std::allocator<T>
class tree
{
public:
	struct tree_node
	{
		std::list<std::shared_ptr<tree<T>>> p_children{};
		std::shared_ptr<tree<T>> p_parent = nullptr;
		std::unique_ptr<T> t_data = nullptr;
	};

	class tree_iterator : public std::iterator<std::bidirectional_iterator_tag, std::unique_ptr<tree<T>>>
	{
		typedef T value_type;
		typedef T& reference;
		typedef T* pointer;
		typedef std::bidirectional_iterator_tag iterator_category;
		typedef int difference_type;

		tree_node* p_it;
		bool b_depth_first;

		// THOUGHTS: Will probably need to implement a kind of next/prev pointer management scheme for ++ and --

	public:
		tree_iterator() = default;
		explicit tree_iterator(bool depth_first = true) : b_depth_first{ depth_first } {}
		// TODO: deep copy
		// TODO: deep copy assign
		// TODO: dtor?

		void swap(tree_iterator& rhs) noexcept
		{
			using std::swap;
			swap(p_it, rhs.p_it);
		}

		T& operator*() const
		{
			return *p_it.get();
		}
		T& operator->() const
		{
			return *p_it.get();
		}
		tree_iterator& operator++() {} // TODO: implement next() for list in tree
		tree_iterator& operator++(int) {}
		tree_iterator& operator--() {} // TODO: implement prev() for list in tree
		bool operator==(const tree_iterator& rhs) const
		{
			return (p_it == rhs.p_it);
		}
		bool operator!=(const tree_iterator& rhs) const {}
	};

private:
	// TODO: Implement depth boundaries
	const size_t c_max_depth = 255;

public:
	typedef tree_iterator iterator;
	iterator begin() { return iterator(); }
	iterator end(bool depth_first) { return iterator(); } // Depth first or bredth first options required

	tree<T>() = delete;
	explicit tree<T>(T p_impl)
	{
		t_data = std::make_unique<T>(p_impl);
	}
	tree<T>(T p_impl, std::shared_ptr<tree<T>> parent)
	{
		t_data = std::make_unique<T>(p_impl);
		p_parent = parent;
	}
	tree<T>(const tree<T>&) = delete; // TODO: Must be implemented
	tree<T>& operator=(const tree<T>&) = delete; // TODO: Must be implemented
	tree<T>(const tree<T>&&) = delete; // TODO: Must be implemented
	tree<T>& operator=(const tree<T>&&) = delete; // TODO: Must be implemented
	~tree<T>()
	{
		t_data.reset();
		t_data = nullptr;
	}

	const size_t size()
	{
		size_t c = 1;
		while (!p_children.empty())
		{
			for (auto& element : p_children)
			{
				c += element.size();
			}
		}
		return c;
	}
	const size_t max_depth()
	{
		return c_max_depth;
	}
	const T& p_impl() const
	{
		return *t_data.get();
	}

	void add_child(std::shared_ptr<tree<T>> child)
	{
		p_children.push_back(child);
	}
	void remove_child(std::shared_ptr<tree<T>> child)
	{
		p_children.remove(child);
	}
	std::shared_ptr<tree<T>> get_parent()
	{
		return p_parent;
	}
	std::shared_ptr<tree<T>> get_root()
	{
		auto ptr = p_parent;
		while (ptr->get_parent() != nullptr)
		{
			ptr = ptr->get_parent();
		}
		return ptr;
	}
};

template<typename T>
inline std::shared_ptr<tree<T>> make_child(T p_impl, std::shared_ptr<tree<T>> parent)
{
	auto c = std::make_shared<tree<T>>(p_impl, parent);
	parent->add_child(c);
	return c;
}
#endif

//#define _RANDOM
#if defined _RANDOM
static std::mt19937 engine{ std::random_device{}() };

unsigned get_dice(size_t count, unsigned sides)
{
	std::vector<unsigned> v;
	std::uniform_int_distribution<unsigned> d{ 1, sides };

	for (size_t itr = 0; itr < count; ++itr)
	{
		v.push_back(d(engine));
	}

	return std::accumulate(v.begin(), v.end(), 0);
}

class accumulator
{
public:
	accumulator() = default;
	~accumulator() = default;

	std::array<unsigned, 1'000'000> a{};

	void fill_array()
	{
		for (auto& element : a)
		{
			element = get_dice(3, 6);
		}
	}
	void sort()
	{
		std::sort(a.begin(), a.end());
	}
};

struct time_data
{
	std::chrono::time_point<std::chrono::steady_clock> begin;
	std::chrono::time_point<std::chrono::steady_clock> end;

	void print_dif()
	{
		auto dif = end - begin;
		std::cout << "Time: " << dif.count() << "ns" << std::endl;
	}
};
#endif

//#define STLPOLY
#if defined _STLPOLY
struct IUpdatable
{
	using pointer = std::shared_ptr<IUpdatable>;
	using const_pointer = const pointer;

	pointer ptr() { return pointer(this); }
	const_pointer cptr() { return ptr(); }

	virtual ~IUpdatable() {}
	virtual bool operator==(IUpdatable& rhs) { return *this == rhs; };
	virtual void Update(float elapsedTime) = 0;
};

class thing_a : public IUpdatable
{
public:
	virtual void Update(float elapsedTime) override { std::cout << "thing_a Update(" << elapsedTime << ")" << std::endl; }
};
class thing_b : public IUpdatable
{
public:
	virtual void Update(float elapsedTime) override { std::cout << "thing_b Update(" << elapsedTime << ")" << std::endl; }
};
class thing_c : public IUpdatable
{
public:
	virtual void Update(float elapsedTime) override { std::cout << "thing_c Update(" << elapsedTime << ")" << std::endl; }
};
#endif

//#define _CONCEPTSPOLY
#if defined _CONCEPTSPOLY
#include "concepts.h"
#endif

//#define _CALLBACKREF
#if defined _CALLBACKREF
int ref_i = 0;

void callback(int i)
{
	std::cout << "CALLBACK " << i << std::endl;
	ref_i = i;
}

class callback_obj
{
	using cb = std::function<void(int)>;

	cb _fn;
public:
	callback_obj(cb fn)
	{
		_fn = fn;
		_fn(1);
	}
	~callback_obj()
	{

	}
};
#endif

//#define _PRCHACK
#if defined _PRCHACK
struct poolradchar_tag
{
	char b_1; // 0
	char name[15]; // 1 - 15
	unsigned char stat_str; // 16
	unsigned char stat_int; // 17
	unsigned char stat_wis; // 18
	unsigned char stat_dex; // 19
	unsigned char stat_con; // 20
	unsigned char stat_cha; // 21
	unsigned char stat_hero_str; // 22
	char b_2; // 23
	char b_3[16]; // 24-39
	char b_4[8]; // 40-47
	unsigned char un1_le; // 48 unknown
	unsigned char un1_be; // 49
	unsigned char un2_le; // 50 unknown
	unsigned char un2_be; // 51
	char b_5[12]; // 52-63
	char b_6[64]; // 64-127
	char b_7a[8]; // 128-135
	unsigned char copper_le; // 136
	unsigned char copper_be; // 137
	unsigned char silver_le; // 138
	unsigned char silver_be; // 139
	unsigned char electrum_le; // 140
	unsigned char electrum_be; // 141
	unsigned char gold_le; // 142
	unsigned char gold_be; // 143
	unsigned char plat_le; // 144
	unsigned char plat_be; // 145
	unsigned char gems_le; // 146
	unsigned char gems_be; // 147
	unsigned char jewel_le; // 148
	unsigned char jewel_be; // 149
	char b_7b[10]; // 150-159
	char b_8[8]; // 160-167
	char b_9[4]; // 168-171
	unsigned char exp_le; // 172 little end
	unsigned char exp_be; // 173 big end
	char b_10[2]; // 174-175
	char b_11[11]; // 176-186
	unsigned char portrait_head; // 187
	unsigned char portrait_body; // 188
	unsigned char icon_head; // 189
	unsigned char icon_weapon; // 190
	unsigned char position; // 191 
	char b_13[8]; // 192-199
	char b_14[8]; // 200-207
	char b_15[48]; // 208-255
	char b_16; // 256
	unsigned char hp; // 257
	unsigned char gold; // 258 - Maybe encumberance?
};

struct PoolRadChar
{
	std::string Name;
	int Strength;
	int Intelligence;
	int Wisdom;
	int Dexterity;
	int Constitution;
	int Charisma;
	int HeroicStrength;
	int Experience;
	int PortraitHeadIndex;
	int PortraitBodyIndex;
	int IconHeadIndex;
	int IconWeaponIndex;
	int Copper;
	int Silver;
	int Electrum;
	int Gold;
	int Platinum;
	int Gems;
	int Jewelry;
	int HitPoints;
};

constexpr auto c_poolradchr_size = sizeof(poolradchar_tag);

std::string _make_name_string(const char arr[], const unsigned long long& len)
{
	std::string s{};
	for (int i = 0; i < len; ++i)
	{
		s.push_back(arr[i]);
	}
	s.push_back('\0');
	return s;
}
int _make_short(const unsigned char& be, const unsigned char& le)
{
	return be << 8 | le;
}
int _make_char(const unsigned char& b)
{
	return static_cast<int>(b);
}
int _make_high_nibble(const unsigned char& b)
{
	// bit shift >> 4
	return b >> 4;
}
int _make_low_nibble(const unsigned char& b)
{
	// & mask 00001111
	return b & 15;
}

PoolRadChar MakePoolRadChar(poolradchar_tag* tag)
{
	PoolRadChar prc{
		_make_name_string(tag->name, 15),
		_make_char(tag->stat_str),
		_make_char(tag->stat_int),
		_make_char(tag->stat_wis),
		_make_char(tag->stat_dex),
		_make_char(tag->stat_con),
		_make_char(tag->stat_cha),
		_make_char(tag->stat_hero_str),
		_make_short(tag->exp_be, tag->exp_le),
		_make_char(tag->portrait_head),
		_make_char(tag->portrait_body),
		_make_char(tag->icon_head),
		_make_char(tag->icon_weapon),
		_make_short(tag->copper_be, tag->copper_le),
		_make_short(tag->silver_be, tag->silver_le),
		_make_short(tag->electrum_be, tag->electrum_le),
		_make_short(tag->gold_be, tag->gold_le),
		_make_short(tag->plat_be, tag->plat_le),
		_make_char(tag->hp)
	};

	return prc;
}
void PrintPoolRadChar(const PoolRadChar& prc)
{
	const size_t c_width = 12;

	std::cout << std::setw(c_width) << "Name: " << prc.Name << std::endl;
	std::cout << std::setw(c_width) << "STR: " << prc.Strength;
	if (prc.HeroicStrength != 0)
	{
		 std::cout << "(" << prc.HeroicStrength << ")";
	}
	std::cout << std::endl;
	std::cout << std::setw(c_width) << "INT: " << prc.Intelligence << std::endl;
	std::cout << std::setw(c_width) << "WIS: " << prc.Wisdom << std::endl;
	std::cout << std::setw(c_width) << "DEX: " << prc.Dexterity << std::endl;
	std::cout << std::setw(c_width) << "CON: " << prc.Constitution << std::endl;
	std::cout << std::setw(c_width) << "CHA: " << prc.Charisma << std::endl;
	std::cout << std::setw(c_width) << "Experience: " << prc.Experience << std::endl;
	std::cout << std::setw(c_width) << "Head: " << prc.PortraitHeadIndex << std::endl;
	std::cout << std::setw(c_width) << "Body: " << prc.PortraitBodyIndex << std::endl;
	std::cout << std::setw(c_width) << "IconHead: " << prc.IconHeadIndex << std::endl;
	std::cout << std::setw(c_width) << "IconWeapon: " << prc.IconWeaponIndex << std::endl;
	std::cout << std::setw(c_width) << "Jewelry: " << prc.Jewelry << std::endl;
	std::cout << std::setw(c_width) << "Gems: " << prc.Gems << std::endl;
	std::cout << std::setw(c_width) << "Platinum: " << prc.Platinum << std::endl;
	std::cout << std::setw(c_width) << "Gold: " << prc.Gold << std::endl;
	std::cout << std::setw(c_width) << "Electrum: " << prc.Electrum << std::endl;
	std::cout << std::setw(c_width) << "Silver: " << prc.Silver << std::endl;
	std::cout << std::setw(c_width) << "Copper: " << prc.Copper << std::endl;
	std::cout << "----------------------------------------" << std::endl;
}
#endif

struct jam_character_tag
{
	unsigned char guid_account[16];
	unsigned char guid_region[16];
	unsigned char guid_server[16];
	unsigned char guid_character[16];
	char name[256];
	short fitness;
	short coordination;
	short acuity;
	short apperception;
};

struct JAMCharacterBase
{
	GUID CharacterGUID;
	std::wstring Name;
	short Fitness;
	short Coordination;
	short Acuity;
	short Apperception;
	float OneBar;
};


int main()
{



	//#include "tree.h"
	//auto t = std::make_unique<ce::tree<int>>();
	std::cout << "[" << std::setfill('0') << std::setw(8) << std::this_thread::get_id() << "] main()" << std::endl;



#if defined _PRCHACK
	namespace fs = std::filesystem;
	using ios = std::ios;
	
	std::vector<fs::path> f_paths;
	fs::path ini_path{ fs::current_path() / "paths.ini" };
	assert(fs::exists(ini_path));
	
	auto ini_size = fs::file_size(ini_path);
	assert(ini_size > 0);

	std::string s;
	auto h_ini = std::ifstream(ini_path, ios::binary | ios::beg);
	assert(h_ini);

	while (h_ini >> s) f_paths.push_back(s);

	std::vector<PoolRadChar> prcs;

	auto lx_extract = [&](auto& element)
	{
		assert(fs::exists(element));

		auto f_size = fs::file_size(element);
		assert(f_size >= c_poolradchr_size);

		auto h_ifs = std::ifstream(element, ios::binary | ios::beg);
		assert(h_ifs);

		std::vector<char> v(f_size);
		h_ifs.read(v.data(), f_size);
		h_ifs.close();

		v.resize(c_poolradchr_size);
		prcs.emplace_back(MakePoolRadChar(reinterpret_cast<poolradchar_tag*>(v.data())));
	};

	std::for_each(f_paths.cbegin(), f_paths.cend(), lx_extract);

	std::for_each(prcs.cbegin(), prcs.cend(), [&](auto& element) { PrintPoolRadChar(element); });
#endif

#if defined _CALLBACKREF
	std::cout << "ref_i before: " << ref_i << std::endl;
	auto o = std::make_unique<callback_obj>(std::bind(&callback, std::placeholders::_1));
	o.reset();
	std::cout << "ref_i after: " << ref_i << std::endl;
#endif

#if defined _CONCEPTSPOLY
	using namespace std;

	document_t document;
	document.reserve(5);
	document.emplace_back(0);
	document.emplace_back(string("Hello!"));
	document.emplace_back(document);
	document.emplace_back(3.123f);
	reverse(document.begin(), document.end());
	draw(document, cout, 0);

	history_t h(1);

	current(h).emplace_back(0);
	current(h).emplace_back(string("Hello!"));

	draw(current(h), cout, 0);
	cout << "--------------------------" << endl;

	commit(h);

	current(h)[0] = 42.5;

	auto saving = async([lx_document = current(h)]() {
		this_thread::sleep_for(chrono::seconds(3));
		cout << "--------- 'save' ---------" << endl;
		draw(lx_document, cout, 0);
	});

	current(h)[1] = string("World!");
	current(h).emplace_back(current(h));
	current(h).emplace_back(3);

	draw(current(h), cout, 0);
	cout << "--------------------------" << endl;

	undo(h);

	draw(current(h), cout, 0);
#endif

#if defined _STLPOLY
	std::vector<IUpdatable::pointer> vUpdatables;

	auto a = new thing_a();
	vUpdatables.emplace_back(a->cptr());
	auto b = new thing_b();
	vUpdatables.emplace_back(b->cptr());
	auto c = new thing_c();
	vUpdatables.emplace_back(c->cptr());
	
	std::for_each(std::begin(vUpdatables), std::end(vUpdatables), [](auto& element) { element->Update(0.f); });
#endif

#if defined _ASCII
#include "maps.inl"

	auto h_cout = GetStdHandle(STD_OUTPUT_HANDLE);

	if (!SetConsoleTextAttribute(h_cout, FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED | BACKGROUND_BLUE)) throw;

	size_t itr{ 0ULL };

	auto v = std::vector<byte>(c_buffer_size);
	std::for_each(v.begin(), v.end(), [&](auto& element) { element = itoh[(itr++ % size_t{ 48ULL }) + size_t{ 176ULL }]; });
	std::cout << v;

	itr = 0ULL;

	SetConsoleCursorPosition(h_cout, COORD{ 0, 0 });
#endif

#if defined _TREE
	auto t = std::make_shared<tree<int>>(1);
	auto u = make_child(11, t);
	auto v = make_child(12, t);
	auto w = make_child(111, u);
	auto x = make_child(112, u);
	auto y = make_child(121, v);

	auto up = u->get_parent();
	auto upd = up->p_impl();
	auto xp = x->get_parent();
	auto xpd = xp->p_impl();
	auto ur = u->get_root();
	auto urd = ur->p_impl();

	std::cout << t->size();
	v->remove_child(y);
	std::cout << t->size();

#endif

#if defined _RANDOM
	using std::cout;
	using std::endl;
	using hrc = std::chrono::high_resolution_clock;
	using ios = std::ios;
	namespace fs = std::filesystem;

	time_data td{};
	td.begin = hrc::now();
	cout << "Allocating array" << endl;
	auto a = std::make_unique<accumulator>();
	td.end = hrc::now();
	td.print_dif();

	fs::file_status file_status{};
	if (!(fs::status_known(file_status) ? fs::exists(file_status) : fs::exists("million3D6.txt")))
	{
		//  If data file does not exist, allocate, generate, and put the values to the file
		std::ofstream h_ofs("million3D6.txt");

		td.begin = hrc::now();
		cout << "Filling array" << endl;
		a->fill_array();
		td.end = hrc::now();
		td.print_dif();

		td.begin = hrc::now();
		cout << "Sorting array" << endl;
		a->sort();
		td.end = hrc::now();
		td.print_dif();

		td.begin = hrc::now();
		cout << "Writing array to million3D6.txt" << endl;
		for (auto& element : a->a)
		{
			//h_ofs << std::hex << std::showbase << element << endl;
			h_ofs << std::hex << element << ' ';
		}
		td.end = hrc::now();
		td.print_dif();

		h_ofs.close();
	}
	else
	{
		std::ifstream h_ifs;
		h_ifs.open("million3D6.txt", ios::in);

		td.begin = hrc::now();
		cout << "Reading array from million3D6.txt" << endl;
		for (auto& element : a->a)
		{
			h_ifs >> std::hex >> element;
		}
		td.end = hrc::now();
		td.print_dif();

		h_ifs.close();
	}

	// Print statistical distribution of values
	cout << std::fixed << std::setprecision(3);

	auto three = float(std::count_if(a->a.begin(), a->a.end(), [](int element) { return element == 3; }));
	cout << std::setw(12) << "Threes:" << std::setw(7) << three / 10'000.f << "%" << endl;
	auto four = float(std::count_if(a->a.begin(), a->a.end(), [](int element) { return element == 4; }));
	cout << std::setw(12) << "Fours:" << std::setw(7) << four / 10'000 << "%" << endl;
	auto five = float(std::count_if(a->a.begin(), a->a.end(), [](int element) { return element == 5; }));
	cout << std::setw(12) << "Fives:" << std::setw(7) << five / 10'000 << "%" << endl;
	auto six = float(std::count_if(a->a.begin(), a->a.end(), [](int element) { return element == 6; }));
	cout << std::setw(12) << "Sixes:" << std::setw(7) << six / 10'000 << "%" << endl;
	auto seven = float(std::count_if(a->a.begin(), a->a.end(), [](int element) { return element == 7; }));
	cout << std::setw(12) << "Sevens:" << std::setw(7) << seven / 10'000 << "%" << endl;
	auto eight = float(std::count_if(a->a.begin(), a->a.end(), [](int element) { return element == 8; }));
	cout << std::setw(12) << "Eights:" << std::setw(7) << eight / 10'000 << "%" << endl;
	auto nine = float(std::count_if(a->a.begin(), a->a.end(), [](int element) { return element == 9; }));
	cout << std::setw(12) << "Nines:" << std::setw(7) << nine / 10'000 << "%" << endl;
	auto ten = float(std::count_if(a->a.begin(), a->a.end(), [](int element) { return element == 10; }));
	cout << std::setw(12) << "Tens:" << std::setw(7) << ten / 10'000 << "%" << endl;
	auto eleven = float(std::count_if(a->a.begin(), a->a.end(), [](int element) { return element == 11; }));
	cout << std::setw(12) << "Elevens:" << std::setw(7) << eleven / 10'000 << "%" << endl;
	auto twelve = float(std::count_if(a->a.begin(), a->a.end(), [](int element) { return element == 12; }));
	cout << std::setw(12) << "Twelves:" << std::setw(7) << twelve / 10'000 << "%" << endl;
	auto thirteen = float(std::count_if(a->a.begin(), a->a.end(), [](int element) { return element == 13; }));
	cout << std::setw(12) << "Thirtheens:" << std::setw(7) << thirteen / 10'000 << "%" << endl;
	auto fourteen = float(std::count_if(a->a.begin(), a->a.end(), [](int element) { return element == 14; }));
	cout << std::setw(12) << "Fourteens:" << std::setw(7) << fourteen / 10'000 << "%" << endl;
	auto fifteen = float(std::count_if(a->a.begin(), a->a.end(), [](int element) { return element == 15; }));
	cout << std::setw(12) << "Fifteens:" << std::setw(7) << fifteen / 10'000 << "%" << endl;
	auto sixteen = float(std::count_if(a->a.begin(), a->a.end(), [](int element) { return element == 16; }));
	cout << std::setw(12) << "Sixteens:" << std::setw(7) << sixteen / 10'000 << "%" << endl;
	auto seventeen = float(std::count_if(a->a.begin(), a->a.end(), [](int element) { return element == 17; }));
	cout << std::setw(12) << "Seventeens:" << std::setw(7) << seventeen / 10'000 << "%" << endl;
	auto eighteen = float(std::count_if(a->a.begin(), a->a.end(), [](int element) { return element == 18; }));
	cout << std::setw(12) << "Eighteens:" << std::setw(7) << eighteen / 10'000 << "%" << endl;
#endif

	return 0;
}

/*
template <typename T>
inline void no_op()
{
	const T* cpt{}; // object const - read only
	volatile T* vpt{}; // object volatile - read on access always

	T* const ptc{}; // pointer const
	T* volatile ptv{}; // pointer volatile

	const T* const cptc{}; // object and pointer const
	const T* volatile cptv{}; // object const, pointer volatile

	volatile T* const vptc{}; // object volatile, pointer const
	volatile T* volatile vptv{}; // object and pointer volatile

	const volatile T* cvpt{}; // object const and volatile
	T* const volatile ptcv{}; // pointer const and volatile
	const volatile T* const volatile cvptcv{}; // object and pointer const and volatile
}
*/
