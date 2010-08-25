#if !defined(UUID_F28B1E6D0B674A5E8C67F0054EC97F3F)
#define UUID_F28B1E6D0B674A5E8C67F0054EC97F3F

#include <boost/function.hpp>
#include <boost/thread.hpp>
#include <boost/detail/lightweight_mutex.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <queue>
#include <utility>

class Loader: boost::noncopyable {
private:
	Loader();
public:
	~Loader();
	static boost::shared_ptr<Loader> instance();
	bool enqueue(const boost::function<bool()>& doLoad, const boost::function<void(bool)>& loadComplete);
	bool enqueue(const boost::function<bool()>& doLoad);
private:
	void run();
private:
	typedef std::pair<boost::function<bool()>, boost::function<void(bool)> > task_t;
	boost::thread		mBackgroundThread;
	boost::mutex		mMutex;
	std::queue<task_t>	mFifo;
	volatile bool		mQuitRequested;
};

#endif // UUID_F28B1E6D0B674A5E8C67F0054EC97F3F

