#include <vulkan/vulkan.h>
#include <iostream>
#include <vector>

// clang++ -o vulkan-logical-device vulkan-logical-device.cc -lvulkan

using std::cout;
using std::endl;


const char* getDeviceTypeString(VkPhysicalDeviceType type)
{
  switch (type) {
    case VK_PHYSICAL_DEVICE_TYPE_OTHER:
      return "other";
    case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
      return "integrated gpu";
    case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
      return "discrete gpu";
    case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
      return "virtual gpu";
    case VK_PHYSICAL_DEVICE_TYPE_CPU:
      return "cpu";
    default:
      return "unknown";
  }
}


bool checkDeviceProperties(VkPhysicalDevice device) {
  VkPhysicalDeviceProperties properties;

  vkGetPhysicalDeviceProperties(device, &properties);

  // TODO: check device properties
  if (VK_VERSION_MAJOR(properties.apiVersion) != 1) { return false; }

  return true;
}


bool selectQueueFamily(VkPhysicalDevice device, uint32_t& queueIndex)
{
  uint32_t numQueueFamilies = 0;

  vkGetPhysicalDeviceQueueFamilyProperties(device, &numQueueFamilies, nullptr);
  if (numQueueFamilies == 0) { return false; }

  std::vector<VkQueueFamilyProperties> properties(numQueueFamilies);
  vkGetPhysicalDeviceQueueFamilyProperties(device, &numQueueFamilies, properties.data());

  for (auto i = 0; i < numQueueFamilies; i++) {
    if (properties[i].queueCount > 0 && properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      queueIndex = i;
      return true;
    }
  }

  return false;
}


int main()
{
  VkApplicationInfo appInfo = {
    VK_STRUCTURE_TYPE_APPLICATION_INFO,
    nullptr,
    "App Name",
    VK_MAKE_VERSION(1, 0, 0),
    "Engine Name",
    VK_MAKE_VERSION(1, 0, 0),
    VK_API_VERSION_1_0
  };
  VkInstanceCreateInfo instanceCreateInfo = {
    VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
    nullptr,
    0,
    &appInfo,
    0, nullptr,
    0, nullptr
  };
  VkInstance instance;

  if (vkCreateInstance(&instanceCreateInfo, nullptr, &instance) != VK_SUCCESS)
  {
    cout << ":: vkCreateInstance() failed" << endl;
    return -1;
  }

  uint32_t numDevices = 0;
  if (vkEnumeratePhysicalDevices(instance, &numDevices, nullptr) != VK_SUCCESS)
  {
    cout << ":: vkEnumeratePhysicalDevices() failed" << endl;
    return -1;
  }

  cout << ":: Vulkan physical devices: " << numDevices << endl;

  std::vector<VkPhysicalDevice> physicalDevices(numDevices);
  if (vkEnumeratePhysicalDevices(instance, &numDevices, physicalDevices.data()) != VK_SUCCESS)
  {
    cout << ":: vkEnumeratePhysicalDevices() failed" << endl;
    return -1;
  }

  // print info
  for (auto i = 0; i < numDevices; i++) {
    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(physicalDevices[i], &properties);

    cout << ":: device " << properties.deviceName << endl;

    cout << ":: driver version ";
    cout << VK_VERSION_MAJOR(properties.driverVersion) << ".";
    cout << VK_VERSION_MINOR(properties.driverVersion) << ".";
    cout << VK_VERSION_PATCH(properties.driverVersion) << endl;

    cout << ":: api version ";
    cout << VK_VERSION_MAJOR(properties.apiVersion) << ".";
    cout << VK_VERSION_MINOR(properties.apiVersion) << ".";
    cout << VK_VERSION_PATCH(properties.apiVersion) << endl;

    cout << ":: device type " << getDeviceTypeString(properties.deviceType) << endl;

    cout << ":: max image dim 1d " << properties.limits.maxImageDimension1D << endl;
    cout << ":: max image dim 2d " << properties.limits.maxImageDimension2D << endl;
    cout << ":: max image dim 3d " << properties.limits.maxImageDimension3D << endl;
    cout << ":: max viewport dim[0] " << properties.limits.maxViewportDimensions[0] << endl;
    cout << ":: max viewport dim[1] " << properties.limits.maxViewportDimensions[1] << endl;

    uint32_t numQueueFamilies = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevices[i], &numQueueFamilies, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilyProperies(numQueueFamilies);
    vkGetPhysicalDeviceQueueFamilyProperties(
      physicalDevices[i],
      &numQueueFamilies,
      queueFamilyProperies.data());
    cout << ":: family queues " << numQueueFamilies << endl;
    for (auto j = 0; j < numQueueFamilies; j++) {
      cout << ":: family queue " << j << ": ";
      cout << queueFamilyProperies[j].queueCount << " queues / ";
      if (queueFamilyProperies[j].queueFlags & VK_QUEUE_GRAPHICS_BIT) cout << "graphics";
      else if (queueFamilyProperies[j].queueFlags & VK_QUEUE_COMPUTE_BIT) cout << "compute";
      else if (queueFamilyProperies[j].queueFlags & VK_QUEUE_TRANSFER_BIT) cout << "transfer";
      else if (queueFamilyProperies[j].queueFlags & VK_QUEUE_SPARSE_BINDING_BIT) cout << "sparse";
      cout << endl;
    }
  }

  // select physical device
  VkPhysicalDevice selectedDevice = VK_NULL_HANDLE;
  uint32_t selectedQueueFamilyIndex = UINT32_MAX;
  for (auto i = 0; i < numDevices; i++) {
    if (checkDeviceProperties(physicalDevices[i]) &&
        selectQueueFamily(physicalDevices[i], selectedQueueFamilyIndex)) {
      selectedDevice = physicalDevices[i];
      break;
    }
  }

  if (selectedDevice == VK_NULL_HANDLE) {
    cout << ":: sorry :( your device is not compatible or have no graphics family queue" << endl;
    vkDestroyInstance(instance, nullptr);
    return -1;
  }

  // create logical device
  std::vector<float> priorities = { 1.0f };
  VkDeviceQueueCreateInfo queueCreateInfo = {
    VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
    nullptr,
    0,
    selectedQueueFamilyIndex,
    static_cast<uint32_t>(priorities.size()),
    priorities.data()
  };
  VkDeviceCreateInfo deviceCreateInfo = {
    VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
    nullptr,
    0,
    1, &queueCreateInfo,
    0, nullptr,
    0, nullptr,
    nullptr
  };
  VkDevice device;

  if (vkCreateDevice(selectedDevice, &deviceCreateInfo, nullptr, &device) != VK_SUCCESS)
  {
    cout << ":: vkCreateDevice() failed" << endl;
    vkDestroyInstance(instance, nullptr);
    return -1;
  }

  // clean up
  vkDeviceWaitIdle(device);
  vkDestroyDevice(device, nullptr);
  vkDestroyInstance(instance, nullptr);
  return 0;
}
