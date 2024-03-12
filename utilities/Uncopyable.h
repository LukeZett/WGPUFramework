#ifndef WGPUFUNCOPYABLE
#define WGPUFUNCOPYABLE

/**
* @brief Simple class ensuring instances are not copied or moved,
* which is mostly undesirable for classes representing graphic API objects
*/
class Uncopyable
{
public:
	Uncopyable() {};
	~Uncopyable() {};
	Uncopyable(const Uncopyable&) = delete;
	Uncopyable(const Uncopyable&&) = delete;

private:

};

#endif // !WGPUFUNCOPYABLE