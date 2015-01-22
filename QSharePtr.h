#ifndef _QSHAREPTR_H
#define _QSHAREPTR_H

#ifdef __GNUG__
#include <execinfo.h>
#include <sstream>
#endif

#include <vector>

using namespace std;


template<class T>
class QSharePtr {
  public:
    class PointerCount {
      private:
        long refCount;
        T* objPtr;
        //QCMutex lock_;
      public:
        void add_ref();
        void release_ref();
        long use_count();
        PointerCount(T*  p_obj);
        ~PointerCount();
    };
    
    QSharePtr();
    explicit QSharePtr(T*  ptr);
    QSharePtr(const QSharePtr<T> & source);
    virtual ~QSharePtr();
    QSharePtr<T> & operator=(const QSharePtr<T> & source);
    //T & operator[](int n);
    int getRefCount();
    T* operator->();
	const T* operator->() const;
    T& operator*();
    inline T* get_pointer() {return p_obj_;}
	inline const T* get_pointer() const { return p_obj_;}
  public:
    operator bool() const;
    bool operator!() const;
    bool operator<(const QSharePtr<T>& a) const;
  private:
    PointerCount* p_count_;
    T* p_obj_;
};
template<class T>
void QSharePtr<T>::PointerCount::add_ref() {
    //lock_.lock();
    refCount++;
    //lock_.unlock();
}
template<class T>
void QSharePtr<T>::PointerCount::release_ref() {
    //lock_.lock();
    if(refCount <= 1)
    {   
        //This time is in destructor, so release lock here and do delete this.
        //lock_.unlock();
        delete this;   
        return;
    }else
    {
        refCount--;
    }
    //lock_.unlock();
}
template<class T>
long QSharePtr<T>::PointerCount::use_count() {
    return refCount;
}
template<class T>
QSharePtr<T>::PointerCount::PointerCount(T*  p_obj) {
    objPtr = p_obj;
    refCount = 1;
}
template<class T>
QSharePtr<T>::PointerCount::~PointerCount() {
    if(objPtr)
    {
        delete objPtr;
    }
}
template<class T>
QSharePtr<T>::QSharePtr() {
    p_count_ = new PointerCount(0);
    p_obj_ = 0;
    //ptr_->objPtr = NULL;
    //ptr_->refCount = 0;
}
template<class T>
QSharePtr<T>::QSharePtr(T*  ptr) {
    p_count_ = new PointerCount(ptr);
    p_obj_ = ptr;
    //p_count_->refCount = 1;  
    //p_count_->add_ref();
}
template<class T>
QSharePtr<T>::QSharePtr(const QSharePtr<T> & source) {
    p_count_ = source.p_count_;    
    p_count_->add_ref();
    p_obj_ = source.p_obj_;
}
template<class T>
QSharePtr<T>::~QSharePtr() {
    p_count_->release_ref();
}
template<class T>
QSharePtr<T> & QSharePtr<T>::operator=(const QSharePtr<T> & source) {
    if(this == &source)
    {
        return *this;
    }
    p_count_->release_ref();
    p_count_ = source.p_count_;
    p_obj_ = source.p_obj_;
    if(p_count_)
    {
        p_count_->add_ref();
    }
    return *this;
}

/*
template<class T>
T & QSharePtr<T>::operator[](int n) {
    
    return (p_obj_[n]);
}
*/

template<class T>
int QSharePtr<T>::getRefCount() {
    return p_count_->use_count();
}
template<class T>
T* QSharePtr<T>::operator->() {
    return p_obj_;
}
template<class T>
const T* QSharePtr<T>::operator->() const {
	return p_obj_;
}
template<class T>
T& QSharePtr<T>::operator*() {
    return *(p_obj_);
}
template<class T>
QSharePtr<T>::operator bool() const {
    return (p_obj_ != 0);
}
template<class T>
bool QSharePtr<T>::operator!() const {
    return (p_obj_ == 0);
}
template<class T>
bool QSharePtr<T>::operator<(const QSharePtr<T>& a) const
{
    return this->p_obj_ < a.p_obj_;
}

#endif