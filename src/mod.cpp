#include "mod.hpp"

using namespace cocos2d;

#pragma comment(lib,"opengl32.lib")

HMODULE ccdll;

GLFWwindow* (__cdecl *host_glfwCreateWindow)(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share) = nullptr;

GLFWwindow* __cdecl hook_glfwCreateWindow(int width, int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share) {
    // Set GLFW_SAMPLES Window Hint to 4 to enable MSAAx4
    *as<int*>((unsigned int)ccdll + 0x1a14e8) = 4;

    return host_glfwCreateWindow(width, height, title, monitor, share);
}

void (_fastcall *host_CCDirector_drawScene)(CCDirector* _this) = nullptr;

void _fastcall hook_CCDirector_drawScene(CCDirector* _this) {
    // Enable multisampling
    glEnable(GL_MULTISAMPLE);

    host_CCDirector_drawScene(_this);
}

bool mod::load() {
    if (MH_Initialize() != MH_OK) [[unlikely]]
        return false;

    ccdll = GetModuleHandleA("libcocos2d.dll");

    if (ccdll == NULL) {
        MessageBoxA(NULL, "Could not access Cocos2d DLL!", "Error", MB_ICONERROR | MB_OK);
        return false;
    }

    // Get the address for CCDirector::drawScene
    auto drawSceneAddr = GetProcAddress(ccdll, "?drawScene@CCDirector@cocos2d@@QAEXXZ");

    if (drawSceneAddr == NULL) {
        MessageBoxA(NULL, "Could not access Cocos2d DLL!", "Error", MB_ICONERROR | MB_OK);
        return false;
    }

    if (MH_CreateHook(
        drawSceneAddr,
        as<LPVOID>(hook_CCDirector_drawScene),
        as<LPVOID*>(&host_CCDirector_drawScene)
    ) != MH_OK) [[unlikely]] {
        MessageBoxA(NULL, "Hook failed!", "Error", MB_ICONERROR | MB_OK);
        return false;
    }

    // Address to glfwCreateWindow in the cocos2d dll
    auto cwinaddr = as<LPVOID>((unsigned int)ccdll + 0x110f50);

    if (MH_CreateHook(
        cwinaddr,
        as<LPVOID>(hook_glfwCreateWindow),
        as<LPVOID*>(&host_glfwCreateWindow)
    ) != MH_OK) [[unlikely]] {
        MessageBoxA(NULL, "Hook failed!", "Error", MB_ICONERROR | MB_OK);
        return false;
    }

    if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK) [[unlikely]] {
        MH_Uninitialize();
        return false;
    }

    return true;
}

void mod::unload() {
    MH_DisableHook(MH_ALL_HOOKS);

    MH_Uninitialize();
}
