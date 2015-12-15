# Embedded Container Library for C++

## Background

In the CppCon 2015 session “Grill the Committee”, Bjarne Stroustrup had [the following to say (at 40:31)](https://youtu.be/PqU_ot4BlNQ?t=2431):

> There is one library I would like to see for the embedded systems community.... I would like to see a set of container classes that does not use the free store.  It can be done; it has been done; I don't see any reason why the comittee couldn't support it.

Bjarne stated ealier that in the same session ["Embedded C++ should not have been brought into existence."](https://youtu.be/PqU_ot4BlNQ?t=2145) and "Subsetting the library or adding a non-compliant library is a very poor solution."

This project is an attempt to provide a possible implementation for a fixed-capacity vector proposal.  I desire to see more containers part of a final proposal, but a fixed-capacity vector is probably the most needed container and will also serve as a good starting point to flesh out ideas.

There are many implementations of fixed-capacity vectors out there.  Two open-source implementations are:

* [estl::vector from ESR Lab's ESTL](https://www.esrlabs.com/blog/estl-for-embedded-developers/)
* [etl::vector from John Wellbelove's ETLCPP project](http://www.etlcpp.com/)

There are of course many closed-source implementations developed within various companies.  

Also, some interesting discussion on the design of fixed-capacity containers can be found [here on reddit](https://www.reddit.com/r/cpp/comments/2mw6xk/c_stl_for_embedded_developers/).

## Whetting Your Appetite - A Short Usage Example

```
#include <algorithm>
#include <iostream>
#include <fixed_vect>

void sort_numbers(std::fixed_vect<int>& v)
{
	std::sort(v.begin(), v.end());
}

int main()
{
	std::fixed_vect<int>::static_sized<5>	v {7, 1, 13, 2};

	v.push_back(3);

	sort_numbers(v);

	for (auto&& i : v)
	{
		std::cout << i << '\n';
	}
}
```

## Design

Most of existing libraries use a class heirarchy to reduce code duplication for users.  The base class forms the container interface and derived classes are used to define the capacity.  This project keeps this high-level idea.  std::fixed_vect<T> defines the interface class and std::fixed_vect<T>::static_sized<N> and std::fixed_vect<T>::runtime_sized serve as the declaration types.

The drawback with most existing libraries is that they do not use many C++11 or C++14 features, since many embedded projects still use old compilers.  This is mostly an effect of having legacy projects.  Embedded compiler vendors and RTOS vendors are providing more up-to-date compilers.  These compilers can compile std::fixed_vect and should a embedded containers proposal be accepted into the standard, these vendors will hopefully be able to provide implementations in a relatively short timeframe for new embedded projects.

Member functions that have the same name as std::vector member functions will match the C++14 signature for std::vector functions.  This is to allow other generic code to take advantage of the similarity of container interfaces.  

Other std::vector member functions don't make sense for a fixed-capacity container and as such are not included in std::fixed_vect (ex: reserve() and shrink_to_fit()).

And there are some new member functions too:

* Functions that make sense for fixed-capacity containers (such as `bool full() const`)
* Functions aimed to help avoid throwing exceptions when the container is full (`emplace_at()` and `inject()`)

The one major area this project deviates from other work is the addition of a runtime-sized declaration class (`std::fixed_vect<T>::runtime_sized`).  There are use cases where the required capacity of a container isn't known at compile-time, but can be determined prior to the long-term running state.  This follows the embedded idiom of "allocate memory from the free store only at the start of the application, and then never thereafter."

### Naming

I am not stuck on the names `fixed_vect`, `inject`, or `emplace_at`.  These are merely the best names I could come up with.  I am open to suggestions.

I do like the names of `static_sized`, `runtime_sized`, and `full`, but am not stuck on these either.  I welcome all naming suggestions.

## TODO

* Update CMake files to generate make and solution files in more appropriate directories.
* Write more tests.

## Do You Wish To Help Out?

I am looking for all types of feedback.  In particular, I am interested in:

* Bug fixes
* Documentation improvements
* Help regarding the TODO items above: CMake configuration, writing more thorough tests.
* Missing methods?  (Did I miss something `std::vector` has that `fixed_vect` should have too?  Are there other methods like `full()` that should be added?)

I am also interested in informaiton that would help make a proposal more compelling:

* Use cases.  Where would you like to use `fixed_vect` or other fixed-sized containers?
* Suggestions regarding usability and organization.  Ex: class structure, naming, etc....
* Suggestions for making a compelling proposal.

Lastly, I would like to know what other containers would be useful?  Some ideas include:

* Linked list
* Flat_map

Which of these would be useful for your work?

## Documentation

### Class Definitions, Related Functions

```
template <class T>
class std::fixed_vect
{
public:
    // types:
    typedef T                                     value_type;
    typedef value_type&                           reference;
    typedef const value_type&                     const_reference;
    typedef value_type*                           pointer;
    typedef const value_type*                     const_pointer;
    typedef implementation-defined		          iterator;
    typedef implementation-defined                const_iterator;
    typedef std::reverse_iterator<iterator>       reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
    typedef implementation-defined                size_type;				// Unsigned integer type
    typedef implementation-defined                difference_type;			// Signed integer type

    template <size_type N>
    class static_sized;

    class runtime_sized;

    // 23.3.6.2, construct/copy/destroy:
private:
    fixed_vect() = delete;	// private or deleted

public:
    fixed_vect(const fixed_vect&&) = delete;
    fixed_vect& operator=(const fixed_vect&&) = delete;
    template <class U>
    fixed_vect(const U&&) = delete;
    template <class U>
    fixed_vect& operator=(const U&&) = delete;
    fixed_vect(const fixed_vect&) = delete;
    fixed_vect& operator=(const fixed_vect&) = delete;
    template <class U>
    fixed_vect(const U&) = delete;
    template <class U>
    fixed_vect& operator=(const U&) = delete;

    // iterators:
    iterator				begin() noexcept
    const_iterator			begin() const noexcept;

    iterator				end() noexcept;
    const_iterator			end() const noexcept;

    reverse_iterator		rbegin() noexcept;
    const_reverse_iterator	rbegin() const noexcept;

    reverse_iterator		rend() noexcept;
    const_reverse_iterator	rend() const noexcept;

    const_iterator			cbegin() const noexcept;
    const_iterator			cend() const noexcept;

    const_reverse_iterator	crbegin() const noexcept;
    const_reverse_iterator	crend() const noexcept;

    // capacity:
    bool empty() const noexcept;

    bool full() const noexcept;

    size_type size() const noexcept;

    size_type max_size() const noexcept;

    size_type capacity() const noexcept;

    void resize(size_type count);

    void resize(size_type count, const value_type& value);

    // element access:
    const_reference operator[](size_type pos) const;

    reference operator[](size_type pos);

    const_reference at(size_type pos) const;

    reference at(size_type pos);

    reference front();

    const_reference front() const;

    reference back();

    const_reference back() const;

    // data access
    pointer data() noexcept;

    const_pointer data() const noexcept;

    // modifiers:
    template <class... Args>
    void emplace_back(Args&&... args);

    void push_back(const T& value);

    void push_back(T&& value);

    void pop_back();

    template <class... Args>
    iterator emplace(const_iterator pos, Args&&... args);

    iterator insert(const_iterator pos, const T& value);

    iterator insert(const_iterator pos, T&& value);

    iterator insert(const_iterator pos, size_type count, const T& value);

    template <class InputIterator>
    iterator insert(const_iterator pos, InputIterator first, InputIterator last);

    iterator insert(const_iterator pos, std::initializer_list<T> ilist);

    iterator erase(const_iterator pos);

    iterator erase(const_iterator first, const_iterator last);

    void clear() noexcept;

    // The following member functions are designed to be exception-free replacements
    // for emplace(const_iterator, Args&&...) and the various insert functions.
    // These are intended to appeal to people who want an interface that doesn't
    // throw exceptions when attempting to add elements to a full container.
    //
    // They still can't be trivially declared noexcept since it is unknown if T's
    // constructor can throw.  It may be possible to specify when the functions can be
    // noexcept with a complicated expression.  I have not put effort into that yet though.
    template <class... Args>
    bool emplace_at(const_iterator pos, Args&&... args);

    bool inject(const_iterator pos, const T& value);

    bool inject(const_iterator pos, T&& value);

    size_type inject(const_iterator pos, size_type count, const T& value);

    template <class InputIterator>
    std::pair<size_type, InputIterator> inject(const_iterator pos, InputIterator first, InputIterator last);

    size_type inject(const_iterator pos, std::initializer_list<T> ilist);
};

template <class T>
bool operator==(const fixed_vect<T>& x, const fixed_vect<T>& y);

template <class T>
bool operator< (const fixed_vect<T>& x, const fixed_vect<T>& y);

template <class T>
bool operator!=(const fixed_vect<T>& x, const fixed_vect<T>& y);

template <class T>
bool operator> (const fixed_vect<T>& x, const fixed_vect<T>& y);

template <class T>
bool operator>=(const fixed_vect<T>& x, const fixed_vect<T>& y);

template <class T>
bool operator<=(const fixed_vect<T>& x, const fixed_vect<T>& y);

namespace detail
{

template <class T, implementation-defined N>		// type of N should match std::fixed_vector<T>::size_type
class std::fixed_vect<T>::static_sized<N>
{
public:
    static_sized() noexcept;

    static_sized(size_type count, const T& value);

    static_sized(size_type count);

    template <class InputIt>
    static_sized(InputIt first, InputIt last);

    static_sized(std::initializer_list<T> ilist);

    ~static_sized();

    // Expensive operations
    static_sized(const static_sized& other) = delete;

    static_sized& operator=(const static_sized& other) = delete;

    static_sized(static_sized&& other) = delete;

    static_sized& operator=(static_sized&& other) = delete;
};

template <class T>
class std::fixed_vect<T>::runtime_sized
{
public:
    runtime_sized();

    runtime_sized(size_type N);

    runtime_sized(size_type N, size_type count);

    runtime_sized(size_type N, size_type count, const T& value);

    template <class InputIt>
    runtime_sized(size_type N, InputIt first, InputIt last);

    runtime_sized(size_type N, std::initializer_list<T> init);

    runtime_sized(runtime_sized&& other);

    ~runtime_sized();

    runtime_sized& operator=(runtime_sized&& other);

    // Expensive operations
    runtime_sized(const runtime_sized&) = delete;

    runtime_sized& operator=(const runtime_sized<T>& other) = delete;
};
```
