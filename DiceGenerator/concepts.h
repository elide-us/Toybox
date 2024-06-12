#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <cassert>

template <class T>
void draw(const T& lhs, std::ostream& out, size_t position) { out << std::string(position, ' ') << lhs << endl; }

struct drawable_t
{
	virtual ~drawable_t() = default;
	virtual void _draw(std::ostream&, size_t) const = 0;
};

template <class T>
struct drawable_model_t final : drawable_t
{
	T data;
	drawable_model_t(T rhs) : data(std::move(rhs)) {}
	void _draw(std::ostream& out, size_t position) const override { draw(data, out, position); }
};

class drawable_object_t
{
	std::shared_ptr<const drawable_t> p_impl;
public:
	template <class T>
	drawable_object_t(T lhs) : p_impl(std::make_shared<drawable_model_t<T>>(std::move(lhs))) {}

	friend void draw(const drawable_object_t& obj, std::ostream& out, size_t position)
	{ obj.p_impl->_draw(out, position); }
};

using document_t = std::vector<drawable_object_t>;

void draw(const document_t& ref, std::ostream& out, size_t position)
{
	out << std::string(position, ' ') << "<document>" << std::endl;
	for (const auto& e : ref) { draw(e, out, position + 2); }
	out << std::string(position, ' ') << "</document>" << std::endl;
}

using history_t = std::vector<document_t>;

void commit(history_t& lhs) { assert(lhs.size()); lhs.push_back(lhs.back()); }
void undo(history_t& lhs) { assert(lhs.size()); lhs.pop_back(); }
document_t& current(history_t& lhs) { assert(lhs.size()); return lhs.back(); }

/*
struct drawable_t
{
	virtual ~drawable_t() = default;
	--virtual unique_ptr<concept_t> _copy() const = 0;
	virtual void _draw(ostream&, size_t) const = 0;
};
*/

/*
object_t(T lhs) : self(make_unique<model<T>>(move(lhs))) {}
object_t(const string& lhs) : self(make_unique<string_model_t>(move(lhs))) {} // Default Ctor for string
object_t(const int& lhs) : self(make_unique<int_model_t>(move(lhs))) {} // Default Ctor for int
object_t(const object_t& lhs) : self(lhs.self->_copy()) { cout << "copy" << endl; } // Virtual copy constructor implementation (cannot implement second copy ctor)
object_t(const object_t& lhs) : self(make_unique<int_model_t>(*lhs.self)) {} // Copy Ctor
object_t(object_t&&) noexcept = default; // Move Ctor
object_t& operator=(const object_t& lhs ) { return *this = object_t(lhs); } // Copy Assign leveraging move assign operator for rvalue temporary
object_t& operator=(const object_t& x) { object_t tmp(x); self = move(tmp.self); return *this; } // Copy Assign (default)
object_t& operator=(object_t x) noexcept { self = move(x.self); return *this; } // Copy Assign (as Sink)
object_t& operator=(object_t&&) noexcept = default; // Move Assign
*/