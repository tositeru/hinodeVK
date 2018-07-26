#pragma once

#include <vector>
#include <vulkan\vulkan.h>

#include "../allocationCallbacks/HVKAllocationCallbacks.h"
#include "../HVKInterface.h"

namespace hinode
{
	namespace graphics
	{
		class HVKShaderModule : public IHVKInterface, public HVKAllocationCallbacks
		{
			HVKShaderModule(HVKShaderModule& right)noexcept;
			HVKShaderModule& operator=(HVKShaderModule& right)noexcept;
		public:
			HVKShaderModule();
			HVKShaderModule(HVKShaderModule&& right)noexcept;
			HVKShaderModule& operator=(HVKShaderModule&& right)noexcept;
			~HVKShaderModule();

			void release()noexcept override;

			/// @brief ì¬
			/// @param[in] device
			/// @param[in] pInfo
			/// @exception HVKException
			void create(VkDevice device, VkShaderModuleCreateInfo* pInfo);

		public:
			bool isGood()const noexcept;
			VkShaderModule shaderModule()noexcept;
			operator VkShaderModule()noexcept { return this->shaderModule(); }

		private:
			VkShaderModule mShaderModule;
			VkDevice mParentDevice;
		};
	}

	namespace graphics
	{
		struct HVKShaderModuleCreateInfo : public VkShaderModuleCreateInfo
		{
			HVKShaderModuleCreateInfo()noexcept;
			HVKShaderModuleCreateInfo(const uint32_t* pCode, size_t codeSize)noexcept;

			static std::vector<unsigned int> sGLSLtoSPV(VkShaderStageFlagBits type, const char* pShaderSource);
		};
	}

}