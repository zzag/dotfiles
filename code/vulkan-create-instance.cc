#include <vulkan/vulkan.h>
#include <iostream>


using std::cout;
using std::endl;


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

  vkDestroyInstance(instance, nullptr);

  return 0;
}
