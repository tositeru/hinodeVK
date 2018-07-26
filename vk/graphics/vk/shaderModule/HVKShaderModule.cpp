#include "HVKShaderModule.h"

#include <SPIRV\GlslangToSpv.h>

#include "../common/Common.h"

namespace hinode
{
	namespace graphics
	{
		HVKShaderModule::HVKShaderModule()
			: mShaderModule(VK_NULL_HANDLE)
			, mParentDevice(VK_NULL_HANDLE)
		{}

		HVKShaderModule::HVKShaderModule(HVKShaderModule&& right)noexcept
			: mShaderModule(right.mShaderModule)
			, mParentDevice(right.mParentDevice)
		{
			right.mShaderModule = VK_NULL_HANDLE;
			right.mParentDevice = VK_NULL_HANDLE;
		}

		HVKShaderModule& HVKShaderModule::operator=(HVKShaderModule&& right)noexcept
		{
			this->mShaderModule = right.mShaderModule;
			this->mParentDevice = right.mParentDevice;

			right.mShaderModule = VK_NULL_HANDLE;
			right.mParentDevice = VK_NULL_HANDLE;
			return *this;
		}

		HVKShaderModule::~HVKShaderModule()
		{
			this->release();
		}

		void HVKShaderModule::release()noexcept
		{
			if (this->isGood()) {
				vkDestroyShaderModule(this->mParentDevice, this->mShaderModule, this->allocationCallbacksPointer());
				this->mParentDevice = VK_NULL_HANDLE;
				this->mShaderModule = VK_NULL_HANDLE;
			}
		}


		void HVKShaderModule::create(VkDevice device, VkShaderModuleCreateInfo* pInfo)
		{
			this->release();

			auto ret = vkCreateShaderModule(device, pInfo, this->allocationCallbacksPointer(), &this->mShaderModule);
			if (VK_SUCCESS != ret) {
				throw HINODE_GRAPHICS_CREATE_EXCEPTION(HVKShaderModule, create, ret) << "ì¬‚ÉŽ¸”s";
			}
			this->mParentDevice = device;
		}

		bool HVKShaderModule::isGood()const noexcept
		{
			return this->mShaderModule != VK_NULL_HANDLE && this->mParentDevice != VK_NULL_HANDLE;
		}

		VkShaderModule HVKShaderModule::shaderModule()noexcept
		{
			assert(this->isGood());
			return this->mShaderModule;
		}
	}

	namespace graphics
	{
		HVKShaderModuleCreateInfo::HVKShaderModuleCreateInfo()noexcept
			: HVKShaderModuleCreateInfo(nullptr, 0)
		{ }

		HVKShaderModuleCreateInfo::HVKShaderModuleCreateInfo(const uint32_t* pCode, size_t codeSize)noexcept
		{
			this->pCode = pCode;
			this->codeSize = codeSize;
			//ˆÈ‰ºŒÅ’è
			this->flags = 0; //«—ˆŽg‚í‚ê‚é‚©‚à‚µ‚ê‚È‚¢
			this->sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			this->pNext = nullptr;
		}

		std::vector<unsigned int> HVKShaderModuleCreateInfo::sGLSLtoSPV(VkShaderStageFlagBits type, const char* pShaderSource)
		{
#ifndef __ANDROID__
			EShLanguage stage = FindLanguage(shader_type);
			glslang::TShader shader(stage);
			glslang::TProgram program;
			const char *shaderStrings[1];
			TBuiltInResource Resources;
			init_resources(Resources);

			// Enable SPIR-V and Vulkan rules when parsing GLSL
			EShMessages messages = (EShMessages)(EShMsgSpvRules | EShMsgVulkanRules);

			shaderStrings[0] = pshader;
			shader.setStrings(shaderStrings, 1);

			if (!shader.parse(&Resources, 100, false, messages)) {
				puts(shader.getInfoLog());
				puts(shader.getInfoDebugLog());
				return false;  // something didn't work
			}

			program.addShader(&shader);

			//
			// Program-level processing...
			//

			if (!program.link(messages)) {
				puts(shader.getInfoLog());
				puts(shader.getInfoDebugLog());
				fflush(stdout);
				return false;
			}

			glslang::GlslangToSpv(*program.getIntermediate(stage), spirv);
#else
			// On Android, use shaderc instead.
			shaderc::Compiler compiler;
			shaderc::SpvCompilationResult module =
				compiler.CompileGlslToSpv(pshader, strlen(pshader), MapShadercType(shader_type), "shader");
			if (module.GetCompilationStatus() != shaderc_compilation_status_success) {
				LOGE("Error: Id=%d, Msg=%s", module.GetCompilationStatus(), module.GetErrorMessage().c_str());
				return false;
			}
			spirv.assign(module.cbegin(), module.cend());
#endif

		}
	}
}