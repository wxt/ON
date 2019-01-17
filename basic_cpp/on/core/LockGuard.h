#ifndef _ON_CORE_LOCKGUARD_H_
#define _ON_CORE_LOCKGUARD_H_

#include <mutex>

namespace on {
namespace core {

///@brief reimplementation of lock_guard based on http://www.cplusplus.com/reference/mutex/lock_guard/
template <typename MutexType>
class lock_guard
{

// ----------------------------------------------------------------------------------------------- //

public:

// ----------------------------------------------------------------------------------------------- //

	///@note spec says both copy and move constructors are disabled 
	///@note http://www.cplusplus.com/reference/mutex/lock_guard/lock_guard/
	lock_guard(const lock_guard&) = default;
	lock_guard(lock_guard&&) = default;
	lock_guard& operator=(const lock_guard&) = default;
	lock_guard& operator=(lock_guard&&) = default;

// ----------------------------------------------------------------------------------------------- //

	///@brief contructor attempts to lock the mutex on construction.
	lock_guard( MutexType& mtx ) : m_mtx{ mtx }
	{
		m_mtx.lock();
	}

// ----------------------------------------------------------------------------------------------- //

	///@brief constructor to take a mutex that assumed it's been locked.
	lock_guard( MutexType& mtx,  std::adopt_lock_t adopt_lock) : m_mtx{ mtx }
	{
	}

// ----------------------------------------------------------------------------------------------- //

	///@brief destructor unlock the mutex before destruction
	~lock_guard()
	{
		m_mtx.unlock();
	}

// ----------------------------------------------------------------------------------------------- //

private:
	MutexType &m_mtx;  ///< the reference to an already exist mutex

};

// ----------------------------------------------------------------------------------------------- //

}
}

#endif
