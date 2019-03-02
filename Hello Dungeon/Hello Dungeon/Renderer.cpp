#include <cstdlib>
#include <iostream>

#include "assert.h"
#include "Renderer.h"

Renderer::Renderer() {
	_SetupDebug();
	_InitInstance();
	_InitDevice();

} //Renderer()


Renderer::~Renderer() {
	_DeInitDevice();
	_DeInitInstance();
} //Renderer()

void Renderer::_InitInstance() {
	VkApplicationInfo app_info{};
	app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	app_info.apiVersion = VK_MAKE_VERSION(1, 0, 3);
	app_info.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
	app_info.pApplicationName = "Hello Dungeon";

	VkInstanceCreateInfo instance_create_info{};
	instance_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instance_create_info.pApplicationInfo = &app_info;
	instance_create_info.enabledLayerCount = _instance_layers.size();
	instance_create_info.ppEnabledLayerNames = _instance_layers.data();
	instance_create_info.enabledExtensionCount = _instance_extensions.size();
	instance_create_info.ppEnabledExtensionNames = _instance_extensions.data();
	
	auto err = vkCreateInstance(&instance_create_info, nullptr, &_instance);
	if (VK_SUCCESS != err) {
		assert(1 && "Vulkan ERROR: Create Instance Failed.");
		std::exit(-1);
	} //if
} //_InitInstance()

void Renderer::_DeInitInstance() {
	vkDestroyInstance(_instance, nullptr);
	_instance = nullptr;
} //_DeInitInstance()

void Renderer::_InitDevice() {

	{
		uint32_t gpu_ct = 0;
		vkEnumeratePhysicalDevices(_instance, &gpu_ct, nullptr);
		std::vector<VkPhysicalDevice> devices(gpu_ct);
		vkEnumeratePhysicalDevices(_instance, &gpu_ct, devices.data());

		for (auto& device : devices) {
			vkGetPhysicalDeviceProperties(device, &_gpu_properties);

			//determine type of the physical device
			if (_gpu_properties.deviceType == VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
				_gpu = device;
				break;
			} //if
			else if (_gpu_properties.deviceType == VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
				_gpu = device;
			} //else if
		} //for
	} //

	{
		uint32_t f_ct = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(_gpu, &f_ct, nullptr);
		std::vector<VkQueueFamilyProperties> family_property_list(f_ct);
		vkGetPhysicalDeviceQueueFamilyProperties(_gpu, &f_ct, family_property_list.data());


		bool found = false;
		for (uint32_t i = 0; i < f_ct; ++i) {
			if (family_property_list[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				found = true;
				_graphics_family_index = i;
			} //if
		} //for
		if (!found) {
			assert(1 && "Vulkan ERROR: Queue Family Supporting Graphics Not Found.");
			std::exit(-1);
		} //if
	} //
	
	{
		uint32_t layer_count = 0;
		vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
		std::vector<VkLayerProperties> layer_property_list(layer_count);
		vkEnumerateInstanceLayerProperties(&layer_count, layer_property_list.data());

		std::cout << "Instance Layers:" << std::endl;
		for (auto& i : layer_property_list) {
			std::cout << "\t" << i.layerName << "\t\t| " << i.description << std::endl;
		} //for
		std::cout << std::endl;
	} //

	{
		uint32_t layer_count = 0;
		vkEnumerateDeviceLayerProperties(_gpu, &layer_count, nullptr);
		std::vector<VkLayerProperties> layer_property_list(layer_count);
		vkEnumerateDeviceLayerProperties(_gpu, &layer_count, layer_property_list.data());

		std::cout << "Device Layers:" << std::endl;
		for (auto& i : layer_property_list) {
			std::cout << "\t" << i.layerName << "\t\t| " << i.description << std::endl;
		} //for
		std::cout << std::endl;
	} //

	float q_priorities[] { 1.0f };
	VkDeviceQueueCreateInfo device_queue_create_info{};
	device_queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	device_queue_create_info.queueFamilyIndex = _graphics_family_index;
	device_queue_create_info.queueCount = 1;
	device_queue_create_info.pQueuePriorities = q_priorities;

	VkDeviceCreateInfo device_create_info{};
	device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	device_create_info.queueCreateInfoCount = 1;
	device_create_info.pQueueCreateInfos = &device_queue_create_info;
	device_create_info.enabledLayerCount = _device_layers.size();
	device_create_info.ppEnabledLayerNames = _device_layers.data();
	device_create_info.enabledExtensionCount = _device_extensions.size();
	device_create_info.ppEnabledExtensionNames = _device_extensions.data();
	
	auto err = vkCreateDevice(_gpu, &device_create_info, nullptr, &_device);
	if (err != VK_SUCCESS) {
		assert(1 && "Vulkan ERROR: Device Creation Failed.");
		std::exit(-1);
	} //if
} //_InitDevice()

void Renderer::_DeInitDevice() {
	vkDestroyDevice(_device, nullptr);
	_device = nullptr;
} //_DeInitDevice()

void Renderer::_SetupDebug() {
	//_instance_layers.push_back("VK_LAYER_LUNARG_standard_validation");
	_instance_layers.push_back("VK_LAYER_LUNARG_threading");
	_instance_layers.push_back("VK_LAYER_LUNARG_draw_state");
	_instance_layers.push_back("VK_LAYER_LUNARG_image");
	_instance_layers.push_back("VK_LAYER_LUNARG_mem_tracker");
	_instance_layers.push_back("VK_LAYER_LUNARG_object_tracker");
	_instance_layers.push_back("VK_LAYER_LUNARG_param_checker");
	
	//_device_layers.push_back("VK_LAYER_LUNARG_standard_validation");
	_device_layers.push_back("VK_LAYER_LUNARG_threading");
	_device_layers.push_back("VK_LAYER_LUNARG_draw_state");
	_device_layers.push_back("VK_LAYER_LUNARG_image");
	_device_layers.push_back("VK_LAYER_LUNARG_mem_tracker");
	_device_layers.push_back("VK_LAYER_LUNARG_object_tracker");
	_device_layers.push_back("VK_LAYER_LUNARG_param_checker");
	
} //_SetupDebug()

void Renderer::_InitDebug()
{
}

void Renderer::_DeInitDebug()
{
}
