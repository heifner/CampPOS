// Copyright 2011 Kevin Heifner.  All rights reserved.
//
// refptr.h
//
// A reference counted pointer based on auto_ptr
//

#ifndef ref_ptr_H
#define ref_ptr_H

template <class X>
class ref_ptr {
public:

  // default constructor creates a ref_ptr that wraps NULL
  ref_ptr() : px_(0), refCount_(new unsigned int(1)), owned_(false) {}
  
  // The constructor to use to wrap a newly created object.
  // owned flag indicates if ref_ptr deletes the object
  explicit ref_ptr(X* px, bool owned = true)
    : px_(px), refCount_(new unsigned int(1)), owned_(owned) {}

  // Copy constructor ups the reference count
  ref_ptr(const ref_ptr<X>& r)
    : px_(r.px_), refCount_(r.refCount_), owned_(r.owned_) { (*refCount_)++; }

  // Destructor decrements the reference count, will call delete if last ref
  ~ref_ptr();
  
  // assignment operator decrements this reference count, will call delete if
  // last ref, increments the reference count of rhs.
  ref_ptr<X>& operator=(const ref_ptr<X>& rhs);

  // If compiler supports this, then allows smart pointer to be assigned up
  // the inhieratance hierarchy like normal pointers.  For example:
  // class A {}; class B : public A {}; ref_ptr<A> a(new B); 
  // ref_ptr_cast is not needed if this works on your compiler.
  template<class Y> operator ref_ptr<Y>() { return ref_ptr<Y>(px_,refCount_); }
  
  // Sets internal pointer and releases counter.
  // Use when you want to set an existing ref_ptr to null (default case)
  // or when you want to reuse an existing ref_ptr as if creating it new.
  // For example, ref_ptr<A> a(new A); a.reset(new A); a.reset();
  void reset(X* px = 0) { *this = ref_ptr<X>(px, true); }

  // Use with care, the returned pointer is still owned by the ref_ptr.
  // Only guarenteed to be valid while ref_ptr is in scope.
  X* get()      const { return px_; }

  // Indicates if the internal pointer is owned
  bool owned()   const { return owned_; }

  // dereference ref_ptr returns dereferenced internal pointer.
  X& operator*()  const { return *px_; }
  // forward function calls on to the internal pointer.
  X* operator->() const { return px_; }
  
  // For most cases, you will want to dereference and compare objects,
  // these methods only compare internal pointers.
  bool operator==(const ref_ptr<X>& r) const { return px_ == r.px_; }
  bool operator!=(const ref_ptr<X>& r) const { return px_ != r.px_; }
  bool operator==(const X* r) const { return px_ == r; }
  bool operator!=(const X* r) const { return px_ != r; }

private:
  X* px_;
  unsigned int* refCount_;
  bool owned_;

  // These methods are needed if your compiler does not support template
  // methods of template classes.  See comment above.
  // Do not use the following methods.
  // - For use only with the ref_ptr_cast class below.
  // If your compiler does not like this friend declaration then just make
  // these public.
  //
  //   template <class Base, class Derived> friend class ref_ptr_cast;
  //   ref_ptr(X* px, unsigned int* refCount, int owned)
  //     : px_(px), refCount_(refCount), owned_(owned) { (*refCount_)++; }
  //  
  //   unsigned int* getRefCount() const { return refCount_; }
  
};

template<class X>
inline
ref_ptr<X>::~ref_ptr() {
  if (*refCount_ == 1) {
    // check for null not needed, but left here so that if you modify this
    // template to call a method on px_ then it will check for null first
    if ((owned_) && (px_ != 0)) {
       delete px_;
    }
    delete refCount_;
  } else {
    (*refCount_)--;
  }
}

template <class X>
inline
ref_ptr<X>&
ref_ptr<X>::operator=(const ref_ptr<X>& r) {
  if (this != &r) {
    if (*refCount_ == 1) {
      if ((owned_) && (px_ != 0)) { // see null check comment above
         delete px_;
      }
      delete refCount_;
    } else {
      (*refCount_)--;
    }
    (*r.refCount_)++;
    owned_ = r.owned_;
    px_ = r.px_;
    refCount_ = r.refCount_;
  }
  return *this;
}


// This class is only needed if compiler does not support template methods
// of template functions. See comment above.
// ref_ptr_cast is a helper class for ref_ptr.
// It is provided to allow conversions from (ref_ptrs of derived classes) to
//  (ref_ptrs of base classes).
//
// For example:
//   Given:
//
//     class Base { .... };
//     class Derived : public Base { ... };
//     ref_ptr<Base> base;
//     ref_ptr<Derived> derived = new Derived();
//
//   Use the ref_ptr_cast class cast method as follows:
//
//     base = ref_ptr_cast<Base,Derived>::cast(derived);
//           -- or --
//     ref_ptr<Base> func(ref_ptr<Derived> d) {
//       return ref_ptr_cast<Base,Derived>::cast(d);
//     }
//
//
//  template <class BaseClass, class DerivedClass>
//  class ref_ptr_cast {
//  public:
//    static ref_ptr<BaseClass> cast(const ref_ptr<DerivedClass>& dc) {
//      return ref_ptr<BaseClass>(dc.get(), dc.getRefCount());
//    }
//
//  private:
//    ref_ptr_cast(); // do not implement
//  };


#endif /* ref_ptr_H */

