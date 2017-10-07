#pragma once 

namespace hinode
{
	namespace graphics
	{
		class IHVKInterface
		{
		public:
			virtual ~IHVKInterface() {}

			virtual void release()noexcept = 0;
			virtual bool isGood()const noexcept = 0;

		};
	}
}