#pragma once

class IRefCounted
{
private:
	volatile long refCount{};
public:
	virtual int destruct( bool del ) = 0;
	virtual int Release() = 0;

	void unreference()
	{
		if ( InterlockedDecrement( &refCount ) == 0 && Release() )
			destruct( true );
	}
};