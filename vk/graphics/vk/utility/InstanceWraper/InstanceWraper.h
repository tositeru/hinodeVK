#pragma once

#include "../../HVKInterface.h"
#include "../../instance/HVKInstance.h"

namespace hinode
{
	namespace graphics
	{
		namespace utility
		{
			class InstanceWraper : public IHVKInterface
			{
			public:
				InstanceWraper();
				~InstanceWraper();

				virtual void release()noexcept override;



			public:
				virtual bool isGood()const noexcept override;

			private:
				HVKInstance mInstance;

			};
		}
	}
}