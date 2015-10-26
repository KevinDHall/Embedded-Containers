In the CppCon 2015 session “Grill the Committee”, Bjarne Stroustrup had [the following to say (at 40:31)](https://youtu.be/PqU_ot4BlNQ?t=2431):

> There is one library I would like to see for the embedded systems community.... I would like to see a set of container classes that does not use the free store.  It can be done; it has been done; I don't see any reason why the comittee couldn't support it.

Bjarne stated ealier that in the same session ["Embedded C++ should not have been brought into existence."](https://youtu.be/PqU_ot4BlNQ?t=2145) and "Subsetting the library or adding a non-compliant library is a very poor solution."
https://youtu.be/PqU_ot4BlNQ?t=2130

This agrees very much with my own views on C++ in the embedded world.  I have a strong desire to see container classes that are more useful to the embedded world.  This project is an attempt to provide a possible implementation for a fixed-capacity vector proposal.  I desire to see more containers part of a final proposal, but a fixed-capacity vector is probably the most needed container and will also serve as a good starting point to flesh out ideas.

There are many implementations of fixed-capacity vectors out there.  Two open-source implementations are:

* [estl::vector from ESR Lab's ESTL](https://www.esrlabs.com/blog/estl-for-embedded-developers/)
* [etl::vector from John Wellbelove's ETLCPP project](http://www.etlcpp.com/)

There are of course many closed-source implementations developed within various companies.  

Most of these libraries use a class heirarchy to achieve code usability where the base class forms the vector interface and derived classes are used to define the capacity.  This project keeps this high-level idea.  std::fixed_vect<T> defines the interface class and std::fixed_vect<T>::static_sized<N> and std::fixed_vect<T>::runtime_sized serve as the declaration types.

The drawback with most existing libraries is that they do not use many C++11 or C++14 features, since many embedded projects still use old compilers.  This is mostly an effect of having legacy projects.  Embedded compiler vendors and RTOS vendors are providing more up-to-date compilers.  These compilers can compile std::fixed_vect and should a embedded containers proposal be accepted into the standard, these vendors will hopefully be able to provide implementations in a relatively short timeframe for new embedded projects.

Member functions that have the same name as std::vector member functions will match the C++14 signature for std::vector functions.  This is to allow other generic code to take advantage of the similarity of container interfaces.  

Other std::vector member functions don't make sense for a fixed-capacity container and as such are not included in std::fixed_vect (ex: reserve() and shrink_to_fit()).

And there are some new member functions too:

* Functions that make sense for fixed-capacity containers (such as `bool full() const`)
* Functions aimed to help avoid throwing exceptions when the container is full (`emplace_at()` and `inject()`)
