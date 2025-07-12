#define GLFW_INCLUDE_VULKAN
#include<GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <iostream>
#include<stdexcept>
#include<functional>
#include<cstdlib>

using namespace std;

#ifdef NDEBUG
const bool enableValidationLayers=false;
#else
const bool enableValidationLayers=true;
#endif

const vector<const char*>validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT*pCallbackData,
    void*pUserData
){
    cerr<<"validation_layer:"<<pCallbackData->pMessage<<endl;
    return VK_FALSE;
}

class HellowTriangleApplication{
    public:
        const int WIDTH = 800;
        const int HEIGHT = 600;
    private:
        GLFWwindow* window;
        VkInstance instance;
    public:
        void run(){
            initWindow();
            initVulkan();
            mainLoop();
            cleanUp();
        }

    private:
        void initVulkan(){
            createInstance();
        }
        //检查glfw扩展
        vector<const char*>getRequireExtensions(){
            uint32_t glfwExtensionCount = 0;
            const char**glfwExtensions;
            glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
            vector<const char*>extensions(glfwExtensions,glfwExtensions+glfwExtensionCount);
            if(enableValidationLayers){
                extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
            }
            return extensions;
        }

        bool checkValidationLayerSupport(){
            uint32_t layerCount;
            vkEnumerateInstanceLayerProperties(&layerCount,nullptr);
            vector<VkLayerProperties> avaliableLayers(layerCount);
            vkEnumerateDeviceLayerProperties(&layerCount,avaliableLayers.data());
    
            for(const char*layerName:validationLayers){
                bool layerFound = false;
                for(const auto&layerProperties:avaliableLayers){
                    if(strcmp(layerName,layerProperties.layerName)==0){
                        layerFound = True;
                        break;
                    }
                }

                if (layerFound)
                    return false;
            }
    
            return true;
        }

        void createInstance(){
            VkApplicationInfo appInfo = {};
            appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
            appInfo.pApplicationName = "Hello_Triangle";
            appInfo.applicationVersion = VK_MAKE_VERSION(1,0,0);
            appInfo.pEngineName = "No_Engine";
            appInfo.engineVersion = VK_MAKE_VERSION(1,0,0)；
            appInfo.apiVersion = VK_API_VERSION_1_0;

            VkInstanceCreateInfo createInfo = {};
            createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
            createInfo.pApplicationInfo = &appInfo;

            unit32_t glfwExtensionCount = 0;
            const char** glfwExtensions;
            glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensions);
            createInfo.enabledExtensionCount = glfwExtensionCount;
            createInfo.ppEnabledExtensionNames = glfwExtensions;
            //校验层
            if(enableValidationLayers){
                createInfo.enabledLayerCount = static_cast<uint32_t>(
                    validationLayers.size()
                );
                createInfo.ppEnabledLayerNames = validationLayers.data();
            }   
            else
                createInfo.enabledLayerCount = 0;

            unit32_t extensionCount = 0
            vkEnumerateDeviceExtensionProperties(nullptr,&extensionCount,nullptr);
            vector<VkExtensionProperties> extensions(extensionCount);
            vkEnumerateDeviceExtensionProperties(nullptr,&extensionCount,extensions.data());
            cout<<"available_extensions"<<endl;
            for(const auto& extension:extensions){
                cout<<extension.extensionName<<endl;
            }
            auto extensions = getRequireExtensions();
            createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
            createInfo.ppEnabledExtensionNames = extensions.data();

            if(vkCreateInstance(&createInfo,nullptr,&instance)!=VK_SUCCESS)
                throw runtime_error("failed to create instance");
            
            if(enableValidationLayers&&!checkValidationLayerSupport()){
                throw runtime_error("validation_layer_requested,but not available")
            }
        }

        void mainLoop(){

        }

        void cleanUp(){
            vkDestroyInstance(instance,nullptr);
            glfwDestoryWindow(window);
            glfTerminate();
        }

        void initWindow(){
            glfwInit();
            glfwWindowHint(GLFW_CLIENT_API,GLFW_NO_API);
            glfwWindowHint(GLFW_RESIZABLE,GLFW_FALSE);
            window = glfwCreateWindow(WIDTH,HEIGHT,"Vulkan_window",nullptr,nullptr);
            uint32_t extensionCount = 0;
            vkEnumerateInstanceExtensionProperties(nullptr,&extensionCount,nullptr);
            cout<<extensionCount<<"extension_supported"<<endl;
            
            glm::mat4 matrix;
            glm::vec4 vec;
            auto test = matrix * vec;
            while(!glfwWindowShouldClose(window)){
                glfwPollEvents();
            }

            glfwDestroyWindow(window);
            glfwTerminate();
        }
}


int main(){
    HellowTriangleApplication app;
    try{
        app.run();
    }
    catch(const std::exception&e){
        cerr<<e.what()<<endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}